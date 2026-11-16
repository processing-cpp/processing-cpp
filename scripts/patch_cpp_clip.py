#!/usr/bin/env python3
"""CppMode: clip()/noClip() in the libprocessing renderer (needs lp-fork with patch_lp_clip.py).
Run from the CppMode folder. Changes nothing unless both functions are found."""
import pathlib, re, sys, shutil
MARK = "cppmode-clip"
P = pathlib.Path("src/Processing.cpp"); s = P.read_text()
if MARK in s: print("already patched"); sys.exit(0)
clip_re = re.compile(r"void PApplet::clip\(float x, float y, float w, float h\)\s*\{")
noclip_re = re.compile(r"void PApplet::noClip\(\)\s*\{")
mc, mn = clip_re.findall(s), noclip_re.findall(s)
if len(mc) != 1 or len(mn) != 1:
    print("NOT PATCHED, nothing changed: clip() found %d times, noClip() %d times" % (len(mc), len(mn))); sys.exit(1)
clip_body = r'''
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // __MARK__: the shader drops pixels outside this rect
        float x1 = x, y1 = y, x2 = x + w, y2 = y + h;           // CORNER
        if (currentImageMode == CORNERS)     { x2 = w; y2 = h; }
        else if (currentImageMode == CENTER) { x1 = x - w / 2; y1 = y - h / 2; x2 = x + w / 2; y2 = y + h / 2; }
        else if (currentImageMode == RADIUS) { x1 = x - w; y1 = y - h; x2 = x + w; y2 = y + h; }
        if (x2 < x1) ::std::swap(x1, x2);
        if (y2 < y1) ::std::swap(y1, y2);
        // the window canvas is in framebuffer pixels (HiDPI), a createGraphics buffer is 1:1
        float s = 1.0f;
        auto* lb = static_cast<LibProcessingBackend*>(_backend);
        if (!lb->_cpp_saved_gid && gWindow) {
            int fw = 0, fh = 0, ww = 0, wh = 0;
            glfwGetFramebufferSize(gWindow, &fw, &fh); glfwGetWindowSize(gWindow, &ww, &wh);
            if (ww > 0) s = (float)fw / (float)ww;
        }
        processing_clip(_backend->graphics_id(), x1 * s, y1 * s, (x2 - x1) * s, (y2 - y1) * s);
        return;
    }
#endif'''.replace("__MARK__", MARK)
noclip_body = "\n    if (_backend) { processing_no_clip(_backend->graphics_id()); return; }  /* " + MARK + " */"
s = clip_re.sub(lambda m: m.group(0) + clip_body, s, count=1)
s = noclip_re.sub(lambda m: m.group(0) + noclip_body, s, count=1)
pathlib.Path(".bak/clip").mkdir(parents=True, exist_ok=True); shutil.copy2(P, ".bak/clip/Processing.cpp")
P.write_text(s); print("patched", P, "(clip, noClip)")
