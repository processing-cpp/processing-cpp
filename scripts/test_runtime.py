#!/usr/bin/env python3
"""Builds scripts/runtime_checks.cpp against the runtime and runs it. Every CHECK in there
compares a CppMode helper's result with what Java Processing gives. No window opens.
Uses the cached Processing.o when it's there (after build-cache.py), else compiles
src/Processing.cpp, which takes a minute.
Run from the CppMode folder: python3 scripts/test_runtime.py"""
import subprocess, sys, pathlib, tempfile, os
src = pathlib.Path("src")
if not (src / "Processing.h").exists(): sys.exit("run from the CppMode folder")
cached = pathlib.Path("cache/linux-x64/Processing.o")
runtime = [str(cached)] if cached.exists() else [str(src / "Processing.cpp")]
std = "-std=c++2c" if cached.exists() else "-std=c++20"
with tempfile.TemporaryDirectory() as d:
    exe = os.path.join(d, "runtime_checks")
    cmd = ["g++", std, "-O0", "-w", "-I", str(src), "-I/usr/include/freetype2", "scripts/runtime_checks.cpp", *runtime,
           "-o", exe, "-lglfw", "-lGLEW", "-lGL", "-lfreetype", "-ldl", "-lpthread"]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        print("build failed:"); print("\n".join(l for l in r.stderr.splitlines() if "error" in l)[:3000]); sys.exit(1)
    r = subprocess.run([exe], capture_output=True, text=True)
    print(r.stdout, end=""); sys.exit(r.returncode)
