import pathlib, re, sys, shutil

S = pathlib.Path("src/java/CppStrings.java"); H = pathlib.Path("src/Processing.h")
for p in (S, H):
    if not p.exists(): print("missing " + str(p) + " (run from the CppMode folder)"); sys.exit(1)
s, h = S.read_text(), H.read_text()
apply = "--apply" in sys.argv
skip = set()
if "--skip" in sys.argv:
    skip = set(sys.argv[sys.argv.index("--skip") + 1].split(","))

hc = re.sub(r"//[^\n]*", "", re.sub(r"/\*.*?\*/", "", h, flags=re.S))
m = re.search(r"\b(?:class|struct)\s+PApplet\b[^;{]*\{", hc)
if not m: print("NOT PATCHED: PApplet not found in Processing.h"); sys.exit(1)
i, depth = m.end(), 1
top = []
while i < len(hc) and depth:
    c = hc[i]
    if c == "{": depth += 1
    elif c == "}": depth -= 1
    elif depth == 1: top.append(c)
    i += 1
body = "".join(top)

KW = {"int","float","double","char","bool","long","short","void","auto","class","struct",
      "this","new","delete","return","if","else","for","while","do","switch","case","default",
      "const","static","public","private","protected","operator","template","typename"}
fields, plain = set(), set()
for stmt in body.split(";"):
    stmt = re.sub(r"\b(?:public|private|protected)\s*:", "", stmt).strip()
    if not stmt or "(" in stmt.split("=")[0]: continue
    if re.match(r"(?:using|typedef|friend|enum|template)\b", stmt): continue
    for d in re.sub(r"\{[^{}]*\}|\([^()]*\)|<[^<>;]*>", "", stmt).split(","):
        nm = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*$", d.split("=")[0].strip())
        if nm:
            (fields if nm.group(1).startswith("_") else plain).add(nm.group(1))

pairs = []
for f in sorted(fields):
    name = f.lstrip("_")
    if not re.match(r"[A-Za-z]\w*$", name) or name in KW or name in plain or name in skip: continue
    if f.startswith("__"): continue
    pairs.append((name, f))

print("will map %d names:" % len(pairs))
for a, b in pairs: print("  %-20s -> PApplet::g_papplet->%s" % (a, b))
if not apply:
    print("\ndry run. Add --apply to patch (and --skip a,b to leave names out)."); sys.exit(0)

java_pairs = "{" + ",".join('{"%s","(PApplet::g_papplet->%s)"}' % p for p in pairs) + "}"
s2, n = re.subn(r"static String\[\]\[\] EVENT_VARS = \{.*?\};(?=\s*// \{name, replacement\})",
                lambda _: "static String[][] EVENT_VARS = " + java_pairs + ";", s, count=1, flags=re.S)
if n != 1: print("NOT PATCHED: EVENT_VARS line not found (run patch_eventvars.py first)"); sys.exit(1)

if "sketchDeclares" not in s2:
    old_start = "    if (EVENT_VARS.length == 0) return src;\n"
    new_start = ("    String[][] vars = activeVars(src);\n"
                 "    if (vars.length == 0) return src;\n")
    old_loop = "  private static String replaceVars(String code) {\n    for (String[] v : EVENT_VARS)\n"
    new_loop = ("  // a sketch's own variable with the same name wins over the runtime field\n"
                "  private static boolean sketchDeclares(String src, String name) {\n"
                "    return java.util.regex.Pattern.compile(\n"
                "      \"(?<![\\\\w.>:])(?:[A-Za-z_][\\\\w:]*(?:<[^;(){}]*>)?)(?:\\\\s*[*&]+\\\\s*|\\\\s+)\" + name\n"
                "      + \"\\\\s*(?:\\\\[[^\\\\]]*\\\\]\\\\s*)*(?:=|;|,|\\\\{)\").matcher(src).find();\n"
                "  }\n\n"
                "  private static String[][] activeVars(String src) {\n"
                "    java.util.List<String[]> out = new java.util.ArrayList<>();\n"
                "    for (String[] v : EVENT_VARS)\n"
                "      if (src.contains(v[0]) && !sketchDeclares(src, v[0])) out.add(v);\n"
                "    return out.toArray(new String[0][]);\n"
                "  }\n\n"
                "  private static String replaceVars(String code, String[][] vars) {\n"
                "    for (String[] v : vars)\n")
    for a, b in ((old_start, new_start), (old_loop, new_loop)):
        if s2.count(a) != 1: print("NOT PATCHED: could not find:\n" + a); sys.exit(1)
        s2 = s2.replace(a, b)
    s2 = s2.replace("out.append(replaceVars(src.substring(seg, i)))", "out.append(replaceVars(src.substring(seg, i), vars))")
    s2 = s2.replace("out.append(replaceVars(src.substring(seg)));", "out.append(replaceVars(src.substring(seg), vars));")
    if "replaceVars(src.substring(seg), vars)" not in s2: print("NOT PATCHED: replaceVars calls not found"); sys.exit(1)

bak = pathlib.Path(".bak/underscore"); bak.mkdir(parents=True, exist_ok=True)
shutil.copy2(S, bak / S.name)
S.write_text(s2)
print("patched CppStrings.java (backup in .bak/underscore)")
