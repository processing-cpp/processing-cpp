#!/usr/bin/env python3
"""Compiles small CppMode-style sketches against src/Processing.h, one per API area, so an
API that stops compiling shows up right away. Header only: no translator, no window.
Run from the CppMode folder: python3 scripts/test_api.py [filter]"""
import subprocess, sys, tempfile, pathlib, os
from concurrent.futures import ThreadPoolExecutor
CASES = {
    'String methods':
        'String s = "Hello"; println(s.length(), s.charAt(1), s.substring(1,3), s.indexOf("l"), s.toUpperCase(), s.equals("Hello"), s.startsWith("He"), s.trim(), s.endsWith("o"), s.lastIndexOf("l"), s.toLowerCase());',
    'String more':
        'String a = "a,b"; println(a.compareTo("b"), a.equalsIgnoreCase("A,B"), a.isEmpty(), a.contains("b"), a.replace("a","c"), a.split(",").size(), a.toCharArray().size(), a.hashCode());',
    'split/join/splitTokens':
        'auto p = split("a,b,c", ","); println(join(p, "-"), splitTokens("a b  c").size(), trim("  x "));',
    'nf/nfc/nfs/nfp':
        'println(nf(3, 3), nf(3.14159f, 1, 2), nfc(1234567), nfs(5, 2), nfp(5, 2), nf(-1.5f, 2, 1));',
    'str/int/float conv':
        'println(str(5), str(1.5f), str(true), parseInt("7"), parseFloat("2.5"), int(3.9f), char(65), boolean(1), unhex("FF"), unbinary("101"));',
    'match/matchAll':
        'auto m = match("a1b2", "[0-9]"); auto ms = matchAll("a1b2", "[0-9]"); println(m.size(), ms.size());',
    'arrays helpers':
        'std::vector<int> a = {3,1,2}; a = sort(a); a = append(a, 4); a = reverse(a); a = shorten(a); a = expand(a); a = concat(a, a); a = subset(a, 1, 2); printArray(a); Array<int> A = {1, 2}; Array<int> B(2); arrayCopy(A, B); A = splice(A, 9, 0);',
    'min/max arrays':
        'std::vector<float> f = {1.f, 5.f, 2.f}; println(min(f), max(f), min(1, 2, 3), max(1.f, 2.f), constrain(5, 0, 3), map(5, 0, 10, 0, 100), norm(5, 0, 10), lerp(0, 10, .5f), mag(3, 4), dist(0, 0, 3, 4), sq(3), sqrt(9.f));',
    'IntList':
        'IntList il; il.append(3); il.append(1); il.sort(); il.reverse(); il.shuffle(); println(il.get(0), il.size(), il.sum(), il.max(), il.min(), il.hasValue(3), il.index(3)); il.increment(0); il.set(0, 5); il.remove(0); il.clear();',
    'FloatList/StringList':
        'FloatList fl; fl.append(1.5f); fl.mult(0, 2); println(fl.get(0), fl.sum()); StringList sl; sl.append("x"); sl.upper(); println(sl.get(0), sl.hasValue("X")); auto arr = sl.array();',
    'IntDict/FloatDict/StringDict':
        'IntDict d; d.set("a", 1); d.increment("a"); d.add("b", 3); d.sortValues(); println(d.get("a"), d.hasKey("b"), d.size()); for (auto k : d.keys()) println(k); FloatDict fd; fd.set("x", 1.5f); StringDict sd; sd.set("k", "v"); println(sd.get("k"));',
    'ArrayList C++':
        'ArrayList<PVector> pts; pts.add(PVector(1,2)); println(pts.size(), pts.get(0)->x, pts.isEmpty()); pts.remove(0); pts.clear(); ArrayList<int> xs; xs.add(3); println(xs.contains(3), xs.indexOf(3));',
    'HashMap C++':
        'HashMap<String, int> m; m.put("a", 1); println(m.get("a"), m.containsKey("a"), m.size()); for (auto k : m.keySet()) println(k); m.remove("a");',
    'PVector':
        'PVector v(1,2,3); PVector w = PVector::random2D(); v.add(w); v.sub(1,1,1); v.mult(2); v.div(2); v.normalize(); v.limit(1); v.setMag(2); v.rotate(0.1f); println(v.mag(), v.magSq(), v.heading(), v.dot(w), v.cross(w), PVector::angleBetween(v, w), PVector::dist(v, w), v.copy(), PVector::fromAngle(1)); v.lerp(w, .5f); v.set(0,0,0); auto arr = v.array();',
    'PVector static ops':
        'PVector a(1,0), b(0,1); PVector c = PVector::add(a, b); PVector d = PVector::sub(a, b); PVector e = PVector::mult(a, 2); PVector f = PVector::div(a, 2); PVector g = PVector::lerp(a, b, .5f); println(c, d, e, f, g, a + b, a * 2);',
    'color fns':
        'color c = color(255, 128, 0); println(red(c), green(c), blue(c), alpha(c), hue(c), saturation(c), brightness(c), hex(c), hex(c, 6)); color l = lerpColor(c, color(0), .5f); colorMode(HSB, 360, 100, 100); color h = color(120, 50, 50); colorMode(RGB, 255);',
    'time/date':
        'println(millis(), second(), minute(), hour(), day(), month(), year());',
    'random/noise':
        'randomSeed(1); noiseSeed(2); noiseDetail(4, .5f); println(random(1), random(1, 2), randomGaussian(), noise(1), noise(1, 2), noise(1, 2, 3), int(random(10)));',
    'trig':
        'println(sin(1), cos(1), tan(1), asin(.5f), acos(.5f), atan(1), atan2(1, 1), degrees(PI), radians(180), TWO_PI, HALF_PI, QUARTER_PI, TAU);',
    'math misc':
        'println(abs(-1), abs(-1.5f), ceil(1.2f), floor(1.8f), round(1.5f), pow(2, 3), exp(1), log(10), sq(2), min(1, 2), max(1, 2));',
    'files':
        'auto lines = loadStrings("x.txt"); saveStrings("y.txt", lines); auto bytes = loadBytes("x.bin"); saveBytes("y.bin", bytes); PrintWriter* w = createWriter("o.txt"); w->println("hi"); w->flush(); w->close(); BufferedReader* r = createReader("o.txt"); String line = r->readLine();',
    'json':
        'JSONObject o; o.setInt("a", 1); o.setString("s", "x"); JSONArray arr; arr.append(1); o.setJSONArray("arr", arr); println(o.getInt("a"), o.getString("s"), o.hasKey("a"), arr.size()); saveJSONObject(o, "o.json"); JSONObject p = parseJSONObject("{\\"x\\": 1}");',
    'table':
        'Table* t = new Table(); t->addColumn("name"); TableRow r = t->addRow(); r.setString("name", "a"); println(t->getRowCount(), t->getString(0, "name")); for (TableRow row : t->rows()) println(row.getString("name")); saveTable(t, "t.csv");',
    'xml':
        'XML x = parseXML("<a><b id=\\"1\\">t</b></a>"); auto kids = x.getChildren("b"); println(kids.size(), x.getName());',
    'PImage ops':
        'PImage* img = createImage(10, 10, RGB); img->loadPixels(); img->pixels[0] = color(255); img->updatePixels(); PImage* c = img->get(0, 0, 5, 5); img->resize(20, 0); img->filter(GRAY); img->filter(BLUR, 2); img->mask(c); img->blend(c, 0, 0, 5, 5, 0, 0, 5, 5, ADD); color p = img->get(1, 1); img->set(1, 1, p); img->copy(c, 0, 0, 5, 5, 0, 0, 5, 5); img->save("i.png"); println(img->width, img->height);',
    'PGraphics':
        'PGraphics* pg = createGraphics(100, 100); pg->beginDraw(); pg->background(0); pg->fill(255); pg->ellipse(50, 50, 20, 20); pg->text("x", 10, 10); pg->endDraw(); image(pg, 0, 0); PImage* snap = pg->get();',
    'PFont/text':
        'PFont* f = createFont("Arial", 16); textFont(f); textSize(12); textAlign(CENTER, CENTER); textLeading(14); println(textWidth("abc"), textAscent(), textDescent()); text("hi", 10, 10); text("box", 0, 0, 50, 50); text(5, 1, 1); text(1.5f, 1, 1); char cs[] = {\'a\', \'b\'}; text(\'c\', 1, 1);',
    'shapes':
        'arc(50, 50, 20, 20, 0, PI, PIE); quad(0,0,1,0,1,1,0,1); triangle(0,0,1,0,0,1); rect(0,0,10,10,3); square(0,0,5); circle(5,5,5); point(1,1); line(0,0,5,5); bezier(0,0,1,1,2,2,3,3); curve(0,0,1,1,2,2,3,3); println(bezierPoint(0,1,2,3,.5f), bezierTangent(0,1,2,3,.5f), curvePoint(0,1,2,3,.5f), curveTangent(0,1,2,3,.5f));',
    'beginShape kinds':
        'beginShape(TRIANGLE_STRIP); vertex(0,0); vertex(1,0); vertex(0,1); endShape(); beginShape(QUADS); endShape(CLOSE); beginShape(POINTS); endShape(); beginShape(LINES); endShape(); beginShape(TRIANGLE_FAN); endShape(); beginShape(TRIANGLES); endShape(); beginShape(QUAD_STRIP); endShape();',
    'transforms':
        'pushMatrix(); translate(1, 2); rotate(1); scale(2); shearX(.1f); shearY(.1f); applyMatrix(1,0,0,0,1,0); resetMatrix(); popMatrix(); push(); pop(); printMatrix(); println(modelX(0,0,0), screenX(1, 2), screenY(1, 2));',
    '3D':
        'translate(0,0,1); rotateX(1); rotateY(1); rotateZ(1); box(10); box(1, 2, 3); sphere(5); sphereDetail(10); camera(); camera(0,0,10, 0,0,0, 0,1,0); perspective(); ortho(); frustum(-1,1,-1,1,1,10); lights(); ambientLight(50,50,50); directionalLight(255,255,255, 0,0,-1); pointLight(255,0,0, 0,0,0); spotLight(255,255,255, 0,0,0, 0,0,-1, PI/4, 2); noLights(); ambient(100); specular(255); shininess(5); emissive(0); lightFalloff(1,0,0); lightSpecular(0,0,0); beginCamera(); endCamera(); println(screenZ(0,0,0), modelY(0,0,0), modelZ(0,0,0));',
    'style':
        'fill(255, 0, 0, 128); fill(0xFFFF0000); stroke(0); strokeWeight(2); strokeJoin(ROUND); strokeCap(SQUARE); noFill(); noStroke(); tint(255, 128); noTint(); smooth(4); noSmooth(); rectMode(CENTER); ellipseMode(RADIUS); imageMode(CORNERS); shapeMode(CENTER); blendMode(MULTIPLY); clip(0,0,10,10); noClip(); background(0, 0, 255); clear(); pushStyle(); popStyle();',
    'input vars':
        'println(mouseX, mouseY, pmouseX, pmouseY, mouseButton == LEFT, keyCode == UP, key == CODED, key == ENTER, focused, frameCount, width, height, pixelWidth, pixelHeight, displayWidth, displayHeight, _mousePressed, _keyPressed);',
    'window/env':
        'surface.setTitle("x"); surface.setResizable(true); surface.setLocation(10, 10); surface.setSize(200, 200); windowTitle("y"); windowResizable(true); windowMove(5, 5); windowResize(300, 300); cursor(HAND); noCursor(); cursor(); frameRate(30); println(_frameRate, pixelDensityValue, displayDensity()); delay(1); redraw(); loop(); noLoop(); println(isLooping());',
    'saveFrame/save':
        'saveFrame(); saveFrame("f-####.png"); save("s.png");',
    'selectInput forms':
        'selectInput("Pick", "fileSelected"); selectFolder("Dir", "folderSelected"); selectOutput("Save", "fileSelected"); String p = selectInput("Pick"); File f("x"); println(f.getName(), f.exists());',
    'exit/launch/link':
        'link("https://processing.org"); launch("a.txt"); String sp = sketchPath("x"); String dp = dataPath("y"); String sv = savePath("z/w.txt");',
    'Thread':
        'thread([&]{ println("bg"); });',
}


def build_source(items):
    """All cases in one file, each in its own struct, so the header is parsed once.
    Returns the source and the line range each case occupies."""
    lines = ['#include "Processing.h"', 'namespace Processing {']
    ranges = []
    for i, (name, body) in enumerate(items):
        start = len(lines) + 1
        lines += [f'struct Case{i} : public PApplet {{',
                  ' void fileSelected(File f) override {}',
                  ' void folderSelected(File f) override {}',
                  ' void setup() override {', body, ' }',
                  ' void draw() override {}', '};']
        ranges.append((name, start, len(lines)))
    lines.append('}')
    return "\n".join(lines) + "\n", ranges

def main():
    if not pathlib.Path("src/Processing.h").exists(): sys.exit("run from the CppMode folder")
    flt = sys.argv[1].lower() if len(sys.argv) > 1 else ""
    items = [(n, b) for n, b in CASES.items() if flt in n.lower()]
    src, ranges = build_source(items)
    with tempfile.NamedTemporaryFile("w", suffix=".cpp", delete=False) as f:
        f.write(src)
    try:
        r = subprocess.run(["g++", "-std=c++20", "-fsyntax-only", "-w", "-fmax-errors=200", "-I", "src",
                            "-I/usr/include/freetype2", f.name], capture_output=True, text=True)
    finally:
        os.unlink(f.name)
    errors = {}   # case name -> messages
    other = []
    for l in r.stderr.splitlines():
        if "error: " not in l: continue
        msg = l.split("error: ", 1)[1]
        where = l.split(":")
        line = int(where[1]) if len(where) > 2 and where[0] == f.name and where[1].isdigit() else None
        owner = next((n for n, a, b in ranges if line is not None and a <= line <= b), None)
        if owner: errors.setdefault(owner, []).append(msg)
        else: other.append(l)
    bad = 0
    for name, _, _ in ranges:
        ok = name not in errors and not (r.returncode and not errors and other)
        print(("ok    " if ok else "FAIL  ") + name)
        for e in errors.get(name, [])[:2]: print("        " + e[:180])
        bad += not ok
    for l in other[:5]: print("        " + l[:200])
    print(f"{len(ranges) - bad}/{len(ranges)} API areas compile")
    sys.exit(1 if bad else 0)

if __name__ == "__main__":
    main()
