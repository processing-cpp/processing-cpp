#!/usr/bin/env python3
"""Runs CppMode's quick test suites (a few minutes, no example run). Run it before
committing or releasing, from the CppMode folder: python3 scripts/test_all.py"""
import subprocess, sys, pathlib, time
SUITES = [
    ("translator rewrites (CppLexer)", "test_java_syntax.py"),
    ("free functions",                "test_free_api.py"),
    ("API compiles",                  "test_api.py"),
    ("runtime values match Java",     "test_runtime.py"),
]
if not pathlib.Path("src/Processing.h").exists(): sys.exit("run from the CppMode folder")
failed = []
for title, script in SUITES:
    path = pathlib.Path("scripts") / script
    if not path.exists(): print(f"skip  {title} ({script} missing)"); continue
    t = time.time()
    r = subprocess.run([sys.executable, str(path)], capture_output=True, text=True)
    last = (r.stdout.strip().splitlines() or [""])[-1]
    print(f"{'ok  ' if r.returncode == 0 else 'FAIL'}  {title:32s} {last}  ({time.time() - t:.0f}s)")
    if r.returncode:
        failed.append(title)
        for l in r.stdout.splitlines():
            if l.startswith("FAIL") or l.startswith("      ") or "error" in l: print("      " + l)
print("\nall suites pass" if not failed else f"\nfailing: {', '.join(failed)}")
sys.exit(1 if failed else 0)
