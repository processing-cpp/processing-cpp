#!/usr/bin/env python3
"""CppMode libprocessing renderer, on top of patch_pg_v4:
  - pg.beginDraw()/endDraw() swap PApplet's style the same way the OpenGL path does
  - pg.save(file) reads pg's own canvas back
  - filter(mode[, param]) = readback + the existing PImage CPU filter + write back
Run from the CppMode folder:  python3 scripts/patch_pg_v5.py [path-to-lp-fork]
Changes nothing unless every anchor matches. Backups in .bak/pg5/.
"""
import pathlib, re, sys, shutil

MARK = "cppmode-pg-v5"
LP = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else pathlib.Path.home() / "lp-fork").expanduser()
FFI = LP / "crates/processing_ffi/src/lib.rs"
BE, PH, PC = pathlib.Path("src/LibProcessingBackend.h"), pathlib.Path("src/Processing.h"), pathlib.Path("src/Processing.cpp")
for p in (FFI, BE, PH, PC):
    if not p.exists(): print("missing", p); sys.exit(1)
ffi, be, ph, pc = (p.read_text() for p in (FFI, BE, PH, PC))
if MARK in pc: print("already patched"); sys.exit(0)
if "cppmode-pg-offscreen" not in pc: print("NOT PATCHED: run patch_pg_v4.py first"); sys.exit(1)
problems = []

m = re.search(r"fn processing_graphics_update\s*\((.*?)\)", ffi, re.S)
if not m or not re.search(r"u64\s*,.*\*const\s+Color\s*,.*usize", " ".join(m.group(1).split())):
    problems.append("processing_graphics_update signature is not (u64, *const Color, usize): " + (" ".join(m.group(1).split()) if m else "not found"))

be_ext_anchor = '    __asm__("processing_graphics_readback");'
be_ext_new = be_ext_anchor + '\nextern "C" void cppmode_lp_graphics_update(uint64_t graphics_id, const CppLpColor* buffer, size_t buffer_len)\n    __asm__("processing_graphics_update");'
be_meth_anchor = "    // saveFrame(): read the window canvas back as 8-bit RGB, top row first\n"
be_meth_new = r'''    // __MARK__: read any canvas back as ARGB (Processing's pixel layout), top row first
    bool cpp_readback_argb(uint64_t gid, int w, int h, std::vector<unsigned int>& out, int& ow, int& oh) {
        processing_flush(gid);
        size_t n = (size_t)w * (size_t)h;
        std::vector<CppLpColor> buf(n);
        cppmode_lp_graphics_readback(gid, buf.data(), n);
        if (const char* e = processing_check_error()) {
            const char* p = strstr(e, "expected ");
            size_t m = p ? (size_t)strtoull(p + 9, nullptr, 10) : 0;
            if (!m) { fprintf(stderr, "[libprocessing] readback: %s\n", e); return false; }
            double s = std::sqrt((double)m / (double)n);
            w = (int)std::lround(w * s); h = w > 0 ? (int)(m / (size_t)w) : 0;
            n = (size_t)w * (size_t)h;
            if (n != m) { fprintf(stderr, "[libprocessing] readback: odd canvas size %zu\n", m); return false; }
            buf.assign(n, CppLpColor{});
            cppmode_lp_graphics_readback(gid, buf.data(), n);
            if (const char* e2 = processing_check_error()) { fprintf(stderr, "[libprocessing] readback: %s\n", e2); return false; }
        }
        auto to8 = [](float v) { v = v < 0 ? 0 : (v > 1 ? 1 : v); return (unsigned int)std::lround(v * 255.0f); };
        out.resize(n);
        for (size_t i = 0; i < n; i++)
            out[i] = (to8(buf[i].a) << 24) | (to8(buf[i].c1) << 16) | (to8(buf[i].c2) << 8) | to8(buf[i].c3);
        ow = w; oh = h;
        return true;
    }
    // __MARK__: write ARGB pixels back onto a canvas (same size readback gave)
    bool cpp_update_argb(uint64_t gid, const std::vector<unsigned int>& px) {
        std::vector<CppLpColor> buf(px.size());
        for (size_t i = 0; i < px.size(); i++) {
            unsigned int c = px[i];
            auto lc = lp_rgba(((c >> 16) & 255) / 255.0f, ((c >> 8) & 255) / 255.0f, (c & 255) / 255.0f, ((c >> 24) & 255) / 255.0f);
            static_assert(sizeof(lc) == sizeof(CppLpColor), "Color layout changed");
            std::memcpy(&buf[i], &lc, sizeof lc);
        }
        cppmode_lp_graphics_update(gid, buf.data(), buf.size());
        if (const char* e = processing_check_error()) { fprintf(stderr, "[libprocessing] update: %s\n", e); return false; }
        return true;
    }
    uint64_t cpp_current_gid() const { return _graphics_id; }
'''.replace("__MARK__", MARK)
for a in (be_ext_anchor, be_meth_anchor):
    if be.count(a) != 1: problems.append("backend anchor: " + a.strip())
be_new = be.replace(be_ext_anchor, be_ext_new, 1).replace(be_meth_anchor, be_meth_new + be_meth_anchor, 1)

ph_anchor = "    void endDraw() { if (cppmodeLpEndDraw(this)) return; _endDrawImpl(); }"
ph_new = ph_anchor + "\n    void save(const ::std::string& filename);  // " + MARK + ": writes this buffer to an image file"
if ph.count(ph_anchor) != 1: problems.append("Processing.h anchor: endDraw hook from v4")
if re.search(r"class PGraphics[^{]*\{(?:(?!\n\};).)*\bvoid save\(", ph, re.S): ph_new = ph_anchor
ph_out = ph.replace(ph_anchor, ph_new, 1)

old_begin = '''bool cppmodeLpBeginDraw(PGraphics* pg) {
    PApplet* app = PApplet::g_papplet;
    if (!pg || !app || !app->_backend) return false;
    return static_cast<LibProcessingBackend*>(app->_backend)->cpp_pg_begin(pg->width, pg->height, pg->lp_surface, pg->lp_graphics);
}'''
old_end = '''    static_cast<LibProcessingBackend*>(app->_backend)->cpp_pg_end(pg->lp_graphics);
    pg->lp_image_id = pg->lp_surface;   // the offscreen surface is an image, image(pg) draws it'''
style_fields = ("fillR fillG fillB fillA strokeR strokeG strokeB strokeA strokeW doFill doStroke smoothing "
                "currentRectMode currentEllipseMode currentImageMode tintR tintG tintB tintA doTint colorModeVal "
                "colorMaxH colorMaxS colorMaxB colorMaxA g_textSize g_textAlignX g_textAlignY g_textLeading").split()
style_macro = "#define CPPMODE_LP_STYLE_FIELDS(X) " + " ".join("X(%s)" % f for f in style_fields)
new_begin = style_macro + r'''
// __MARK__: same style swap the OpenGL beginDraw()/endDraw() do, so pg keeps its own
// rectMode, textSize, tint and so on, and the main canvas gets its own back after
template <class S> static void cppmodeLpStyleSave(S& s, PApplet* p) {
#define X(f) s.f = p->f;
    CPPMODE_LP_STYLE_FIELDS(X)
#undef X
}
template <class S> static void cppmodeLpStyleLoad(const S& s, PApplet* p) {
#define X(f) p->f = s.f;
    CPPMODE_LP_STYLE_FIELDS(X)
#undef X
}
bool cppmodeLpBeginDraw(PGraphics* pg) {
    PApplet* app = PApplet::g_papplet;
    if (!pg || !app || !app->_backend) return false;
    if (!static_cast<LibProcessingBackend*>(app->_backend)->cpp_pg_begin(pg->width, pg->height, pg->lp_surface, pg->lp_graphics)) return true;
    cppmodeLpStyleSave(pg->_savedMainStyle, app);
    cppmodeLpStyleLoad(pg->myStyle, app);
    return true;
}'''.replace("__MARK__", MARK)
new_end = '''    static_cast<LibProcessingBackend*>(app->_backend)->cpp_pg_end(pg->lp_graphics);
    cppmodeLpStyleSave(pg->myStyle, app);          // remember pg's style for next time
    cppmodeLpStyleLoad(pg->_savedMainStyle, app);  // and give the main canvas its own back
    pg->lp_image_id = pg->lp_surface;   // the offscreen surface is an image, image(pg) draws it'''
helpers_anchor = "#else\nbool cppmodeLpBeginDraw(PGraphics*) { return false; }"
PGSAVE = r'''void PGraphics::save(const ::std::string& filename) {
    PApplet* app = PApplet::g_papplet;
    if (app && app->_backend) {
        if (!lp_graphics) { fprintf(stderr, "pg.save(): draw into it with beginDraw()/endDraw() first\n"); return; }
        auto* be = static_cast<LibProcessingBackend*>(app->_backend);
        ::std::vector<unsigned int> px; int w = 0, h = 0;
        if (be->cpp_readback_argb(lp_graphics, width, height, px, w, h)) cppmodeLpWriteArgb(filename, px, w, h);
        return;
    }
    if (pixels.size() == (size_t)width * height) cppmodeLpWriteArgb(filename, pixels, width, height);
}
'''
helpers_new = r'''// __MARK__: filter() and pg.save() for the libprocessing renderer
static void cppmodeLpFilter(PApplet* app, int mode, float param) {
    auto* be = static_cast<LibProcessingBackend*>(app->_backend);
    PImage img; int w = 0, h = 0;
    if (!be->cpp_readback_argb(be->cpp_current_gid(), app->width, app->height, img.pixels, w, h)) return;
    img.width = w; img.height = h;
    img.filter(mode, param);
    be->cpp_update_argb(be->cpp_current_gid(), img.pixels);
}
static bool cppmodeLpWriteArgb(const ::std::string& fn0, const ::std::vector<unsigned int>& px, int w, int h) {
    ::std::string fn = fn0;
    if (!fn.empty() && fn[0] != '/') {
        const char* sp = getenv("PROCESSING_SKETCH_PATH");
        if (sp && sp[0]) fn = ::std::string(sp) + "/" + fn;
    }
    ::std::vector<unsigned char> rgb((size_t)w * h * 3);
    for (size_t i = 0; i < (size_t)w * h; i++) {
        rgb[i*3+0] = (px[i] >> 16) & 255; rgb[i*3+1] = (px[i] >> 8) & 255; rgb[i*3+2] = px[i] & 255;
    }
    ::std::string ext = fn.size() >= 4 ? fn.substr(fn.size() - 4) : "";
    for (auto& c : ext) c = (char)tolower((unsigned char)c);
    int ok = 0;
    if (ext == ".jpg" || ext == "jpeg")      ok = stbi_write_jpg(fn.c_str(), w, h, 3, rgb.data(), 95);
    else if (ext == ".bmp")                  ok = stbi_write_bmp(fn.c_str(), w, h, 3, rgb.data());
    else if (ext == ".tga" || ext == ".tif") ok = stbi_write_tga(fn.c_str(), w, h, 3, rgb.data());
    else { if (ext != ".png") fn += ".png"; ok = stbi_write_png(fn.c_str(), w, h, 3, rgb.data(), w * 3); }
    if (!ok) fprintf(stderr, "save: could not write %s\n", fn.c_str());
    return ok != 0;
}
__PGSAVE__'''.replace("__MARK__", MARK).replace("__PGSAVE__", PGSAVE if ph_new != ph_anchor else "") + helpers_anchor
gl_save = "\nvoid PGraphics::save(const ::std::string& filename) {\n    fprintf(stderr, \"pg.save() works in the libprocessing renderer for now\\n\"); (void)filename;\n}"
gl_anchor = "bool cppmodeLpEndDraw(PGraphics*) { return false; }\n#endif"
filt_anchor = "void PApplet::filter(int mode, float param) {"
filt_new = filt_anchor + "\n#ifdef CPPMODE_BACKEND_LIBPROCESSING\n    if (_backend) { cppmodeLpFilter(this, mode, param); return; }  // " + MARK + "\n#endif"
for a in (old_begin, old_end, helpers_anchor, gl_anchor, filt_anchor):
    if pc.count(a) != 1: problems.append("Processing.cpp anchor: " + a.splitlines()[0])
pc_new = (pc.replace(old_begin, new_begin, 1).replace(old_end, new_end, 1)
            .replace(helpers_anchor, helpers_new, 1).replace(filt_anchor, filt_new, 1))
if ph_new != ph_anchor:
    pc_new = pc_new.replace(gl_anchor, gl_anchor.replace("\n#endif", gl_save + "\n#endif"), 1)

if problems:
    print("NOT PATCHED, nothing changed:"); [print("  -", x) for x in problems]; sys.exit(1)
bak = pathlib.Path(".bak/pg5"); bak.mkdir(parents=True, exist_ok=True)
for p, old, new in ((BE, be, be_new), (PH, ph, ph_out), (PC, pc, pc_new)):
    if old != new: shutil.copy2(p, bak / p.name); p.write_text(new); print("patched", p)
