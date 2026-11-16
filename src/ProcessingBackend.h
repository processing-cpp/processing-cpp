#pragma once
#include <cstdint>
#include <string>

// Universal rendering backend interface for CppMode.
//
// PApplet holds one ProcessingBackend* selected at startup based on how
// the sketch was compiled. All rendering, input, and lifecycle calls go
// through this interface. Adding a new backend (WebGPU, Metal, etc.)
// means implementing this interface -- nothing in Processing.cpp changes.
//
// Current implementations:
//   OpenGLBackend        -- existing GLFW + OpenGL path (default)
//   LibProcessingBackend -- libprocessing FFI (Bevy + wgpu + Vulkan)

struct ProcessingBackend {
    virtual ~ProcessingBackend() = default;

    // -------------------------------------------------------------------------
    // Lifecycle
    // -------------------------------------------------------------------------

    // Called once after the native window exists.
    // window_handle and display_handle are platform-specific:
    //   Linux X11:    window=X11 Window ID, display=X11 Display*
    //   Linux Wayland: window=wl_surface*, display=wl_display*
    //   Windows:      window=HWND, display=ignored
    //   macOS:        window=NSWindow*, display=ignored
    // OpenGLBackend ignores these (it created the window itself).
    virtual void init(uint64_t window_handle,
                      uint64_t display_handle,
                      uint32_t width,
                      uint32_t height,
                      float    scale_factor) = 0;

    virtual void destroy()                          = 0;
    virtual void resize(uint32_t width,
                        uint32_t height)            = 0;

    // -------------------------------------------------------------------------
    // Frame lifecycle
    // -------------------------------------------------------------------------

    // Called once per frame before any draw calls.
    virtual void begin_draw()  = 0;
    // Called once per frame after all draw calls. Presents the frame.
    virtual void end_draw()    = 0;
    // Flush pending geometry without presenting (for PGraphics offscreen).
    virtual void flush()       = 0;
    // Tick the backend's internal event loop. Returns false when the app
    // should exit. Called once per frame before begin_draw.
    virtual bool poll_events() = 0;

    // -------------------------------------------------------------------------
    // Color and style state
    // -------------------------------------------------------------------------

    // All color components are linear [0,1] floats.
    virtual void background(float r, float g, float b, float a) = 0;
    virtual void fill(float r, float g, float b, float a)       = 0;
    virtual void no_fill()                                       = 0;
    virtual void stroke(float r, float g, float b, float a)     = 0;
    virtual void no_stroke()                                     = 0;
    virtual void stroke_weight(float w)                          = 0;

    // -------------------------------------------------------------------------
    // 2D primitives
    // -------------------------------------------------------------------------

    virtual void rect(float x, float y, float w, float h)                        = 0;
    virtual void rect(float x, float y, float w, float h,
                      float tl, float tr, float br, float bl)                    = 0;
    virtual void ellipse(float cx, float cy, float w, float h)                   = 0;
    virtual void circle(float cx, float cy, float d)                             = 0;
    virtual void line(float x1, float y1, float x2, float y2)                   = 0;
    virtual void line(float x1, float y1, float z1,
                      float x2, float y2, float z2)                              = 0;
    virtual void point(float x, float y)                                         = 0;
    virtual void point(float x, float y, float z)                                = 0;
    virtual void triangle(float x1, float y1,
                          float x2, float y2,
                          float x3, float y3)                                    = 0;
    virtual void quad(float x1, float y1, float x2, float y2,
                      float x3, float y3, float x4, float y4)                   = 0;
    virtual void arc(float x, float y, float w, float h,
                     float start, float stop, uint8_t mode)                      = 0;
    virtual void square(float x, float y, float s)                               = 0;

    // -------------------------------------------------------------------------
    // 3D primitives
    // -------------------------------------------------------------------------

    virtual void box(float w, float h, float d)        = 0;
    virtual void sphere(float r)                       = 0;
    virtual void sphereDetail(int ures, int vres) {}  // cppmode-spheredetail

    // -------------------------------------------------------------------------
    // Transform
    // -------------------------------------------------------------------------

    virtual void push_matrix()                              = 0;
    virtual void pop_matrix()                               = 0;
    virtual void reset_matrix()                             = 0;
    virtual void translate(float x, float y, float z)      = 0;
    virtual void rotate(float angle, float x, float y,
                        float z)                            = 0;
    virtual void rotate_x(float angle)                      = 0;
    virtual void rotate_y(float angle)                      = 0;
    virtual void rotate_z(float angle)                      = 0;
    virtual void scale(float x, float y, float z)           = 0;
    virtual void apply_matrix(const float* mat4_col_major)  = 0;
    virtual void shear_x(float angle)                       = 0;
    virtual void shear_y(float angle)                       = 0;

    // -------------------------------------------------------------------------
    // Input -- feed events in from your window loop
    // -------------------------------------------------------------------------

    virtual void input_mouse_move(float x, float y)                 = 0;
    virtual void input_mouse_button(uint8_t button, bool pressed)   = 0;
    virtual void input_scroll(float x, float y)                     = 0;
    virtual void input_key(uint32_t key_code, bool pressed)         = 0;
    virtual void input_char(uint32_t key_code, uint32_t codepoint)  = 0;
    virtual void input_flush()                                       = 0;

    // -------------------------------------------------------------------------
    // Input query
    // -------------------------------------------------------------------------

    virtual float    mouse_x()                      = 0;
    virtual float    mouse_y()                      = 0;
    virtual float    pmouse_x()                     = 0;
    virtual float    pmouse_y()                     = 0;
    virtual bool     mouse_pressed()                = 0;
    virtual int8_t   mouse_button()                 = 0;
    virtual bool     key_pressed()                  = 0;
    virtual bool     key_is_down(uint32_t key_code) = 0;
    virtual uint32_t key()                          = 0;
    virtual uint32_t key_code()                     = 0;
    virtual float    mouse_wheel()                  = 0;

    // -------------------------------------------------------------------------
    // Window
    // -------------------------------------------------------------------------

    virtual void set_title(const std::string& title)    = 0;
    virtual void set_size(uint32_t w, uint32_t h)       = 0;
    virtual bool should_close()                          = 0;

    // -------------------------------------------------------------------------
    // Accessors
    // -------------------------------------------------------------------------

    // Opaque handle IDs -- meaningful only to the backend that created them.
    virtual uint64_t surface_id()  const = 0;
    virtual uint64_t graphics_id() const = 0;

    // Backend identifier for logging/debugging.
    // Text (optional; default does nothing)
    virtual void text(const std::string& s, float x, float y) { (void)s; (void)x; (void)y; }
    virtual void text_size(float size) { (void)size; }
    // h: 0 left, 1 center, 2 right   v: 0 baseline, 1 top, 2 center, 3 bottom
    virtual void text_align(int h, int v) { (void)h; (void)v; }
    virtual void text_font_file(const std::string& path) { (void)path; }
    // Letter images: when true, text is drawn as pieces of an image we supply
    // (see PApplet::drawTTFStr) instead of through the backend's own text calls.
    virtual bool glyph_images() { return false; }
    virtual uint64_t image_create_rgba(uint32_t w, uint32_t h, const unsigned char* rgba) { (void)w; (void)h; (void)rgba; return 0; }
    virtual void image_region(uint64_t img, float dx, float dy, float dw, float dh,
                              float sx, float sy, float sw, float sh) {
        (void)img; (void)dx; (void)dy; (void)dw; (void)dh; (void)sx; (void)sy; (void)sw; (void)sh; }
    virtual void image_tint(float r, float g, float b, float a) { (void)r; (void)g; (void)b; (void)a; }
    virtual void image_no_tint() {}
    // Measurements return a negative number when the backend can't answer.
    virtual float text_width(const std::string& s) { (void)s; return -1.0f; }
    virtual float text_ascent()  { return -1.0f; }
    virtual float text_descent() { return -1.0f; }
    virtual void  text_leading(float leading) { (void)leading; }
    virtual void  text_box(const std::string& s, float x, float y, float w, float h) { (void)s; (void)x; (void)y; (void)w; (void)h; }
    virtual const char* name() const = 0;
};
