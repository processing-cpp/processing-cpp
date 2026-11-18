#!/usr/bin/env python3
"""Unit tests for the token-based Java syntax rewrite (CppLexer + CppJavaSyntax).
Compiles just those two classes plus the test, no Processing build needed.
Run from the CppMode folder: python3 scripts/test_java_syntax.py"""
import subprocess, sys, tempfile, pathlib
src = pathlib.Path("src/java")
files = [src / "CppLexer.java", src / "CppJavaSyntax.java", pathlib.Path("scripts/CppJavaSyntaxTest.java")]
if not all(f.exists() for f in files): sys.exit("run from the CppMode folder")
with tempfile.TemporaryDirectory() as d:
    r = subprocess.run(["javac", "-nowarn", "-d", d] + [str(f) for f in files], capture_output=True, text=True)
    if r.returncode: print(r.stderr); sys.exit(1)
    r = subprocess.run(["java", "-cp", d, "processing.mode.cpp.CppJavaSyntaxTest"], capture_output=True, text=True)
    print(r.stdout, end=""); sys.exit(0 if "all ok" in r.stdout else 1)
