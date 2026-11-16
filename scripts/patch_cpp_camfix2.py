#!/usr/bin/env python3
"""On top of patch_cpp_camextras (cppmode-camfix2):
  libprocessing: screenY was y-up (186.8 instead of 113.2): flip to Processing's y-down
  libprocessing: printMatrix printed the model matrix only; Java prints camera * model
  OpenGL: modelX/Y/Z returned camera-space coords (51.8); Java returns world coords (251.8)
  OpenGL: printCamera/printProjection printed identity; now the same values as libprocessing
Run from the CppMode folder."""
import pathlib, re, sys, shutil
MARK = "cppmode-camfix2"
PC = pathlib.Path("src/Processing.cpp"); pc = PC.read_text()
if MARK in pc: print("already patched"); sys.exit(0)
if "cppmode-camextras" not in pc: print("NOT PATCHED: needs patch_cpp_camextras.py first"); sys.exit(1)
problems = []
def rep(name, old, new):
    global pc
    if pc.count(old) != 1: problems.append("%s (found %d)" % (name, pc.count(old))); return
    pc = pc.replace(old, new, 1)

CAMHELP = r'''// __MARK__: the camera as Processing sees it: camera() values, or Java's default one
static void cppmodeCamBasis(PApplet* a, float e[3], float x[3], float y[3], float z[3]) {
    float cz = (a->height * 0.5f) / ::std::tan(PI / 6.0f);
    float c[3] = {a->width * 0.5f, a->height * 0.5f, 0}, u[3] = {0, 1, 0};
    e[0] = a->width * 0.5f; e[1] = a->height * 0.5f; e[2] = cz;
    if (g_lpCamSet) for (int i = 0; i < 3; i++) { e[i] = g_lpCam[i]; c[i] = g_lpCam[3 + i]; u[i] = g_lpCam[6 + i]; }
    for (int i = 0; i < 3; i++) z[i] = e[i] - c[i];
    float zl = ::std::sqrt(z[0]*z[0] + z[1]*z[1] + z[2]*z[2]); if (zl > 0) for (int i = 0; i < 3; i++) z[i] /= zl;
    x[0] = u[1]*z[2] - u[2]*z[1]; x[1] = u[2]*z[0] - u[0]*z[2]; x[2] = u[0]*z[1] - u[1]*z[0];
    float xl = ::std::sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]); if (xl > 0) for (int i = 0; i < 3; i++) x[i] /= xl;
    y[0] = z[1]*x[2] - z[2]*x[1]; y[1] = z[2]*x[0] - z[0]*x[2]; y[2] = z[0]*x[1] - z[1]*x[0];
}
static void cppmodeCamMatrix(PApplet* a, float m[16]) {  // column-major view matrix
    float e[3], x[3], y[3], z[3]; cppmodeCamBasis(a, e, x, y, z);
    float v[16] = { x[0], y[0], z[0], 0,  x[1], y[1], z[1], 0,  x[2], y[2], z[2], 0,
                    -(x[0]*e[0] + x[1]*e[1] + x[2]*e[2]), -(y[0]*e[0] + y[1]*e[1] + y[2]*e[2]), -(z[0]*e[0] + z[1]*e[1] + z[2]*e[2]), 1 };
    for (int i = 0; i < 16; i++) m[i] = v[i];
}
// OpenGL modelX/Y/Z: the modelview gives camera-space coords; Java's modelX is world space
static float cppmodeGlModel(PApplet* a, const float mv[16], float px, float py, float pz, int axis) {
    float ec[3] = { mv[0]*px + mv[4]*py + mv[8]*pz + mv[12], mv[1]*px + mv[5]*py + mv[9]*pz + mv[13], mv[2]*px + mv[6]*py + mv[10]*pz + mv[14] };
    if (!a->is3DMode) return ec[axis];
    float e[3], x[3], y[3], z[3]; cppmodeCamBasis(a, e, x, y, z);
    return e[axis] + x[axis] * ec[0] + y[axis] * ec[1] + z[axis] * ec[2];
}
'''.replace("__MARK__", MARK)
rep("helpers", "void PApplet::printMatrix(){\n", CAMHELP + "void PApplet::printMatrix(){\n")
rep("printMatrix (camera * model)",
    "    if (_backend) { Matrix mm = processing_get_matrix(_backend->graphics_id()); cppmodePrint4x4(nullptr, mm.m); return; }  // cppmode-camextras\n",
    "    if (_backend) {  // " + MARK + ": camera * model, like Java's printMatrix in P3D\n"
    "        Matrix mm = processing_get_matrix(_backend->graphics_id());\n"
    "        float c[16], r[16]; cppmodeCamMatrix(this, c);\n"
    "        for (int col = 0; col < 4; col++) for (int row = 0; row < 4; row++) {\n"
    "            float s = 0; for (int k = 0; k < 4; k++) s += c[k * 4 + row] * mm.m[col * 4 + k]; r[col * 4 + row] = s; }\n"
    "        cppmodePrint4x4(nullptr, is3DMode ? r : mm.m); return; }\n")
rep("screenY flip", "    if (_backend) return processing_screen_y(_backend->graphics_id(), x, y, z);  // cppmode-camextras\n",
    "    if (_backend) return (float)height - processing_screen_y(_backend->graphics_id(), x, y, z);  // " + MARK + ": the fork's y points up\n")
for axis, i in (("X", 0), ("Y", 1), ("Z", 2)):
    idx = [(0, 4, 8, 12), (1, 5, 9, 13), (2, 6, 10, 14)][i]
    old = "   float mv[16];glGetFloatv(GL_MODELVIEW_MATRIX,mv);return mv[%d]*x+mv[%d]*y+mv[%d]*z+mv[%d];}" % idx
    rep("OpenGL model" + axis, old, "   float mv[16];glGetFloatv(GL_MODELVIEW_MATRIX,mv);return cppmodeGlModel(this, mv, x, y, z, %d);}  // %s" % (i, MARK))
# printCamera/printProjection: use the computed values in both renderers
for fn, tag in (("printCamera", "camera() values, or Java's default camera"), ("printProjection", "last perspective()/ortho(), or Java's default perspective")):
    start = pc.find("void PApplet::%s(){" % fn)
    old_open = "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n    if (_backend) {  // cppmode-camextras: " + tag + "\n"
    j = pc.find(old_open, start)
    k = pc.find("        return;\n    }\n#endif\n", j)
    if start < 0 or j < 0 or k < 0 or j - start > 200: problems.append(fn); continue
    pc = (pc[:j] + "    {  // " + MARK + ": both renderers: " + tag + "\n" + pc[j + len(old_open):k]
          + "        return;\n    }\n" + pc[k + len("        return;\n    }\n#endif\n"):])
# OpenGL camera()/perspective()/ortho() also record their values for printCamera()/printProjection()
for sig, body in (
    ("void PApplet::camera(float ex,float ey,float ez,float cx,float cy,float cz,float ux,float uy,float uz){\n",
     "    { float v[9] = {ex, ey, ez, cx, cy, cz, ux, uy, uz}; for (int i = 0; i < 9; i++) g_lpCam[i] = v[i]; g_lpCamSet = true; }  // " + MARK + "\n"),
    ("void PApplet::perspective(float fov, float aspect, float zNear, float zFar) {\n",
     "    g_lpProjKind = 1; g_lpProj[0] = fov; g_lpProj[1] = aspect; g_lpProj[2] = zNear; g_lpProj[3] = zFar;  // " + MARK + "\n"),
    ("void PApplet::ortho(float l, float r, float b, float t, float n, float f) {\n",
     "    { g_lpProjKind = 2; float v[6] = {l, r, b, t, n, f}; for (int i = 0; i < 6; i++) g_lpProj[i] = v[i]; }  // " + MARK + "\n")):
    rep(sig.split("(")[0], sig, sig + body)
if problems: print("NOT PATCHED, nothing changed:"); [print("  -", p) for p in problems]; sys.exit(1)
pathlib.Path(".bak/camfix2").mkdir(parents=True, exist_ok=True); shutil.copy2(PC, ".bak/camfix2/Processing.cpp")
PC.write_text(pc); print("patched", PC, "(screenY flip, printMatrix, OpenGL modelX/Y/Z, printCamera/printProjection in both renderers)")
