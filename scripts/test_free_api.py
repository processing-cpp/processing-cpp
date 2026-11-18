#!/usr/bin/env python3
"""Syntax-checks raw C++ that calls Processing's free functions (outside a sketch class).
Generated sketches call the PApplet members, so a broken free overload (like the stray
template header that made size(int, int) uncallable) only shows up here.
All cases go in one file, so the header is parsed once.
Run from the CppMode folder: python3 scripts/test_free_api.py"""
import subprocess, sys, tempfile, pathlib, os
CASES = {
    "size(w, h)":             "void setup(){ size(200,200); } void draw(){ background(0); }",
    "size(w, h, mode)":       "void setup(){ size(200,200,P2D); } void draw(){}",
    "fullScreen()":           "void setup(){ fullScreen(); } void draw(){}",
    "fullScreen(mode)":       "void setup(){ fullScreen(P2D); } void draw(){}",
    "fullScreen(mode, disp)": "void setup(){ fullScreen(P2D, 2); } void draw(){}",
    "fullScreen(SPAN)":       "void setup(){ fullScreen(SPAN); } void draw(){}",
    "line(mixed)":            "void draw(){ line(0, 1.5f, 2.0, 3); }",
}
if not pathlib.Path("src/Processing.h").exists(): sys.exit("run from the CppMode folder")
lines = ['#include "Processing.h"']
ranges = []
for i, (name, body) in enumerate(CASES.items()):
    start = len(lines) + 1
    lines += [f"namespace free_case{i} {{", "using namespace Processing;", body, "}"]
    ranges.append((name, start, len(lines)))
with tempfile.NamedTemporaryFile("w", suffix=".cpp", delete=False) as f:
    f.write("\n".join(lines) + "\n")
try:
    r = subprocess.run(["g++", "-std=c++17", "-fsyntax-only", "-w", "-fmax-errors=100", "-I", "src",
                        "-I/usr/include/freetype2", f.name], capture_output=True, text=True)
finally:
    os.unlink(f.name)
errors, other = {}, []
for l in r.stderr.splitlines():
    if "error: " not in l: continue
    parts = l.split(":")
    line = int(parts[1]) if len(parts) > 2 and parts[0] == f.name and parts[1].isdigit() else None
    owner = next((n for n, a, b in ranges if line is not None and a <= line <= b), None)
    if owner: errors.setdefault(owner, []).append(l.split("error: ", 1)[1])
    else: other.append(l)
bad = 0
for name, _, _ in ranges:
    ok = name not in errors and not (r.returncode and not errors and other)
    print(("ok    " if ok else "FAIL  ") + name + ("" if ok or name not in errors else "\n      " + errors[name][0][:180]))
    bad += not ok
for l in other[:5]: print("      " + l[:200])
print(f"{len(ranges) - bad}/{len(ranges)} free functions compile")
sys.exit(1 if bad else 0)
