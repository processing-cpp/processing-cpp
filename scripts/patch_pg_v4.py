#!/usr/bin/env python3
"""CppMode + lp-fork: createGraphics() and saveFrame() for the libprocessing renderer.

Run from the CppMode folder:  python3 scripts/patch_pg_saveframe.py [path-to-lp-fork]
Changes nothing unless every anchor matches. Backups go to .bak/pg/.
"""
import pathlib, re, sys, shutil

MARK = "cppmode-pg-offscreen"
LP = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else pathlib.Path.home() / "lp-fork").expanduser()
FFI = LP / "crates/processing_ffi/src/lib.rs"
REN = LP / "crates/processing_render/src/lib.rs"
BE = pathlib.Path("src/LibProcessingBackend.h")
PH = pathlib.Path("src/Processing.h")
PC = pathlib.Path("src/Processing.cpp")
for p in (FFI, REN, BE, PH, PC):
    if not p.exists(): print("missing", p); sys.exit(1)
ffi, ren, be, ph, pc = (p.read_text() for p in (FFI, REN, BE, PH, PC))
problems = []

# ---------------------------------------------------------------- Rust side
if "fn processing_surface_create_offscreen" in ffi:
    ffi_new = ffi
else:
    m = re.search(r"pub fn surface_create_offscreen\s*\((.*?)\)\s*->", ren, re.S)
    if not m: problems.append("surface_create_offscreen signature in processing_render")
    else:
        args = []
        for prm in [x.strip() for x in m.group(1).split(",") if x.strip()]:
            name, _, ty = prm.partition(":"); name, ty = name.strip(), ty.strip()
            if name in ("width", "height", "scale_factor"): args.append(name)
            elif "TextureFormat" in ty: args.append("TextureFormat::Rgba16Float")
            elif "Extent3d" in ty: args.append("Extent3d { width, height, depth_or_array_layers: 1 }")
            elif ty == "bool": args.append("false")
            else: problems.append("unknown offscreen param: " + prm)
        rust_fn = (
            "/// Create an offscreen surface (no window). CppMode uses it for createGraphics().\n"
            "/// " + MARK + "\n"
            "///\n/// SAFETY:\n/// - Init has been called.\n/// - This is called from the same thread as init.\n"
            "#[unsafe(no_mangle)]\n"
            "pub extern \"C\" fn processing_surface_create_offscreen(width: u32, height: u32, scale_factor: f32) -> u64 {\n"
            "    error::clear_error();\n"
            "    error::check(|| surface_create_offscreen(" + ", ".join(args) + "))\n"
            "        .map(|e| e.to_bits())\n"
            "        .unwrap_or(0)\n"
            "}\n\n")
        anchor = "/// Destroy a graphics context."
        if ffi.count(anchor) != 1: problems.append("ffi anchor: " + anchor)
        ffi_new = ffi.replace(anchor, rust_fn + anchor, 1)

# ---------------------------------------------------------------- backend
# Readback is reached through a private alias of the same C symbol, so it never
# clashes with (or depends on) whatever header declares the libprocessing API.
externs = ["// " + MARK + ": entry points the bundled header may not declare yet",
           "extern \"C\" uint64_t processing_surface_create_offscreen(uint32_t width, uint32_t height, float scale_factor);",
           "struct CppLpColor { float c1, c2, c3, a; uint8_t space; };  // same layout as libprocessing's Color",
           "extern \"C\" void cppmode_lp_graphics_readback(uint64_t graphics_id, CppLpColor* buffer, size_t buffer_len)",
           "    __asm__(\"processing_graphics_readback\");"]

be_class = "class LibProcessingBackend final : public ProcessingBackend {"
be_check = "    static void check(const char* where) {"
be_font = "if (font) processing_text_font(_graphics_id, font);"
be_methods = r'''public:
    // ---- __MARK__: createGraphics() and saveFrame() ----
    uint64_t _cpp_default_font = 0;
    uint64_t _cpp_saved_gid    = 0;
    // pg.beginDraw(): make the offscreen canvas on first use, then send all drawing to it
    bool cpp_pg_begin(int w, int h, uint64_t& surf, uint64_t& gid) {
        if (w <= 0 || h <= 0) return false;
        if (!gid) {
            surf = processing_surface_create_offscreen((uint32_t)w, (uint32_t)h, 1.0f);
            check("processing_surface_create_offscreen");
            gid = processing_graphics_create(surf, (uint32_t)w, (uint32_t)h);
            check("processing_graphics_create (offscreen)");
            processing_color_mode(gid, 0, 1.0f, 1.0f, 1.0f, 1.0f);
            if (_cpp_default_font) processing_text_font(gid, _cpp_default_font);
            processing_text_size(gid, 12.0f);
        }
        _cpp_saved_gid = _graphics_id;
        _graphics_id = gid;
        processing_begin_draw(gid); check("begin_draw (offscreen)");
        return true;
    }
    // pg.endDraw(): finish the offscreen frame and go back to the window
    void cpp_pg_end(uint64_t gid) {
        processing_end_draw(gid); check("end_draw (offscreen)");
        if (_cpp_saved_gid) _graphics_id = _cpp_saved_gid;
        _cpp_saved_gid = 0;
    }
    // saveFrame(): read the window canvas back as 8-bit RGB, top row first
    bool cpp_readback_rgb(int w, int h, std::vector<unsigned char>& out, int& ow, int& oh) {
        using C = CppLpColor;
        processing_flush(_graphics_id);
        size_t n = (size_t)w * (size_t)h;
        std::vector<C> buf(n);
        cppmode_lp_graphics_readback(_graphics_id, buf.data(), n);
        if (const char* e = processing_check_error()) {
            // canvas is in physical pixels (HiDPI): the error says the real size
            const char* p = strstr(e, "expected ");
            size_t m = p ? (size_t)strtoull(p + 9, nullptr, 10) : 0;
            if (!m) { fprintf(stderr, "[libprocessing] saveFrame: %s\n", e); return false; }
            double s = std::sqrt((double)m / (double)n);
            w = (int)std::lround(w * s); h = w > 0 ? (int)(m / (size_t)w) : 0;
            n = (size_t)w * (size_t)h;
            if (n != m) { fprintf(stderr, "[libprocessing] saveFrame: odd canvas size %zu\n", m); return false; }
            buf.assign(n, C{});
            cppmode_lp_graphics_readback(_graphics_id, buf.data(), n);
            if (const char* e2 = processing_check_error()) { fprintf(stderr, "[libprocessing] saveFrame: %s\n", e2); return false; }
        }
        auto to8 = [](float v) { v = v < 0 ? 0 : (v > 1 ? 1 : v); return (unsigned char)std::lround(v * 255.0f); };
        out.resize(n * 3);
        for (size_t i = 0; i < n; i++) {
            out[i*3+0] = to8(buf[i].c1); out[i*3+1] = to8(buf[i].c2); out[i*3+2] = to8(buf[i].c3);
        }
        ow = w; oh = h;
        return true;
    }
'''.replace("__MARK__", MARK)
if MARK in be:
    be_new = be
else:
    for a in (be_class, be_check, be_font):
        if be.count(a) != 1: problems.append("backend anchor: " + a)
    be_new = be.replace(be_class, "#include <cmath>\n#include <cstring>\n#include <cstdlib>\n#include <vector>\n#include <cstdint>\n#include <cstddef>\n" + "\n".join(externs) + "\n\n" + be_class, 1)
    be_new = be_new.replace(be_check, be_methods + be_check, 1)
    be_new = be_new.replace(be_font, be_font + "\n            _cpp_default_font = font;", 1)

# ---------------------------------------------------------------- Processing.h
ph_class = "class PGraphics : public PImage {"
ph_end = "void endDraw() { _endDrawImpl(); }"
ph_begin = "inline void PGraphics::beginDraw() {"
ph_is3d = re.search(r"^[ \t]*bool\s+is3D\s*=\s*false;[^\n]*\n", ph, re.M)
if MARK in ph:
    ph_new = ph
else:
    for a in (ph_class, ph_end, ph_begin):
        if ph.count(a) != 1: problems.append("Processing.h anchor: " + a)
    if not ph_is3d: problems.append("Processing.h anchor: bool is3D = false;")
    ph_new = ph
    if ph_is3d:
        ph_new = ph_new[:ph_is3d.end()] + "    uint64_t lp_surface = 0, lp_graphics = 0;  // " + MARK + ": libprocessing offscreen canvas\n" + ph_new[ph_is3d.end():]
    ph_new = ph_new.replace(ph_class, "class PGraphics;\n// " + MARK + ": libprocessing renderer hooks (return false in the OpenGL build)\nbool cppmodeLpBeginDraw(PGraphics* pg);\nbool cppmodeLpEndDraw(PGraphics* pg);\n" + ph_class, 1)
    ph_new = ph_new.replace(ph_end, "void endDraw() { if (cppmodeLpEndDraw(this)) return; _endDrawImpl(); }", 1)
    ph_new = ph_new.replace(ph_begin, ph_begin + "\n    if (cppmodeLpBeginDraw(this)) return;  // " + MARK, 1)

# ---------------------------------------------------------------- Processing.cpp
pc_cg = "PGraphics* PApplet::createGraphics(int w,int h){return new PGraphics(w,h);}"
pc_sf = '    ::std::string fn = filename;\n    size_t pos = fn.find("####");'
helpers = r'''// ---- __MARK__: libprocessing createGraphics() / saveFrame() ----
#ifdef CPPMODE_BACKEND_LIBPROCESSING
bool cppmodeLpBeginDraw(PGraphics* pg) {
    PApplet* app = PApplet::g_papplet;
    if (!pg || !app || !app->_backend) return false;
    return static_cast<LibProcessingBackend*>(app->_backend)->cpp_pg_begin(pg->width, pg->height, pg->lp_surface, pg->lp_graphics);
}
bool cppmodeLpEndDraw(PGraphics* pg) {
    PApplet* app = PApplet::g_papplet;
    if (!pg || !app || !app->_backend) return false;
    if (!pg->lp_graphics) return true;  // endDraw() without beginDraw()
    static_cast<LibProcessingBackend*>(app->_backend)->cpp_pg_end(pg->lp_graphics);
    pg->lp_image_id = pg->lp_surface;   // the offscreen surface is an image, image(pg) draws it
    pg->dirty = false;
    return true;
}
static void cppmodeLpSaveFrame(PApplet* app, const ::std::string& filename) {
    ::std::string fn = filename.empty() ? ::std::string("screen-####.png") : filename;
    size_t p = fn.find('#');
    if (p != ::std::string::npos) {
        size_t q = p; while (q < fn.size() && fn[q] == '#') q++;
        char num[32]; snprintf(num, sizeof num, "%0*d", (int)(q - p), app->frameCount);
        fn.replace(p, q - p, num);
    }
    if (!fn.empty() && fn[0] != '/') {
        const char* sp = getenv("PROCESSING_SKETCH_PATH");
        if (sp && sp[0]) fn = ::std::string(sp) + "/" + fn;
    }
    ::std::vector<unsigned char> px; int w = 0, h = 0;
    if (!static_cast<LibProcessingBackend*>(app->_backend)->cpp_readback_rgb(app->width, app->height, px, w, h)) return;
    ::std::string ext = fn.size() >= 4 ? fn.substr(fn.size() - 4) : "";
    for (auto& c : ext) c = (char)tolower((unsigned char)c);
    int ok = 0;
    if (ext == ".jpg" || ext == "jpeg")      ok = stbi_write_jpg(fn.c_str(), w, h, 3, px.data(), 95);
    else if (ext == ".bmp")                  ok = stbi_write_bmp(fn.c_str(), w, h, 3, px.data());
    else if (ext == ".tga" || ext == ".tif") ok = stbi_write_tga(fn.c_str(), w, h, 3, px.data());
    else { if (ext != ".png") fn += ".png"; ok = stbi_write_png(fn.c_str(), w, h, 3, px.data(), w * 3); }
    if (!ok) fprintf(stderr, "saveFrame: could not write %s\n", fn.c_str());
}
#else
bool cppmodeLpBeginDraw(PGraphics*) { return false; }
bool cppmodeLpEndDraw(PGraphics*) { return false; }
#endif
'''.replace("__MARK__", MARK)
sf_hook = "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n    if (_backend) { cppmodeLpSaveFrame(this, filename); return; }  // " + MARK + "\n#endif\n"
if MARK in pc:
    pc_new = pc
else:
    for a in (pc_cg, pc_sf):
        if pc.count(a) != 1: problems.append("Processing.cpp anchor: " + a.splitlines()[0])
    pc_new = pc.replace(pc_cg, helpers + pc_cg, 1).replace(pc_sf, sf_hook + pc_sf, 1)

if problems:
    print("NOT PATCHED, nothing changed:"); [print("  -", x) for x in problems]; sys.exit(1)
changed = [(p, new) for p, old, new in ((FFI, ffi, ffi_new), (BE, be, be_new), (PH, ph, ph_new), (PC, pc, pc_new)) if old != new]
if not changed: print("already patched"); sys.exit(0)
bak = pathlib.Path(".bak/pg"); bak.mkdir(parents=True, exist_ok=True)
for p, new in changed:
    shutil.copy2(p, bak / (p.name + (".ffi" if p == FFI else "")))
    p.write_text(new); print("patched", p)
