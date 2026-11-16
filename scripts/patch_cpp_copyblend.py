#!/usr/bin/env python3
"""CppMode: get(x,y), get(x,y,w,h), copy() and blend() in the libprocessing renderer
(cppmode-lpcopy). Reads the canvas back, works on the pixels on the CPU the way Java's
PImage.blend does (blendColor per pixel), and writes the result back. HiDPI-aware.
Needs patch_pg_v5 (cpp_readback_argb / cpp_update_argb). Run from the CppMode folder."""
import pathlib, sys, shutil
MARK = "cppmode-lpcopy"
PC = pathlib.Path("src/Processing.cpp"); pc = PC.read_text()
if MARK in pc: print("already patched"); sys.exit(0)
if "cpp_update_argb" not in pathlib.Path("src/LibProcessingBackend.h").read_text():
    print("NOT PATCHED: needs patch_pg_v5.py first"); sys.exit(1)
HELPER = r'''// __MARK__: canvas pixels for get()/copy()/blend() in the libprocessing renderer
#ifdef CPPMODE_BACKEND_LIBPROCESSING
static bool cppmodeLpRead(PApplet* app, ::std::vector<unsigned int>& px, int& W, int& H, float& s) {
    auto* be = static_cast<LibProcessingBackend*>(app->_backend);
    if (!be->cpp_readback_argb(be->cpp_current_gid(), app->width, app->height, px, W, H)) return false;
    s = app->width > 0 ? (float)W / (float)app->width : 1.0f;   // framebuffer pixels per sketch pixel
    return W > 0 && H > 0;
}
static void cppmodeLpWrite(PApplet* app, const ::std::vector<unsigned int>& px) {
    auto* be = static_cast<LibProcessingBackend*>(app->_backend);
    be->cpp_update_argb(be->cpp_current_gid(), px);
}
// copy/blend a region of the canvas onto itself; replace=true is copy()
static void cppmodeLpBlit(PApplet* app, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh, int mode, bool replace) {
    if (sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0) return;
    ::std::vector<unsigned int> px; int W = 0, H = 0; float s = 1;
    if (!cppmodeLpRead(app, px, W, H, s)) return;
    ::std::vector<unsigned int> out = px;
    int X0 = (int)(dx * s), Y0 = (int)(dy * s), X1 = (int)((dx + dw) * s), Y1 = (int)((dy + dh) * s);
    for (int Y = ::std::max(0, Y0); Y < ::std::min(H, Y1); Y++) {
        int srcY = (int)((sy + (Y - Y0) / s * (float)sh / dh) * s);
        if (srcY < 0 || srcY >= H) continue;
        for (int X = ::std::max(0, X0); X < ::std::min(W, X1); X++) {
            int srcX = (int)((sx + (X - X0) / s * (float)sw / dw) * s);
            if (srcX < 0 || srcX >= W) continue;
            unsigned int c = px[(size_t)srcY * W + srcX];
            out[(size_t)Y * W + X] = replace ? c : blendColor(color(px[(size_t)Y * W + X]), color(c), mode).value;
        }
    }
    cppmodeLpWrite(app, out);
}
#endif
'''.replace("__MARK__", MARK)
edits = [
 ("get(x,y)", "color PApplet::get(int x, int y) {\n    unsigned char p[4];\n",
  HELPER + "color PApplet::get(int x, int y) {\n"
  "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n"
  "    if (_backend) {  // " + MARK + "\n"
  "        ::std::vector<unsigned int> px; int W = 0, H = 0; float s = 1;\n"
  "        if (x < 0 || y < 0 || x >= width || y >= height || !cppmodeLpRead(this, px, W, H, s)) return color(0u);\n"
  "        int X = ::std::min(W - 1, (int)(x * s)), Y = ::std::min(H - 1, (int)(y * s));\n"
  "        return color(px[(size_t)Y * W + X]);\n"
  "    }\n"
  "#endif\n"
  "    unsigned char p[4];\n"),
 ("blend()", "void PApplet::blend(int sx,int sy,int sw,int sh,int dx,int dy,int dw,int dh,int mode){\n",
  "void PApplet::blend(int sx,int sy,int sw,int sh,int dx,int dy,int dw,int dh,int mode){\n"
  "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n"
  "    if (_backend) { cppmodeLpBlit(this, sx, sy, sw, sh, dx, dy, dw, dh, mode, false); return; }  // " + MARK + "\n"
  "#endif\n"),
 ("copy()", "void PApplet::copy(int sx,int sy,int sw,int sh,int dx,int dy,int dw,int dh){\n",
  "void PApplet::copy(int sx,int sy,int sw,int sh,int dx,int dy,int dw,int dh){\n"
  "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n"
  "    if (_backend) { cppmodeLpBlit(this, sx, sy, sw, sh, dx, dy, dw, dh, BLEND, true); return; }  // " + MARK + ": copy replaces, like Java\n"
  "#endif\n"),
 ("get(x,y,w,h)", "PImage getRegion(int x,int y,int w,int h){\n",
  "PImage getRegion(int x,int y,int w,int h){\n"
  "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n"
  "    if (PApplet::g_papplet && PApplet::g_papplet->_backend) {  // " + MARK + "\n"
  "        PImage img(w > 0 ? w : 1, h > 0 ? h : 1);\n"
  "        ::std::vector<unsigned int> px; int W = 0, H = 0; float s = 1;\n"
  "        if (w <= 0 || h <= 0 || !cppmodeLpRead(PApplet::g_papplet, px, W, H, s)) return img;\n"
  "        for (int iy = 0; iy < h; iy++) for (int ix = 0; ix < w; ix++) {\n"
  "            int X = (int)((x + ix) * s), Y = (int)((y + iy) * s);\n"
  "            img.pixels[(size_t)iy * w + ix] = (X >= 0 && Y >= 0 && X < W && Y < H) ? px[(size_t)Y * W + X] : 0u;\n"
  "        }\n"
  "        img.dirty = true;\n"
  "        return img;\n"
  "    }\n"
  "#endif\n"),
]
bad = [n for n, a, _ in edits if pc.count(a) != 1]
if bad: print("NOT PATCHED, nothing changed. Not found:", ", ".join(bad)); sys.exit(1)
for _, a, b in edits: pc = pc.replace(a, b, 1)
pathlib.Path(".bak/lpcopy").mkdir(parents=True, exist_ok=True); shutil.copy2(PC, ".bak/lpcopy/Processing.cpp")
PC.write_text(pc); print("patched", PC, "(get, get region, copy, blend)")
