#!/usr/bin/env python3
"""Issue #6: on Windows, run g++ with its own bin folder first on PATH, so cc1plus
loads its own DLLs instead of another mingw's (chocolatey etc.) and doesn't die silently.
Run from the CppMode folder. Changes nothing unless the anchors match."""
import pathlib, sys, shutil
MARK = "cppmode-issue6"
J = pathlib.Path("src/java")
build = J / "CppBuild.java"
files = [build, J / "CppLinter.java", J / "CppMode.java"]
for f in files:
    if not f.exists(): print("missing", f); sys.exit(1)
if MARK in build.read_text(): print("already patched"); sys.exit(0)

HELPER = r'''
  // __MARK__: on Windows, put the compiler's own bin folder first on PATH. Its helpers
  // (cc1plus, as, ld) sit in folders without DLLs, so without this Windows loads the
  // DLLs of whatever other mingw comes first on PATH, and the build dies with no output.
  public static ProcessBuilder toolchainEnv(ProcessBuilder pb) {
    try {
      if (!System.getProperty("os.name").toLowerCase().contains("win")) return pb;
      java.util.List<String> c = pb.command();
      if (c.isEmpty()) return pb;
      File exe = new File(c.get(0));
      String n = exe.getName().toLowerCase();
      if (!exe.isAbsolute() || exe.getParentFile() == null) return pb;
      if (!(n.contains("g++") || n.contains("gcc") || n.equals("c++.exe") || n.equals("cpp.exe"))) return pb;
      java.util.Map<String, String> env = pb.environment();   // case-insensitive on Windows
      String bin = exe.getParentFile().getAbsolutePath();
      String old = env.getOrDefault("PATH", "");
      if (!old.toLowerCase().startsWith(bin.toLowerCase() + ";"))
        env.put("PATH", bin + (old.isEmpty() ? "" : ";" + old));
    } catch (Exception ignored) {}
    return pb;
  }
'''.replace("__MARK__", MARK)

def wrap_all(src, prefix):
    """Wrap every `new ProcessBuilder(...)` (balanced parens, any line breaks) once."""
    out, i, n = [], 0, 0
    key = "new ProcessBuilder("
    while True:
        j = src.find(key, i)
        if j < 0: out.append(src[i:]); break
        if src[max(0, j - len(prefix) - 20):j].rstrip().endswith("toolchainEnv("):
            out.append(src[i:j + len(key)]); i = j + len(key); continue
        k, depth = j + len(key), 1
        while k < len(src) and depth:
            depth += {"(": 1, ")": -1}.get(src[k], 0); k += 1
        out.append(src[i:j]); out.append(prefix + "toolchainEnv(" + src[j:k] + ")")
        i = k; n += 1
    return "".join(out), n

new, report = {}, []
b = build.read_text()
cls = b.find("public class CppBuild")
brace = b.find("{", cls) if cls >= 0 else -1
if brace < 0: print("NOT PATCHED: class CppBuild not found"); sys.exit(1)
b2, nb = wrap_all(b, "")
b2 = b2[:brace + 1] + HELPER + b2[brace + 1:] if b2.find("{", cls) == brace else None
if b2 is None: print("NOT PATCHED: could not place the helper"); sys.exit(1)
new[build] = b2; report.append("CppBuild.java: %d" % nb)
for f in files[1:]:
    t = f.read_text(); t2, n = wrap_all(t, "CppBuild.")
    new[f] = t2; report.append("%s: %d" % (f.name, n))
print("wrapped ProcessBuilders ->", ", ".join(report))
bak = pathlib.Path(".bak/issue6"); bak.mkdir(parents=True, exist_ok=True)
for f, t in new.items():
    if t != f.read_text(): shutil.copy2(f, bak / f.name); f.write_text(t); print("patched", f)
