#!/usr/bin/env python3
"""Port PR #8 (rebuild-jar.py Windows support) into the local rebuild-jar.py,
keeping everything local has added since. Run from the CppMode folder."""
import pathlib, re, sys, shutil
P = pathlib.Path("scripts/rebuild-jar.py"); s = P.read_text(); orig = s
if "gradlew.bat" in s: print("already has the Windows changes"); sys.exit(0)
done, missing = [], []

def sub(name, pattern, repl, flags=0):
    global s
    s2, n = re.subn(pattern, repl, s, count=1, flags=flags)
    (done if n else missing).append(name); s = s2

head = s.split("\ndef ", 1)[0]   # only imports before the first function count
for mod in ("platform", "tempfile", "shutil"):
    if not re.search(r"^import %s\b|^import .*\b%s\b" % (mod, mod), head, re.M):
        s = re.sub(r"^(import os\b.*)$", r"\1\nimport %s" % mod, s, count=1, flags=re.M)
sub("TMP_DIR", r'^TMP_DIR\s*=\s*"/tmp/_cpp"\s*$', 'TMP_DIR = os.path.join(tempfile.gettempdir(), "_cpp")', re.M)
sub("gradlew", r'^(\s*)gradlew\s*=\s*os\.path\.join\((\w+),\s*"gradlew"\)\s*$',
    r'\1gradlew = os.path.join(\2, "gradlew.bat" if platform.system() == "Windows" else "gradlew")', re.M)
sub("rm temp", r'^(\s*)run\(\["rm",\s*"-rf",\s*TMP_DIR\]\)\s*$', r'\1print(f"$ remove {TMP_DIR}"); shutil.rmtree(TMP_DIR, ignore_errors=True)', re.M)
sub("mkdir temp", r'^(\s*)run\(\["mkdir",\s*TMP_DIR\]\)\s*$', r'\1print(f"$ mkdir {TMP_DIR}"); os.makedirs(TMP_DIR, exist_ok=True)', re.M)
n = len(re.findall(r'"/home/pep/', s))
s = re.sub(r'f?"/home/pep/', 'os.path.expanduser("~") + f"/', s)
(done if n else missing).append("/home/pep paths (%d)" % n)

print("ported:", ", ".join(done) or "nothing")
if missing: print("not found (check by hand):", ", ".join(missing))
if s == orig: print("nothing changed"); sys.exit(1)
try: compile(s, str(P), "exec")
except SyntaxError as e: print("NOT WRITTEN, result has a syntax error:", e); sys.exit(1)
pathlib.Path(".bak/pr8").mkdir(parents=True, exist_ok=True); shutil.copy2(P, ".bak/pr8/rebuild-jar.py")
P.write_text(s); print("patched", P, "(backup in .bak/pr8)")
