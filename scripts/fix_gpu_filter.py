#!/usr/bin/env python3
"""CppMode: filter() on the GPU in the libprocessing renderer (cppmode-gpu-filter).
Uses lp-fork's built-in filters (gray, invert, threshold, posterize, blur, opaque, erode,
dilate); anything else, or a failure, falls back to the CPU readback path from patch_pg_v5.
Run from the CppMode folder. Changes nothing unless every anchor matches."""
import pathlib, re, sys, shutil
MARK = "cppmode-gpu-filter"
PC, PH, BE = pathlib.Path("src/Processing.cpp"), pathlib.Path("src/Processing.h"), pathlib.Path("src/LibProcessingBackend.h")
pc, ph, be = PC.read_text(), PH.read_text(), BE.read_text()
if MARK in pc: print("already patched"); sys.exit(0)
if "cppmode-pg-v5" not in pc: print("NOT PATCHED: needs patch_pg_v5.py first"); sys.exit(1)

kinds = [("GRAY", 0), ("INVERT", 1), ("THRESHOLD", 2), ("POSTERIZE", 3), ("BLUR", 4), ("OPAQUE", 5), ("ERODE", 6), ("DILATE", 7)]
have = [(n, k) for n, k in kinds if re.search(r"\b(?:constexpr|const|#define)\b[^\n]*\b%s\b" % n, ph)]
missing = [n for n, _ in kinds if n not in dict(have)]
print("filter constants found:", ", ".join(n for n, _ in have) or "none", ("| not defined: " + ", ".join(missing)) if missing else "")
if not have: print("NOT PATCHED: no filter constants found in Processing.h"); sys.exit(1)
mapping = " : ".join("mode == %s ? %d" % (n, k) for n, k in have) + " : -1"

be_anchor = "    uint64_t cpp_current_gid() const { return _graphics_id; }\n"
be_new = be_anchor + r'''    // __MARK__: libprocessing's built-in GPU filters, one filter entity per kind, reused.
    // kind: 0 gray, 1 invert, 2 threshold, 3 posterize, 4 blur, 5 opaque, 6 erode, 7 dilate
    std::map<int, uint64_t> _cpp_filters;
    bool cpp_gpu_filter(int kind, float param) {
        uint64_t& f = _cpp_filters[kind];
        if (!f) {
            switch (kind) {
                case 0: f = processing_filter_gray(); break;
                case 1: f = processing_filter_invert(); break;
                case 2: f = processing_filter_threshold(); break;
                case 3: f = processing_filter_posterize(); break;
                case 4: f = processing_filter_blur(); break;
                case 5: f = processing_filter_opaque(); break;
                case 6: f = processing_filter_erode(); break;
                case 7: f = processing_filter_dilate(); break;
                default: return false;
            }
            if (processing_check_error() || !f) { f = 0; return false; }
        }
        if (kind == 2)      processing_shader_set_float(f, "cutoff", param);
        else if (kind == 3) processing_shader_set_uint(f, "levels", (uint32_t)param);
        else if (kind == 4) processing_shader_set_float(f, "radius", param);
        if (const char* e = processing_check_error()) { fprintf(stderr, "[libprocessing] filter param: %s\n", e); return false; }
        processing_graphics_apply_filter(_graphics_id, f);
        if (const char* e = processing_check_error()) { fprintf(stderr, "[libprocessing] filter: %s\n", e); return false; }
        return true;
    }
'''.replace("__MARK__", MARK)

pc_anchor = "static void cppmodeLpFilter(PApplet* app, int mode, float param) {\n    auto* be = static_cast<LibProcessingBackend*>(app->_backend);\n"
pc_new = pc_anchor + ("    {   // " + MARK + ": GPU first, the CPU readback below is the fallback\n"
    "        int k = " + mapping + ";\n"
    "        float p = param;\n"
    "        if (k == 4 && p == 0.5f) p = 1.0f;         // filter(BLUR) with no radius: Java's default is 1\n"
    "        if (k == 3 && (p < 2.0f || p > 255.0f)) k = -1;  // odd posterize levels: let the CPU path deal with it\n"
    "        if (k >= 0 && be->cpp_gpu_filter(k, p)) return;\n"
    "    }\n")
bad = [n for n, a, t in (("backend", be_anchor, be), ("Processing.cpp", pc_anchor, pc)) if t.count(a) != 1]
if bad: print("NOT PATCHED, nothing changed. Anchor not found in:", ", ".join(bad)); sys.exit(1)
if "#include <map>" not in be: be = "#include <map>\n" + be
pathlib.Path(".bak/gpufilter").mkdir(parents=True, exist_ok=True)
shutil.copy2(PC, ".bak/gpufilter/Processing.cpp"); shutil.copy2(BE, ".bak/gpufilter/LibProcessingBackend.h")
BE.write_text(be.replace(be_anchor, be_new, 1)); PC.write_text(pc.replace(pc_anchor, pc_new, 1))
print("patched", PC, "and", BE)
