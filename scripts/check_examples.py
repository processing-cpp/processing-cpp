#!/usr/bin/env python3
"""CppMode: compile-check every example in both renderers.

For each sketch folder under examples/: joins its tabs (main tab first, then
the rest alphabetically, like Processing), runs CppCLI to get the C++, then
g++ -fsyntax-only twice: OpenGL build and libprocessing build. Nothing is
linked or run. Writes a report to ~/cppmode-examples.txt.

Run from the CppMode folder:  python3 scripts/check_examples.py [filter]
An optional filter only checks example paths containing that text."""
import os, re, subprocess, sys, tempfile, collections, concurrent.futures as cf
from pathlib import Path

MODE = Path.cwd()
if not (MODE / "mode" / "CppMode.jar").exists():
    sys.exit("run this from the CppMode folder")
HOME = Path.home()
LP_INC = HOME / "lp-fork/crates/processing_ffi/include"
P4 = HOME / "Projects/processing4"
jars = [str(MODE / "mode/CppMode.jar")]
if P4.exists():
    jars += [str(j) for j in P4.glob("*/build/libs/*.jar")] + [str(j) for j in P4.glob("app/build/**/core*.jar")]
CP = os.pathsep.join(jars)
CLI = ["java", "-cp", CP, "processing.mode.cpp.CppCLI", "--mode-folder", str(MODE),
       "--native-lib", str(MODE / "libs/linux-x64/libcppmode-ts.so")]
STD = "-std=c++2c"
BASE = ["g++", STD, "-fsyntax-only", "-w", "-x", "c++", "-I", str(MODE / "src")]
if (MODE / "libs/include").exists(): BASE += ["-I", str(MODE / "libs/include")]
FLT = sys.argv[1] if len(sys.argv) > 1 else ""

def sketches():
    for d in sorted({p.parent for p in (MODE / "examples").rglob("*.pde")}):
        if FLT and FLT not in str(d): continue
        main = d / (d.name + ".pde")
        tabs = ([main] if main.exists() else []) + sorted(p for p in d.glob("*.pde") if p != main)
        yield d, tabs

def first_error(text):
    for line in text.splitlines():
        if " error: " in line or line.startswith("Error"):
            return re.sub(r"^.*?error: ", "", line).strip()[:160]
    return text.strip().splitlines()[0][:160] if text.strip() else "unknown"

def check(item):
    d, tabs = item
    rel = str(d.relative_to(MODE / "examples"))
    src = "\n".join(t.read_text(errors="replace") for t in tabs)
    gen = subprocess.run(CLI, input=src, capture_output=True, text=True)
    if gen.returncode != 0 or not gen.stdout.strip():
        return rel, "translate", first_error(gen.stderr), None
    with tempfile.NamedTemporaryFile("w", suffix=".cpp", delete=False) as f:
        f.write(gen.stdout); cpp = f.name
    res = {}
    for name, extra in (("opengl", []), ("libprocessing", ["-DCPPMODE_BACKEND_LIBPROCESSING", "-I", str(LP_INC)])):
        r = subprocess.run(BASE + extra + [cpp], capture_output=True, text=True)
        res[name] = None if r.returncode == 0 else first_error(r.stderr)
    os.unlink(cpp)
    return rel, "compiled", res["opengl"], res["libprocessing"]

items = list(sketches())
print(f"checking {len(items)} examples (this takes a few minutes)...", flush=True)
rows = []
with cf.ThreadPoolExecutor(max_workers=2) as ex:
    for i, row in enumerate(ex.map(check, items), 1):
        rows.append(row)
        if i % 20 == 0: print(f"  {i}/{len(items)}", flush=True)

trans = [r for r in rows if r[1] == "translate"]
gl = [r for r in rows if r[1] == "compiled" and r[2]]
lp = [r for r in rows if r[1] == "compiled" and r[3]]
ok = [r for r in rows if r[1] == "compiled" and not r[2] and not r[3]]
out = [f"CppMode examples: {len(rows)} checked, {len(ok)} compile in both renderers",
       f"  translation failed: {len(trans)}   OpenGL errors: {len(gl)}   libprocessing errors: {len(lp)}", ""]
common = collections.Counter(re.sub(r"[‘'][^’']*[’']", "'…'", e) for r in rows for e in (r[2], r[3]) if e)
out.append("Most common errors (names replaced by '…'):")
out += [f"  {n:3d}x  {e}" for e, n in common.most_common(15)] + [""]
for title, lst, idx in (("Translation failed (CppCLI)", trans, 2), ("OpenGL build errors", gl, 2), ("libprocessing build errors", lp, 3)):
    out.append(title + ":")
    out += [f"  {r[0]}\n      {r[idx]}" for r in lst] or ["  (none)"]
    out.append("")
report = "\n".join(out)
(HOME / "cppmode-examples.txt").write_text(report)
print(report.split("\n\nTranslation")[0])
print(f"\nfull report: {HOME / 'cppmode-examples.txt'}")
