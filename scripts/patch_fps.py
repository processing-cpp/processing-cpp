import pathlib, re, sys, shutil
MARK = "cppmode-measured-fps"
files = [pathlib.Path(p) for p in ("src/Processing.h", "src/Processing.cpp", "src/LibProcessingBackend.h")]
files = [p for p in files if p.exists()]
txt = {p: p.read_text() for p in files}
if any(MARK in t for t in txt.values()): print("fps already patched"); sys.exit(0)
H = pathlib.Path("src/Processing.h")
if H not in txt: print("NOT PATCHED: src/Processing.h missing (run from the CppMode folder)"); sys.exit(1)
decl = re.search(r"^([ \t]*)float\s+_frameRate\s*=\s*([^;]+);", txt[H], re.M)
if not decl: print("NOT PATCHED: float _frameRate = ...; not found in Processing.h"); sys.exit(1)
sites = sum(len(re.findall(r"[\w>.\-]*frameCount\s*\+\+|\+\+\s*[\w>.\-]*frameCount\b", t)) for t in txt.values())
if sites == 0: print("NOT PATCHED: no frameCount++ found"); sys.exit(1)
print("current uses of _frameRate (these become the target rate):")
for p, t in txt.items():
    for n, line in enumerate(t.splitlines(), 1):
        if "_frameRate" in line: print("  %s:%d: %s" % (p, n, line.strip()))
out = {}
for p, t in txt.items():
    if p == H:
        a, b = t[:decl.start()], t[decl.end():]
        t = (re.sub(r"\b_frameRate\b", "_targetFrameRate", a) + decl.group(0) +
             "  // " + MARK + ": measured, what the sketch reads as frameRate\n" +
             decl.group(1) + "float _targetFrameRate = " + decl.group(2) + ";\n" +
             re.sub(r"\b_frameRate\b", "_targetFrameRate", b))
    else:
        t = re.sub(r"\b_frameRate\b", "_targetFrameRate", t)
    def meas(m):
        pre = m.group("pre") or ""
        fr = pre + "_frameRate"; fc = pre + "frameCount"
        return (m.group(0) + "; { /* " + MARK + " */ static auto _cppT = std::chrono::steady_clock::now();"
                " auto _cppN = std::chrono::steady_clock::now();"
                " double _cppD = std::chrono::duration<double>(_cppN - _cppT).count(); _cppT = _cppN;"
                " if (" + fc + " > 1 && _cppD > 0) { float _cppR = (float)(1.0 / _cppD);"
                " " + fr + " = " + fc + " == 2 ? _cppR : " + fr + " * 0.9f + _cppR * 0.1f; } }")
    t2 = re.sub(r"(?P<pre>[\w]+(?:->|\.))?frameCount\s*\+\+(?=\s*;)", meas, t)
    t2 = re.sub(r"\+\+\s*(?P<pre>[\w]+(?:->|\.))?frameCount\b(?=\s*;)", meas, t2)
    if t2 != t and "<chrono>" not in t2:
        t2 = "#include <chrono>\n" + t2
    out[p] = t2
bak = pathlib.Path(".bak/fps"); bak.mkdir(parents=True, exist_ok=True)
for p in files:
    shutil.copy2(p, bak / p.name)
    if out[p] != txt[p]: p.write_text(out[p]); print("patched", p)
print("measured at %d frameCount++ site(s); frameRate(n) now sets _targetFrameRate" % sites)
