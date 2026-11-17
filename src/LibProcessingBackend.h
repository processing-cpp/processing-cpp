#include <array>
#include <tuple>
#pragma once
#include "ProcessingBackend.h"
#include <processing.h>   // cbindgen-generated C header from libprocessing
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdlib>
#include <map>

#define LP_COLOR_RGB 0

static inline Color lp_rgba(float r, float g, float b, float a) {
    return Color{ r, g, b, a, LP_COLOR_RGB };
}

// ProcessingBackend backed by libprocessing (Bevy + wgpu + Vulkan/Metal/DX12).
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <cstdint>
#include <cstddef>
// cppmode-pg-offscreen: entry points the bundled header may not declare yet
extern "C" uint64_t processing_surface_create_offscreen(uint32_t width, uint32_t height, float scale_factor);
struct CppLpColor { float c1, c2, c3, a; uint8_t space; };  // same layout as libprocessing's Color
extern "C" void cppmode_lp_graphics_readback(uint64_t graphics_id, CppLpColor* buffer, size_t buffer_len)
    __asm__("processing_graphics_readback");
extern "C" void cppmode_lp_graphics_update(uint64_t graphics_id, const CppLpColor* buffer, size_t buffer_len)
    __asm__("processing_graphics_update");

class LibProcessingBackend final : public ProcessingBackend {
public:
    LibProcessingBackend() = default;
    ~LibProcessingBackend() override { destroy(); }

    void init(uint64_t window_handle, uint64_t display_handle,
              uint32_t width, uint32_t height, float scale_factor) override
    {
        // Images are decoded by CppMode and fonts load by full path,
        // so the default asset root is fine.
        processing_init();
        check("processing_init");

        // Use the auto-detecting Linux variant -- it picks X11 or Wayland
        // from the environment automatically.
        _surface_id = processing_surface_create(
            window_handle, display_handle, width, height, scale_factor);
        check("processing_surface_create");

        _graphics_id = processing_graphics_create(_surface_id, width, height);
        check("processing_graphics_create");
        // graphics_create always makes an HDR canvas with bloom and a filmic
        // tonemapper, which dulls colors and lighting compared to Processing.
        // remove_bloom drops both and keeps the float texture.
        processing_graphics_remove_bloom(_graphics_id);
        processing_color_mode(_graphics_id, 0 /* sRGB */, 1.0f, 1.0f, 1.0f, 1.0f);  // we send 0..1
        {   // default font so text() works without textFont()
            uint64_t font = 0;
            {   // Same default font as Java mode: Processing Sans Pro, shipped in CppMode/fonts
                const char* mp = getenv("PROCESSING_MODE_PATH");
                std::string base = mp ? mp : ".";
                const char* file = "ProcessingSansPro-Regular.ttf";
                for (std::string dir : { base + "/fonts/", base + "/../fonts/", base + "/", std::string("fonts/") }) {
                    FILE* f = fopen((dir + file).c_str(), "rb");
                    if (!f) continue;
                    fclose(f);
                    // libprocessing reads the path as given, so pass the full one
                    font = processing_load_font((dir + file).c_str());
                    if (font) { fprintf(stderr, "[CppMode] libprocessing font: %s%s\n", dir.c_str(), file); break; }
                }
            }
            if (!font) { font = processing_create_font("sans-serif");
                         fprintf(stderr, "[CppMode] libprocessing font: system sans-serif (Processing Sans Pro not found)\n"); }
            if (!font) font = processing_load_font("default.ttf");
            _font = font;
            if (font) processing_text_font(_graphics_id, font);
            _cpp_default_font = font;
            processing_text_size(_graphics_id, 12.0f);
        }
        _width = width; _height = height;
    }

    void destroy() override {
        if (_graphics_id) { processing_graphics_destroy(_graphics_id); _graphics_id = 0; }
        if (_surface_id)  { processing_surface_destroy(_surface_id);   _surface_id  = 0; }
        processing_exit(0);
    }

    void resize(uint32_t width, uint32_t height) override {
        processing_surface_resize(_surface_id, width, height);
        check("processing_surface_resize");
        _width = width; _height = height;
    }

    // Frame
    bool poll_events() override { return true; }  // end_draw() runs the update
    void begin_draw()  override { model_reset_frame(); /* cppmode-lightxform */ processing_begin_draw(_graphics_id);  check("begin_draw"); _drawing = true; }
    void end_draw()    override { processing_end_draw(_graphics_id);    check("end_draw");   _drawing = false; }
    void flush()       override { processing_flush(_graphics_id); }

    // Color
    void background(float r, float g, float b, float a) override {
        processing_background_color(_graphics_id, lp_rgba(r, g, b, 1.0f)); }
    void fill(float r, float g, float b, float a) override {
        processing_set_fill(_graphics_id, lp_rgba(r, g, b, a)); }
    void no_fill()   override { processing_no_fill(_graphics_id); }
    void stroke(float r, float g, float b, float a) override {
        processing_set_stroke_color(_graphics_id, lp_rgba(r, g, b, a)); }
    void no_stroke() override { processing_no_stroke(_graphics_id); }
    void stroke_weight(float w) override { processing_set_stroke_weight(_graphics_id, w); }

    // 2D
    void rect(float x, float y, float w, float h) override {
        processing_rect(_graphics_id, x, y, w, h, 0, 0, 0, 0); }
    void rect(float x, float y, float w, float h,
              float tl, float tr, float br, float bl) override {
        processing_rect(_graphics_id, x, y, w, h, tl, tr, br, bl); }
    void ellipse(float cx, float cy, float w, float h) override {
        processing_ellipse(_graphics_id, cx, cy, w, h); }

    // ── Text ─────────────────────────────────────────────────────────────
    // libprocessing seems to pick up a new alignment only when the size is set,
    // so re-apply both (align first, then size) before every draw.
    float   _ts = 12.0f;
    uint8_t _th = 0, _tv = 0;
    void text(const std::string& s, float x, float y) override {
        processing_text_align(_graphics_id, _th, _tv);
        processing_text_size(_graphics_id, _ts);
        if (_tl > 0) processing_text_leading(_graphics_id, _tl);
        if (_th == 0) { processing_text(_graphics_id, s.c_str(), x, y); return; }
        // libprocessing applies CENTER/RIGHT only inside a text box, so use a
        // very wide box placed around x: centered on it, or ending at it.
        const float W = 20000.0f;
        float bx = (_th == 1) ? x - W * 0.5f : x - W;
        processing_text_box(_graphics_id, s.c_str(), bx, y, W, 20000.0f); }
    void text_size(float size) override { _ts = size; _tl = -1.0f; processing_text_size(_graphics_id, size); }
    void text_align(int h, int v) override {
        _th = (uint8_t)h; _tv = (uint8_t)v;
        processing_text_align(_graphics_id, _th, _tv); }
    // Fonts loaded from files, one handle per path.
    std::map<std::string, uint64_t> _fonts;
    void text_font_file(const std::string& path) override {
        auto it = _fonts.find(path);
        uint64_t id = (it != _fonts.end()) ? it->second : (_fonts[path] = processing_load_font(path.c_str()));
        if (!id && path.find('/') == std::string::npos && path.find('\\') == std::string::npos)
            id = _fonts[path] = processing_create_font(path.c_str());  // cppmode-fontname: by family name
        if (!id) { fprintf(stderr, "[CppMode] libprocessing could not load font: %s\n", path.c_str()); return; }
        _font = id;
        processing_text_font(_graphics_id, id); }
    // Text as letter images (see PApplet::drawTTFStr). CPPMODE_LP_NATIVE_TEXT=1
    // switches back to libprocessing's own text drawing.
    bool glyph_images() override {
        static const bool on = (getenv("CPPMODE_LP_NATIVE_TEXT") == nullptr); return on; }
    uint64_t image_create_rgba(uint32_t w, uint32_t h, const unsigned char* rgba) override {
        return processing_image_create(w, h, rgba, (size_t)w * (size_t)h * 4); }
    void image_region(uint64_t img, float dx, float dy, float dw, float dh,
                      float sx, float sy, float sw, float sh) override {
        processing_image_region(_graphics_id, img, dx, dy, dw, dh, sx, sy, sw, sh); }
    void image_tint(float r, float g, float b, float a) override {
        // libprocessing only blends an image when the tint alpha is below 1,
        // so keep it a hair under (254/255) to respect the image's own alpha
        processing_tint(_graphics_id, lp_rgba(r, g, b, (lp_alpha_fix() && a > 0.996f) ? 0.996f : a)); }
    static bool lp_alpha_fix() {
        static const bool on = (getenv("CPPMODE_LP_ALPHA_FIX") != nullptr); return on; }
    void image_no_tint() override {
        if (lp_alpha_fix()) processing_tint(_graphics_id, lp_rgba(1.0f, 1.0f, 1.0f, 0.996f));
        else processing_no_tint(_graphics_id); }
    float _tl = -1.0f;   // user leading; setting the size resets it in libprocessing
    float text_width(const std::string& s) override {
        processing_text_size(_graphics_id, _ts);
        return processing_text_width(_graphics_id, s.c_str()); }
    float text_ascent()  override { processing_text_size(_graphics_id, _ts); return processing_text_ascent(_graphics_id); }
    float text_descent() override { processing_text_size(_graphics_id, _ts); return processing_text_descent(_graphics_id); }
    void  text_leading(float leading) override { _tl = leading; processing_text_leading(_graphics_id, leading); }
    // Processing's boxed text: x,y is the box's top-left. BASELINE and TOP
    // start at the top, CENTER and BOTTOM are placed within the box height.
    void text_box(const std::string& s, float x, float y, float w, float h) override {
        uint8_t v = (_tv == 2 || _tv == 3) ? _tv : 0;   // default: y is the first baseline
        float yy = (_tv == 2) ? y + h * 0.5f : (_tv == 3) ? y + h : y;
        processing_text_align(_graphics_id, _th, v);
        processing_text_size(_graphics_id, _ts);
        if (_tl > 0) processing_text_leading(_graphics_id, _tl);
        processing_text_box(_graphics_id, s.c_str(), x, yy, w, h);
        processing_text_align(_graphics_id, _th, _tv); }
    void circle(float cx, float cy, float d) override {
        processing_circle(_graphics_id, cx, cy, d); }
    void line(float x1, float y1, float x2, float y2) override {
        processing_line(_graphics_id, x1, y1, x2, y2); }
    // cppmode-api3: a 3D line is a 2D line drawn in a frame whose x axis runs from p1 to p2
    void line(float x1, float y1, float z1, float x2, float y2, float z2) override {
        if (z1 == 0.0f && z2 == 0.0f) { processing_line(_graphics_id, x1, y1, x2, y2); return; }
        float dx = x2 - x1, dy = y2 - y1, dz = z2 - z1, len = std::sqrt(dx*dx + dy*dy + dz*dz);
        if (len <= 0.0f) { point(x1, y1, z1); return; }
        dx /= len; dy /= len; dz /= len;
        float ux = 0, uy = 0, uz = 1;                     /* side axis: perpendicular to the line and to z */
        if (std::fabs(dz) > 0.99f) { ux = 0; uy = 1; uz = 0; }
        float sx = dy*uz - dz*uy, sy = dz*ux - dx*uz, sz = dx*uy - dy*ux;
        float sl = std::sqrt(sx*sx + sy*sy + sz*sz); sx /= sl; sy /= sl; sz /= sl;
        float tx = dy*sz - dz*sy, ty = dz*sx - dx*sz, tz = dx*sy - dy*sx;
        const float m[16] = { dx, dy, dz, 0,  sx, sy, sz, 0,  tx, ty, tz, 0,  x1, y1, z1, 1 };
        processing_push_matrix(_graphics_id);
        processing_apply_matrix(_graphics_id, m);
        processing_line(_graphics_id, 0, 0, len, 0);
        processing_pop_matrix(_graphics_id); }
    void point(float x, float y) override { processing_point(_graphics_id, x, y); }
    void point(float x, float y, float z) override {  // cppmode-api3: move there, draw at the origin
        if (z == 0.0f) { processing_point(_graphics_id, x, y); return; }
        processing_push_matrix(_graphics_id);
        processing_translate(_graphics_id, x, y, z);
        processing_point(_graphics_id, 0, 0);
        processing_pop_matrix(_graphics_id); }
    void triangle(float x1, float y1, float x2, float y2, float x3, float y3) override {
        processing_triangle(_graphics_id, x1, y1, x2, y2, x3, y3); }
    void quad(float x1, float y1, float x2, float y2,
              float x3, float y3, float x4, float y4) override {
        processing_quad(_graphics_id, x1, y1, x2, y2, x3, y3, x4, y4); }
    void arc(float x, float y, float w, float h,
             float start, float stop, uint8_t mode) override {
        processing_arc(_graphics_id, x, y, w, h, start, stop, mode); }
    void square(float x, float y, float s) override {
        processing_square(_graphics_id, x, y, s); }

    // 3D
    // Geometry is drawn when the frame flushes in end_draw(), so it has to outlive
    // the call. Keep one mesh per size and reuse it every frame.
    // cppmode-perf: one unit box, scaled per call. A mesh per size grew forever and was rebuilt
    // every frame when a sketch animated the size.
    void box(float w, float h, float d) override {
        if (!_unitBox) _unitBox = processing_geometry_box(1, 1, 1);
        push_matrix(); scale(w, h, d); processing_model(_graphics_id, _unitBox); pop_matrix();
    }
    void sphereDetail(int ures, int vres) override { _sphU = ures < 3 ? 3 : ures; _sphV = vres < 2 ? 2 : vres; }
    // cppmode-perf: one unit sphere per detail level, scaled per call
    void sphere(float r) override {
        auto key = ::std::make_pair(_sphU, _sphV);
        auto it = _unitSpheres.find(key);
        uint64_t geo = it != _unitSpheres.end() ? it->second
                     : (_unitSpheres[key] = processing_geometry_sphere(1, (uint32_t)_sphU, (uint32_t)_sphV));
        push_matrix(); scale(r, r, r); processing_model(_graphics_id, geo); pop_matrix();
    }
    uint64_t _unitBox = 0;
    ::std::map<::std::pair<int, int>, uint64_t> _unitSpheres;
    int _sphU = 30, _sphV = 30;

    // Transform
    // cppmode-lightxform: a CPU copy of the model matrix (column-major), kept in step
    // with every transform call, so lights can use it without processing_get_matrix's flush.
    float _m[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    ::std::vector<::std::array<float, 16>> _mstack;
    void m_identity() { for (int i = 0; i < 16; i++) _m[i] = (i % 5 == 0) ? 1.0f : 0.0f; }
    void m_mul(const float* t) {
        float r[16];
        for (int col = 0; col < 4; col++) for (int row = 0; row < 4; row++) {
            float s = 0; for (int k = 0; k < 4; k++) s += _m[k * 4 + row] * t[col * 4 + k];
            r[col * 4 + row] = s; }
        for (int i = 0; i < 16; i++) _m[i] = r[i];
    }
    void m_translate(float x, float y, float z) { const float t[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, x,y,z,1}; m_mul(t); }
    void m_scale(float x, float y, float z) { const float t[16] = {x,0,0,0, 0,y,0,0, 0,0,z,0, 0,0,0,1}; m_mul(t); }
    void m_rotate(float a, float x, float y, float z) {
        float l = ::std::sqrt(x * x + y * y + z * z); if (l <= 0) return; x /= l; y /= l; z /= l;
        float c = ::std::cos(a), s = ::std::sin(a), k = 1 - c;
        const float t[16] = {k*x*x + c,   k*x*y + s*z, k*x*z - s*y, 0,
                             k*x*y - s*z, k*y*y + c,   k*y*z + s*x, 0,
                             k*x*z + s*y, k*y*z - s*x, k*z*z + c,   0,
                             0, 0, 0, 1};
        m_mul(t);
    }
public:
    // start of a frame: libprocessing resets its model matrix too
    void model_reset_frame() { m_identity(); _mstack.clear(); }
    // w = 1 for a point, 0 for a direction
    void model_point(float x, float y, float z, float w, float o[3]) const {
        for (int r = 0; r < 3; r++) o[r] = _m[r] * x + _m[4 + r] * y + _m[8 + r] * z + _m[12 + r] * w;
    }
    void push_matrix()  override { ::std::array<float, 16> a; for (int i = 0; i < 16; i++) a[i] = _m[i];
        _mstack.push_back(a); processing_push_matrix(_graphics_id);  }
    void pop_matrix()   override { if (!_mstack.empty()) { for (int i = 0; i < 16; i++) _m[i] = _mstack.back()[i]; _mstack.pop_back(); }
        processing_pop_matrix(_graphics_id);   }
    void reset_matrix() override { m_identity(); processing_reset_matrix(_graphics_id); }
    void translate(float x, float y, float z) override { m_translate(x, y, z);
        processing_translate(_graphics_id, x, y, z); }
    void rotate(float angle, float x, float y, float z) override { m_rotate(angle, x, y, z);
        processing_rotate(_graphics_id, angle, x, y, z); }
    void rotate_x(float a) override { m_rotate(a, 1, 0, 0); processing_rotate(_graphics_id, a, 1, 0, 0); }
    void rotate_y(float a) override { m_rotate(a, 0, 1, 0); processing_rotate(_graphics_id, a, 0, 1, 0); }
    void rotate_z(float a) override { m_rotate(a, 0, 0, 1); processing_rotate(_graphics_id, a, 0, 0, 1); }
    void scale(float x, float y, float z) override { m_scale(x, y, z);
        processing_scale(_graphics_id, x, y, z); }
    void apply_matrix(const float* m) override { m_mul(m);
        processing_apply_matrix(_graphics_id, m); }
    void shear_x(float angle) override { const float t[16] = {1,0,0,0, ::std::tan(angle),1,0,0, 0,0,1,0, 0,0,0,1}; m_mul(t);
        processing_shear_x(_graphics_id, angle); }
    void shear_y(float angle) override { const float t[16] = {1,::std::tan(angle),0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}; m_mul(t);
        processing_shear_y(_graphics_id, angle); }

    // Input feed
    void input_mouse_move(float x, float y) override {
        processing_input_mouse_move(_surface_id, x, y); }
    void input_mouse_button(uint8_t button, bool pressed) override {
        processing_input_mouse_button(_surface_id, button, pressed); }
    void input_scroll(float x, float y) override {
        processing_input_scroll(_surface_id, x, y); }
    void input_key(uint32_t key_code, bool pressed) override {
        processing_input_key(_surface_id, key_code, pressed); }
    void input_char(uint32_t key_code, uint32_t codepoint) override {
        processing_input_char(_surface_id, key_code, codepoint); }
    void input_flush() override { processing_input_flush(); }

    // Input query
    float    mouse_x()                override { return processing_mouse_x(_surface_id);  }
    float    mouse_y()                override { return processing_mouse_y(_surface_id);  }
    float    pmouse_x()               override { return processing_pmouse_x(_surface_id); }
    float    pmouse_y()               override { return processing_pmouse_y(_surface_id); }
    bool     mouse_pressed()          override { return processing_mouse_is_pressed();    }
    int8_t   mouse_button()           override { return processing_mouse_button();        }
    bool     key_pressed()            override { return processing_key_is_pressed();      }
    bool     key_is_down(uint32_t kc) override { return processing_key_is_down(kc);      }
    uint32_t key()                    override { return processing_key();                 }
    uint32_t key_code()               override { return processing_key_code();            }
    float    mouse_wheel()            override { return processing_mouse_wheel();         }

    // Window
    void set_title(const std::string&) override {}  // TODO: via Bevy Window component
    bool     _drawing = false;
    uint64_t _font = 0;
    void set_size(uint32_t w, uint32_t h) override {
        processing_surface_resize(_surface_id, w, h); _width = w; _height = h; }
    bool should_close() override { return false; }  // poll_events() returning false is the signal

    // Accessors
    uint64_t    surface_id()  const override { return _surface_id;  }
    uint64_t    graphics_id() const override { return _graphics_id; }
    const char* name()        const override { return "libprocessing"; }

private:
    uint64_t _surface_id  = 0;
    uint64_t _graphics_id = 0;
    uint32_t _width       = 0;
    uint32_t _height      = 0;

public:
    // ---- cppmode-pg-offscreen: createGraphics() and saveFrame() ----
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
    // cppmode-pg-v5: read any canvas back as ARGB (Processing's pixel layout), top row first
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
        // cppmode-readback-srgb: the fork hands back linear colors; Processing pixels are sRGB
        { const uint8_t srgb_space = lp_rgba(0.0f, 0.0f, 0.0f, 0.0f).space;
          auto enc = [](float v) { v = v < 0 ? 0 : (v > 1 ? 1 : v); return v <= 0.0031308f ? 12.92f * v : 1.055f * std::pow(v, 1.0f / 2.4f) - 0.055f; };
          for (auto& c : buf) if (c.space != srgb_space) { c.c1 = enc(c.c1); c.c2 = enc(c.c2); c.c3 = enc(c.c3); c.space = srgb_space; } }
        for (size_t i = 0; i < n; i++)
            out[i] = (to8(buf[i].a) << 24) | (to8(buf[i].c1) << 16) | (to8(buf[i].c2) << 8) | to8(buf[i].c3);
        ow = w; oh = h;
        return true;
    }
    // cppmode-pg-v5: write ARGB pixels back onto a canvas (same size readback gave)
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
    // cppmode-gpu-filter: libprocessing's built-in GPU filters, one filter entity per kind, reused.
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
        // cppmode-readback-srgb: the fork hands back linear colors; Processing pixels are sRGB
        { const uint8_t srgb_space = lp_rgba(0.0f, 0.0f, 0.0f, 0.0f).space;
          auto enc = [](float v) { v = v < 0 ? 0 : (v > 1 ? 1 : v); return v <= 0.0031308f ? 12.92f * v : 1.055f * std::pow(v, 1.0f / 2.4f) - 0.055f; };
          for (auto& c : buf) if (c.space != srgb_space) { c.c1 = enc(c.c1); c.c2 = enc(c.c2); c.c3 = enc(c.c3); c.space = srgb_space; } }
        for (size_t i = 0; i < n; i++) {
            out[i*3+0] = to8(buf[i].c1); out[i*3+1] = to8(buf[i].c2); out[i*3+2] = to8(buf[i].c3);
        }
        ow = w; oh = h;
        return true;
    }
    static void check(const char* where) {
        const char* err = processing_check_error();
        if (err) { fprintf(stderr, "[libprocessing] %s: %s\n", where, err); abort(); }
    }
};
