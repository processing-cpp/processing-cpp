import pathlib, sys, shutil
P = pathlib.Path("src/Processing.cpp"); s = P.read_text()
MARK = "cppmode-cursor-frustum"
if MARK in s: print("already patched"); sys.exit(0)
edits = [
 ("noCursor hides",
  "void PApplet::noCursor()     {if(_backend){processing_cursor(_backend->surface_id(),255);return;}",
  "void PApplet::noCursor()     {if(_backend){processing_no_cursor(_backend->surface_id());return;}/* " + MARK + " */"),
 ("cursor(type) in libprocessing",
  "void PApplet::cursor(int type){if(!gWindow)return;",
  "void PApplet::cursor(int type){\n"
  "    if(_backend){  // " + MARK + ": GLFW cursor shapes -> libprocessing's PROCESSING_CURSOR_* (0..5)\n"
  "        uint8_t k = 0;  // arrow\n"
  "        if      (type == GLFW_CROSSHAIR_CURSOR) k = 1;\n"
  "        else if (type == GLFW_HAND_CURSOR)      k = 2;\n"
  "        else if (type == GLFW_HRESIZE_CURSOR)   k = 3;  // MOVE\n"
  "        else if (type == GLFW_IBEAM_CURSOR)     k = 4;  // TEXT\n"
  "        else if (type == GLFW_VRESIZE_CURSOR)   k = 5;  // WAIT\n"
  "        processing_cursor(_backend->surface_id(), k); return;\n"
  "    }\n"
  "    if(!gWindow)return;"),
 ("frustum in libprocessing",
  "void PApplet::frustum(float l, float r, float b, float t, float n, float f) {",
  "void PApplet::frustum(float l, float r, float b, float t, float n, float f) {\n"
  "    if(_backend){  // " + MARK + ": libprocessing has no off-center frustum, so use the matching perspective\n"
  "        static bool warned = false;\n"
  "        if (!warned && (::std::fabs(l + r) > 1e-4f || ::std::fabs(b + t) > 1e-4f)) {\n"
  "            fprintf(stderr, \"frustum(): off-center frustums aren't supported in the libprocessing renderer yet, using a centered one\\n\");\n"
  "            warned = true;\n"
  "        }\n"
  "        float hh = (t - b) * 0.5f, hw = (r - l) * 0.5f;\n"
  "        if (n > 0 && hh > 0 && hw > 0)\n"
  "            processing_perspective(_backend->graphics_id(), 2.0f * ::std::atan(hh / n), hw / hh, n, f);\n"
  "        return;\n"
  "    }"),
]
bad = [name for name, old, _ in edits if s.count(old) != 1]
if bad: print("NOT PATCHED, nothing changed. Not found:", ", ".join(bad)); sys.exit(1)
for _, old, new in edits: s = s.replace(old, new, 1)
pathlib.Path(".bak/cursor").mkdir(parents=True, exist_ok=True); shutil.copy2(P, ".bak/cursor/Processing.cpp")
P.write_text(s); print("patched", P, "(noCursor, cursor(type), frustum)")
