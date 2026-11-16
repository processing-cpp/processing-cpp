#!/usr/bin/env python3
"""Keep the OpenGL build compiling when Processing.cpp calls libprocessing directly.

Finds every processing_xxx( call in src/Processing.cpp and writes an empty
stand-in for each one into a marked block, used only when
CPPMODE_BACKEND_LIBPROCESSING is not defined (the calls sit behind
if (_backend), so they never run in the OpenGL build). Run before building;
build-cache.py does it automatically."""
import pathlib, re
p = pathlib.Path(__file__).resolve().parent.parent / "src" / "Processing.cpp"
s = p.read_text()
BEGIN, END = "// BEGIN CPPMODE LIBPROCESSING STUBS (generated)\n", "// END CPPMODE LIBPROCESSING STUBS\n"
anchor = "static inline void processing_image_scaled(uint64_t, uint64_t, float, float, float, float) {}\n"
s = re.sub(re.escape(BEGIN) + r".*?" + re.escape(END), "", s, flags=re.S)
manual = set(re.findall(r"static inline [\w:]+ (processing_\w+)\(", s))
used = sorted(set(re.findall(r"\b(processing_[a-z0-9_]+)\s*\(", s)) - manual)
block = BEGIN + "#ifndef CPPMODE_BACKEND_LIBPROCESSING\n" + "".join(
    f"template<class... A> inline uint64_t {n}(A&&...) {{ return 0; }}\n" for n in used) + "#endif\n" + END
if anchor not in s:
    print("stub-libprocessing: anchor line not found, nothing changed"); raise SystemExit(1)
s = s.replace(anchor, anchor + block, 1)
p.write_text(s)
print(f"stub-libprocessing: {len(used)} stand-ins for the OpenGL build")
