#!/usr/bin/env python3
"""CppMode, libprocessing renderer (cppmode-camextras):
  modelX/Y/Z, screenX/Y/Z -> the fork's processing_model_* / processing_screen_*
  printMatrix             -> processing_get_matrix
  printCamera/printProjection -> from the last camera()/perspective()/ortho()/frustum() (Java defaults until then)
  beginCamera/endCamera   -> push/pop, same as the OpenGL path does today
Run from the CppMode folder."""
import pathlib, re, sys, shutil
MARK = "cppmode-camextras"
PC = pathlib.Path("src/Processing.cpp"); pc = PC.read_text()
if MARK in pc: print("already patched"); sys.exit(0)
problems = []
def rep(name, old, new, count=1):
    global pc
    if pc.count(old) != count: problems.append("%s (found %d, want %d)" % (name, pc.count(old), count)); return
    pc = pc.replace(old, new)

HELPERS = r'''
// __MARK__: remember the camera and projection given to libprocessing so printCamera()
// and printProjection() can show them (the fork has no getter for those)
static float g_lpCam[9];  static bool g_lpCamSet = false;
static int   g_lpProjKind = 0;  // 0 = Java default, 1 = perspective, 2 = ortho
static float g_lpProj[6];
static void cppmodeLpCamera(uint64_t g, float ex, float ey, float ez, float cx, float cy, float cz, float ux, float uy, float uz) {
    float v[9] = {ex, ey, ez, cx, cy, cz, ux, uy, uz}; for (int i = 0; i < 9; i++) g_lpCam[i] = v[i]; g_lpCamSet = true;
    processing_camera(g, ex, ey, ez, cx, cy, cz, ux, uy, uz);
}
static void cppmodeLpPerspective(uint64_t g, float fov, float aspect, float n, float f) {
    g_lpProjKind = 1; g_lpProj[0] = fov; g_lpProj[1] = aspect; g_lpProj[2] = n; g_lpProj[3] = f;
    processing_perspective(g, fov, aspect, n, f);
}
static void cppmodeLpOrtho(uint64_t g, float l, float r, float b, float t, float n, float f) {
    g_lpProjKind = 2; float v[6] = {l, r, b, t, n, f}; for (int i = 0; i < 6; i++) g_lpProj[i] = v[i];
    processing_ortho(g, l, r, b, t, n, f);
}
static void cppmodePrint4x4(const char* title, const float m[16]) {  // m is column-major
    if (title) ::std::cout << title << "\n";
    for (int i = 0; i < 4; i++) { for (int j = 0; j < 4; j++) ::std::cout << m[j * 4 + i] << " "; ::std::cout << "\n"; }
}
'''.replace("__MARK__", MARK)
anchor_cam = "void PApplet::camera(float ex,float ey,float ez,float cx,float cy,float cz,float ux,float uy,float uz){\n"
# route every existing backend camera/projection call through the recorders
for fn, wrap in (("processing_camera(_backend->graphics_id(),", "cppmodeLpCamera(_backend->graphics_id(),"),
                 ("processing_perspective(_backend->graphics_id(),", "cppmodeLpPerspective(_backend->graphics_id(),"),
                 ("processing_ortho(_backend->graphics_id(),", "cppmodeLpOrtho(_backend->graphics_id(),")):
    n = pc.count(fn)
    if n == 0: problems.append("no call to " + fn.split("(")[0])
    pc = pc.replace(fn, wrap)
for fn, wrap in (("processing_perspective(_backend->graphics_id(), ", "cppmodeLpPerspective(_backend->graphics_id(), "),):
    pc = pc.replace(fn, wrap)

LP = "#ifdef CPPMODE_BACKEND_LIBPROCESSING\n"
for axis in "XYZ":
    a = axis.lower()
    rep("modelX/Y/Z", "float PApplet::model%s(float x,float y,float z) {" % axis,
        "float PApplet::model%s(float x,float y,float z) {\n%s    if (_backend) return processing_model_%s(_backend->graphics_id(), x, y, z);  // %s\n#endif\n   " % (axis, LP, a, MARK))
    rep("screenX/Y/Z", "float PApplet::screen%s(float x,float y,float z){" % axis,
        "float PApplet::screen%s(float x,float y,float z){\n%s    if (_backend) return processing_screen_%s(_backend->graphics_id(), x, y, z);  // %s\n#endif\n   " % (axis, LP, a, MARK))
rep("printMatrix", "void PApplet::printMatrix(){",
    HELPERS + "void PApplet::printMatrix(){\n" + LP +
    "    if (_backend) { Matrix mm = processing_get_matrix(_backend->graphics_id()); cppmodePrint4x4(nullptr, mm.m); return; }  // " + MARK + "\n#endif\n   ")
rep("beginCamera/endCamera", "void PApplet::beginCamera(){glMatrixMode(GL_MODELVIEW);glPushMatrix();}\nvoid PApplet::endCamera()  {glPopMatrix();}\n",
    "void PApplet::beginCamera(){\n" + LP + "    if (_backend) { processing_push_matrix(_backend->graphics_id()); return; }  // " + MARK + "\n#endif\n    glMatrixMode(GL_MODELVIEW);glPushMatrix();}\n"
    "void PApplet::endCamera()  {\n" + LP + "    if (_backend) { processing_pop_matrix(_backend->graphics_id()); return; }  // " + MARK + "\n#endif\n    glPopMatrix();}\n")
PRINTS = r'''
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // __MARK__: camera() values, or Java's default camera
        float cz = (height * 0.5f) / ::std::tan(PI / 6.0f);
        float e[3] = {width * 0.5f, height * 0.5f, cz}, c[3] = {width * 0.5f, height * 0.5f, 0}, u[3] = {0, 1, 0};
        if (g_lpCamSet) for (int i = 0; i < 3; i++) { e[i] = g_lpCam[i]; c[i] = g_lpCam[3 + i]; u[i] = g_lpCam[6 + i]; }
        float z[3] = {e[0] - c[0], e[1] - c[1], e[2] - c[2]};
        float zl = ::std::sqrt(z[0]*z[0] + z[1]*z[1] + z[2]*z[2]); if (zl > 0) for (float& k : z) k /= zl;
        float x[3] = {u[1]*z[2] - u[2]*z[1], u[2]*z[0] - u[0]*z[2], u[0]*z[1] - u[1]*z[0]};
        float xl = ::std::sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]); if (xl > 0) for (float& k : x) k /= xl;
        float y[3] = {z[1]*x[2] - z[2]*x[1], z[2]*x[0] - z[0]*x[2], z[0]*x[1] - z[1]*x[0]};
        float m[16] = { x[0], y[0], z[0], 0,  x[1], y[1], z[1], 0,  x[2], y[2], z[2], 0,
                        -(x[0]*e[0] + x[1]*e[1] + x[2]*e[2]), -(y[0]*e[0] + y[1]*e[1] + y[2]*e[2]), -(z[0]*e[0] + z[1]*e[1] + z[2]*e[2]), 1 };
        cppmodePrint4x4("Camera matrix:", m);
        return;
    }
#endif
'''.replace("__MARK__", MARK)
PRINTP = r'''
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // __MARK__: last perspective()/ortho(), or Java's default perspective
        float m[16] = {0};
        if (g_lpProjKind == 2) {
            float l = g_lpProj[0], r = g_lpProj[1], b = g_lpProj[2], t = g_lpProj[3], n = g_lpProj[4], f = g_lpProj[5];
            m[0] = 2 / (r - l); m[5] = 2 / (t - b); m[10] = -2 / (f - n);
            m[12] = -(r + l) / (r - l); m[13] = -(t + b) / (t - b); m[14] = -(f + n) / (f - n); m[15] = 1;
        } else {
            float cz = (height * 0.5f) / ::std::tan(PI / 6.0f);
            float fov = PI / 3.0f, asp = height > 0 ? (float)width / height : 1, n = cz / 10, f = cz * 10;
            if (g_lpProjKind == 1) { fov = g_lpProj[0]; asp = g_lpProj[1]; n = g_lpProj[2]; f = g_lpProj[3]; }
            float t = 1.0f / ::std::tan(fov / 2);
            m[0] = t / asp; m[5] = t; m[10] = (f + n) / (n - f); m[11] = -1; m[14] = 2 * f * n / (n - f);
        }
        cppmodePrint4x4("Projection matrix:", m);
        return;
    }
#endif
'''.replace("__MARK__", MARK)
rep("printCamera", "void PApplet::printCamera(){", "void PApplet::printCamera(){" + PRINTS + "   ")
rep("printProjection", "void PApplet::printProjection(){", "void PApplet::printProjection(){" + PRINTP + "   ")
if problems: print("NOT PATCHED, nothing changed:"); [print("  -", p) for p in problems]; sys.exit(1)
pathlib.Path(".bak/camextras").mkdir(parents=True, exist_ok=True); shutil.copy2(PC, ".bak/camextras/Processing.cpp")
PC.write_text(pc); print("patched", PC, "(modelX/Y/Z, screenX/Y/Z, printMatrix, printCamera, printProjection, beginCamera, endCamera)")
