#!/usr/bin/env python3
"""CppMode: shaders in the libprocessing renderer (cppmode-lpshader).
  loadShader("x.wesl" | "x.wgsl" | "x.glsl") -> finds x.wesl / x.wgsl in data/ or the sketch folder
  shader(s) / resetShader()                  -> custom material on / off (needs patch_lp_resetmat)
  s.set("name", ...)                         -> uniform by struct field name
  filter(s)                                  -> fullscreen filter shaders (import processing::filter)
The OpenGL renderer keeps its GLSL path. Run from the CppMode folder."""
import pathlib, re, sys, shutil
MARK = "cppmode-lpshader"
PH, PC = pathlib.Path("src/Processing.h"), pathlib.Path("src/Processing.cpp")
ph, pc = PH.read_text(), PC.read_text()
if MARK in pc: print("already patched"); sys.exit(0)
problems = []
def rep(which, old, new):
    global ph, pc
    t = ph if which == "h" else pc
    if t.count(old) != 1: problems.append(("Processing.h" if which == "h" else "Processing.cpp") + ": " + old.strip()[:70]); return
    t = t.replace(old, new, 1)
    if which == "h": ph = t
    else: pc = t

# --- PShader: libprocessing handles + uniforms go to the material/filter when set
rep("h", "    bool linked = false;\n",
    "    bool linked = false;\n"
    "    uint64_t lp_shader = 0, lp_material = 0, lp_filter = 0;  // " + MARK + ": libprocessing handles\n"
    "    bool lpSetF(const ::std::string& n, int count, float x, float y, float z, float w);  // true if handled\n"
    "    bool lpSetI(const ::std::string& n, int v);\n")
for old, guard in [
  ("    void set(const ::std::string& n, float v)                      { glUniform1f(", "if (lpSetF(n, 1, v, 0, 0, 0)) return; "),
  ("    void set(const ::std::string& n, int v)                        { glUniform1i(", "if (lpSetI(n, v)) return; "),
  ("    void set(const ::std::string& n, float x, float y)             { glUniform2f(", "if (lpSetF(n, 2, x, y, 0, 0)) return; "),
  ("    void set(const ::std::string& n, float x, float y, float z)    { glUniform3f(", "if (lpSetF(n, 3, x, y, z, 0)) return; "),
  ("    void set(const ::std::string& n, float x, float y, float z, float w){ glUniform4f(", "if (lpSetF(n, 4, x, y, z, w)) return; "),
]:
    rep("h", old, old.replace("{ gl", "{ " + guard + "gl"))
# --- filter(PShader)
rep("h", "    void filter(int mode, float param);\n",
    "    void filter(int mode, float param);\n"
    "    void filter(PShader& s);                              // " + MARK + "\n"
    "    void filter(PShader* s) { if (s) filter(*s); }\n")

# --- Processing.cpp
LP_IMPL = r'''
// __MARK__: shaders in the libprocessing renderer. libprocessing takes WGSL/WESL, not GLSL:
// uniforms are fields of a uniform struct and set() goes by field name.
#ifdef CPPMODE_BACKEND_LIBPROCESSING
bool PShader::lpSetF(const ::std::string& n, int count, float x, float y, float z, float w) {
    uint64_t target = lp_filter ? lp_filter : lp_material;
    if (!target) return false;
    switch (count) {
        case 1: processing_shader_set_float(target, n.c_str(), x); break;
        case 2: processing_shader_set_vec2(target, n.c_str(), x, y); break;
        case 3: processing_shader_set_vec3(target, n.c_str(), x, y, z); break;
        default: processing_shader_set_vec4(target, n.c_str(), x, y, z, w); break;
    }
    if (const char* e = processing_check_error()) fprintf(stderr, "PShader.set(\"%s\"): %s\n", n.c_str(), e);
    return true;
}
bool PShader::lpSetI(const ::std::string& n, int v) {
    uint64_t target = lp_filter ? lp_filter : lp_material;
    if (!target) return false;
    processing_shader_set_int(target, n.c_str(), v);
    if (const char* e = processing_check_error()) fprintf(stderr, "PShader.set(\"%s\"): %s\n", n.c_str(), e);
    return true;
}
static PShader* cppmodeLpLoadShader(const ::std::string& fragPath) {
    // try name.wesl / name.wgsl in data/ and the sketch folder; a .glsl/.frag name maps to the same base
    ::std::string base = fragPath;
    for (const char* ext : {".glsl", ".frag", ".fs", ".wesl", ".wgsl"}) {
        size_t n = ::std::strlen(ext);
        if (base.size() > n && base.compare(base.size() - n, n, ext) == 0) { base = base.substr(0, base.size() - n); break; }
    }
    const char* sp = getenv("PROCESSING_SKETCH_PATH");
    ::std::string root = (sp && sp[0]) ? ::std::string(sp) + "/" : ::std::string();
    ::std::vector<::std::string> tries;
    bool wgslName = fragPath.size() > 5 && (fragPath.compare(fragPath.size() - 5, 5, ".wesl") == 0 || fragPath.compare(fragPath.size() - 5, 5, ".wgsl") == 0);
    if (wgslName) { tries.push_back("data/" + fragPath); tries.push_back(fragPath); }
    for (const char* ext : {".wesl", ".wgsl"}) { tries.push_back("data/" + base + ext); tries.push_back(base + ext); }
    ::std::string rel, src;
    for (auto& t : tries) {
        ::std::string full = (!t.empty() && t[0] == '/') ? t : root + t;
        ::std::ifstream f(full);
        if (f) { rel = t; src.assign(::std::istreambuf_iterator<char>(f), ::std::istreambuf_iterator<char>()); break; }
    }
    PShader* s = new PShader();
    if (rel.empty()) {
        fprintf(stderr, "loadShader(\"%s\"): the libprocessing renderer uses WGSL/WESL shaders, not GLSL.\n"
                        "  Put a %s.wesl (or .wgsl) in the sketch's data/ folder, or switch the renderer to OpenGL.\n",
                fragPath.c_str(), base.c_str());
        return s;
    }
    s->lp_shader = processing_shader_load(rel.c_str());
    if (const char* e = processing_check_error()) { fprintf(stderr, "loadShader(\"%s\"): %s\n", rel.c_str(), e); s->lp_shader = 0; return s; }
    if (src.find("processing::filter") != ::std::string::npos) s->lp_filter = processing_filter_create(s->lp_shader);
    else s->lp_material = processing_material_create_custom(s->lp_shader);
    if (const char* e = processing_check_error()) fprintf(stderr, "loadShader(\"%s\"): %s\n", rel.c_str(), e);
    return s;
}
#else
bool PShader::lpSetF(const ::std::string&, int, float, float, float, float) { return false; }
bool PShader::lpSetI(const ::std::string&, int) { return false; }
#endif
'''.replace("__MARK__", MARK)
rep("c", "// PSHADER IMPLEMENTATION\n// =============================================================================\n",
    "// PSHADER IMPLEMENTATION\n// =============================================================================\n" + LP_IMPL)
rep("c", "PShader* PApplet::loadShader(const ::std::string& fragPath,const ::std::string& vertPath){\n",
    "PShader* PApplet::loadShader(const ::std::string& fragPath,const ::std::string& vertPath){\n"
    "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n    if (_backend) { (void)vertPath; return cppmodeLpLoadShader(fragPath); }  // " + MARK + "\n#endif\n")
rep("c", "void PApplet::shader(PShader& s){ s.bind(); activeShader=&s; }\n",
    "void PApplet::shader(PShader& s){\n"
    "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n"
    "    if (_backend) {  // " + MARK + "\n"
    "        if (s.lp_material) processing_material(_backend->graphics_id(), s.lp_material);\n"
    "        else if (s.lp_filter) fprintf(stderr, \"shader(): that's a filter shader (processing::filter), use filter(shader) instead\\n\");\n"
    "        activeShader = &s; return;\n"
    "    }\n"
    "#endif\n"
    "    s.bind(); activeShader=&s; }\n")
rep("c", "void PApplet::resetShader(){ glUseProgram(0); activeShader=nullptr; }\n",
    "void PApplet::resetShader(){\n"
    "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n"
    "    if (_backend) { processing_reset_material(_backend->graphics_id()); activeShader = nullptr; return; }  // " + MARK + "\n"
    "#endif\n"
    "    glUseProgram(0); activeShader=nullptr; }\n"
    "void PApplet::filter(PShader& s){  // " + MARK + "\n"
    "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n"
    "    if (_backend) {\n"
    "        if (!s.lp_filter) { fprintf(stderr, \"filter(shader): not a filter shader (it needs import processing::filter)\\n\"); return; }\n"
    "        processing_graphics_apply_filter(_backend->graphics_id(), s.lp_filter);\n"
    "        if (const char* e = processing_check_error()) fprintf(stderr, \"filter(shader): %s\\n\", e);\n"
    "        return;\n"
    "    }\n"
    "#endif\n"
    "    static bool warned = false;\n"
    "    if (!warned) { fprintf(stderr, \"filter(shader) works in the libprocessing renderer for now\\n\"); warned = true; }\n"
    "    (void)s;\n}\n")
if problems:
    print("NOT PATCHED, nothing changed:"); [print("  -", p) for p in problems]; sys.exit(1)
pathlib.Path(".bak/lpshader").mkdir(parents=True, exist_ok=True)
shutil.copy2(PH, ".bak/lpshader/Processing.h"); shutil.copy2(PC, ".bak/lpshader/Processing.cpp")
PH.write_text(ph); PC.write_text(pc); print("patched", PH, "and", PC)
