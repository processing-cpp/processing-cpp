#include <mutex>
#if __has_include("stb_truetype.h")
#  define PROCESSING_HAS_STB_TRUETYPE 1
#  define STB_TRUETYPE_IMPLEMENTATION
#  if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable: 4365)
#  endif
#  include "stb_truetype.h"
#  if defined(_MSC_VER)
#    pragma warning(pop)
#  endif
#else
#  define PROCESSING_HAS_STB_TRUETYPE 0
#endif

#include "Processing.h"
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <cstdint>
#ifdef PROCESSING_USE_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#include <cstring>
#include <cstddef>
#endif
#ifndef CPPMODE_BACKEND_LIBPROCESSING
// OpenGL build: the libprocessing image branches are only reached when a
// backend exists, which never happens here. These stand-ins let them compile.
static inline uint64_t processing_image_load(const char*) { return 0; }
static inline void processing_image(uint64_t, uint64_t, float, float) {}
static inline void processing_image_scaled(uint64_t, uint64_t, float, float, float, float) {}
// BEGIN CPPMODE LIBPROCESSING STUBS (generated)
#ifndef CPPMODE_BACKEND_LIBPROCESSING
template<class... A> inline uint64_t processing_background_image(A&&...) { return 0; }
template<class... A> inline uint64_t processing_begin_contour(A&&...) { return 0; }
template<class... A> inline uint64_t processing_begin_shape(A&&...) { return 0; }
template<class... A> inline uint64_t processing_camera(A&&...) { return 0; }
template<class... A> inline uint64_t processing_check_error(A&&...) { return 0; }
template<class... A> inline uint64_t processing_clear(A&&...) { return 0; }
template<class... A> inline uint64_t processing_clip(A&&...) { return 0; }
template<class... A> inline uint64_t processing_cursor(A&&...) { return 0; }
template<class... A> inline uint64_t processing_depth_test(A&&...) { return 0; }
template<class... A> inline uint64_t processing_ellipse_mode(A&&...) { return 0; }
template<class... A> inline uint64_t processing_end_contour(A&&...) { return 0; }
template<class... A> inline uint64_t processing_end_shape(A&&...) { return 0; }
template<class... A> inline uint64_t processing_filter_create(A&&...) { return 0; }
template<class... A> inline uint64_t processing_geometry_color(A&&...) { return 0; }
template<class... A> inline uint64_t processing_geometry_create(A&&...) { return 0; }
template<class... A> inline uint64_t processing_geometry_destroy(A&&...) { return 0; }
template<class... A> inline uint64_t processing_geometry_normal(A&&...) { return 0; }
template<class... A> inline uint64_t processing_geometry_uv(A&&...) { return 0; }
template<class... A> inline uint64_t processing_geometry_vertex(A&&...) { return 0; }
template<class... A> inline uint64_t processing_get_matrix(A&&...) { return 0; }
template<class... A> inline uint64_t processing_graphics_apply_filter(A&&...) { return 0; }
template<class... A> inline uint64_t processing_graphics_readback(A&&...) { return 0; }
template<class... A> inline uint64_t processing_graphics_set(A&&...) { return 0; }
template<class... A> inline uint64_t processing_graphics_set_msaa(A&&...) { return 0; }
template<class... A> inline uint64_t processing_graphics_update_region(A&&...) { return 0; }
template<class... A> inline uint64_t processing_image_set_sampler(A&&...) { return 0; }
template<class... A> inline uint64_t processing_light_destroy(A&&...) { return 0; }
template<class... A> inline uint64_t processing_light_set_ambient(A&&...) { return 0; }
template<class... A> inline uint64_t processing_material(A&&...) { return 0; }
template<class... A> inline uint64_t processing_material_create_custom(A&&...) { return 0; }
template<class... A> inline uint64_t processing_material_create_pbr(A&&...) { return 0; }
template<class... A> inline uint64_t processing_material_set_alpha_mode(A&&...) { return 0; }
template<class... A> inline uint64_t processing_material_set_double_sided(A&&...) { return 0; }
template<class... A> inline uint64_t processing_material_set_float4(A&&...) { return 0; }
template<class... A> inline uint64_t processing_material_set_texture(A&&...) { return 0; }
template<class... A> inline uint64_t processing_material_set_unlit(A&&...) { return 0; }
template<class... A> inline uint64_t processing_mode_3d(A&&...) { return 0; }
template<class... A> inline uint64_t processing_model(A&&...) { return 0; }
template<class... A> inline uint64_t processing_model_x(A&&...) { return 0; }
template<class... A> inline uint64_t processing_model_y(A&&...) { return 0; }
template<class... A> inline uint64_t processing_model_z(A&&...) { return 0; }
template<class... A> inline uint64_t processing_no_clip(A&&...) { return 0; }
template<class... A> inline uint64_t processing_no_cursor(A&&...) { return 0; }
template<class... A> inline uint64_t processing_ortho(A&&...) { return 0; }
template<class... A> inline uint64_t processing_perspective(A&&...) { return 0; }
template<class... A> inline uint64_t processing_pop_matrix(A&&...) { return 0; }
template<class... A> inline uint64_t processing_pop_style(A&&...) { return 0; }
template<class... A> inline uint64_t processing_push_matrix(A&&...) { return 0; }
template<class... A> inline uint64_t processing_push_style(A&&...) { return 0; }
template<class... A> inline uint64_t processing_rect_mode(A&&...) { return 0; }
template<class... A> inline uint64_t processing_reset_material(A&&...) { return 0; }
template<class... A> inline uint64_t processing_screen_x(A&&...) { return 0; }
template<class... A> inline uint64_t processing_screen_y(A&&...) { return 0; }
template<class... A> inline uint64_t processing_screen_z(A&&...) { return 0; }
template<class... A> inline uint64_t processing_set_blend_mode(A&&...) { return 0; }
template<class... A> inline uint64_t processing_set_fill(A&&...) { return 0; }
template<class... A> inline uint64_t processing_set_stroke_cap(A&&...) { return 0; }
template<class... A> inline uint64_t processing_set_stroke_join(A&&...) { return 0; }
template<class... A> inline uint64_t processing_shader_create(A&&...) { return 0; }
template<class... A> inline uint64_t processing_shader_set_float(A&&...) { return 0; }
template<class... A> inline uint64_t processing_shader_set_int(A&&...) { return 0; }
template<class... A> inline uint64_t processing_shader_set_vec2(A&&...) { return 0; }
template<class... A> inline uint64_t processing_shader_set_vec3(A&&...) { return 0; }
template<class... A> inline uint64_t processing_shader_set_vec4(A&&...) { return 0; }
template<class... A> inline uint64_t processing_vertex(A&&...) { return 0; }
#endif
// END CPPMODE LIBPROCESSING STUBS
#endif
#include <map>
#include <array>
// defined with the light helpers inside namespace Processing
namespace Processing { static uint64_t lpLitTexMatFor(uint64_t, float, float, float, float); static bool lpLitOn();
                       static void lpLitForgetSent();
                       static void lpLitFillChanged(float, float, float, float);
                       static void lpSetEye(float, float, float); }
using Processing::lpLitFillChanged;
using Processing::lpLitTexMatFor;
using Processing::lpLitOn;
using Processing::lpLitForgetSent;
using Processing::lpSetEye;
// cppmode-texsmooth: smooth()/noSmooth() under libprocessing, applied once the backend exists
static int g_lpMsaaWanted = -1;
static int lpMsaaSamples(bool on, int level) { return (!on || level <= 1) ? 0 : 4; }  // 4x is the count every GPU supports
#ifdef CPPMODE_BACKEND_LIBPROCESSING
// the window handle libprocessing draws into: HWND on Windows, NSWindow on
// macOS, X11 or Wayland on Linux
#  if defined(_WIN32)
#    define GLFW_EXPOSE_NATIVE_WIN32
#  elif defined(__APPLE__)
#    define GLFW_EXPOSE_NATIVE_COCOA
#  else
#    define GLFW_EXPOSE_NATIVE_X11
#    define GLFW_EXPOSE_NATIVE_WAYLAND
#  endif
#  include <GLFW/glfw3native.h>
#  include "LibProcessingBackend.h"
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4365 4245 4100 4189)
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
// Remap unsupported GL primitives to WebGL-compatible equivalents
#undef GL_QUADS
#define GL_QUADS GL_TRIANGLES
#undef GL_QUAD_STRIP
#define GL_QUAD_STRIP GL_TRIANGLE_STRIP
#endif

#include <csignal>
#include <cstdlib>
#include <cmath>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <chrono>
#ifdef __APPLE__
#include <objc/runtime.h>
#include <objc/message.h>
#include <dlfcn.h>
#endif

#ifndef _WIN32
#include <thread>
#endif

#include <vector>
#include <array>
#include <string>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <memory>

// stb_image.h must be in the src/ folder for loadImage() to work.
// Download from: https://github.com/nothings/stb/blob/master/stb_image.h
#ifdef PROCESSING_HAS_STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

// Uncomment + drop stb_image_write.h to enable saveFrame()/save():
// #define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4365)
#endif
#include "stb_image_write.h"
#ifdef _MSC_VER
#pragma warning(pop)
#endif
// cppmode-savefile: .tif/.tiff are written as real (uncompressed RGB) TIFF files. They used
// to be TGA data under a .tif name, which nothing could open. Other extensions keep TGA.
static int cppmodeWriteTgaOrTiff(const char* fn, const ::std::string& ext, int w, int h, const unsigned char* rgb) {
    if (ext != ".tif" && ext != "tiff" && ext != ".tiff") return stbi_write_tga(fn, w, h, 3, rgb);
    FILE* f = fopen(fn, "wb"); if (!f) return 0;
    auto u16 = [&](unsigned v) { unsigned char b[2] = {(unsigned char)v, (unsigned char)(v >> 8)}; fwrite(b, 1, 2, f); };
    auto u32 = [&](unsigned v) { unsigned char b[4] = {(unsigned char)v, (unsigned char)(v >> 8), (unsigned char)(v >> 16), (unsigned char)(v >> 24)}; fwrite(b, 1, 4, f); };
    unsigned dataSize = (unsigned)w * (unsigned)h * 3, nEntries = 10;
    unsigned ifd = 8 + dataSize, bpsOff = ifd + 2 + nEntries * 12 + 4;
    fwrite("II", 1, 2, f); u16(42); u32(ifd);
    fwrite(rgb, 1, dataSize, f);
    u16(nEntries);
    auto entry = [&](unsigned tag, unsigned type, unsigned count, unsigned value) { u16(tag); u16(type); u32(count); u32(value); };
    entry(256, 4, 1, (unsigned)w);            // width
    entry(257, 4, 1, (unsigned)h);            // height
    entry(258, 3, 3, bpsOff);                 // bits per sample -> 8,8,8 below
    entry(259, 3, 1, 1);                      // no compression
    entry(262, 3, 1, 2);                      // RGB
    entry(273, 4, 1, 8);                      // strip offset
    entry(277, 3, 1, 3);                      // samples per pixel
    entry(278, 4, 1, (unsigned)h);            // rows per strip
    entry(279, 4, 1, dataSize);               // strip byte count
    entry(284, 3, 1, 1);                      // chunky
    u32(0);
    u16(8); u16(8); u16(8);
    fclose(f); return 1;
}
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

// ── Manual glu replacements (no GLU header needed) ───────────────────────────
static void _gluPerspective(double fovY_deg, double aspect, double zNear, double zFar) {
    // Identical to gluPerspective -- sets up a perspective projection matrix
    double f = 1.0 / std::tan(fovY_deg * M_PI / 360.0);
    double m[16] = {0};
    m[0]  = f / aspect;
    m[5]  = f;
    m[10] = (zFar + zNear) / (zNear - zFar);
    m[11] = -1.0;
    m[14] = (2.0 * zFar * zNear) / (zNear - zFar);
    glMultMatrixd(m);
}
static void _gluLookAt(double ex,double ey,double ez,
                       double cx,double cy,double cz,
                       double ux,double uy,double uz) {
    // Identical to gluLookAt
    double fx=cx-ex, fy=cy-ey, fz=cz-ez;
    double len=std::sqrt(fx*fx+fy*fy+fz*fz); fx/=len;fy/=len;fz/=len;
    double rx=fy*uz-fz*uy, ry=fz*ux-fx*uz, rz=fx*uy-fy*ux;
    len=std::sqrt(rx*rx+ry*ry+rz*rz); rx/=len;ry/=len;rz/=len;
    double upx=ry*fz-rz*fy, upy=rz*fx-rx*fz, upz=rx*fy-ry*fx;
    double m[16]={
        rx,  upx, -fx, 0,
        ry,  upy, -fy, 0,
        rz,  upz, -fz, 0,
        0,   0,   0,   1
    };
    glMultMatrixd(m);
    glTranslated(-ex,-ey,-ez);
}

// stb_truetype -- drop stb_truetype.h next to this file for TTF font rendering.
// Workaround for MinGW 14+ locale initialization crash on Windows
#ifdef _WIN32
struct _LocaleInit {
    _LocaleInit() {
        // Initialize locale early to prevent crash in ios_base::_M_init
        try { ::std::locale::global(::std::locale("C")); } catch(...) {}
    }
};
static _LocaleInit _locale_init_instance;
#endif

// On Windows, avoid std::cerr/cout which crash during ios_base::_M_init
// when no console is attached (MinGW 14+ bug with -mwindows)
#ifdef _WIN32
  #define CPP_ERR(...) fprintf(stderr, __VA_ARGS__)
  #define CPP_OUT(...) fprintf(stdout, __VA_ARGS__)
#else
  #define CPP_ERR(...) fprintf(stderr, __VA_ARGS__)
  #define CPP_OUT(...) fprintf(stdout, __VA_ARGS__)
#endif

// default.ttf in the project root is loaded automatically as the default font.


// =============================================================================
// STATE
// =============================================================================

// Window / canvas size
static int fbW  = 640, fbH       = 480;    // actual framebuffer (may be 2× on HiDPI)

// Mouse state

// Keyboard state

// Frame timing

// Current draw style

// Lighting state

// Color mode
// User event callbacks
std::function<void()>    _onKeyPressed;
std::function<void()>    _onKeyReleased;
std::function<void()>    _onKeyTyped;
std::function<void()>    _onMousePressed;
std::function<void()>    _onMouseReleased;
std::function<void()>    _onMouseClicked;
std::function<void()>    _onMouseMoved;
std::function<void()>    _onMouseDragged;
std::function<void(int)> _onMouseWheel;
std::function<void()>    _onWindowMoved;
// On Windows: IDE.cpp stores a raw function pointer here before static init
// of Processing.cpp completes. Raw function pointers are POD -- zero-initialized
// at program start before ANY constructor runs, so this is always safe to write.

// Shape state


// =============================================================================
// NOISE - exact Java Processing implementation (PApplet.java)
// Uses a 4096-entry float lookup table with cosine interpolation.
// This matches Processing Java's noise() output exactly.
// =============================================================================


namespace Processing {

static FILE* processingOpenFile(const char* path, const char* mode) {
#ifdef _WIN32
    FILE* file = nullptr;
    return ::fopen_s(&file, path, mode) == 0 ? file : nullptr;
#else
    return ::fopen(path, mode);
#endif
}

static FILE* processingOpenPipe(const char* command, const char* mode) {
#ifdef _WIN32
    return ::_popen(command, mode);
#else
    return ::popen(command, mode);
#endif
}

static int processingClosePipe(FILE* pipe) {
#ifdef _WIN32
    return ::_pclose(pipe);
#else
    return ::pclose(pipe);
#endif
}

static void _doEnableDebugConsole() {
#ifdef _WIN32
    if (AllocConsole()) {
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);
        freopen_s(&f, "CONIN$",  "r", stdin);
        fprintf(stderr, "[debug] processing-cpp debug console enabled\n");
    }
#endif
}
void enableDebugConsole() { _doEnableDebugConsole(); }

static ::std::string* _s_objDir_ptr = nullptr;
static ::std::string& _s_objDir = *(_s_objDir_ptr ? _s_objDir_ptr : (_s_objDir_ptr = new ::std::string()));

static const int PERLIN_YWRAPB = 4;
static const int PERLIN_YWRAP  = 1 << PERLIN_YWRAPB;  // 16
static const int PERLIN_ZWRAPB = 8;
static const int PERLIN_ZWRAP  = 1 << PERLIN_ZWRAPB;  // 256
static const int PERLIN_SIZE   = 4095;

void PApplet::initPerlin(unsigned int seed) {
    // Java Processing seeds with a simple LCG and fills with rand values in [0,1)
    // It uses its own random to not disturb the sketch's random()
    uint32_t s = seed;
    auto jrand = [&]() -> float {
        s = s * 1664525u + 1013904223u; // LCG like Java's Random
        return (s >> 8) / (float)(1 << 24);
    };
    for (int i = 0; i < PERLIN_SIZE + 1; i++)
        perlinTable[i] = jrand();
    perlinInit = true;
}

void PApplet::noiseSeed(int s) { initPerlin((unsigned int)s); }
void PApplet::noiseDetail(int o, float f) { noiseOctaves = o; noiseFalloff = f; }

// Cosine interpolation curve -- Java Processing's noise_fsc()
static inline float noise_fsc(float i) {
    return 0.5f * (1.0f - ::std::cos(i * PI));
}

float PApplet::noise(float x, float y, float z) {
    if (!perlinInit) initPerlin(0);  // default seed 0 like Java
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    if (z < 0) z = -z;
    int xi = (int)x, yi = (int)y, zi = (int)z;
    float xf = x - xi, yf = y - yi, zf = z - zi;
    float r = 0.0f, ampl = 0.5f;
    for (int oct = 0; oct < noiseOctaves; oct++) {
        int of = xi + (yi << PERLIN_YWRAPB) + (zi << PERLIN_ZWRAPB);
        float rxf = noise_fsc(xf), ryf = noise_fsc(yf);
        float n1 = perlinTable[of & PERLIN_SIZE];
        n1 += rxf * (perlinTable[(of+1) & PERLIN_SIZE] - n1);
        float n2 = perlinTable[(of + PERLIN_YWRAP) & PERLIN_SIZE];
        n2 += rxf * (perlinTable[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n2);
        n1 += ryf * (n2 - n1);
        of += PERLIN_ZWRAP;
        n2 = perlinTable[of & PERLIN_SIZE];
        n2 += rxf * (perlinTable[(of+1) & PERLIN_SIZE] - n2);
        float n3 = perlinTable[(of + PERLIN_YWRAP) & PERLIN_SIZE];
        n3 += rxf * (perlinTable[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n3);
        n2 += ryf * (n3 - n2);
        n1 += noise_fsc(zf) * (n2 - n1);
        r   += n1 * ampl;
        ampl *= noiseFalloff;
        // Double frequency each octave
        xi <<= 1; xf *= 2.0f; if (xf >= 1.0f) { xi++; xf--; }
        yi <<= 1; yf *= 2.0f; if (yf >= 1.0f) { yi++; yf--; }
        zi <<= 1; zf *= 2.0f; if (zf >= 1.0f) { zi++; zf--; }
    }
    return r;
}
float PApplet::noise(float x)           { return noise(x, 0.0f, 0.0f); }
float PApplet::noise(float x, float y)  { return noise(x, y,    0.0f); }

// Seeded random -- Mersenne Twister for reproducibility matching Java Processing

void PApplet::randomSeed(long s) {
    _rng.seed(static_cast<uint32_t>(s));
    _rngDist.reset();
}
float PApplet::random(float lo, float hi) {
    return lo + _rngDist(_rng) * (hi - lo);
}
float PApplet::random(float hi) { return random(0.f, hi); }

float PApplet::randomGaussian(){
    static float spare; static bool has=false;
    if(has){has=false;return spare;}
    float u,v,s;
    do{u=_rngDist(_rng)*2-1;v=_rngDist(_rng)*2-1;s=u*u+v*v;}while(s>=1||s==0);
    s=::std::sqrt(-2*::std::log(s)/s);spare=v*s;has=true;return u*s;
}

// =============================================================================
// COLOR MODE & HELPERS
// =============================================================================

void PApplet::hsbToRgb(float h, float s, float b, float& outR, float& outG, float& outB) {
    // Normalise each channel to [0, 1]
    h /= colorMaxH;
    s /= colorMaxS;
    b /= colorMaxB;

    if (s == 0.0f) {
        // Achromatic (grey): all channels equal brightness
        outR = outG = outB = b;
        return;
    }

    float sector  = h * 6.0f;           // which 60-degree sector of the hue wheel
    int   i       = (int)sector;
    float frac    = sector - i;          // fractional part within sector

    float p = b * (1.0f - s);
    float q = b * (1.0f - s * frac);
    float t = b * (1.0f - s * (1.0f - frac));

    switch (i % 6) {
        case 0:  outR = b; outG = t; outB = p; break;
        case 1:  outR = q; outG = b; outB = p; break;
        case 2:  outR = p; outG = b; outB = t; break;
        case 3:  outR = p; outG = q; outB = b; break;
        case 4:  outR = t; outG = p; outB = b; break;
        default: outR = b; outG = p; outB = q; break;
    }
}

color PApplet::makeColor(float a, float b, float c, float d) {
    float r = 0, g = 0, bv = 0, aa = 0;
    if (colorModeVal == HSB) {
        hsbToRgb(a, b, c, r, g, bv);
        aa = d / colorMaxA;
    } else {
        r  = a / colorMaxH;
        g  = b / colorMaxS;
        bv = c / colorMaxB;
        aa = d / colorMaxA;
    }
    return colorVal((int)(r*255), (int)(g*255), (int)(bv*255), (int)(aa*255));
}
color PApplet::makeColor(float gray,float alpha){
    // In HSB mode, a single-value gray maps to brightness only (hue=0, sat=0)
    // matching Processing Java behavior -- background(v) in HSB gives gray
    if(colorModeVal==HSB){
        float br=gray/colorMaxB;
        br=::std::fmax(0.f,::std::fmin(1.f,br));
        int v=(int)(br*255);
        unsigned int a=static_cast<unsigned int>(::std::fmax(0.f,::std::fmin(1.f,alpha/colorMaxA))*255.0f);
        return colorVal(v,v,v,static_cast<int>(a));
    }
    return makeColor(gray,gray,gray,alpha);
}

// =============================================================================
// color STRUCT CONSTRUCTORS
// =============================================================================
// File-scope shims so color:: constructors can call makeColor
static color _makeColor(float a,float b,float c,float d=255){
    if(PApplet::g_papplet) return PApplet::g_papplet->makeColor(a,b,c,d);
    return colorVal((int)a,(int)b,(int)c,(int)d);
}
static color _makeColor(float gray,float alpha=255){
    if(PApplet::g_papplet) return PApplet::g_papplet->makeColor(gray,alpha);
    return colorVal((int)gray,(int)gray,(int)gray,(int)alpha);
}
static float _colorMaxA(){
    return PApplet::g_papplet ? PApplet::g_papplet->colorMaxA : 255.f;
}

color::color(int gray)              { value = _makeColor((float)gray, _colorMaxA()).value; }
color::color(int gray, int a)       { value = _makeColor((float)gray, (float)a).value; }
color::color(int r, int g, int b)   { value = _makeColor((float)r, (float)g, (float)b, _colorMaxA()).value; }
color::color(int r, int g, int b, int a) { value = _makeColor((float)r, (float)g, (float)b, (float)a).value; }
color::color(float gray)            { value = _makeColor(gray, _colorMaxA()).value; }
color::color(float gray, float a)   { value = _makeColor(gray, a).value; }
color::color(float r,float g,float b){ value = _makeColor(r,g,b,_colorMaxA()).value; }
color::color(float r,float g,float b,float a){ value = _makeColor(r,g,b,a).value; }

void PApplet::colorMode(int mode, float mx) { colorModeVal=mode; colorMaxH=colorMaxS=colorMaxB=colorMaxA=mx; }
void PApplet::colorMode(int mode, float mH, float mS, float mB, float mA) { colorModeVal=mode; colorMaxH=mH; colorMaxS=mS; colorMaxB=mB; colorMaxA=mA; }

// Color channel accessors -- scaled to current colorMode range
float PApplet::red(color c)       { unsigned int v=c.value; return (v>>16&0xFF)/255.0f*colorMaxH; }
float PApplet::green(color c)     { unsigned int v=c.value; return (v>>8&0xFF)/255.0f*colorMaxS; }
float PApplet::blue(color c)      { unsigned int v=c.value; return (v&0xFF)/255.0f*colorMaxB; }
float PApplet::alpha(color c)     { unsigned int v=c.value; return (v>>24&0xFF)/255.0f*colorMaxA; }
float PApplet::brightness(color c) {
    unsigned int v=c.value;
    float r=(v>>16&0xFF)/255.f, g=(v>>8&0xFF)/255.f, b=(v&0xFF)/255.f;
    return max(r, max(g, b)) * colorMaxB;
}
float PApplet::saturation(color c) {
    unsigned int v = c.value;
    float r  = (v >> 16 & 0xFF) / 255.f;
    float g  = (v >>  8 & 0xFF) / 255.f;
    float b  = (v       & 0xFF) / 255.f;
    float mx = max(r, max(g, b));
    float mn = min(r, min(g, b));
    return (mx == 0 ? 0 : (mx - mn) / mx) * colorMaxS;
}
float PApplet::hue(color c){
    unsigned int v=c.value;
    float r=(v>>16&0xFF)/255.f,g=(v>>8&0xFF)/255.f,b=(v&0xFF)/255.f;
    float mx=max(r,max(g,b)),mn=min(r,min(g,b)),d=mx-mn;
    if(d==0)return 0;
    float h=(mx==r)?(g-b)/d:(mx==g)?2+(b-r)/d:4+(r-g)/d;
    h*=60;if(h<0)h+=360;return h/360.0f*colorMaxH;
}
color PApplet::lerpColor(color c1, color c2, float t) {
    unsigned int v1 = c1.value;
    unsigned int v2 = c2.value;

    int r1 = (v1 >> 16) & 0xFF,  r2 = (v2 >> 16) & 0xFF;
    int g1 = (v1 >>  8) & 0xFF,  g2 = (v2 >>  8) & 0xFF;
    int b1 =  v1        & 0xFF,  b2 =  v2        & 0xFF;
    int a1 = (v1 >> 24) & 0xFF,  a2 = (v2 >> 24) & 0xFF;

    return colorVal(
        (int)(r1 + t * (r2 - r1)),
        (int)(g1 + t * (g2 - g1)),
        (int)(b1 + t * (b2 - b1)),
        (int)(a1 + t * (a2 - a1))
    );
}

// =============================================================================
// INTERNAL HELPERS
// =============================================================================

// cppmode-glblend: the blend mode blendMode() set. applyFill/applyStroke and the image and
// text paths used to reset GL to normal alpha blending on every draw, so ADD, MULTIPLY
// and the rest never showed
static int g_glBlendMode = BLEND;
static void glApplyBlend(int mode) {
    glEnable(GL_BLEND);
    // Reset equation first (SUBTRACT changes it)
    glBlendEquation(GL_FUNC_ADD);
    switch (mode) {
        case BLEND:    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
        case ADD:      glBlendFunc(GL_SRC_ALPHA, GL_ONE);                break;
        case MULTIPLY: glBlendFunc(GL_DST_COLOR, GL_ZERO);               break;
        case SCREEN:   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);      break;
        case DARKEST:  glBlendFunc(GL_ONE, GL_ONE); glBlendEquation(GL_MIN); break;
        case LIGHTEST: glBlendFunc(GL_ONE, GL_ONE); glBlendEquation(GL_MAX); break;
        case DIFFERENCE:
            glBlendEquation(GL_FUNC_SUBTRACT);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case SUBTRACT:
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case REPLACE:  glBlendFunc(GL_ONE, GL_ZERO);                     break;
        default:       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
    }
}
void PApplet::applyFill() {
    glColor4f(fillR, fillG, fillB, fillA);
    // Always enable blend -- shapes with alpha need it, opaque shapes don't hurt
    glEnable(GL_BLEND);
    glApplyBlend(g_glBlendMode);
}

// Temporarily suspend lighting so stroke lines/points render with their exact
// colour. Processing Java does the same -- strokes are never affected by lights.
void PApplet::applyStroke() {
    if (lightsEnabled) {
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
    }
    glColor4f(strokeR, strokeG, strokeB, strokeA);
    glEnable(GL_BLEND);
    glApplyBlend(g_glBlendMode);
}

// Restore lighting after a stroke draw call.
void PApplet::restoreLighting() {
    if (lightsEnabled) {
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
        // Reapply fill colour so the next lit shape uses the right material
        glColor4f(fillR, fillG, fillB, fillA);
    }
}
void PApplet::initPersistFBO(){
    if(_backend) return;
    if(_backend) return; // backend owns framebuffer
    if(persistFBO){glDeleteFramebuffers(1,&persistFBO);glDeleteTextures(1,&persistTex);persistFBO=0;}
    glGenFramebuffers(1,&persistFBO);
    glGenTextures(1,&persistTex);
    glBindTexture(GL_TEXTURE_2D,persistTex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,fbW,fbH,0,GL_RGBA,GL_UNSIGNED_BYTE,nullptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D,0);
    glBindFramebuffer(GL_FRAMEBUFFER,persistFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,persistTex,0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

// Copy current back buffer into persist FBO
void PApplet::saveToPersist(){
    if(_backend) return;
    if(_backend) return;
    if(!persistFBO) initPersistFBO();
    // Blit back buffer -> persist FBO
    glBindFramebuffer(GL_READ_FRAMEBUFFER,0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,persistFBO);
    glBlitFramebuffer(0,0,fbW,fbH,0,0,fbW,fbH,GL_COLOR_BUFFER_BIT,GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

// Restore persist FBO into back buffer
void PApplet::restoreFromPersist(){
    if(_backend) return;
    if(_backend) return;
    if(!persistFBO) return;
    glBindFramebuffer(GL_READ_FRAMEBUFFER,persistFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    glBlitFramebuffer(0,0,fbW,fbH,0,0,fbW,fbH,GL_COLOR_BUFFER_BIT,GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void PApplet::_restoreMainCanvas(){
    // Restore main window viewport and projection after PGraphics endDraw()
    glViewport(0,0,fbW,fbH);
    glMatrixMode(GL_PROJECTION);glLoadIdentity();
    glOrtho(0,logicalW,logicalH,0,-1,1);
    glMatrixMode(GL_MODELVIEW);glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}

void PApplet::setProjection(int,int){
    // Viewport = actual framebuffer size (handles HiDPI where fb > logical).
    // Ortho = logical sketch size (coordinates always match what size() set).
    if (gWindow) {
        int fw=logicalW, fh=logicalH;
        glfwGetFramebufferSize(gWindow, &fw, &fh);
        if (fw > 0) fbW = fw;
        if (fh > 0) fbH = fh;
    }
    glViewport(0, 0, fbW, fbH);
    glMatrixMode(GL_PROJECTION);glLoadIdentity();
    glOrtho(0, logicalW, logicalH, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}

void PApplet::drawEllipseGeom(float cx,float cy,float rx,float ry,
                             float sa,float ea,int segs){
    if(segs < 0){
        float maxR = rx > ry ? rx : ry;
        segs = (int)(maxR * 2.5f);
        if(segs < 64)  segs = 64;
        if(segs > 512) segs = 512;
    }
    float range=ea-sa;
    bool isFullCircle = (::std::fabs(range) >= TWO_PI - 0.001f);
    if(doFill){
        applyFill();
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx,cy); // center
        for(int i=0;i<=segs;i++){
            float a=sa+range*i/segs;
            glVertex2f(cx+rx*::std::cos(a),cy+ry*::std::sin(a));
        }
        if(isFullCircle){
            // close the circle back to the first arc point
            glVertex2f(cx+rx*::std::cos(sa),cy+ry*::std::sin(sa));
        }
        glEnd();
    }
    if(doStroke){
        applyStroke();glLineWidth(strokeW);
        if(isFullCircle){
            glBegin(GL_LINE_LOOP);
        } else {
            glBegin(GL_LINE_STRIP);
        }
        for(int i=0;i<=segs;i++){
            float a=sa+range*i/segs;
            glVertex2f(cx+rx*::std::cos(a),cy+ry*::std::sin(a));
        }
        glEnd();
    }
}

void PApplet::resolveRect(float& x,float& y,float& w,float& h){
    if(currentRectMode==CENTER){x-=w*0.5f;y-=h*0.5f;}
    else if(currentRectMode==RADIUS){x-=w;y-=h;w*=2;h*=2;}
    else if(currentRectMode==CORNERS){w=w-x;h=h-y;}
}
void PApplet::resolveEllipse(float& cx,float& cy,float& rx,float& ry){
    if(currentEllipseMode==CORNER){rx*=0.5f;ry*=0.5f;cx+=rx;cy+=ry;}  /* cppmode-glshapes2: w/h are the full size, like Java */
    else if(currentEllipseMode==CORNERS){float ex=rx,ey=ry;rx=(ex-cx)*0.5f;ry=(ey-cy)*0.5f;cx=(cx+ex)*0.5f;cy=(cy+ey)*0.5f;}
    else if(currentEllipseMode==CENTER){rx*=0.5f;ry*=0.5f;}
}
void PApplet::setFillFromColor(color c) {
    unsigned int v = c.value;
    fillR = (v >> 16 & 0xFF) / 255.f;
    fillG = (v >>  8 & 0xFF) / 255.f;
    fillB = (v       & 0xFF) / 255.f;
    fillA = (v >> 24 & 0xFF) / 255.f;
    doFill = true;
    if (_backend) { _backend->fill(fillR, fillG, fillB, fillA); lpLitFillChanged(fillR, fillG, fillB, fillA); }
}
void PApplet::setStrokeFromColor(color c) {
    unsigned int v = c.value;
    strokeR = (v >> 16 & 0xFF) / 255.f;
    strokeG = (v >>  8 & 0xFF) / 255.f;
    strokeB = (v       & 0xFF) / 255.f;
    strokeA = (v >> 24 & 0xFF) / 255.f;
    doStroke = true;
    if (_backend) _backend->stroke(strokeR, strokeG, strokeB, strokeA);
}

// =============================================================================
// ENVIRONMENT
// =============================================================================

// cppmode-java2dfill: per-vertex fill() blends only in P2D and P3D, like Java. The default
// renderer (JAVA2D) fills a polygon with the last fill, and each triangle or quad of the
// explicit kinds with the fill at its last vertex.
static int g_cppRendererKind = 0;
static bool cppBlendsVertexColors() { return g_cppRendererKind == P2D || g_cppRendererKind == P3D; }
void PApplet::size(int w,int h){
    winWidth=w;winHeight=h;
    logicalW=w;logicalH=h;  // remember what the sketch requested
    pixelWidth=w;pixelHeight=h;
    if(gWindow && _backend){
        // libprocessing path: resize the window and tell the surface
        { int cw = 0, ch = 0; glfwGetWindowSize(gWindow, &cw, &ch);
          static bool warned = false;
          if (!warned && (cw != w || ch != h)) { warned = true;
            fprintf(stderr, "[CppMode] size(%d, %d) was called after the window was created at %d x %d. "
                            "With the libprocessing renderer the picture may look stretched. "
                            "Put size() on its own line in setup(), or in settings().\n", w, h, cw, ch); } }
        glfwSetWindowSize(gWindow,w,h);
        _backend->set_size((uint32_t)w,(uint32_t)h);
    }
    if(gWindow && !_backend){
        glfwSetWindowSize(gWindow,w,h);
        // Poll until the framebuffer is actually the requested size.
        // glfwSetWindowSize is async; without this the window stays 100x100
        // when setup() calls background() or draws anything.
        // Poll until the window is actually resized (or timeout after 500ms)
        for(int _wait=0; _wait<500; _wait++){
            glfwPollEvents();
            int fw=0,fh=0;
            glfwGetFramebufferSize(gWindow,&fw,&fh);
            // Accept exact match or close match (HiDPI scaling)
            if(fw>0 && fh>0){
                pixelWidth=fw; pixelHeight=fh;
                // Check if the logical window size matches what we requested
                int lw=0,lh=0;
                glfwGetWindowSize(gWindow,&lw,&lh);
                if(lw==w && lh==h) break;  // exact match
            }
            if(_wait>=50){ // after 50ms, accept whatever we have
                int fw2=0,fh2=0;
                glfwGetFramebufferSize(gWindow,&fw2,&fh2);
                if(fw2>0&&fh2>0){ pixelWidth=fw2; pixelHeight=fh2; }
                break;
            }
            #ifdef _WIN32
            Sleep(1);
            #elif !defined(__EMSCRIPTEN__)
            #ifdef _WIN32
            Sleep(1);
            #else
            ::std::this_thread::sleep_for(::std::chrono::milliseconds(1));
            #endif
            #endif
        }
        // Force coordinate system to requested size regardless of WM.
        // logicalW/H are the sketch's coordinate space; viewport uses actual size.
        logicalW = w; logicalH = h;
        winWidth = w; winHeight = h;
        setProjection(w,h);
    }
}
void PApplet::size(int w,int h,int renderer){
    defaultP3D=(renderer==P3D); g_cppRendererKind=renderer;  /* cppmode-java2dfill */
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    // P3D gets libprocessing's 3D camera (Java's P3D defaults: 60 degree fov,
    // camera in front of the sketch). Without it, the 2D camera clips anything behind z = 0.
    if (defaultP3D && _backend) processing_mode_3d(_backend->graphics_id());
#endif
    size(w,h);
    // For P3D mode: set up depth test and apply default camera/perspective
    // immediately so setup() draws with the correct projection.
    if(defaultP3D && gWindow){
        {int fw=logicalW,fh=logicalH;if(gWindow)glfwGetFramebufferSize(gWindow,&fw,&fh);if(fw>0)fbW=fw;if(fh>0)fbH=fh;}
        glViewport(0,0,fbW,fbH);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDisable(GL_CULL_FACE);
        glFrontFace(GL_CW);
            glClearColor(0.8f,0.8f,0.8f,1); // Java Processing default grey (204,204,204)
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        applyDefaultCamera();
    }
}
// cppmode-select: fullScreen() sizes the sketch to the display first (width/height used to stay
// 640x480), and a call in settings() is applied once the window exists (it stayed windowed).
static bool g_fullScreenPending = false;
// cppmode-fsdisplay: which monitor fullScreen() uses. 0 = primary, 1..N = that monitor, -1 = SPAN.
static int g_fsDisplay = 0;
static GLFWmonitor* cppmodeFsMonitor() {
    int n = 0; GLFWmonitor** ms = glfwGetMonitors(&n);
    if (g_fsDisplay >= 1 && g_fsDisplay <= n) return ms[g_fsDisplay - 1];
    if (g_fsDisplay >= 1) fprintf(stderr, "[CppMode] fullScreen: display %d not found (%d connected), using the primary one\n", g_fsDisplay, n);
    return glfwGetPrimaryMonitor();
}
// Bounding box of every monitor, for fullScreen(SPAN).
static bool cppmodeSpanRect(int& x, int& y, int& w, int& h) {
    int n = 0; GLFWmonitor** ms = glfwGetMonitors(&n); if (n <= 0) return false;
    int x0 = 1 << 30, y0 = 1 << 30, x1 = -(1 << 30), y1 = -(1 << 30);
    for (int i = 0; i < n; i++) {
        int mx, my; glfwGetMonitorPos(ms[i], &mx, &my);
        const GLFWvidmode* v = glfwGetVideoMode(ms[i]); if (!v) continue;
        x0 = ::std::min(x0, mx); y0 = ::std::min(y0, my);
        x1 = ::std::max(x1, mx + v->width); y1 = ::std::max(y1, my + v->height);
    }
    if (x1 <= x0) return false;
    x = x0; y = y0; w = x1 - x0; h = y1 - y0; return true;
}
// Size the sketch for the chosen display (or the span) before the window exists.
static void cppmodeFsSize(int& w, int& h) {
    int x, y;
    if (g_fsDisplay < 0 && cppmodeSpanRect(x, y, w, h)) return;
    GLFWmonitor* m = cppmodeFsMonitor(); const GLFWvidmode* v = m ? glfwGetVideoMode(m) : nullptr;
    if (v) { w = v->width; h = v->height; }
}
static void cppmodeGoFullScreen(GLFWwindow* w) {
    if (!w) { g_fullScreenPending = true; return; }
    if (g_fsDisplay < 0) {
        int x, y, sw, sh;
        if (!cppmodeSpanRect(x, y, sw, sh)) return;
        glfwSetWindowAttrib(w, GLFW_DECORATED, GLFW_FALSE);
        glfwSetWindowMonitor(w, nullptr, x, y, sw, sh, GLFW_DONT_CARE);
        return;
    }
    GLFWmonitor* m = cppmodeFsMonitor(); if (!m) return;
    const GLFWvidmode* v = glfwGetVideoMode(m); if (!v) return;
    glfwSetWindowMonitor(w, m, 0, 0, v->width, v->height, v->refreshRate);
}
static constexpr int CPPMODE_FS_PRIMARY = -1000;  // SPAN is 0, so the default needs its own value
static void cppmodeFullScreen(PApplet* app, int mode, int display) {
    g_fsDisplay = display == CPPMODE_FS_PRIMARY ? 0 : display == SPAN ? -1 : display;
    int w = app->displayWidth, h = app->displayHeight;
    cppmodeFsSize(w, h);
    if (w > 0 && h > 0) { if (mode) app->size(w, h, mode); else app->size(w, h); }
}
// gWindow is read after size(), since size(w, h, P2D) can create or replace the window.
void PApplet::fullScreen() { cppmodeFullScreen(this, 0, CPPMODE_FS_PRIMARY); cppmodeGoFullScreen(gWindow); }
void PApplet::fullScreen(int m) {
    if (m == P2D || m == P3D) cppmodeFullScreen(this, m, CPPMODE_FS_PRIMARY);
    else cppmodeFullScreen(this, 0, m);
    cppmodeGoFullScreen(gWindow);
}
void PApplet::fullScreen(int mode, int display) { cppmodeFullScreen(this, mode, display); cppmodeGoFullScreen(gWindow); }
void PApplet::fullScreen(const char*, int display) { cppmodeFullScreen(this, 0, display); cppmodeGoFullScreen(gWindow); }
void PApplet::frameRate(int fps){ targetFrameTime = 1.0/fps; }
void PApplet::noLoop(){looping=false;}
void PApplet::loop()  {looping=true;}
void PApplet::redraw(){redrawOnce=true;}
static bool g_exitRequested = false;  // cppmode-strapi: exit() may come before the loop starts
void PApplet::exit_sketch(){g_exitRequested=true;if(gWindow)glfwSetWindowShouldClose(gWindow,GLFW_TRUE);}
void PApplet::windowTitle(const ::std::string& t){if(gWindow)glfwSetWindowTitle(gWindow,t.c_str());}
void PApplet::windowMove(int x,int y){if(gWindow)glfwSetWindowPos(gWindow,x,y);}
void PApplet::windowResize(int w,int h){size(w,h);}
void PApplet::windowResizable(bool r){isResizable=r;if(gWindow)glfwSetWindowAttrib(gWindow,GLFW_RESIZABLE,r?GLFW_TRUE:GLFW_FALSE);}
// ---------------------------------------------------------------------------
// Clipboard
// ---------------------------------------------------------------------------
void PApplet::setClipboard(const ::std::string& s) {
    if (s.empty()) return;
    if (gWindow) glfwSetClipboardString(gWindow, s.c_str());
}
::std::string PApplet::getClipboard() {
    if (!gWindow) return "";
    const char* cb = glfwGetClipboardString(gWindow);
    return cb ? ::std::string(cb) : "";
}

// ---------------------------------------------------------------------------
// Window icon
// ---------------------------------------------------------------------------
void PApplet::setWindowIcon(PImage* img) {
    if (!img || !gWindow) return;
    // Convert ARGB pixels (Processing internal) to RGBA (GLFW wants RGBA)
    ::std::vector<unsigned char> rgba(img->width * img->height * 4);
    for (int p = 0; p < img->width * img->height; p++) {
        unsigned int px = img->pixels[p];
        rgba[p*4+0] = (px >> 16) & 0xFF;  // R
        rgba[p*4+1] = (px >>  8) & 0xFF;  // G
        rgba[p*4+2] = (px      ) & 0xFF;  // B
        rgba[p*4+3] = (px >> 24) & 0xFF;  // A
    }
    GLFWimage gi;
    gi.width  = img->width;
    gi.height = img->height;
    gi.pixels = rgba.data();
    glfwSetWindowIcon(gWindow, 1, &gi);
}

// ---------------------------------------------------------------------------
// Modifier key state
// ---------------------------------------------------------------------------

bool PApplet::isCtrlDown() {
    // Use GLFW mods bitmask (reliable from callbacks) OR glfwGetKey (for polling)
    if (g_currentMods & GLFW_MOD_CONTROL) return true;
    if (!gWindow) return false;
    return glfwGetKey(gWindow, GLFW_KEY_LEFT_CONTROL)  == GLFW_PRESS
        || glfwGetKey(gWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
}
bool PApplet::isShiftDown() {
    if (g_currentMods & GLFW_MOD_SHIFT) return true;
    if (!gWindow) return false;
    return glfwGetKey(gWindow, GLFW_KEY_LEFT_SHIFT)  == GLFW_PRESS
        || glfwGetKey(gWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
}
bool PApplet::isAltDown() {
    if (g_currentMods & GLFW_MOD_ALT) return true;
    if (!gWindow) return false;
    return glfwGetKey(gWindow, GLFW_KEY_LEFT_ALT)  == GLFW_PRESS
        || glfwGetKey(gWindow, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS;
}

void PApplet::windowRatio(int w,int h){if(gWindow)glfwSetWindowAspectRatio(gWindow,w,h);}
void PApplet::pixelDensity(int d){pixelDensityValue=d;}
void PApplet::smooth(int level) {
    // The MAIN window's actual MSAA sample count is fixed at window-creation
    // time (GLFW_SAMPLES is a window hint, can't change after the window
    // exists) -- so smooth(level) can't retroactively change the main
    // canvas's hardware sample count the way real Processing's P2D/P3D
    // renderers can. What we CAN do: store the requested level (used by
    // PGraphics buffers below, which DO get to choose their own sample
    // count fresh at creation time), and toggle the same smoothing flags
    // smooth() always has, regardless of which level was requested.
    smoothing = true;
    smoothLevel = level;
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    g_lpMsaaWanted = lpMsaaSamples(true, level);  // cppmode-texsmooth
    if(_backend){ processing_graphics_set_msaa(_backend->graphics_id(), (uint32_t)g_lpMsaaWanted); return; }
#endif
#ifndef __EMSCRIPTEN__
    glEnable(GL_LINE_SMOOTH);glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    // GL_POINT_SMOOTH removed in OpenGL 3.2+ core -- skip to avoid crash
    glEnable(GL_MULTISAMPLE);
#endif
}
void PApplet::noSmooth(){
    smoothing=false;
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    g_lpMsaaWanted = 0;
    if(_backend){ processing_graphics_set_msaa(_backend->graphics_id(), 0); return; }
#endif
#ifndef __EMSCRIPTEN__
    glDisable(GL_LINE_SMOOTH);glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_POINT_SMOOTH);glDisable(GL_MULTISAMPLE);
#endif
}
void PApplet::hint(int which){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend){
        if (which == ENABLE_DEPTH_TEST)  processing_depth_test(_backend->graphics_id(), true);
        if (which == DISABLE_DEPTH_TEST) processing_depth_test(_backend->graphics_id(), false);
        return; }
#endif
    switch(which){
        case ENABLE_DEPTH_TEST:  glEnable(GL_DEPTH_TEST);  break;
        case DISABLE_DEPTH_TEST: glDisable(GL_DEPTH_TEST); break;
        case ENABLE_DEPTH_SORT:  glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL); break;
        case DISABLE_DEPTH_SORT: glDepthFunc(GL_LESS); break;
        default: break;
    }
}
void PApplet::cursor()       {if(_backend){processing_cursor(_backend->surface_id(),0);return;}if(gWindow)glfwSetInputMode(gWindow,GLFW_CURSOR,GLFW_CURSOR_NORMAL);}
void PApplet::cursor(int type){
    if(_backend){  // cppmode-cursor-frustum: GLFW cursor shapes -> libprocessing's PROCESSING_CURSOR_* (0..5)
        uint8_t k = 0;  // arrow
        if      (type == GLFW_CROSSHAIR_CURSOR) k = 1;
        else if (type == GLFW_HAND_CURSOR)      k = 2;
        else if (type == GLFW_HRESIZE_CURSOR)   k = 3;  // MOVE
        else if (type == GLFW_IBEAM_CURSOR)     k = 4;  // TEXT
        else if (type == GLFW_VRESIZE_CURSOR)   k = 5;  // WAIT
        processing_cursor(_backend->surface_id(), k); return;
    }
    if(!gWindow)return;GLFWcursor* c=glfwCreateStandardCursor(type);if(c)glfwSetCursor(gWindow,c);}
void PApplet::noCursor()     {if(_backend){processing_no_cursor(_backend->surface_id());return;}/* cppmode-cursor-frustum */if(gWindow)glfwSetInputMode(gWindow,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);}
// captureMouse(): locks cursor to window and provides unlimited delta movement.
// Use releaseMouse() or press ESC to free it.
void PApplet::captureMouse() {if(gWindow){glfwSetInputMode(gWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    // Enable raw motion if supported (removes OS acceleration)
    if(glfwRawMouseMotionSupported())
        glfwSetInputMode(gWindow,GLFW_RAW_MOUSE_MOTION,GLFW_TRUE);}}
void PApplet::releaseMouse(){if(gWindow){glfwSetInputMode(gWindow,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    glfwSetInputMode(gWindow,GLFW_RAW_MOUSE_MOTION,GLFW_FALSE);}}

// =============================================================================
// STYLE STACK
// =============================================================================

void PApplet::captureStyle(Style& s) {
    s = { fillR, fillG, fillB, fillA,
          strokeR, strokeG, strokeB, strokeA, strokeW,
          doFill, doStroke,
          currentRectMode, currentEllipseMode, currentImageMode,
          tintR, tintG, tintB, tintA, doTint,
          colorModeVal, colorMaxH, colorMaxS, colorMaxB, colorMaxA };
}
void PApplet::restoreStyle(const Style& s) {
    fillR = s.fillR; fillG = s.fillG; fillB = s.fillB; fillA = s.fillA;
    strokeR = s.strokeR; strokeG = s.strokeG; strokeB = s.strokeB;
    strokeA = s.strokeA; strokeW = s.strokeW;
    doFill = s.doFill; doStroke = s.doStroke;
    currentRectMode   = s.rectMode;
    currentEllipseMode = s.ellipseMode;
    currentImageMode  = s.imageMode;
    tintR = s.tintR; tintG = s.tintG; tintB = s.tintB; tintA = s.tintA;
    doTint = s.doTint;
    colorModeVal = s.colorMode;
    colorMaxH = s.cmH; colorMaxS = s.cmS; colorMaxB = s.cmB; colorMaxA = s.cmA;
}
void PApplet::pushStyle() { lpLitForgetSent();  /* cppmode-sentmat */
    if(_backend) processing_push_style(_backend->graphics_id());
    Style s;
    captureStyle(s);
    styleStack.push_back(s);
}

void PApplet::popStyle() { lpLitForgetSent();  /* cppmode-sentmat */
    if(_backend) processing_pop_style(_backend->graphics_id());
    if (!styleStack.empty()) {
        restoreStyle(styleStack.back());
        styleStack.pop_back();
    }
}

void PApplet::push()       { if(_backend){_backend->push_matrix();pushStyle();return;} glPushMatrix(); pushStyle(); }
void PApplet::pop()        { if(_backend){_backend->pop_matrix();popStyle();return;} glPopMatrix();  popStyle(); }
void PApplet::pushMatrix() { if(_backend){_backend->push_matrix();return;} glPushMatrix(); }
void PApplet::popMatrix()  { if(_backend){_backend->pop_matrix();return;} glPopMatrix(); }

// =============================================================================
// BACKGROUND / CLEAR
// =============================================================================
void PApplet::setBg(float r,float g,float b,float a){
    bgR=r;bgG=g;bgB=b;bgA=a;
    if (_backend) { _backend->background(r,g,b,a); return; }
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
void PApplet::background(float gray, float a) {
    _backgroundCalledThisFrame = true;
    color c = makeColor(gray, a);
    unsigned int v = c.value;
    setBg((v>>16&0xFF)/255.f, (v>>8&0xFF)/255.f, (v&0xFF)/255.f, (v>>24&0xFF)/255.f);
}
void PApplet::background(float r, float g, float b, float a) {
    _backgroundCalledThisFrame = true;
    color c = makeColor(r, g, b, a);
    unsigned int v = c.value;
    setBg((v>>16&0xFF)/255.f, (v>>8&0xFF)/255.f, (v&0xFF)/255.f, (v>>24&0xFF)/255.f);
}
void PApplet::background(color c) {
    _backgroundCalledThisFrame = true;
    unsigned int v = c.value;
    setBg((v>>16&0xFF)/255.f, (v>>8&0xFF)/255.f, (v&0xFF)/255.f, (v>>24&0xFF)/255.f);
}
#ifdef CPPMODE_BACKEND_LIBPROCESSING
static bool lpEnsureImage(ProcessingBackend* be, PImage* img);   // cppmode-api3
#endif
void PApplet::background(const PImage& img) {
    _backgroundCalledThisFrame = true;
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // cppmode-api3
        PImage* p = const_cast<PImage*>(&img);
        if (lpEnsureImage(_backend, p)) processing_background_image(_backend->graphics_id(), p->lp_image_id);
        return; }
#endif
    // Draw image as full-canvas background
    if (img.width == 0 || img.height == 0) return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    glOrtho(0, logicalW, logicalH, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    drawImageRect(const_cast<PImage&>(img), 0, 0, (float)logicalW, (float)logicalH);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
    if (defaultP3D) glEnable(GL_DEPTH_TEST);
}
void PApplet::clear(){
    if(_backend){processing_clear(_backend->graphics_id());return;}glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT);}

// =============================================================================
// FILL / STROKE
// =============================================================================

void PApplet::fill(float gray,float a)              {setFillFromColor(makeColor(gray,a));}
void PApplet::fill(float gray)                      {setFillFromColor(makeColor(gray,colorMaxA));}
void PApplet::fill(float r,float g,float b,float a) {setFillFromColor(makeColor(r,g,b,a));}
void PApplet::fill(float r,float g,float b)           {setFillFromColor(makeColor(r,g,b,colorMaxA));}
void PApplet::fill(color c)                         {setFillFromColor(c);}
// cppmode-coloralpha: fill(rgb, alpha) / stroke(rgb, alpha) like Java: alpha is in colorMode's alpha
// range, and the color goes through setFillFromColor so libprocessing gets it too. stroke(rgb,
// alpha) used to fall through to stroke(gray, alpha), so a red stroke came out black.
static color cppmodeWithAlpha(color c, float a, float maxA) {
    float t = maxA > 0 ? a / maxA : 1.f;
    int ai = (int)(::std::min(1.f, ::std::max(0.f, t)) * 255.f + 0.5f);
    return colorVal((int)(c.value >> 16 & 0xFF), (int)(c.value >> 8 & 0xFF), (int)(c.value & 0xFF), ai);
}
void PApplet::fill(color c, float a)   { setFillFromColor(cppmodeWithAlpha(c, a, colorMaxA)); }
void PApplet::stroke(color c, float a) { setStrokeFromColor(cppmodeWithAlpha(c, a, colorMaxA)); }
void PApplet::noFill()                              {doFill=false; if(_backend) _backend->no_fill();}
void PApplet::stroke(float gray,float a)            {setStrokeFromColor(makeColor(gray,a));}
void PApplet::stroke(float gray)                    {setStrokeFromColor(makeColor(gray,colorMaxA));}
void PApplet::stroke(float r,float g,float b,float a){setStrokeFromColor(makeColor(r,g,b,a));}
void PApplet::stroke(float r,float g,float b)          {setStrokeFromColor(makeColor(r,g,b,colorMaxA));}
void PApplet::stroke(color c)                       {setStrokeFromColor(c);}
void PApplet::noStroke()                            {doStroke=false; if(_backend) _backend->no_stroke();}
void PApplet::strokeWeight(float w)                 {strokeW=w; if(_backend) _backend->stroke_weight(w);}
void PApplet::strokeCap(int mode) {
    currentStrokeCap = mode;
    if(_backend){processing_set_stroke_cap(_backend->graphics_id(),(uint8_t)mode);return;}
    // ROUND cap: enable line smoothing; SQUARE/PROJECT: disable
    if(mode==ROUND) { glEnable(GL_LINE_SMOOTH); glHint(GL_LINE_SMOOTH_HINT,GL_NICEST); }
    else { glDisable(GL_LINE_SMOOTH); }
}
void PApplet::strokeJoin(int mode) {
    currentStrokeJoin = mode;
    if(_backend) processing_set_stroke_join(_backend->graphics_id(),(uint8_t)mode);
}

// =============================================================================
// PCOLOR CONVENIENCE OVERLOADS
// =============================================================================

void PApplet::fill(const PColor& c)      { fill(c.r, c.g, c.b, c.a); }
void PApplet::stroke(const PColor& c)    { stroke(c.r, c.g, c.b, c.a); }
void PApplet::background(const PColor& c){ background(c.r, c.g, c.b, c.a); }
void PApplet::tint(const PColor& c)      { tint(c.r, c.g, c.b, c.a); }
// cppmode-modemap: CppMode's mode numbers follow Java; libprocessing numbers them differently
static inline uint8_t lpShapeMode(int m) { return m == CENTER ? 2 : m == RADIUS ? 3 : (uint8_t)m; }
static inline uint8_t lpArcMode(int m)   { return m == PIE ? 2 : m == CHORD ? 1 : 0; }
void PApplet::rectMode(int m)    {currentRectMode=m;
    if(_backend) processing_rect_mode(_backend->graphics_id(),lpShapeMode(CORNER));}  /* cppmode-everything: CppMode converted the coordinates already */
void PApplet::ellipseMode(int m) {currentEllipseMode=m;
    if(_backend) processing_ellipse_mode(_backend->graphics_id(),lpShapeMode(m));}

// =============================================================================
// 2D PRIMITIVES
// =============================================================================

static void flushPoints(){} // no-op, points drawn immediately now

void PApplet::point(float x, float y) {
    if (_backend) { _backend->point(x, y); return; }
    if (!doStroke) return;
    applyStroke();
    if (!smoothing && strokeW <= 1.0f) {
        glPointSize(1.0f);
        glBegin(GL_POINTS); glVertex2f(::std::floor(x)+0.5f, ::std::floor(y)+0.5f); glEnd();
    } else {
        glPointSize(strokeW);
        glBegin(GL_POINTS); glVertex2f(x, y); glEnd();
    }
    restoreLighting();
}
void PApplet::point(float x, float y, float z) {
    if (_backend) { _backend->point(x, y, z); return; }  // cppmode-api3
    if (!doStroke) return;
    applyStroke();
    glPointSize(strokeW);
    glBegin(GL_POINTS); glVertex3f(x, y, z); glEnd();
    restoreLighting();
}
void PApplet::line(float x1, float y1, float x2, float y2) {
    if (_backend) { _backend->line(x1, y1, x2, y2); return; }
    {
        GLint mvDepth=0, projDepth=0;
        glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &mvDepth);
        glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &projDepth);
        double mv[16], proj[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, mv);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        GLint vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
        GLint curFbo=0; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFbo);
        PDEBUG("PApplet::line(%.1f,%.1f,%.1f,%.1f) ENTRY: mvDepth=%d projDepth=%d curFbo=%d viewport=(%d,%d,%d,%d)\n",
               x1,y1,x2,y2, mvDepth, projDepth, curFbo, vp[0],vp[1],vp[2],vp[3]);
        PDEBUG("  mv[0]=%.3f mv[5]=%.3f mv[12]=%.3f mv[13]=%.3f\n", mv[0],mv[5],mv[12],mv[13]);
        PDEBUG("  proj[0]=%.6f proj[5]=%.6f proj[12]=%.6f proj[13]=%.6f\n", proj[0],proj[5],proj[12],proj[13]);
    }
    if (!doStroke) return;
    applyStroke();
    float w = strokeW;
    if (w <= 1.0f) {
        // Thin lines: use GL_LINES
        glLineWidth(1.0f);
        glBegin(GL_LINES); glVertex2f(x1,y1); glVertex2f(x2,y2); glEnd();
        restoreLighting();
        return;
    }
    // Thick line: quad body + round caps using stencil to prevent double-blend
    float dx = x2-x1, dy = y2-y1;
    float len = ::std::sqrt(dx*dx+dy*dy);
    if(len < 0.0001f){ restoreLighting(); return; }
    float ux = dx/len, uy = dy/len;
    float r = w*0.5f;
    float px2 = -uy*r, py2 = ux*r; // perpendicular
    int segs = ::std::max(16, (int)(r*4.0f));
    // cppmode-glshapes2: strokeCap. Only ROUND gets round ends; PROJECT extends the line by
    // half the weight with square ends; SQUARE stops at the end points
    bool roundCap = currentStrokeCap == ROUND;
    if (currentStrokeCap == PROJECT) { x1 -= ux*r; y1 -= uy*r; x2 += ux*r; y2 += uy*r; }
    // Use stencil to draw all geometry, then fill once -- prevents double-blend
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
    // Draw body into stencil
#ifdef __EMSCRIPTEN__
    glBegin(GL_TRIANGLES);
    glVertex2f(x1+px2,y1+py2); glVertex2f(x2+px2,y2+py2); glVertex2f(x2-px2,y2-py2);
    glVertex2f(x1+px2,y1+py2); glVertex2f(x2-px2,y2-py2); glVertex2f(x1-px2,y1-py2);
    glEnd();
#else
    glBegin(GL_QUADS);
    glVertex2f(x1+px2,y1+py2); glVertex2f(x2+px2,y2+py2);
    glVertex2f(x2-px2,y2-py2); glVertex2f(x1-px2,y1-py2);
    glEnd();
#endif
    // Draw end caps into stencil
    for(int ep=0;ep<(roundCap?2:0);ep++){
        float cx2=ep?x2:x1, cy2=ep?y2:y1;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx2,cy2);
        for(int s=0;s<=segs;s++){
            float a=s*TWO_PI/segs;
            glVertex2f(cx2+::std::cos(a)*r, cy2+::std::sin(a)*r);
        }
        glEnd();
    }
    // Now draw color only where stencil=1
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
    glEnable(GL_BLEND);
    glApplyBlend(g_glBlendMode);
    // Fill bounding box with stroke color
    float minx=::std::min(x1,x2)-r, maxx=::std::max(x1,x2)+r;
    float miny=::std::min(y1,y2)-r, maxy=::std::max(y1,y2)+r;
#ifdef __EMSCRIPTEN__
    glBegin(GL_TRIANGLES);
    glVertex2f(minx,miny); glVertex2f(maxx,miny); glVertex2f(maxx,maxy);
    glVertex2f(minx,miny); glVertex2f(maxx,maxy); glVertex2f(minx,maxy);
    glEnd();
#else
    glBegin(GL_QUADS);
    glVertex2f(minx,miny); glVertex2f(maxx,miny);
    glVertex2f(maxx,maxy); glVertex2f(minx,maxy);
    glEnd();
#endif
    glDisable(GL_STENCIL_TEST);
    restoreLighting();
}
void PApplet::line(float x1, float y1, float z1, float x2, float y2, float z2) {
    if (_backend) { _backend->line(x1, y1, z1, x2, y2, z2); return; }  // cppmode-api3
    if (!doStroke) return;
    applyStroke();
    glLineWidth(strokeW);
    glBegin(GL_LINES); glVertex3f(x1,y1,z1); glVertex3f(x2,y2,z2); glEnd();
    restoreLighting();
}
void PApplet::ellipse(float cx, float cy, float w, float h) {
    if (_backend) { _backend->ellipse(cx, cy, w, h); return; }
    float rx = w, ry = h;
    resolveEllipse(cx, cy, rx, ry);
    drawEllipseGeom(cx, cy, rx, ry);
}

void PApplet::circle(float cx, float cy, float diameter) { if (_backend) { _backend->circle(cx, cy, diameter); return; }
    ellipse(cx, cy, diameter, diameter);
}

void PApplet::arc(float cx, float cy, float w, float h, float startAngle, float endAngle) {
    if (_backend) { _backend->arc(cx, cy, w, h, startAngle, endAngle, 0); return; }
    float rx = w, ry = h;
    resolveEllipse(cx, cy, rx, ry);
    drawEllipseGeom(cx, cy, rx, ry, startAngle, endAngle);
}

void PApplet::arc(float cx, float cy, float w, float h, float startAngle, float endAngle, int mode) {
    if (_backend) { _backend->arc(cx, cy, w, h, startAngle, endAngle, lpArcMode(mode)); return; }
    float rx = w, ry = h;
    resolveEllipse(cx, cy, rx, ry);
    int steps = 64;
    float da = (endAngle - startAngle) / steps;
    // Fill
    if (doFill) {
        applyFill();
        glBegin(GL_TRIANGLE_FAN);
        if (mode == PIE) glVertex2f(cx, cy); // PIE: fan from center
        else             glVertex2f(cx + rx * ::std::cos(startAngle),
                                   cy + ry * ::std::sin(startAngle)); // CHORD/OPEN: fan from first point (cppmode-glshapes2: was at half radius)
        for (int i = 0; i <= steps; i++) {
            float a = startAngle + i * da;
            glVertex2f(cx + rx * ::std::cos(a), cy + ry * ::std::sin(a));
        }
        if (mode == PIE) glVertex2f(cx, cy);
        glEnd();
    }
    // Stroke
    if (doStroke) {
        applyStroke();
        glBegin(GL_LINE_STRIP);
        if (mode == PIE) { glVertex2f(cx, cy); }
        for (int i = 0; i <= steps; i++) {
            float a = startAngle + i * da;
            glVertex2f(cx + rx * ::std::cos(a), cy + ry * ::std::sin(a));
        }
        if (mode == PIE)   { glVertex2f(cx, cy); }
        else if (mode == CHORD) { // close the chord line
            glVertex2f(cx + rx * ::std::cos(startAngle), cy + ry * ::std::sin(startAngle));
        }
        glEnd();
    }
}
void PApplet::rect(float x, float y, float w, float h) {
    resolveRect(x, y, w, h);
    if (_backend) { _backend->rect(x, y, w, h); return; }

    if (doFill) {
        applyFill();
#ifdef __EMSCRIPTEN__
        glBegin(GL_TRIANGLES);
            glVertex2f(x,     y    ); glVertex2f(x + w, y    ); glVertex2f(x + w, y + h);
            glVertex2f(x,     y    ); glVertex2f(x + w, y + h); glVertex2f(x,     y + h);
        glEnd();
#else
        glBegin(GL_QUADS);
            glVertex2f(x,     y    );
            glVertex2f(x + w, y    );
            glVertex2f(x + w, y + h);
            glVertex2f(x,     y + h);
        glEnd();
#endif
    }

    if (doStroke) {
        // Real Processing centers the stroke ON the rectangle's boundary
        // (half the weight extends inward over the fill, half extends
        // outward) -- NOT entirely outside it. The previous version drew
        // the stroke loop expanded fully outward, which meant increasing
        // strokeWeight never visually shrank the filled interior (it
        // does in real Processing, since the inward half of a thick
        // stroke covers part of the fill), and glLineWidth-rendered
        // GL_LINE_LOOP gives flat, unmitered corners at large widths
        // (visible gaps/seams at corners instead of a clean joined
        // outline). Drawing the stroke as actual filled quad geometry --
        // matching real Processing's approach -- fixes both: correct
        // centering AND clean mitered corners via overlapping quads.
        applyStroke();
        float half = strokeW * 0.5f;
        float xo = x - half, yo = y - half;       // outer edge
        float xi = x + half, yi = y + half;       // inner edge (top-left side)
        float xow = x + w + half, yoh = y + h + half; // outer edge (bottom-right side)
        float xiw = x + w - half, yih = y + h - half; // inner edge (bottom-right side)
#ifdef __EMSCRIPTEN__
        glBegin(GL_TRIANGLES);
            glVertex2f(xo,yo);glVertex2f(xow,yo);glVertex2f(xow,yi); glVertex2f(xo,yo);glVertex2f(xow,yi);glVertex2f(xo,yi);
            glVertex2f(xo,yih);glVertex2f(xow,yih);glVertex2f(xow,yoh); glVertex2f(xo,yih);glVertex2f(xow,yoh);glVertex2f(xo,yoh);
            glVertex2f(xo,yi);glVertex2f(xi,yi);glVertex2f(xi,yih); glVertex2f(xo,yi);glVertex2f(xi,yih);glVertex2f(xo,yih);
            glVertex2f(xiw,yi);glVertex2f(xow,yi);glVertex2f(xow,yih); glVertex2f(xiw,yi);glVertex2f(xow,yih);glVertex2f(xiw,yih);
        glEnd();
#else
        glBegin(GL_QUADS);
            // Top edge
            glVertex2f(xo,  yo); glVertex2f(xow, yo); glVertex2f(xow, yi); glVertex2f(xo,  yi);
            // Bottom edge
            glVertex2f(xo,  yih); glVertex2f(xow, yih); glVertex2f(xow, yoh); glVertex2f(xo,  yoh);
            // Left edge (between top and bottom strips already drawn)
            glVertex2f(xo,  yi); glVertex2f(xi,  yi); glVertex2f(xi,  yih); glVertex2f(xo,  yih);
            // Right edge
            glVertex2f(xiw, yi); glVertex2f(xow, yi); glVertex2f(xow, yih); glVertex2f(xiw, yih);
        glEnd();
#endif
        restoreLighting();
    }
}
void PApplet::rect(float x, float y, float w, float h, float r) {
    resolveRect(x, y, w, h);
    if (_backend) { float rr = min(r, min(::std::fabs(w), ::std::fabs(h)) * 0.5f); _backend->rect(x, y, w, h, rr, rr, rr, rr); return; }  // cppmode-api3

    // Clamp radius so it never exceeds half the shortest side
    r = min(r, min(w, h) * 0.5f);

    const int cornerSegs = 8;  // segments per 90-degree corner arc

    // Emit a corner arc: center (cx,cy), from angle sa to ea
    auto corner = [&](float cx, float cy, float sa, float ea) {
        for (int i = 0; i <= cornerSegs; i++) {
            float a = sa + (ea - sa) * i / cornerSegs;
            glVertex2f(cx + r * ::std::cos(a), cy + r * ::std::sin(a));
        }
    };

    if (doFill) {
        applyFill();
        glBegin(GL_TRIANGLE_FAN);
            // Fan center at rect midpoint
            glVertex2f(x + w * 0.5f, y + h * 0.5f);
            // Four corners in CCW order, closing back to the first arc point
            corner(x + r,     y + r,     PI,             PI + HALF_PI);  // top-left
            corner(x + w - r, y + r,     PI + HALF_PI,   TWO_PI      );  // top-right
            corner(x + w - r, y + h - r, 0,              HALF_PI     );  // bottom-right
            corner(x + r,     y + h - r, HALF_PI,        PI          );  // bottom-left
            // Close the fan back to the first arc point (avoids gap/seam)
            glVertex2f(x + r + r * ::std::cos(PI), y + r + r * ::std::sin(PI));
        glEnd();
    }

    if (doStroke) {
        applyStroke();
        glLineWidth(strokeW);
        glBegin(GL_LINE_LOOP);
            corner(x + r,     y + r,     PI,           PI + HALF_PI);
            corner(x + w - r, y + r,     PI + HALF_PI, TWO_PI      );
            corner(x + w - r, y + h - r, 0,            HALF_PI     );
            corner(x + r,     y + h - r, HALF_PI,      PI          );
        glEnd();
    }
}
void PApplet::rect(float x,float y,float w,float h,float tl,float tr,float br,float bl){
    if (_backend) { resolveRect(x, y, w, h); _backend->rect(x, y, w, h, tl, tr, br, bl); return; }  // cppmode-api3
    if(!doFill && !doStroke) return;
    float x2=x+w, y2=y+h;
    beginShape();
    vertex(x+tl,y); vertex(x2-tr,y);
    if(tr>0){for(int i=0;i<=8;i++){float a=PI+HALF_PI+HALF_PI*i/8.f;vertex(x2-tr+::std::cos(a)*tr,y+tr+::std::sin(a)*tr);}}
    vertex(x2,y+tr); vertex(x2,y2-br);
    if(br>0){for(int i=0;i<=8;i++){float a=HALF_PI*i/8.f;vertex(x2-br+::std::cos(a)*br,y2-br+::std::sin(a)*br);}}
    vertex(x2-br,y2); vertex(x+bl,y2);
    if(bl>0){for(int i=0;i<=8;i++){float a=HALF_PI+HALF_PI*i/8.f;vertex(x+bl+::std::cos(a)*bl,y2-bl+::std::sin(a)*bl);}}
    vertex(x,y2-bl); vertex(x,y+tl);
    if(tl>0){for(int i=0;i<=8;i++){float a=PI+HALF_PI*i/8.f;vertex(x+tl+::std::cos(a)*tl,y+tl+::std::sin(a)*tl);}}
    endShape(CLOSE);
}

void PApplet::square(float x,float y,float s){ if (_backend) { _backend->square(x, y, s); return; }rect(x,y,s,s);}
void PApplet::triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    if (_backend) { _backend->triangle(x1,y1,x2,y2,x3,y3); return; }
    if (doFill) {
        applyFill();
        glBegin(GL_TRIANGLES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glVertex2f(x3, y3);
        glEnd();
    }
    if (doStroke) {
        applyStroke();
        glLineWidth(strokeW);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glVertex2f(x3, y3);
        glEnd();
        restoreLighting();
    }
}
void PApplet::quad(float x1, float y1, float x2, float y2,
          float x3, float y3, float x4, float y4){
    if (_backend) { _backend->quad(x1,y1,x2,y2,x3,y3,x4,y4); return; }

    if (doFill) {
        applyFill();
#ifdef __EMSCRIPTEN__
        glBegin(GL_TRIANGLES);
            glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3);
            glVertex2f(x1,y1); glVertex2f(x3,y3); glVertex2f(x4,y4);
        glEnd();
#else
        glBegin(GL_QUADS);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glVertex2f(x3, y3);
            glVertex2f(x4, y4);
        glEnd();
#endif
    }
    if (doStroke) {
        applyStroke();
        glLineWidth(strokeW);
        glBegin(GL_LINE_LOOP);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glVertex2f(x3, y3);
            glVertex2f(x4, y4);
        glEnd();
        restoreLighting();
    }
}

// =============================================================================
// 3D PRIMITIVES
// =============================================================================

void PApplet::rotateX(float a){if(_backend){_backend->rotate_x(a);return;}glRotatef(a*180.0f/PI,1,0,0);}
void PApplet::rotateY(float a){if(_backend){_backend->rotate_y(a);return;}glRotatef(a*180.0f/PI,0,1,0);}
void PApplet::rotateZ(float a){if(_backend){_backend->rotate_z(a);return;}glRotatef(a*180.0f/PI,0,0,1);}
void PApplet::sphereDetail(int r){sphereRes=r; if(_backend) _backend->sphereDetail(r, r);}  /* cppmode-spheredetail */
void PApplet::sphereDetail(int u, int v){sphereRes=u; if(_backend) _backend->sphereDetail(u, v);}

// =============================================================================
// PHONG SHADING SHADER
// =============================================================================

static GLuint compileShader(GLenum type, const char* code) {
    GLuint sh = glCreateShader(type);
    glShaderSource(sh, 1, &code, nullptr);
    glCompileShader(sh);
    GLint ok; glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) { char b[512]; glGetShaderInfoLog(sh,512,nullptr,b); fprintf(stderr,"[sh] %s\n",b); }
    return sh;
}
void PApplet::initPhongShader() {
    if (phongProg) return;
#ifdef __EMSCRIPTEN__
    // Phong shader uses GLSL 1.20 compatibility built-ins (gl_LightSource,
    // gl_LightModel, gl_NormalMatrix) that don't exist in WebGL GLSL ES.
    // Fall back to fixed-function emulation lighting instead.
    return;
#endif
    // Phong shader using compatibility profile built-ins.
    // Works on any system that supports our fixed-function GL pipeline.
    const char* vs = R"VERT(
varying vec3 vN;
varying vec3 vP;
void main(){
    // gl_NormalMatrix = inverse-transpose of modelview upper 3x3
    vN = gl_NormalMatrix * gl_Normal;
    vP = vec3(gl_ModelViewMatrix * gl_Vertex);
    gl_Position = ftransform();
    gl_FrontColor = gl_Color;
}
)VERT";
    const char* fs = R"FRAG(
varying vec3 vN;
varying vec3 vP;
uniform int uNumLights;
uniform float uLightConc[8];
uniform float uLightCutCos[8];
void main(){
    vec3 N = normalize(vN);
    vec3 V = normalize(-vP);
    vec3 col = gl_Color.rgb * gl_LightModel.ambient.rgb;
    for(int i=0;i<8;i++){
        if(i>=uNumLights) break;
        vec4 lp = gl_LightSource[i].position;
        bool isDir = (lp.w < 0.5);
        vec3 L = isDir ? normalize(lp.xyz) : normalize(lp.xyz - vP);
        float spotAtten = 1.0;
        if(!isDir && uLightCutCos[i] > -0.5){
            vec3 sd = normalize(gl_LightSource[i].spotDirection);
            float sc = dot(L, -sd);
            if(sc < uLightCutCos[i]) spotAtten = 0.0;
            else spotAtten = pow(max(sc,0.0), uLightConc[i]);
        }
        if(spotAtten < 0.0001) continue;
        col += gl_Color.rgb * gl_LightSource[i].ambient.rgb;
        float d = max(dot(N, L), 0.0);
        col += gl_Color.rgb * gl_LightSource[i].diffuse.rgb * d * spotAtten;
        vec3 H = normalize(L + V);
        float nh = max(dot(N, H), 0.0);
        float sh = pow(nh, max(gl_FrontMaterial.shininess, 1.0));  // like Java's shininess()
        col += gl_FrontMaterial.specular.rgb * gl_LightSource[i].specular.rgb * sh * spotAtten;
    }
    col += gl_FrontMaterial.emission.rgb;  // emissive(): glows whether lit or not
    gl_FragColor = vec4(clamp(col, 0.0, 1.0), gl_Color.a);
}
)FRAG";
    GLuint v=compileShader(GL_VERTEX_SHADER,vs);
    GLuint f=compileShader(GL_FRAGMENT_SHADER,fs);
    phongProg=glCreateProgram();
    glAttachShader(phongProg,v); glAttachShader(phongProg,f);
    glLinkProgram(phongProg);
    GLint ok; glGetProgramiv(phongProg,GL_LINK_STATUS,&ok);
    if(!ok){char b[512];glGetProgramInfoLog(phongProg,512,nullptr,b);
            fprintf(stderr,"[sh link] %s\n",b);phongProg=0;}
    glDeleteShader(v); glDeleteShader(f);
}

void PApplet::box(float s){if(_backend){_backend->box(s,s,s);return;}box(s,s,s);}
void PApplet::box(float bw,float bh,float bd){
    if(_backend){_backend->box(bw,bh,bd);return;}
    {
        GLint curFbo=0; glGetIntegerv(GL_FRAMEBUFFER_BINDING,&curFbo);
        GLboolean depthOn = glIsEnabled(GL_DEPTH_TEST);
        GLint depthFunc=0; glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
        GLboolean depthMask; glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);
        GLint vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
        PDEBUG("PApplet::box(%.1f,%.1f,%.1f) ENTRY: doFill=%d fillR=%.3f fillG=%.3f fillB=%.3f curFbo=%d\n",
               bw,bh,bd, doFill, fillR, fillG, fillB, curFbo);
        PDEBUG("  depthTest=%d depthFunc=0x%x depthWriteMask=%d viewport=(%d,%d,%d,%d)\n",
               depthOn, depthFunc, depthMask, vp[0], vp[1], vp[2], vp[3]);
    }
    float hw=bw/2,hh=bh/2,hd=bd/2;
    struct Face{ float nx,ny,nz; float v[4][3]; };
    Face faces[]={
        { 0, 0, 1,{{-hw,-hh,hd},{hw,-hh,hd},{hw,hh,hd},{-hw,hh,hd}}},
        { 0, 0,-1,{{hw,-hh,-hd},{-hw,-hh,-hd},{-hw,hh,-hd},{hw,hh,-hd}}},
        {-1, 0, 0,{{-hw,-hh,-hd},{-hw,-hh,hd},{-hw,hh,hd},{-hw,hh,-hd}}},
        { 1, 0, 0,{{hw,-hh,hd},{hw,-hh,-hd},{hw,hh,-hd},{hw,hh,hd}}},
        { 0,-1, 0,{{-hw,-hh,-hd},{hw,-hh,-hd},{hw,-hh,hd},{-hw,-hh,hd}}},
        { 0, 1, 0,{{-hw,hh,hd},{hw,hh,hd},{hw,hh,-hd},{-hw,hh,-hd}}},
    };
    if(doFill){
        applyFill();
        for(auto& f:faces){
#ifdef __EMSCRIPTEN__
            glBegin(GL_TRIANGLES);
            glNormal3f(f.nx,f.ny,f.nz); glVertex3f(f.v[0][0],f.v[0][1],f.v[0][2]);
            glNormal3f(f.nx,f.ny,f.nz); glVertex3f(f.v[1][0],f.v[1][1],f.v[1][2]);
            glNormal3f(f.nx,f.ny,f.nz); glVertex3f(f.v[2][0],f.v[2][1],f.v[2][2]);
            glNormal3f(f.nx,f.ny,f.nz); glVertex3f(f.v[0][0],f.v[0][1],f.v[0][2]);
            glNormal3f(f.nx,f.ny,f.nz); glVertex3f(f.v[2][0],f.v[2][1],f.v[2][2]);
            glNormal3f(f.nx,f.ny,f.nz); glVertex3f(f.v[3][0],f.v[3][1],f.v[3][2]);
            glEnd();
#else
            glBegin(GL_QUADS);
            for(auto& v:f.v){
                glNormal3f(f.nx,f.ny,f.nz);
                glVertex3f(v[0],v[1],v[2]);
            }
            glEnd();
#endif
        }
        {
            GLenum err = glGetError();
            PDEBUG("PApplet::box() AFTER glEnd(): glError=0x%x firstFaceVertex=(%.1f,%.1f,%.1f)\n",
                   err, faces[0].v[0][0], faces[0].v[0][1], faces[0].v[0][2]);
        }
    }
    if(doStroke){
        applyStroke();glLineWidth(strokeW);
        float vx[]={-hw,-hw,hw,hw,-hw,-hw,hw,hw};
        float vy[]={-hh,hh,hh,-hh,-hh,hh,hh,-hh};
        float vz[]={hd,hd,hd,hd,-hd,-hd,-hd,-hd};
        int e[][2]={{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}};
        glBegin(GL_LINES);
        for(auto& ee:e){glVertex3f(vx[ee[0]],vy[ee[0]],vz[ee[0]]);glVertex3f(vx[ee[1]],vy[ee[1]],vz[ee[1]]);}
        glEnd();
        restoreLighting();
    }
}
void PApplet::sphere(float r){
    if(_backend){_backend->sphere(r);return;}
    int stacks=sphereRes, slices=sphereRes;
    if(doFill){
        // Use Phong (per-pixel) shading when lighting is on for smooth highlights
        bool usePhong = lightsEnabled;
        if (usePhong) {
            initPhongShader();
            if (phongProg) {
                glUseProgram(phongProg);
                GLint loc = glGetUniformLocation(phongProg, "uNumLights");
                if (loc >= 0) glUniform1i(loc, lightIndex);
                GLint locC = glGetUniformLocation(phongProg, "uLightConc");
                if (locC >= 0) glUniform1fv(locC, 8, lightConcentration);
                GLint locK = glGetUniformLocation(phongProg, "uLightCutCos");
                if (locK >= 0) glUniform1fv(locK, 8, lightCutoffCos);
            } else usePhong = false;
        }
        applyFill();
        for(int i=0;i<stacks;i++){
            float a0=PI*i/stacks-HALF_PI, a1=PI*(i+1)/stacks-HALF_PI;
            for(int j=0;j<slices;j++){
                float b0=TWO_PI*j/slices, b1=TWO_PI*(j+1)/slices;
                float x00=::std::cos(a0)*::std::cos(b0),y00=::std::sin(a0),z00=::std::cos(a0)*::std::sin(b0);
                float x01=::std::cos(a0)*::std::cos(b1),y01=::std::sin(a0),z01=::std::cos(a0)*::std::sin(b1);
                float x10=::std::cos(a1)*::std::cos(b0),y10=::std::sin(a1),z10=::std::cos(a1)*::std::sin(b0);
                float x11=::std::cos(a1)*::std::cos(b1),y11=::std::sin(a1),z11=::std::cos(a1)*::std::sin(b1);
                glBegin(GL_TRIANGLES);
                // tri 1
                glNormal3f(x10,y10,z10); glVertex3f(r*x10,r*y10,r*z10);
                glNormal3f(x00,y00,z00); glVertex3f(r*x00,r*y00,r*z00);
                glNormal3f(x01,y01,z01); glVertex3f(r*x01,r*y01,r*z01);
                // tri 2
                glNormal3f(x10,y10,z10); glVertex3f(r*x10,r*y10,r*z10);
                glNormal3f(x01,y01,z01); glVertex3f(r*x01,r*y01,r*z01);
                glNormal3f(x11,y11,z11); glVertex3f(r*x11,r*y11,r*z11);
                glEnd();
            }
        }
        if (usePhong) glUseProgram(0);
    }
    if(doStroke){
        applyStroke(); glLineWidth(strokeW);
        // Draw sphere as a wireframe of triangles -- matches Processing Java's
        // sphere appearance with diagonal lines across each quad cell.
        auto sv = [&](float lat, float lng) {
            glVertex3f(r*::std::cos(lat)*::std::cos(lng),
                       r*::std::sin(lat),
                       r*::std::cos(lat)*::std::sin(lng));
        };
        glBegin(GL_LINES);
        for(int i=0;i<stacks;i++){
            float lat0=PI*(-0.5f+(float)i/stacks);
            float lat1=PI*(-0.5f+(float)(i+1)/stacks);
            for(int j=0;j<slices;j++){
                float lng0=TWO_PI*(float)j/slices;
                float lng1=TWO_PI*(float)(j+1)/slices;
                // Four edges of the quad + one diagonal (like Processing Java)
                // Bottom edge (latitude ring)
                sv(lat0,lng0); sv(lat0,lng1);
                // Left edge (longitude line)
                sv(lat0,lng0); sv(lat1,lng0);
                // Diagonal (gives the characteristic triangulated look)
                sv(lat0,lng1); sv(lat1,lng0);
            }
        }
        // Top latitude ring
        {
            float lat1=PI*(-0.5f+(float)stacks/stacks);
            for(int j=0;j<slices;j++){
                float lng0=TWO_PI*(float)j/slices;
                float lng1=TWO_PI*(float)(j+1)/slices;
                sv(lat1,lng0); sv(lat1,lng1);
            }
        }
        glEnd();
        restoreLighting();
    }
}

// =============================================================================
// VERTEX / SHAPES
// =============================================================================

#ifdef CPPMODE_BACKEND_LIBPROCESSING
// beginShape under libprocessing: points are recorded first. endShape draws
// them as a 2D shape, or as a mesh when any vertex has a z (the 2D shape path
// has no z). Meshes live until the next frame starts.
// cppmode-curves: bezier/quadratic/curve vertices are sampled here, so bezierDetail(),
// curveDetail() and curveTightness() apply in libprocessing too
static float g_lpLast[3] = {0, 0, 0};
static bool  g_lpHasLast = false;
static ::std::vector<::std::array<float,3>> g_lpCurve;
struct LpShapeVert { float x, y, z, r, g, b, a; float u = 0, v = 0; float nx = 0, ny = 0, nz = 0; bool hasN = false; };
// cppmode-api3: normal() for beginShape vertices; without one, a triangle gets its flat normal
static float g_lpNormal[3] = {0, 0, 1};
static bool  g_lpHasNormal = false;
static void lpPushV(LpShapeVert q);  // cppmode-texsmooth: u, v for texture()
static ::std::vector<LpShapeVert> g_lpShape;
static bool g_lpShapeOpen = false;
static int  g_lpShapeKind = -1;
static ::std::vector<uint64_t> g_lpFrameGeos;
static void lpPushV(LpShapeVert q) {
    if (g_lpHasNormal) { q.nx = g_lpNormal[0]; q.ny = g_lpNormal[1]; q.nz = g_lpNormal[2]; q.hasN = true; }
    g_lpShape.push_back(q);
}
// cppmode-texsmooth: texture() state for the shape being built
static uint64_t g_lpTexImg = 0;
static float    g_lpTexW = 1, g_lpTexH = 1;
static bool     g_lpTexAlpha = false;
static bool lpEnsureImage(ProcessingBackend* be, PImage* img);
static constexpr uint8_t LP_TOPO_TRIANGLES = 3;
// cppmode-geocache: 3D shape meshes are kept by content and reused while they keep being drawn,
// so a shape that only moves (the usual case) builds its mesh once. Unused ones go after 2 frames.
struct LpGeoEntry { uint64_t geo; uint64_t lastUsed; ::std::vector<float> data; bool hasUV; };
static ::std::unordered_map<uint64_t, LpGeoEntry> g_lpGeoCache;
static uint64_t g_lpFrameNo = 0;
static void lpFrameGeosFree() {
    for (uint64_t g : g_lpFrameGeos) processing_geometry_destroy(g);
    g_lpFrameGeos.clear();
    g_lpFrameNo++;
    for (auto it = g_lpGeoCache.begin(); it != g_lpGeoCache.end(); ) {
        if (it->second.lastUsed + 2 < g_lpFrameNo) { processing_geometry_destroy(it->second.geo); it = g_lpGeoCache.erase(it); }
        else ++it;
    }
}
// data per vertex: normal (3), color (4), uv (2, only when hasUV), position (3)
static uint64_t lpGeoCached(const ::std::vector<float>& d, bool hasUV) {
    uint64_t h = 1469598103934665603ull ^ (hasUV ? 0x9e3779b97f4a7c15ull : 0);
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(d.data());
    for (size_t i = 0; i < d.size() * sizeof(float); i++) { h ^= bytes[i]; h *= 1099511628211ull; }
    auto it = g_lpGeoCache.find(h);
    if (it != g_lpGeoCache.end() && it->second.hasUV == hasUV && it->second.data == d) {
        it->second.lastUsed = g_lpFrameNo;
        return it->second.geo;
    }
    uint64_t geo = processing_geometry_create(LP_TOPO_TRIANGLES);
    if (!geo) return 0;
    size_t stride = hasUV ? 12 : 10;
    for (size_t i = 0; i + stride <= d.size(); i += stride) {
        const float* q = &d[i];
        processing_geometry_normal(geo, q[0], q[1], q[2]);
        processing_geometry_color(geo, q[3], q[4], q[5], q[6]);
        if (hasUV) processing_geometry_uv(geo, q[7], q[8]);
        const float* pos = q + (hasUV ? 9 : 7);
        processing_geometry_vertex(geo, pos[0], pos[1], pos[2]);
    }
    if (it != g_lpGeoCache.end()) g_lpFrameGeos.push_back(geo);  // hash collision: just this frame
    else g_lpGeoCache[h] = {geo, g_lpFrameNo, d, hasUV};
    return geo;
}
static void lpShapeOpen(uint64_t gid) {
    if (g_lpShapeOpen) return;
    g_lpShapeOpen = true;
    processing_begin_shape(gid, (uint8_t)(g_lpShapeKind == -1 ? 0 : g_lpShapeKind));
    for (const auto& v : g_lpShape) processing_vertex(gid, v.x, v.y);
}
static float lpSrgbToLinear(float v) {
    v = v < 0 ? 0 : (v > 1 ? 1 : v);
    return v <= 0.04045f ? v / 12.92f : ::std::pow((v + 0.055f) / 1.055f, 2.4f);
}
static void lpShapeDraw3D(uint64_t gid, int kind, float cr, float cg, float cb, float ca) {
    const auto& v = g_lpShape;
    int n = (int)v.size();
    ::std::vector<::std::array<int, 3>> t;
    if (kind == TRIANGLES)           for (int i = 0; i + 2 < n; i += 3) t.push_back({i, i + 1, i + 2});
    else if (kind == TRIANGLE_STRIP) for (int i = 2; i < n; i++) {
        if (i % 2) t.push_back({i - 1, i - 2, i}); else t.push_back({i - 2, i - 1, i}); }
    else if (kind == QUADS)          for (int i = 0; i + 3 < n; i += 4) {
        t.push_back({i, i + 1, i + 2}); t.push_back({i, i + 2, i + 3}); }
    else if (kind == QUAD_STRIP)     for (int i = 0; i + 3 < n; i += 2) {
        t.push_back({i, i + 1, i + 3}); t.push_back({i, i + 3, i + 2}); }
    else                             for (int i = 2; i < n; i++) t.push_back({0, i - 1, i});  // polygon, fan
    if (t.empty()) return;
    // libprocessing paints a mesh with the current fill, not its vertex colors,
    // so draw one mesh per fill color and set that fill first.
    ::std::vector<int> order(t.size());
    for (size_t i = 0; i < t.size(); i++) order[i] = (int)i;
    auto colKey = [&](int ti) { const auto& q = v[t[ti][0]]; return ::std::make_tuple(q.r, q.g, q.b, q.a); };
    ::std::stable_sort(order.begin(), order.end(), [&](int a, int b) { return colKey(a) < colKey(b); });
    size_t i = 0;
    while (i < order.size()) {
        auto key = colKey(order[i]);
        ::std::vector<float> buf;  // cppmode-geocache
        for (; i < order.size() && colKey(order[i]) == key; i++) {
            const auto& tr = t[order[i]];
            const auto &A = v[tr[0]], &B = v[tr[1]], &C = v[tr[2]];
            // flat normal per triangle, like P3D's automatic normals
            float ux = B.x - A.x, uy = B.y - A.y, uz = B.z - A.z;
            float wx = C.x - A.x, wy = C.y - A.y, wz = C.z - A.z;
            float nx = uy * wz - uz * wy, ny = uz * wx - ux * wz, nz = ux * wy - uy * wx;
            float len = ::std::sqrt(nx * nx + ny * ny + nz * nz);
            if (len > 0) { nx /= len; ny /= len; nz /= len; } else { nx = 0; ny = 0; nz = 1; }
            for (int j : tr) {
                const auto& q = v[j];
                if (q.hasN) buf.insert(buf.end(), {q.nx, q.ny, q.nz}); else buf.insert(buf.end(), {nx, ny, nz});
                buf.insert(buf.end(), {lpSrgbToLinear(q.r), lpSrgbToLinear(q.g), lpSrgbToLinear(q.b), q.a, q.x, q.y, q.z});
            }
        }
        uint64_t geo = lpGeoCached(buf, false);
        if (!geo) return;
        processing_set_fill(gid, lp_rgba(::std::get<0>(key), ::std::get<1>(key), ::std::get<2>(key), ::std::get<3>(key)));
        lpLitFillChanged(::std::get<0>(key), ::std::get<1>(key), ::std::get<2>(key), ::std::get<3>(key));
        processing_model(gid, geo);
    }
    // back to the sketch's current fill
    processing_set_fill(gid, lp_rgba(cr, cg, cb, ca));
    lpLitFillChanged(cr, cg, cb, ca);
}
// cppmode-texsmooth: a textured shape is one mesh with UVs, drawn with an unlit
// material holding the image; tint() colors it like Java (fill is ignored)
static void lpShapeDrawTex(uint64_t gid, int kind, bool doTint, float tr, float tg, float tb, float ta) {
    const auto& v = g_lpShape;
    int n = (int)v.size();
    ::std::vector<::std::array<int, 3>> t;
    if (kind == TRIANGLES)           for (int i = 0; i + 2 < n; i += 3) t.push_back({i, i + 1, i + 2});
    else if (kind == TRIANGLE_STRIP) for (int i = 2; i < n; i++) {
        if (i % 2) t.push_back({i - 1, i - 2, i}); else t.push_back({i - 2, i - 1, i}); }
    else if (kind == QUADS)          for (int i = 0; i + 3 < n; i += 4) {
        t.push_back({i, i + 1, i + 2}); t.push_back({i, i + 2, i + 3}); }
    else if (kind == QUAD_STRIP)     for (int i = 0; i + 3 < n; i += 2) {
        t.push_back({i, i + 1, i + 3}); t.push_back({i, i + 3, i + 2}); }
    else                             for (int i = 2; i < n; i++) t.push_back({0, i - 1, i});
    if (t.empty()) return;
    if (!doTint) { tr = tg = tb = ta = 1.0f; }
    static ::std::map<::std::tuple<uint64_t, float, float, float, float, bool>, uint64_t> mats;
    auto key = ::std::make_tuple(g_lpTexImg, tr, tg, tb, ta, g_lpTexAlpha);
    uint64_t mat = lpLitOn() ? lpLitTexMatFor(g_lpTexImg, tr, tg, tb, ta) : 0;  // cppmode-littex
    auto it = mats.find(key);
    if (mat) {}
    else if (it != mats.end()) mat = it->second;
    else {
        mat = processing_material_create_pbr();
        if (!mat) return;
        processing_material_set_unlit(mat, true);
        processing_material_set_double_sided(mat, true);
        processing_material_set_texture(mat, "texture", g_lpTexImg);
        processing_material_set_float4(mat, "base_color", tr, tg, tb, ta);
        if (g_lpTexAlpha || ta < 1.0f) processing_material_set_alpha_mode(mat, 2, 0.5f);
        mats[key] = mat;
    }
    ::std::vector<float> buf;  // cppmode-geocache
    for (const auto& tri : t) {
        const auto &A = v[tri[0]], &B = v[tri[1]], &C = v[tri[2]];
        float ux = B.x - A.x, uy = B.y - A.y, uz = B.z - A.z;
        float wx = C.x - A.x, wy = C.y - A.y, wz = C.z - A.z;
        float nx = uy * wz - uz * wy, ny = uz * wx - ux * wz, nz = ux * wy - uy * wx;
        float len = ::std::sqrt(nx * nx + ny * ny + nz * nz);
        if (len > 0) { nx /= len; ny /= len; nz /= len; } else { nx = 0; ny = 0; nz = 1; }
        for (int j : tri) {
            const auto& q = v[j];
            if (q.hasN) buf.insert(buf.end(), {q.nx, q.ny, q.nz}); else buf.insert(buf.end(), {nx, ny, nz});
            buf.insert(buf.end(), {1.0f, 1.0f, 1.0f, 1.0f, q.u, q.v, q.x, q.y, q.z});
        }
    }
    uint64_t geo = lpGeoCached(buf, true);
    if (!geo) return;
    processing_material(gid, mat);
    processing_model(gid, geo);
    processing_reset_material(gid);
}
#else
static void lpFrameGeosFree() {}
static void lpShapeOpen(uint64_t) {}
#endif

// cppmode-glshapefill: holes from beginContour()/endContour() for the OpenGL fill
static ::std::vector<::std::vector<::std::array<float,3>>> g_glContours;
// cppmode-glcurves: curve points of the shape being built, for both renderers
static ::std::vector<::std::array<float,3>> g_curvePts;
// cppmode-curves: Java's curve basis (Catmull-Rom with curveTightness). Each new vertex past
// the fourth draws the segment between the middle two, curveDetail steps long.
static void lpCurveVertex(PApplet* p, float x, float y, float z) {
    g_curvePts.push_back({x, y, z});
    size_t n = g_curvePts.size();
    if (n < 4) return;
    const auto &P0 = g_curvePts[n-4], &P1 = g_curvePts[n-3], &P2 = g_curvePts[n-2], &P3 = g_curvePts[n-1];
    float s = p->curveTightnessVal;
    float m[4][4] = { {(s-1)/2, (s+3)/2, (-3-s)/2, (1-s)/2},
                      {(1-s),   (-5-s)/2, (s+2),   (s-1)/2},
                      {(s-1)/2, 0,       (1-s)/2,  0},
                      {0,       1,       0,        0} };
    bool flat = P0[2] == 0 && P1[2] == 0 && P2[2] == 0 && P3[2] == 0;
    int d = p->curveDetailVal > 0 ? p->curveDetailVal : 20;
    for (int i = (n == 4 ? 0 : 1); i <= d; i++) {
        float t = i / (float)d, T[4] = {t*t*t, t*t, t, 1}, w[4];
        for (int k = 0; k < 4; k++) w[k] = T[0]*m[0][k] + T[1]*m[1][k] + T[2]*m[2][k] + T[3]*m[3][k];
        float q[3];
        for (int a = 0; a < 3; a++) q[a] = w[0]*P0[a] + w[1]*P1[a] + w[2]*P2[a] + w[3]*P3[a];
        if (flat) p->vertex(q[0], q[1]); else p->vertex(q[0], q[1], q[2]);
    }
}
void PApplet::beginShape(int kind){
    g_curvePts.clear();  // cppmode-glcurves
    g_glContours.clear();
    shapeKind = kind;
    inShape   = true;
    shape3D   = false;
    shapeVerts.clear();
    shapeVerts3D.clear(); shapeCols.clear();
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend){ g_lpShapeKind = kind; g_lpShapeOpen = false; g_lpShape.clear(); g_lpTexImg = 0; g_lpHasNormal = false; g_lpHasLast = false; g_lpCurve.clear(); return; }
#endif
    // Vertices are always collected in shapeVerts / shapeVerts3D.
    // endShape() draws fill and/or stroke from those collections.
    // shape3D is set when 3-component vertices are used (vertex(x,y,z)).
}
void PApplet::vertex(float x, float y) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    g_lpLast[0] = x; g_lpLast[1] = y; g_lpLast[2] = 0; g_lpHasLast = true;  // cppmode-curves
#endif
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend && inShape){
        if (g_lpShapeOpen) processing_vertex(_backend->graphics_id(), x, y);
        else lpPushV({x, y, 0.0f, fillR, fillG, fillB, fillA});
        return; }
#endif
    if (inContour) { contourVerts.push_back({x,y}); return; }
    if (!inShape)  return;
    shapeVerts.push_back({x, y});
    shapeCols.push_back({fillR, fillG, fillB, fillA}); shapeVerts3D.push_back({x, y, 0.0f});
}
void PApplet::vertex(float x, float y, float z) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    g_lpLast[0] = x; g_lpLast[1] = y; g_lpLast[2] = z; g_lpHasLast = true;  // cppmode-curves
#endif
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend && inShape){
        if (z != 0.0f) shape3D = true;
        if (g_lpShapeOpen) processing_vertex(_backend->graphics_id(), x, y);
        else lpPushV({x, y, z, fillR, fillG, fillB, fillA});
        return; }
#endif
    if (inContour) { contourVerts.push_back({x,y}); return; }
    if (!inShape)  return;
    shapeVerts.push_back({x, y});
    shapeCols.push_back({fillR, fillG, fillB, fillA}); shapeVerts3D.push_back({x, y, z});
    if (z != 0.0f) shape3D = true;
}
void PApplet::vertex(float x, float y, float u, float v) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend){ vertex(x, y, 0.0f, u, v); return; }
#endif
    if (!inShape) return;
    shapeVerts.push_back({x, y});
    shapeCols.push_back({fillR, fillG, fillB, fillA}); shapeVerts3D.push_back({x, y, 0.0f});
    // UV stored for future texture mapping support
}
void PApplet::vertex(float x, float y, float z, float u, float v) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend){
        if (!inShape) return;
        if (z != 0.0f) shape3D = true;
        if (g_lpShapeOpen) { processing_vertex(_backend->graphics_id(), x, y); return; }
        LpShapeVert q{x, y, z, fillR, fillG, fillB, fillA};
        if (textureModeVal == IMAGE) { u /= g_lpTexW; v /= g_lpTexH; }
        q.u = u; q.v = v;
        lpPushV(q);
        return; }
#endif
    if (!inShape) return;
    shapeVerts.push_back({x, y});
    shapeCols.push_back({fillR, fillG, fillB, fillA}); shapeVerts3D.push_back({x, y, z});
    if (z != 0.0f) shape3D = true;
}
void PApplet::beginContour(){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend && inShape){ lpShapeOpen(_backend->graphics_id()); processing_begin_contour(_backend->graphics_id()); return; }  // cppmode-api4
#endif
    inContour=true;contourVerts.clear();}
void PApplet::endContour()  {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend && inShape){ processing_end_contour(_backend->graphics_id()); return; }
#endif
    if (inContour && contourVerts.size() >= 3) {  // cppmode-glshapefill: keep the hole for endShape()
        ::std::vector<::std::array<float,3>> ring;
        for (auto& p : contourVerts) ring.push_back({p.first, p.second, 0.0f});
        g_glContours.push_back(ring);
    }
    inContour=false; contourVerts.clear();}

void PApplet::endShape(int mode){
    if(!inShape){return;}
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend){
        uint64_t gid = _backend->graphics_id();
        if (g_lpTexImg && !g_lpShapeOpen && shapeKind != POINTS && shapeKind != LINES)
            lpShapeDrawTex(gid, shapeKind, doTint, tintR, tintG, tintB, tintA);
        else if (g_lpShapeOpen) processing_end_shape(gid, mode == CLOSE);
        else if (shape3D && shapeKind != POINTS && shapeKind != LINES) { if (doFill) lpShapeDraw3D(gid, shapeKind, fillR, fillG, fillB, fillA); }
        else { lpShapeOpen(gid); processing_end_shape(gid, mode == CLOSE); }
        g_lpShapeOpen = false; g_lpShape.clear(); inShape = false; return; }
#endif
    bool cl=(mode==CLOSE);
    int  n3 = (int)shapeVerts3D.size();

    // -- Explicit shape kinds (QUADS, QUAD_STRIP, TRIANGLES, etc.) ----------
    // Draw fill and/or stroke from the collected vertex arrays.
    if(shapeKind != -1 && n3 > 0){
        GLenum gm;
        switch(shapeKind){
            case POINTS:        gm=GL_POINTS;        break;
            case LINES:         gm=GL_LINES;         break;
            case TRIANGLES:     gm=GL_TRIANGLES;     break;
            case TRIANGLE_FAN:  gm=GL_TRIANGLE_FAN;  break;
            case TRIANGLE_STRIP:gm=GL_TRIANGLE_STRIP;break;
#ifdef __EMSCRIPTEN__
            case QUADS:         gm=GL_TRIANGLES;     break; // GL_QUADS unsupported in WebGL
            case QUAD_STRIP:    gm=GL_TRIANGLE_STRIP; break; // GL_QUAD_STRIP unsupported in WebGL
#else
            case QUADS:         gm=GL_QUADS;         break;
            case QUAD_STRIP:    gm=GL_QUAD_STRIP;    break;
#endif
            default:            gm=GL_POLYGON;       break;
        }

        if(doFill && shapeKind != POINTS && shapeKind != LINES){
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f, 1.0f);
            applyFill();
            // cppmode-vertexfill: each vertex keeps the fill it was given, like Java
            bool perVertex = shapeCols.size() == shapeVerts3D.size();
            bool flatJava2D = perVertex && !cppBlendsVertexColors();
            if (flatJava2D) glShadeModel(GL_FLAT);  // each primitive takes its last vertex's color
            glBegin(gm);
            for (size_t i = 0; i < shapeVerts3D.size(); i++) {
                if (perVertex) { const auto& k = shapeCols[i]; glColor4f(k[0], k[1], k[2], k[3]); }
                const auto& v = shapeVerts3D[i];
                glVertex3f(v[0], v[1], v[2]);
            }
            glEnd();
            if (perVertex) applyFill();
            if (flatJava2D) glShadeModel(GL_SMOOTH);
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
        // POINTS and LINES use stroke color drawn directly
        if(shapeKind == POINTS){
            applyStroke();
            glPointSize(strokeW);
            glBegin(GL_POINTS);
            for(auto& v : shapeVerts3D) glVertex3f(v[0], v[1], v[2]);
            glEnd();
            restoreLighting();
            inShape=false; shape3D=false; shapeVerts.clear(); shapeVerts3D.clear(); shapeCols.clear();
            return;
        }
        if(shapeKind == LINES){
            applyStroke(); glLineWidth(strokeW);
            glBegin(GL_LINES);
            for(auto& v : shapeVerts3D) glVertex3f(v[0], v[1], v[2]);
            glEnd();
            restoreLighting();
            inShape=false; shape3D=false; shapeVerts.clear(); shapeVerts3D.clear(); shapeCols.clear();
            return;
        }
        // Draw stroke outlines -- shapeVerts was collected in vertex()
        if(doStroke && !shapeVerts.empty()){
            applyStroke(); glLineWidth(strokeW);
            int n=(int)shapeVerts.size();
            // Use 3D verts for correct depth in transformed space
            auto lv=[&](int i)->::std::array<float,3>{ return shapeVerts3D[i]; };
            switch(shapeKind){
                case TRIANGLE_STRIP:
                    glBegin(GL_LINES);
                    for(int i=0;i+2<n;i++){
                        auto a=lv(i),b=lv(i+1),c=lv(i+2);
                        glVertex3f(a[0],a[1],a[2]);glVertex3f(b[0],b[1],b[2]);
                        glVertex3f(b[0],b[1],b[2]);glVertex3f(c[0],c[1],c[2]);
                        glVertex3f(c[0],c[1],c[2]);glVertex3f(a[0],a[1],a[2]);
                    }
                    glEnd();
                    break;
                case TRIANGLE_FAN:
                    glBegin(GL_LINES);
                    for(int i=1;i+1<n;i++){
                        auto a=lv(0),b=lv(i),c=lv(i+1);
                        glVertex3f(a[0],a[1],a[2]);glVertex3f(b[0],b[1],b[2]);
                        glVertex3f(b[0],b[1],b[2]);glVertex3f(c[0],c[1],c[2]);
                        glVertex3f(c[0],c[1],c[2]);glVertex3f(a[0],a[1],a[2]);
                    }
                    glEnd();
                    break;
                case TRIANGLES:
                    glBegin(GL_LINES);
                    for(int i=0;i+2<n;i+=3){
                        auto a=lv(i),b=lv(i+1),c=lv(i+2);
                        glVertex3f(a[0],a[1],a[2]);glVertex3f(b[0],b[1],b[2]);
                        glVertex3f(b[0],b[1],b[2]);glVertex3f(c[0],c[1],c[2]);
                        glVertex3f(c[0],c[1],c[2]);glVertex3f(a[0],a[1],a[2]);
                    }
                    glEnd();
                    break;
                case QUADS:
                    glBegin(GL_LINES);
                    for(int i=0;i+3<n;i+=4){
                        auto a=lv(i),b=lv(i+1),c=lv(i+2),d=lv(i+3);
                        glVertex3f(a[0],a[1],a[2]);glVertex3f(b[0],b[1],b[2]);
                        glVertex3f(b[0],b[1],b[2]);glVertex3f(c[0],c[1],c[2]);
                        glVertex3f(c[0],c[1],c[2]);glVertex3f(d[0],d[1],d[2]);
                        glVertex3f(d[0],d[1],d[2]);glVertex3f(a[0],a[1],a[2]);
                    }
                    glEnd();
                    break;
                case QUAD_STRIP:
                    glBegin(GL_LINES);
                    for(int i=0;i+3<n;i+=2){
                        auto a=lv(i),b=lv(i+1),c=lv(i+3),d=lv(i+2);
                        glVertex3f(a[0],a[1],a[2]);glVertex3f(b[0],b[1],b[2]);
                        glVertex3f(b[0],b[1],b[2]);glVertex3f(c[0],c[1],c[2]);
                        glVertex3f(c[0],c[1],c[2]);glVertex3f(d[0],d[1],d[2]);
                        glVertex3f(d[0],d[1],d[2]);glVertex3f(a[0],a[1],a[2]);
                    }
                    glEnd();
                    break;
                default:
                    glBegin(cl?GL_LINE_LOOP:GL_LINE_STRIP);
                    for(auto& v:shapeVerts3D)glVertex3f(v[0],v[1],v[2]);
                    glEnd();
                    break;
            }
        }
        restoreLighting();
        inShape=false; shape3D=false; shapeVerts.clear(); shapeVerts3D.clear(); shapeCols.clear();
        return;
    }
    if(shapeVerts.empty()){inShape=false;return;}

    // Default beginShape() / endShape(CLOSE) path.
    // Use stencil buffer to fill arbitrary (including concave) polygons correctly.
    // This matches Java Processing's polygon fill behavior.
    if(shapeKind==-1 || shapeKind==CLOSE){
        if(doFill && shapeVerts.size()>=3){
            glDisable(GL_CULL_FACE);

            // Step 1: Write polygon to stencil buffer using odd-even fill rule
            glEnable(GL_STENCIL_TEST);
            glClear(GL_STENCIL_BUFFER_BIT);
            glStencilFunc(GL_ALWAYS, 0, ~0);
            glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT); // toggle stencil bit
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // don't write color
            glDepthMask(GL_FALSE);  // cppmode-glshapefill: or step 2, at the same depth, fails the depth test
            glBegin(GL_TRIANGLE_FAN);
            for(auto& v : shapeVerts3D) glVertex3f(v[0], v[1], v[2]);
            glEnd();
            for (auto& ring : g_glContours) {  // cppmode-glshapefill: holes flip the stencil back
                glBegin(GL_TRIANGLE_FAN);
                for (auto& v : ring) glVertex3f(v[0], v[1], v[2]);
                glEnd();
            }
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);

            // Step 2: draw the fill where the stencil is set.
            // cppmode-glshapefill: cover with the shape's own triangles, not a box at the
            // middle depth: a box misses parts of a rotated or non-flat shape and gets the
            // depth wrong. GL_ZERO clears each pixel as it is drawn, so overlaps draw once.
            glStencilFunc(GL_NOTEQUAL, 0, ~0);
            glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
            applyFill();
            // cppmode-polyfill: each vertex keeps the fill it was given, blended across the shape like Java
            bool perVertex = shapeCols.size() == shapeVerts3D.size() && cppBlendsVertexColors();
            glBegin(GL_TRIANGLE_FAN);
            for (size_t i = 0; i < shapeVerts3D.size(); i++) {
                if (perVertex) { const auto& k = shapeCols[i]; glColor4f(k[0], k[1], k[2], k[3]); }
                const auto& v = shapeVerts3D[i];
                glVertex3f(v[0], v[1], v[2]);
            }
            glEnd();
            if (perVertex) applyFill();
            glDisable(GL_STENCIL_TEST);
        }
        if(doStroke){
            applyStroke(); glLineWidth(strokeW);
            glBegin(cl ? GL_LINE_LOOP : GL_LINE_STRIP);
            // Use 3D coords so transforms (rotateX/Y) are respected
            for(auto& v : shapeVerts3D) glVertex3f(v[0], v[1], v[2]);
            glEnd();
            for (auto& ring : g_glContours) {  // cppmode-glshapefill
                glBegin(GL_LINE_LOOP);
                for (auto& v : ring) glVertex3f(v[0], v[1], v[2]);
                glEnd();
            }
            restoreLighting();
        }
    }
    inShape=false; shapeVerts.clear(); shapeVerts3D.clear(); shapeCols.clear(); g_glContours.clear();
}

void PApplet::bezierVertex(float cx1,float cy1,float cx2,float cy2,float x,float y) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend && inShape){  // cppmode-curves: sample with bezierDetail, like Java
        if (!g_lpHasLast) { vertex(x, y); return; }
        float x0 = g_lpLast[0], y0 = g_lpLast[1]; int sg = bezierDetailVal > 0 ? bezierDetailVal : 20;
        for (int i = 1; i <= sg; i++) { float t = i / (float)sg, u = 1 - t;
            vertex(u*u*u*x0 + 3*u*u*t*cx1 + 3*u*t*t*cx2 + t*t*t*x, u*u*u*y0 + 3*u*u*t*cy1 + 3*u*t*t*cy2 + t*t*t*y); }
        return; }
#endif
    if(!inShape||shapeVerts.empty())return;
    auto[x0,y0]=shapeVerts.back();const int sg=bezierDetailVal;
    for(int i=1;i<=sg;i++){float t=i/(float)sg,u=1-t;
        float bx=u*u*u*x0+3*u*u*t*cx1+3*u*t*t*cx2+t*t*t*x, by=u*u*u*y0+3*u*u*t*cy1+3*u*t*t*cy2+t*t*t*y;
        shapeVerts.push_back({bx,by}); shapeCols.push_back({fillR, fillG, fillB, fillA}); shapeVerts3D.push_back({bx,by,0.0f});}
}
void PApplet::quadraticVertex(float cx,float cy,float x,float y){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend && inShape){  // cppmode-curves
        if (!g_lpHasLast) { vertex(x, y); return; }
        float x0 = g_lpLast[0], y0 = g_lpLast[1]; int sg = bezierDetailVal > 0 ? bezierDetailVal : 20;
        for (int i = 1; i <= sg; i++) { float t = i / (float)sg, u = 1 - t;
            vertex(u*u*x0 + 2*u*t*cx + t*t*x, u*u*y0 + 2*u*t*cy + t*t*y); }
        return; }
#endif
    if(!inShape||shapeVerts.empty())return;
    auto[x0,y0]=shapeVerts.back();const int sg=bezierDetailVal;
    for(int i=1;i<=sg;i++){float t=i/(float)sg,u=1-t;float qx=u*u*x0+2*u*t*cx+t*t*x,qy=u*u*y0+2*u*t*cy+t*t*y;shapeVerts.push_back({qx,qy});shapeCols.push_back({fillR, fillG, fillB, fillA}); shapeVerts3D.push_back({qx,qy,0.0f});}
}
void PApplet::curveVertex(float x,float y){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend && inShape){ lpCurveVertex(this, x, y, 0.0f); return; }  // cppmode-curves
#endif
   if(inShape) lpCurveVertex(this, x, y, 0.0f); }  // cppmode-glcurves: OpenGL drew the raw control points as a polygon
void PApplet::curveVertex(float x,float y,float z){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend && inShape){ lpCurveVertex(this, x, y, z); return; }  // cppmode-curves
#endif
   if(inShape) lpCurveVertex(this, x, y, z); }  // cppmode-glcurves
void PApplet::bezierVertex(float cx1,float cy1,float cz1,float cx2,float cy2,float cz2,float x,float y,float z){
    if(!inShape) return;
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend){  // cppmode-api4: sample the curve into the mesh path's vertices
        if (cz1 == 0.0f && cz2 == 0.0f && z == 0.0f && !shape3D) { bezierVertex(cx1, cy1, cx2, cy2, x, y); return; }
        if (g_lpShapeOpen || g_lpShape.empty()) { vertex(x, y, z); return; }
        LpShapeVert a = g_lpShape.back();
        int steps = bezierDetailVal > 0 ? bezierDetailVal : 20;
        shape3D = true;
        for (int i = 1; i <= steps; i++) {
            float t = (float)i / steps, mt = 1 - t;
            float b0 = mt*mt*mt, b1 = 3*mt*mt*t, b2 = 3*mt*t*t, b3 = t*t*t;
            lpPushV({b0*a.x + b1*cx1 + b2*cx2 + b3*x, b0*a.y + b1*cy1 + b2*cy2 + b3*y, b0*a.z + b1*cz1 + b2*cz2 + b3*z,
                     fillR, fillG, fillB, fillA});
        }
        return; }
#endif
    if(shapeVerts3D.empty()){ vertex(x, y, z); return; }  // cppmode-glcurves
    float ax=shapeVerts3D.back()[0],ay=shapeVerts3D.back()[1],az=shapeVerts3D.back()[2];
    int steps=bezierDetailVal>0?bezierDetailVal:20;
    shape3D=true;
    for(int i=1;i<=steps;i++){
        float t=(float)i/steps,t2=t*t,t3=t2*t,mt=1-t,mt2=mt*mt,mt3=mt2*mt;
        float px=mt3*ax+3*mt2*t*cx1+3*mt*t2*cx2+t3*x;
        float py=mt3*ay+3*mt2*t*cy1+3*mt*t2*cy2+t3*y;
        float pz=mt3*az+3*mt2*t*cz1+3*mt*t2*cz2+t3*z;
        shapeVerts.push_back({px,py}); shapeCols.push_back({fillR, fillG, fillB, fillA}); shapeVerts3D.push_back({px,py,pz});
    }
}

void PApplet::bezier(float x1,float y1,float cx1,float cy1,float cx2,float cy2,float x2,float y2){
    { beginShape(); vertex(x1, y1); bezierVertex(cx1, cy1, cx2, cy2, x2, y2); endShape(); return; }  // cppmode-glcurves: both renderers, fills like Java

    if(!doStroke)return;applyStroke();glLineWidth(strokeW);glBegin(GL_LINE_STRIP);
    for(int i=0;i<=bezierDetailVal;i++){float t=i/(float)bezierDetailVal,u=1-t;
        glVertex2f(u*u*u*x1+3*u*u*t*cx1+3*u*t*t*cx2+t*t*t*x2,u*u*u*y1+3*u*u*t*cy1+3*u*t*t*cy2+t*t*t*y2);}
    glEnd();
}
void PApplet::curve(float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3){
    { beginShape(); curveVertex(x0, y0); curveVertex(x1, y1); curveVertex(x2, y2); curveVertex(x3, y3); endShape(); return; }  // cppmode-glcurves
    if(!doStroke)return;applyStroke();glLineWidth(strokeW);glBegin(GL_LINE_STRIP);
    float s=curveTightnessVal;
    for(int i=0;i<=curveDetailVal;i++){
        float t=i/(float)curveDetailVal,t2=t*t,t3=t2*t;
        float b0=(-s*t3+2*s*t2-s*t)/2.f,b1=((2-s)*t3+(s-3)*t2+1)/2.f;
        float b2=((s-2)*t3+(3-2*s)*t2+s*t)/2.f,b3=(s*t3-s*t2)/2.f;
        glVertex2f(b0*x0+b1*x1+b2*x2+b3*x3,b0*y0+b1*y1+b2*y2+b3*y3);
    }
    glEnd();
}
float PApplet::bezierPoint(float a,float b,float c,float d,float t){float u=1-t;return u*u*u*a+3*u*u*t*b+3*u*t*t*c+t*t*t*d;}
float PApplet::bezierTangent(float a,float b,float c,float d,float t){float u=1-t;return 3*u*u*(b-a)+6*u*t*(c-b)+3*t*t*(d-c);}
float PApplet::curvePoint(float a,float b,float c,float d,float t){float t2=t*t,t3=t2*t,s=curveTightnessVal;return 0.5f*((-s*t3+2*s*t2-s*t)*a+((2-s)*t3+(s-3)*t2+1)*b+((s-2)*t3+(3-2*s)*t2+s*t)*c+(s*t3-s*t2)*d);}
float PApplet::curveTangent(float a,float b,float c,float d,float t){float t2=t*t,s=curveTightnessVal;return 0.5f*((-3*s*t2+4*s*t-s)*a+(3*(2-s)*t2+2*(s-3)*t)*b+(3*(s-2)*t2+2*(3-2*s)*t+s)*c+(3*s*t2-2*s*t)*d);}
void PApplet::curveDetail(int d)     {curveDetailVal=d;}
void PApplet::curveTightness(float t){curveTightnessVal=t;}
void PApplet::bezierDetail(int d)    {bezierDetailVal=d;}

// =============================================================================
// MATRIX
// =============================================================================

void PApplet::resetMatrix(){ if (_backend) { _backend->reset_matrix(); return; }
    if (defaultP3D) { applyStandardModelview(); return; }  // P3D: back to the camera view like Java, not raw identity
    glLoadIdentity();}
void PApplet::applyMatrix(float n00,float n01,float n02,float n03,float n10,float n11,float n12,float n13,float n20,float n21,float n22,float n23,float n30,float n31,float n32,float n33){
    if(_backend){
        float m[16]={n00,n01,n02,n03,n10,n11,n12,n13,n20,n21,n22,n23,n30,n31,n32,n33};
        _backend->apply_matrix(m);return;}  // cppmode-lightxform
    if(_backend){
        float m[16]={n00,n01,n02,n03,n10,n11,n12,n13,n20,n21,n22,n23,n30,n31,n32,n33};
        _backend->apply_matrix(m);return;}  // cppmode-lightxform
    float m[]={n00,n10,n20,n30,n01,n11,n21,n31,n02,n12,n22,n32,n03,n13,n23,n33};
    glMultMatrixf(m);
}
void PApplet::translate(float x,float y)        {if(_backend){_backend->translate(x,y,0);return;}glTranslatef(x,y,0);}
void PApplet::translate(float x,float y,float z){
    if(_backend){_backend->translate(x,y,z);return;}glTranslatef(x,y,z);}
void PApplet::scale(float s)                    {if(_backend){_backend->scale(s,s,1);return;}glScalef(s,s,1);}
void PApplet::scale(float sx,float sy)          {if(_backend){_backend->scale(sx,sy,1);return;}glScalef(sx,sy,1);}
void PApplet::rotate(float a)                   {if(_backend){_backend->rotate_z(a);return;}glRotatef(a*180.0f/PI,0,0,1);}
void PApplet::shearX(float a)                   {if(_backend){_backend->shear_x(a);return;}float m[]={1,0,0,0,::std::tan(a),1,0,0,0,0,1,0,0,0,0,1};glMultMatrixf(m);}
void PApplet::shearY(float a)                   {if(_backend){_backend->shear_y(a);return;}float m[]={1,::std::tan(a),0,0,0,1,0,0,0,0,1,0,0,0,0,1};glMultMatrixf(m);}

// cppmode-camextras: remember the camera and projection given to libprocessing so printCamera()
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
// cppmode-camfix2: the camera as Processing sees it: camera() values, or Java's default one
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
    if (!a->defaultP3D) return ec[axis];
    float e[3], x[3], y[3], z[3]; cppmodeCamBasis(a, e, x, y, z);
    return e[axis] + x[axis] * ec[0] + y[axis] * ec[1] + z[axis] * ec[2];
}
void PApplet::printMatrix(){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // cppmode-camfix2: camera * model, like Java's printMatrix in P3D
        Matrix mm = processing_get_matrix(_backend->graphics_id());
        float c[16], r[16]; cppmodeCamMatrix(this, c);
        for (int col = 0; col < 4; col++) for (int row = 0; row < 4; row++) {
            float s = 0; for (int k = 0; k < 4; k++) s += c[k * 4 + row] * mm.m[col * 4 + k]; r[col * 4 + row] = s; }
        cppmodePrint4x4(nullptr, defaultP3D ? r : mm.m); return; }
#endif
   float m[16];glGetFloatv(GL_MODELVIEW_MATRIX,m);for(int i=0;i<4;i++){for(int j=0;j<4;j++)::std::cout<<m[j*4+i]<<" ";::std::cout<<"\n";}}

static void projectPoint(float x,float y,float z,float& ox,float& oy,float& oz){
    float mv[16],proj[16];int vp[4];
    glGetFloatv(GL_MODELVIEW_MATRIX,mv);glGetFloatv(GL_PROJECTION_MATRIX,proj);glGetIntegerv(GL_VIEWPORT,vp);
    float cx=mv[0]*x+mv[4]*y+mv[8]*z+mv[12];
    float cy=mv[1]*x+mv[5]*y+mv[9]*z+mv[13];
    float cz=mv[2]*x+mv[6]*y+mv[10]*z+mv[14];
    float cw=mv[3]*x+mv[7]*y+mv[11]*z+mv[15];
    float px=proj[0]*cx+proj[4]*cy+proj[8]*cz+proj[12]*cw;
    float py=proj[1]*cx+proj[5]*cy+proj[9]*cz+proj[13]*cw;
    float pz=proj[2]*cx+proj[6]*cy+proj[10]*cz+proj[14]*cw;
    float pw=proj[3]*cx+proj[7]*cy+proj[11]*cz+proj[15]*cw;
    if(pw!=0){px/=pw;py/=pw;pz/=pw;}
    ox=vp[0]+(px+1)*0.5f*vp[2];
    oy=vp[1]+(1-py)*0.5f*vp[3];
    oz=(pz+1)*0.5f;
}
float PApplet::screenX(float x,float y,float z){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) return processing_screen_x(_backend->graphics_id(), x, y, z);  // cppmode-camextras
#endif
   float ox,oy,oz;projectPoint(x,y,z,ox,oy,oz);return ox;}
float PApplet::screenY(float x,float y,float z){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) return processing_screen_y(_backend->graphics_id(), x, y, z);
#endif
   float ox,oy,oz;projectPoint(x,y,z,ox,oy,oz);return oy;}
float PApplet::screenZ(float x,float y,float z){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) return processing_screen_z(_backend->graphics_id(), x, y, z);  // cppmode-camextras
#endif
   float ox,oy,oz;projectPoint(x,y,z,ox,oy,oz);return oz;}
float PApplet::modelX(float x,float y,float z) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) return processing_model_x(_backend->graphics_id(), x, y, z);  // cppmode-camextras
#endif
   float mv[16];glGetFloatv(GL_MODELVIEW_MATRIX,mv);return cppmodeGlModel(this, mv, x, y, z, 0);}  // cppmode-camfix2
float PApplet::modelY(float x,float y,float z) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) return processing_model_y(_backend->graphics_id(), x, y, z);  // cppmode-camextras
#endif
   float mv[16];glGetFloatv(GL_MODELVIEW_MATRIX,mv);return cppmodeGlModel(this, mv, x, y, z, 1);}  // cppmode-camfix2
float PApplet::modelZ(float x,float y,float z) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) return processing_model_z(_backend->graphics_id(), x, y, z);  // cppmode-camextras
#endif
   float mv[16];glGetFloatv(GL_MODELVIEW_MATRIX,mv);return cppmodeGlModel(this, mv, x, y, z, 2);}  // cppmode-camfix2

// =============================================================================
// CAMERA
// =============================================================================

// Internal helper -- sets up the standard Processing Y-flipped perspective
// camera. Called by camera() and perspective() so they stay in sync.
void PApplet::applyDefaultCamera() {
    float eyeZ  = ((float)logicalH / 2.0f) / ::std::tan(PI * 60.0f / 360.0f);
    float near_ = eyeZ / 10.0f;
    float far_  = eyeZ * 10.0f;
    if(gWindow){int fw=logicalW,fh=logicalH;glfwGetFramebufferSize(gWindow,&fw,&fh);if(fw>0)fbW=fw;if(fh>0)fbH=fh;}
    glViewport(0, 0, fbW, fbH);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glScalef(1,-1,1);
    _gluPerspective(60.0, (double)logicalW / logicalH, near_, far_);
    applyStandardModelview();
}

void PApplet::camera(){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        // Java's defaults, through the routed full-argument version
        float cz = (height * 0.5f) / ::std::tan(PI / 6.0f);
        camera(width * 0.5f, height * 0.5f, cz, width * 0.5f, height * 0.5f, 0, 0, 1, 0); return;
    }
#endif
    applyDefaultCamera();
}
void PApplet::camera(float ex,float ey,float ez,float cx,float cy,float cz,float ux,float uy,float uz){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) lpSetEye(ex, ey, ez);
#endif
    { float v[9] = {ex, ey, ez, cx, cy, cz, ux, uy, uz}; for (int i = 0; i < 9; i++) g_lpCam[i] = v[i]; g_lpCamSet = true; }  // cppmode-camfix2
    if(_backend){cppmodeLpCamera(_backend->graphics_id(),ex,ey,ez,cx,cy,cz,ux,uy,uz);return;}
    float eyeZ = ((float)logicalH/2.0f) / ::std::tan(PI*60.0f/360.0f);
    float near_ = eyeZ/10.0f, far_ = eyeZ*10.0f;
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glScalef(1,-1,1);
    _gluPerspective(60.0,(double)logicalW/logicalH,near_,far_);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    _gluLookAt(ex,ey,ez,cx,cy,cz,ux,uy,uz);
    glFrontFace(GL_CW);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}
void PApplet::beginCamera(){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { processing_push_matrix(_backend->graphics_id()); return; }  // cppmode-camextras
#endif
    glMatrixMode(GL_MODELVIEW);glPushMatrix();}
void PApplet::endCamera()  {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { processing_pop_matrix(_backend->graphics_id()); return; }  // cppmode-camextras
#endif
    glPopMatrix();}

void PApplet::perspective(){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        // Java's defaults, through the routed full-argument version
        float cz = (height * 0.5f) / ::std::tan(PI / 6.0f);
        perspective(PI / 3.0f, (float)width / (float)height, cz / 10.0f, cz * 10.0f); return;
    }
#endif
    applyDefaultCamera();
}
void PApplet::perspective(float fov, float aspect, float zNear, float zFar) {
    g_lpProjKind = 1; g_lpProj[0] = fov; g_lpProj[1] = aspect; g_lpProj[2] = zNear; g_lpProj[3] = zFar;  // cppmode-camfix2
    if(_backend){cppmodeLpPerspective(_backend->graphics_id(),fov,aspect,zNear,zFar);return;}
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glScalef(1,-1,1);
    _gluPerspective(degrees(fov), aspect, zNear, zFar);
    applyStandardModelview();
}
// Helper shared by ortho() and perspective() -- sets up the standard
// Processing modelview camera (eye at eyeZ looking at canvas centre,
// Y-down screen coordinates) and enables depth test.
void PApplet::applyStandardModelview() {
    float eyeZ = ((float)logicalH / 2.0f) / ::std::tan(PI * 60.0f / 360.0f);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    _gluLookAt(logicalW/2.0, logicalH/2.0, eyeZ,
              logicalW/2.0, logicalH/2.0, 0,
              0, 1, 0);
    glFrontFace(GL_CW);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void PApplet::ortho() {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        // Java's defaults, through the routed full-argument version
        float cz = (height * 0.5f) / ::std::tan(PI / 6.0f);
        ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f, 0.0f, cz * 10.0f); return;
    }
#endif
    // Default ortho: 1:1 pixel mapping, origin at top-left, Y increases downward.
    // glOrtho(l, r, bottom, top, near, far):
    //   bottom = winHeight (screen bottom = large Y in Processing)
    //   top    = 0         (screen top    = Y=0 in Processing)
    // gluLookAt is NOT used here -- we want raw screen-space coordinates,
    // so we use a simple identity modelview with Y-flipped glOrtho.
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glOrtho(0, logicalW, logicalH, 0, -logicalH, logicalH);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glFrontFace(GL_CW);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void PApplet::ortho(float l, float r, float b, float t, float n, float f) {
    { g_lpProjKind = 2; float v[6] = {l, r, b, t, n, f}; for (int i = 0; i < 6; i++) g_lpProj[i] = v[i]; }  // cppmode-camfix2
    if(_backend){cppmodeLpOrtho(_backend->graphics_id(),l,r,b,t,n,f);return;}
    // Processing Java ortho() uses the SAME standard modelview as perspective()
    // (eye at eyeZ above canvas center, looking at canvas center).
    // Only the projection changes: orthographic instead of frustum.
    // The Y-flip (glScalef 1,-1,1) is applied in projection space so that
    // Processing's Y-down screen coordinate convention is preserved.
    //
    // With the standard camera, world point (w/2, h/2, 0) is at screen center.
    // ortho(-w/2, w/2, -h/2, h/2) covers that range around the camera target.
    // So translate(w/2, h/2) correctly lands at the center of the screen.
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glScalef(1,-1,1);  // Y-flip before ortho (Java order)
    glOrtho(l, r, b, t, n, f);
    applyStandardModelview();
}

void PApplet::frustum(float l, float r, float b, float t, float n, float f) {
    if(_backend){  // cppmode-cursor-frustum: libprocessing has no off-center frustum, so use the matching perspective
        static bool warned = false;
        if (!warned && (::std::fabs(l + r) > 1e-4f || ::std::fabs(b + t) > 1e-4f)) {
            fprintf(stderr, "frustum(): off-center frustums aren't supported in the libprocessing renderer yet, using a centered one\n");
            warned = true;
        }
        float hh = (t - b) * 0.5f, hw = (r - l) * 0.5f;
        if (n > 0 && hh > 0 && hw > 0)
            cppmodeLpPerspective(_backend->graphics_id(), 2.0f * ::std::atan(hh / n), hw / hh, n, f);
        return;
    }
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glFrustum(l, r, b, t, n, f);
    applyStandardModelview();
}
void PApplet::printCamera(){
    {  // cppmode-camfix2: both renderers: camera() values, or Java's default camera
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
   float m[16];glGetFloatv(GL_MODELVIEW_MATRIX,m);::std::cout<<"Camera matrix:\n";for(int i=0;i<4;i++){for(int j=0;j<4;j++)::std::cout<<m[j*4+i]<<" ";::std::cout<<"\n";}}
void PApplet::printProjection(){
    {  // cppmode-camfix2: both renderers: last perspective()/ortho(), or Java's default perspective
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
   float m[16];glGetFloatv(GL_PROJECTION_MATRIX,m);::std::cout<<"Projection matrix:\n";for(int i=0;i<4;i++){for(int j=0;j<4;j++)::std::cout<<m[j*4+i]<<" ";::std::cout<<"\n";}}

// =============================================================================
// LIGHTS
// =============================================================================

// ---------------------------------------------------------------------------
// LIGHTING
//
// Processing Java passes light colors in the range [0..255].
// OpenGL fixed-function expects [0..1].  We normalise on entry.
//
// Light positions/directions must be submitted while the modelview matrix
// is in EYE SPACE (camera transform only, no object transforms).
// We use glPushMatrix/glLoadIdentity to achieve this for every light.
// ---------------------------------------------------------------------------

// Convenience: normalise a 0-255 colour component to 0.0-1.0
static inline float lc(float v) { return PApplet::g_papplet ? v/PApplet::g_papplet->colorMaxH : v/255.f; } // respects colorMode max

// Apply all common light state and reset the index counter.
// Call this at the start of a draw() that uses lights.
#ifdef CPPMODE_BACKEND_LIBPROCESSING
// Lights under libprocessing. Java clears lights every frame, so lights live in
// this list and are destroyed before the next frame's draw().
// Bevy uses physical units; these map Processing's 0..255 colors and are
// tuned by eye against the OpenGL renderer.
// Each can be overridden from the environment while tuning, e.g.
//   CPPMODE_LP_POINT=2e10 processing
static float lpEnvF(const char* name, float def) {
    const char* v = getenv(name); return (v && *v) ? (float)atof(v) : def; }
static const float LP_AMBIENT_BRIGHTNESS = lpEnvF("CPPMODE_LP_AMBIENT", 1000.0f);   // cd/m^2 at full white
static const float LP_DIRECTIONAL_LUX    = lpEnvF("CPPMODE_LP_DIRECTIONAL", 3141.6f); // lux at full white
// Bevy point lights fall off with distance squared in meters, and sketch units are
// pixels, so this has to be huge to light things a few hundred pixels away
static const float LP_POINT_LUMENS       = lpEnvF("CPPMODE_LP_POINT", 1.6e9f);         // lumens at full white
static const float LP_LIGHT_RANGE        = lpEnvF("CPPMODE_LP_RANGE", 10000.0f);     // in sketch pixels
// Processing adds light colors in sRGB space (128 + 128 = full brightness).
// Bevy adds them in linear space, and lp_rgba is decoded from sRGB, which turns
// 128 into 0.216. Encode first so the decoded linear value is Processing's 0.5.
static float lpLinToSrgb(float v) {
    v = v < 0 ? 0 : (v > 1 ? 1 : v);
    return v <= 0.0031308f ? v * 12.92f : 1.055f * ::std::pow(v, 1.0f / 2.4f) - 0.055f;
}
static Color lpLightColor(float r, float g, float b) {
    static bool shown = false;
    if (!shown) { shown = true;
        fprintf(stderr, "[CppMode] lights: ambient=%g directional=%g point=%g\n",
                LP_AMBIENT_BRIGHTNESS, LP_DIRECTIONAL_LUX, LP_POINT_LUMENS); }
    // Keep the hue as normal sRGB, then scale so the brightest channel lands on
    // Processing's level. Per-channel mapping washes colored lights out to pastel.
    auto dec = [](float v) { v = v < 0 ? 0 : (v > 1 ? 1 : v);
        return v <= 0.04045f ? v / 12.92f : ::std::pow((v + 0.055f) / 1.055f, 2.4f); };
    float m = ::std::max(r, ::std::max(g, b));
    if (m <= 0.0f) return lp_rgba(0, 0, 0, 1.0f);
    float k = m / dec(m);
    return lp_rgba(lpLinToSrgb(dec(r) * k), lpLinToSrgb(dec(g) * k), lpLinToSrgb(dec(b) * k), 1.0f);
}
static ::std::vector<uint64_t> g_lpLights;
static bool g_lpAmbientSet = false;
// fill() is unlit by default; lights only show on a lit material. The first
// light in a frame switches to a shared PBR material (fill still comes through
// vertex colors); noLights() and the next frame switch back by popping the style.
// ---- Processing-exact lighting --------------------------------------------
// Light calls fill slots of the shader's PLights uniform instead of creating
// Bevy lights; lit shapes draw with a custom material running P3D's formula.
static const char* LP_PLIGHTS_SHADER = R"WESL(
import processing::VertexOutput;
// Bevy's view uniform, declared here like processing/vertex.wesl does
// (that submodule isn't in every libprocessing build). Layout from bevy_render view.wgsl.
struct View {
    clip_from_world: mat4x4<f32>,
    unjittered_clip_from_world: mat4x4<f32>,
    world_from_clip: mat4x4<f32>,
    world_from_view: mat4x4<f32>,
    view_from_world: mat4x4<f32>,
    clip_from_view: mat4x4<f32>,
    view_from_clip: mat4x4<f32>,
}
@group(0) @binding(0) var<uniform> view: View;

// Processing's P3D lighting (LightVert.glsl), computed per pixel.
// pos.w is the kind: 0 ambient, 1 directional, 2 point, 3 spot.
// dir.w is cos(spot angle), color.w is the spot concentration.
struct PLights {
    count: vec4<f32>,
    fill: vec4<f32>,
    eye: vec4<f32>,
    mat_spec: vec4<f32>,
    mat_emissive: vec4<f32>,
    mat_ambient: vec4<f32>,
    l0_pos: vec4<f32>, l0_dir: vec4<f32>, l0_color: vec4<f32>, l0_spec: vec4<f32>, l0_fall: vec4<f32>,
    l1_pos: vec4<f32>, l1_dir: vec4<f32>, l1_color: vec4<f32>, l1_spec: vec4<f32>, l1_fall: vec4<f32>,
    l2_pos: vec4<f32>, l2_dir: vec4<f32>, l2_color: vec4<f32>, l2_spec: vec4<f32>, l2_fall: vec4<f32>,
    l3_pos: vec4<f32>, l3_dir: vec4<f32>, l3_color: vec4<f32>, l3_spec: vec4<f32>, l3_fall: vec4<f32>,
    l4_pos: vec4<f32>, l4_dir: vec4<f32>, l4_color: vec4<f32>, l4_spec: vec4<f32>, l4_fall: vec4<f32>,
    l5_pos: vec4<f32>, l5_dir: vec4<f32>, l5_color: vec4<f32>, l5_spec: vec4<f32>, l5_fall: vec4<f32>,
    l6_pos: vec4<f32>, l6_dir: vec4<f32>, l6_color: vec4<f32>, l6_spec: vec4<f32>, l6_fall: vec4<f32>,
    l7_pos: vec4<f32>, l7_dir: vec4<f32>, l7_color: vec4<f32>, l7_spec: vec4<f32>, l7_fall: vec4<f32>,
}
@group(3) @binding(0) var<uniform> plights: PLights;

struct Acc { amb: vec3<f32>, dif: vec3<f32>, spe: vec3<f32> }

fn to_linear(c: vec3<f32>) -> vec3<f32> {
    return select(pow((c + 0.055) / 1.055, vec3(2.4)), c / 12.92, c <= vec3(0.04045));
}
fn to_srgb(c: vec3<f32>) -> vec3<f32> {
    return select(1.055 * pow(c, vec3(1.0 / 2.4)) - 0.055, c * 12.92, c <= vec3(0.0031308));
}

fn light(acc: ptr<function, Acc>, p: vec4<f32>, d: vec4<f32>, c: vec4<f32>, s: vec4<f32>, f: vec4<f32>,
         P: vec3<f32>, N: vec3<f32>, V: vec3<f32>, shin: f32) {
    if (p.w < 0.5) {
        let dist = length(p.xyz - P);
        (*acc).amb += c.rgb / (f.x + f.y * dist + f.z * dist * dist);
        return;
    }
    var L = -normalize(d.xyz);
    var fo = 1.0;
    if (p.w > 1.5) {
        let to_l = p.xyz - P;
        let dist = length(to_l);
        L = to_l / max(dist, 1e-5);
        fo = 1.0 / (f.x + f.y * dist + f.z * dist * dist);
    }
    var spot = 1.0;
    if (p.w > 2.5) {
        let sc = dot(-L, normalize(d.xyz));
        spot = select(pow(max(sc, 0.0), c.w), 0.0, sc <= d.w);
    }
    let w = fo * spot;
    (*acc).dif += c.rgb * w * max(dot(L, N), 0.0);
    if (any(s.rgb > vec3(0.0))) {
        let H = normalize(L + V);
        (*acc).spe += s.rgb * w * pow(max(dot(N, H), 0.0), shin);
    }
}

@fragment
fn fragment(mesh: VertexOutput) -> @location(0) vec4<f32> {
    let P = mesh.world_position.xyz;
    let V = normalize(plights.eye.xyz - P);
    // 2D shapes have no normals: face the camera, like P3D does for them
    var N = V;
    if (length(mesh.world_normal) > 1e-4) { N = normalize(mesh.world_normal); }
    // P3D lights back faces with the flipped normal
    if (dot(N, V) < 0.0) { N = -N; }
    let fill = plights.fill.rgb;
    let shin = max(plights.mat_spec.w, 1.0);
    let n = i32(plights.count.x);
    var acc = Acc(vec3(0.0), vec3(0.0), vec3(0.0));
    if (n > 0) { light(&acc, plights.l0_pos, plights.l0_dir, plights.l0_color, plights.l0_spec, plights.l0_fall, P, N, V, shin); }
    if (n > 1) { light(&acc, plights.l1_pos, plights.l1_dir, plights.l1_color, plights.l1_spec, plights.l1_fall, P, N, V, shin); }
    if (n > 2) { light(&acc, plights.l2_pos, plights.l2_dir, plights.l2_color, plights.l2_spec, plights.l2_fall, P, N, V, shin); }
    if (n > 3) { light(&acc, plights.l3_pos, plights.l3_dir, plights.l3_color, plights.l3_spec, plights.l3_fall, P, N, V, shin); }
    if (n > 4) { light(&acc, plights.l4_pos, plights.l4_dir, plights.l4_color, plights.l4_spec, plights.l4_fall, P, N, V, shin); }
    if (n > 5) { light(&acc, plights.l5_pos, plights.l5_dir, plights.l5_color, plights.l5_spec, plights.l5_fall, P, N, V, shin); }
    if (n > 6) { light(&acc, plights.l6_pos, plights.l6_dir, plights.l6_color, plights.l6_spec, plights.l6_fall, P, N, V, shin); }
    if (n > 7) { light(&acc, plights.l7_pos, plights.l7_dir, plights.l7_color, plights.l7_spec, plights.l7_fall, P, N, V, shin); }
    let amb_mat = select(fill, plights.mat_ambient.rgb, plights.mat_ambient.w > 0.5);
    let rgb = clamp(acc.amb * amb_mat + acc.dif * fill + acc.spe * plights.mat_spec.rgb
                    + plights.mat_emissive.rgb, vec3(0.0), vec3(1.0));
    return vec4(to_linear(rgb), plights.fill.a);
}
)WESL";
static constexpr int LP_MAX_LIGHTS = 8;
static int   g_lpLightCount = 0;
static float g_lpFalloff[3] = {1, 0, 0};
static float g_lpLightSpec[3] = {0, 0, 0};
static bool g_lpLitFailed = false;
static uint64_t g_lpLitShader = 0;
struct LpLightSlot { float pos[4], dir[4], color[4], spec[4], fall[4]; };
static LpLightSlot g_lpSlots[LP_MAX_LIGHTS];
// one lit material per fill color; the fill is a shader value, the lights are shared
// specular(), shininess(), emissive(), ambient(): stored like Java's style state (0..1)
static float g_lpMatSpec[3] = {0, 0, 0}, g_lpMatShin = 1, g_lpMatEmis[3] = {0, 0, 0}, g_lpMatAmb[3] = {0, 0, 0};
static bool  g_lpMatAmbSet = false;
static float g_lpEye[3] = {0, 0, 0};  // camera position, set by camera()  // until ambient() is called, ambient follows the fill
static ::std::map<::std::array<float, 15>, uint64_t> g_lpLitMats;
static uint64_t g_lpPbr = 0;  // the lit material currently selected
static void lpUploadSlotRaw(uint64_t m, int i) {
    char nm[32];
    const LpLightSlot& L = g_lpSlots[i];
    auto set = [&](const char* f, const float* v) {
        snprintf(nm, sizeof nm, "l%d_%s", i, f);
        processing_material_set_float4(m, nm, v[0], v[1], v[2], v[3]); };
    set("pos", L.pos); set("dir", L.dir); set("color", L.color); set("spec", L.spec); set("fall", L.fall);
}
// cppmode-lightcache: what each lit material last received, so unchanged lights
// (the usual case: lights() every frame) cost nothing after the first frame
struct LpUploaded { LpLightSlot slots[LP_MAX_LIGHTS]; bool valid[LP_MAX_LIGHTS] = {}; int count = -1; };
static ::std::map<uint64_t, LpUploaded> g_lpUploaded;
static void lpUploadSlot(uint64_t m, int i) {
    LpUploaded& up = g_lpUploaded[m];
    if (up.valid[i] && ::std::memcmp(&up.slots[i], &g_lpSlots[i], sizeof(LpLightSlot)) == 0) return;
    lpUploadSlotRaw(m, i);
    up.slots[i] = g_lpSlots[i]; up.valid[i] = true;
}
static void lpUploadCount(uint64_t m) {
    LpUploaded& up = g_lpUploaded[m];
    if (up.count == g_lpLightCount) return;
    processing_material_set_float4(m, "count", (float)g_lpLightCount, 0, 0, 0);
    up.count = g_lpLightCount;
}
static uint64_t lpLitMatFor(float r, float g, float b, float a) {
    if (g_lpLitFailed) return 0;
    ::std::array<float, 15> key{r, g, b, a,
        g_lpMatSpec[0], g_lpMatSpec[1], g_lpMatSpec[2], g_lpMatShin,
        g_lpMatEmis[0], g_lpMatEmis[1], g_lpMatEmis[2],
        g_lpMatAmb[0], g_lpMatAmb[1], g_lpMatAmb[2], g_lpMatAmbSet ? 1.0f : 0.0f};
    auto it = g_lpLitMats.find(key);
    if (it != g_lpLitMats.end()) return it->second;
    if (!g_lpLitShader) {
        g_lpLitShader = processing_shader_create(LP_PLIGHTS_SHADER);
        if (!g_lpLitShader) { const char* e = processing_check_error();
            fprintf(stderr, "[CppMode] lights: shader failed to compile: %s\n", e ? e : "(no message)");
            g_lpLitFailed = true; return 0; }
    }
    uint64_t m = processing_material_create_custom(g_lpLitShader);
    if (!m) { const char* e = processing_check_error();
        fprintf(stderr, "[CppMode] lights: material failed: %s\n", e ? e : "(no message)");
        g_lpLitFailed = true; return 0; }
    processing_material_set_double_sided(m, true);
    processing_material_set_float4(m, "fill", r, g, b, a);
    processing_material_set_float4(m, "eye", g_lpEye[0], g_lpEye[1], g_lpEye[2], 1);
    processing_material_set_float4(m, "mat_spec", g_lpMatSpec[0], g_lpMatSpec[1], g_lpMatSpec[2], g_lpMatShin);
    processing_material_set_float4(m, "mat_emissive", g_lpMatEmis[0], g_lpMatEmis[1], g_lpMatEmis[2], 0);
    processing_material_set_float4(m, "mat_ambient", g_lpMatAmb[0], g_lpMatAmb[1], g_lpMatAmb[2], g_lpMatAmbSet ? 1.0f : 0.0f);
    for (int i = 0; i < g_lpLightCount; i++) lpUploadSlot(m, i);
    lpUploadCount(m);
    g_lpLitMats[key] = m;
    return m;
}
// cppmode-littex: a textured shape under lights() uses the lighting shader with its texture
// multiplied into the base color (in screen colors, like the rest of the shader), like Java.
static uint64_t g_lpLitTexShader = 0;
static ::std::map<::std::pair<uint64_t, ::std::array<float, 15>>, uint64_t> g_lpLitTexMats;
static uint64_t lpLitTexMatFor(uint64_t img, float r, float g, float b, float a) {
    if (!img || g_lpLitFailed) return 0;
    ::std::array<float, 15> k{r, g, b, a,
        g_lpMatSpec[0], g_lpMatSpec[1], g_lpMatSpec[2], g_lpMatShin,
        g_lpMatEmis[0], g_lpMatEmis[1], g_lpMatEmis[2],
        g_lpMatAmb[0], g_lpMatAmb[1], g_lpMatAmb[2], g_lpMatAmbSet ? 1.0f : 0.0f};
    auto key = ::std::make_pair(img, k);
    auto it = g_lpLitTexMats.find(key);
    if (it != g_lpLitTexMats.end()) return it->second;
    if (!g_lpLitTexShader) {
        ::std::string src = LP_PLIGHTS_SHADER;
        auto swap = [&](const char* a2, const char* b2) { auto at = src.find(a2); if (at != ::std::string::npos) src.replace(at, ::std::strlen(a2), b2); };
        swap("@group(3) @binding(0) var<uniform> plights: PLights;\n",
             "@group(3) @binding(0) var<uniform> plights: PLights;\n"
             "@group(3) @binding(1) var tex: texture_2d<f32>;\n"
             "@group(3) @binding(2) var tex_samp: sampler;\n");
        swap("    let fill = plights.fill.rgb;\n",
             "    let texel = textureSample(tex, tex_samp, mesh.uv);\n"
             "    let fill = plights.fill.rgb * to_srgb(texel.rgb);\n");
        swap("plights.fill.a);", "plights.fill.a * texel.a);");
        g_lpLitTexShader = processing_shader_create(src.c_str());
        if (!g_lpLitTexShader) { const char* e = processing_check_error();
            fprintf(stderr, "[CppMode] lit textures: shader failed to compile: %s\n", e ? e : "(no message)");
            g_lpLitFailed = true; return 0; }
    }
    uint64_t m = processing_material_create_custom(g_lpLitTexShader);
    if (!m) return 0;
    processing_material_set_double_sided(m, true);
    processing_material_set_float4(m, "fill", r, g, b, a);
    processing_material_set_float4(m, "eye", g_lpEye[0], g_lpEye[1], g_lpEye[2], 1);
    processing_material_set_float4(m, "mat_spec", g_lpMatSpec[0], g_lpMatSpec[1], g_lpMatSpec[2], g_lpMatShin);
    processing_material_set_float4(m, "mat_emissive", g_lpMatEmis[0], g_lpMatEmis[1], g_lpMatEmis[2], 0);
    processing_material_set_float4(m, "mat_ambient", g_lpMatAmb[0], g_lpMatAmb[1], g_lpMatAmb[2], g_lpMatAmbSet ? 1.0f : 0.0f);
    processing_material_set_texture(m, "tex", img);
    processing_material_set_texture(m, "tex_samp", img);
    for (int i = 0; i < g_lpLightCount; i++) lpUploadSlot(m, i);
    lpUploadCount(m);
    // also listed with the plain lit materials (under a key no fill can have), so every
    // light, eye and count upload reaches it too
    ::std::array<float, 15> listed{};
    listed[0] = -1.0f - (float)g_lpLitTexMats.size();
    g_lpLitMats[listed] = m;
    g_lpLitTexMats[key] = m;
    return m;
}
// kind: 0 ambient, 1 directional, 2 point, 3 spot. Positions and directions are
// in sketch coordinates; with P3D's camera, world and sketch axes agree.
static void lpAddLight(int kind, float r, float g, float b,
                       float x, float y, float z, float nx, float ny, float nz,
                       float spotCos, float conc) {
    if (!g_lpPbr || g_lpLightCount >= LP_MAX_LIGHTS) return;
    int i = g_lpLightCount++;
    LpLightSlot& L = g_lpSlots[i];
    // cppmode-lightxform: Java runs light positions and directions through the model matrix
    if (PApplet::g_papplet && PApplet::g_papplet->_backend) {
        auto* be = static_cast<LibProcessingBackend*>(PApplet::g_papplet->_backend);
        float o[3];
        be->model_point(x, y, z, 1, o);    x = o[0];  y = o[1];  z = o[2];
        be->model_point(nx, ny, nz, 0, o); nx = o[0]; ny = o[1]; nz = o[2];
    }
    float pos[4] = {x, y, z, (float)kind}, dir[4] = {nx, ny, nz, spotCos}, col[4] = {r, g, b, conc};
    float spe[4] = {g_lpLightSpec[0], g_lpLightSpec[1], g_lpLightSpec[2], 0};
    float fal[4] = {g_lpFalloff[0], g_lpFalloff[1], g_lpFalloff[2], 0};
    for (int k = 0; k < 4; k++) { L.pos[k] = pos[k]; L.dir[k] = dir[k]; L.color[k] = col[k]; L.spec[k] = spe[k]; L.fall[k] = fal[k]; }
    for (auto& kv : g_lpLitMats) {
        lpUploadSlot(kv.second, i);
        lpUploadCount(kv.second);
    }
}
static bool g_lpLit = false;
static bool lpLitOn() { return g_lpLit; }  // cppmode-littex
// cppmode-sentmat: the material last sent to libprocessing. push/popStyle make libprocessing
// restore its own previous material, so after them this is unknown and the next fill resends.
static uint64_t g_lpSentMat = 0;
static void lpLitForgetSent() { g_lpSentMat = 0; }
static void lpEnsureLit(uint64_t gid) {
    if (g_lpLit) return;
    PApplet* pa = PApplet::g_papplet;
    uint64_t m = pa ? lpLitMatFor(pa->fillR, pa->fillG, pa->fillB, pa->fillA) : 0;
    if (!m) return;
    g_lpPbr = m;
    processing_push_style(gid);
    processing_material(gid, m);
    g_lpLit = true;
}
static void lpLitFillChanged(float r, float g, float b, float a) {
    if (!g_lpLit || !PApplet::g_papplet || !PApplet::g_papplet->_backend) return;
    uint64_t m = lpLitMatFor(r, g, b, a);
    if (m) g_lpPbr = m;
    if (m && m != g_lpSentMat) { g_lpSentMat = m; processing_material(PApplet::g_papplet->_backend->graphics_id(), m); }  // cppmode-lightcache
}
// a material property changed: reselect the lit material for the current fill
static void lpSetEye(float x, float y, float z) {
    g_lpEye[0] = x; g_lpEye[1] = y; g_lpEye[2] = z;
    for (auto& kv : g_lpLitMats) processing_material_set_float4(kv.second, "eye", x, y, z, 1);
}
static void lpLitRefresh() {
    PApplet* pa = PApplet::g_papplet;
    if (pa) lpLitFillChanged(pa->fillR, pa->fillG, pa->fillB, pa->fillA);
}
static void lpUnlit(uint64_t gid) {
    if (!g_lpLit) return;
    processing_pop_style(gid);
    g_lpLit = false;
}
static void lpLightsReset(uint64_t gid) {
    lpUnlit(gid);
    g_lpLightCount = 0;
    g_lpFalloff[0] = 1; g_lpFalloff[1] = 0; g_lpFalloff[2] = 0;
    g_lpLightSpec[0] = g_lpLightSpec[1] = g_lpLightSpec[2] = 0;
    for (uint64_t id : g_lpLights) processing_light_destroy(id);
    g_lpLights.clear();
    // only touch ambient if the sketch set it, and then go back to Bevy's default
    if (g_lpAmbientSet) { processing_light_set_ambient(gid, lp_rgba(1, 1, 1, 1), 80.0f); g_lpAmbientSet = false; }
}
#else
static void lpLightsReset(uint64_t) {}
static uint64_t lpLitTexMatFor(uint64_t, float, float, float, float) { return 0; }
static bool lpLitOn() { return false; }
static void lpLitForgetSent() {}
static void lpSetEye(float, float, float) {}
static void lpLitFillChanged(float, float, float, float) {}
#endif

void PApplet::lights() {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        // as in Java: reset falloff and specular, gray ambient, gray light into the screen
        g_lpFalloff[0] = 1; g_lpFalloff[1] = 0; g_lpFalloff[2] = 0;
        g_lpLightSpec[0] = g_lpLightSpec[1] = g_lpLightSpec[2] = 0;
        ambientLight(colorMaxH * 0.5f, colorMaxS * 0.5f, colorMaxB * 0.5f);
        directionalLight(colorMaxH * 0.5f, colorMaxS * 0.5f, colorMaxB * 0.5f, 0, 0, -1); return;
    }
#endif
    // Matches Processing Java lights() exactly:
    //   ambientLight(128, 128, 128)
    //   directionalLight(128, 128, 128,  0, 0, -1)
    // Result: shadow faces = 50% brightness, front face = 100%, sides interpolated.
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    // GL_COLOR_MATERIAL drives AMBIENT_AND_DIFFUSE from fill color
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    lightsEnabled = true;
    lightIndex    = 0;

    // Zero out global ambient -- we set it explicitly via LIGHT0 ambient below
    GLfloat noGlobalAmb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noGlobalAmb);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

    GLfloat zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    // LIGHT0: ambient component = 128/255 (matches Java ambientLight(128,128,128))
    // This contributes equally to all faces regardless of normal.
    GLfloat amb[]  = { 0.502f, 0.502f, 0.502f, 1.0f }; // 128/255
    // LIGHT0: diffuse component = 128/255 (matches Java directionalLight(128,128,128,...))
    GLfloat dif[]  = { 0.502f, 0.502f, 0.502f, 1.0f }; // 128/255
    // Direction (0,0,-1) in Java = light travels toward -Z = toward-light vector (0,0,1)
    // In eye space with default camera looking down -Z, (0,0,1) points at camera = front-lit
    GLfloat pos[]  = { 0.0f, 0.0f, 1.0f, 0.0f }; // w=0 = directional

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, zero);

    // Set in eye space so light direction is fixed relative to camera
    glPushMatrix();
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glPopMatrix();

    lightIndex = 1;
    {
        GLint curFbo = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &curFbo);
        double mv[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, mv);
        GLboolean depthTestOn = glIsEnabled(GL_DEPTH_TEST);
        GLboolean lightingOn = glIsEnabled(GL_LIGHTING);
        GLboolean colorMatOn = glIsEnabled(GL_COLOR_MATERIAL);
        GLfloat curFillColor[4];
        glGetFloatv(GL_CURRENT_COLOR, curFillColor);
        PDEBUG("PApplet::lights() TRACE: curFbo=%d depthTest=%d lighting=%d colorMaterial=%d\n",
               curFbo, depthTestOn, lightingOn, colorMatOn);
        PDEBUG("  mv[0]=%.3f mv[5]=%.3f mv[10]=%.3f mv[12]=%.3f mv[13]=%.3f mv[14]=%.3f\n",
               mv[0], mv[5], mv[10], mv[12], mv[13], mv[14]);
        double proj[16];
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        PDEBUG("  proj[0]=%.3f proj[5]=%.3f proj[10]=%.3f proj[11]=%.3f proj[14]=%.3f\n",
               proj[0], proj[5], proj[10], proj[11], proj[14]);
        PDEBUG("  currentColor (glColor state at this moment): r=%.3f g=%.3f b=%.3f a=%.3f\n",
               curFillColor[0], curFillColor[1], curFillColor[2], curFillColor[3]);
    }
}

void PApplet::noLights() {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        lpLightsReset(_backend->graphics_id()); return;  // also restores unlit fill
    }
#endif
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    lightsEnabled = false;
    lightIndex    = 0;
}

// Ambient light: emits equally in all directions, no diffuse.
void PApplet::ambientLight(float r, float g, float b) { ambientLight(r,g,b,0,0,0); }
void PApplet::ambientLight(float r, float g, float b, float x, float y, float z) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        lpEnsureLit(_backend->graphics_id()); lpAddLight(0, r/colorMaxH, g/colorMaxS, b/colorMaxB, x, y, z, 0, 0, 0, 0, 0); return;
    }
#endif
    if (lightIndex >= 8) return;
    GLenum lt = GL_LIGHT0 + lightIndex++;

    GLfloat col[] = { lc(r), lc(g), lc(b), 1.0f };
    GLfloat pos[] = { x, y, z, 1.0f };
    GLfloat zero[]= { 0.0f, 0.0f, 0.0f, 1.0f };

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(lt);
    glLightfv(lt, GL_AMBIENT,  col);
    glLightfv(lt, GL_DIFFUSE,  zero);
    glLightfv(lt, GL_SPECULAR, zero);
    // Position is transformed by the current modelview (world space)
    glLightfv(lt, GL_POSITION, pos);
}

// Directional light: parallel rays from an infinite distance (w=0).
// 'nx,ny,nz' is the direction the light TRAVELS (toward the scene).
// OpenGL position with w=0 points TOWARD the light source, so we negate.
void PApplet::directionalLight(float r, float g, float b, float nx, float ny, float nz) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        lpEnsureLit(_backend->graphics_id()); lpAddLight(1, r/colorMaxH, g/colorMaxS, b/colorMaxB, 0, 0, 0, nx, ny, nz, 0, 0); return;
    }
#endif
    if (lightIndex >= 8) return;
    GLenum lt = GL_LIGHT0 + lightIndex++;

    GLfloat col[]  = { lc(r), lc(g), lc(b), 1.0f };
    // Negate direction and flip Y to compensate for glScalef(1,-1,1) in modelview
    GLfloat pos[]  = { -nx, ny, -nz, 0.0f };
    GLfloat zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    // No global ambient for directionalLight alone (matches Java Processing)
    GLfloat gAmb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gAmb);
    lightsEnabled = true;
    GLfloat spec[] = { pendingSpecR, pendingSpecG, pendingSpecB, 1.0f };
    glEnable(lt);
    glLightfv(lt, GL_AMBIENT,  zero);
    glLightfv(lt, GL_DIFFUSE,  col);
    glLightfv(lt, GL_SPECULAR, spec);
    // Set in pure eye space (no world transforms) like Java Processing.
    // toward-light in eye space: negate direction, flip Y for our Y-down convention.
    glPushMatrix();
    glLoadIdentity();
    // toward-light = negate direction. Y negated because Processing Y-down maps to eye Y-up
    // (glScalef(1,-1,1) in projection flips screen Y, so world +Y = eye -Y)
    GLfloat posE[] = { -nx, -ny, -nz, 0.0f };
    glLightfv(lt, GL_POSITION, posE);
    glPopMatrix();
}

// Point light: emits in all directions from a world-space position.
void PApplet::pointLight(float r, float g, float b, float x, float y, float z) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        lpEnsureLit(_backend->graphics_id()); lpAddLight(2, r/colorMaxH, g/colorMaxS, b/colorMaxB, x, y, z, 0, 0, 0, 0, 0); return;
    }
#endif
    if (lightIndex >= 8) return;
    GLenum lt = GL_LIGHT0 + lightIndex++;

    GLfloat col[]  = { lc(r), lc(g), lc(b), 1.0f };
    GLfloat pos[]  = { x, y, z, 1.0f };            // w=1 = positional
    GLfloat zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(lt);
    glLightfv(lt, GL_AMBIENT,  zero);
    glLightfv(lt, GL_DIFFUSE,  col);
    glLightfv(lt, GL_SPECULAR, zero);
    glLightf(lt, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(lt, GL_LINEAR_ATTENUATION,    0.0f);
    glLightf(lt, GL_QUADRATIC_ATTENUATION, 0.0f);
    // Position transformed by current modelview (world space)
    glLightfv(lt, GL_POSITION, pos);
}

// Spot light: cone of light from a position toward a direction.
// angle  = half-angle of the cone in radians (OpenGL takes degrees)
// conc   = concentration exponent (higher = tighter beam)
void PApplet::spotLight(float r, float g, float b,
               float x,  float y,  float z,
               float nx, float ny, float nz,
               float angle, float conc) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        lpEnsureLit(_backend->graphics_id()); lpAddLight(3, r/colorMaxH, g/colorMaxS, b/colorMaxB, x, y, z, nx, ny, nz, ::std::cos(angle), conc); return;
    }
#endif
    // Java Processing sets spotlight pos/dir through the CURRENT modelview
    // (which includes LookAt * Scale(1,-1,1)) -- same as world space draw.
    // We flip Y to compensate for our glScalef(1,-1,1) baked into modelview.
    if (lightIndex >= 8) return;
    GLenum lt = GL_LIGHT0 + lightIndex++;

    GLfloat col[]  = { lc(r), lc(g), lc(b), 1.0f };
    GLfloat zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    lightsEnabled = true;

    glEnable(lt);
    glLightfv(lt, GL_AMBIENT,  zero);
    glLightfv(lt, GL_DIFFUSE,  col);
    glLightfv(lt, GL_SPECULAR, zero);

    // Store concentration and cutoff cosine for the shader
    // (GL_SPOT_EXPONENT caps at 128, but Java uses values like 600)
    int li = lightIndex - 1;
    lightConcentration[li] = conc;
    lightCutoffCos[li]     = ::std::cos(angle); // precompute cos(cutoff)

    // Still set GL_SPOT_CUTOFF so fixed-function fallback works
    float cutDeg = ::std::min(angle * 180.0f / PI, 90.0f);
    glLightf(lt, GL_SPOT_CUTOFF,   cutDeg);
    glLightf(lt, GL_SPOT_EXPONENT, ::std::min(conc, 128.0f)); // capped for GL
    glLightf(lt, GL_CONSTANT_ATTENUATION,  1.0f);
    glLightf(lt, GL_LINEAR_ATTENUATION,    0.0f);
    glLightf(lt, GL_QUADRATIC_ATTENUATION, 0.0f);

    // Position: NO Y-flip (modelview Scale already handles it correctly)
    // Direction: Y-flip ny to compensate for Scale(1,-1,1) in modelview
    GLfloat posW[] = { x,   y,  z,  1.0f };
    GLfloat dirW[] = { nx, -ny, nz };
    glLightfv(lt, GL_POSITION,       posW);
    glLightfv(lt, GL_SPOT_DIRECTION, dirW);
}

// Override attenuation on all active lights (call after the light functions)
void PApplet::lightFalloff(float c, float l, float q) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        g_lpFalloff[0] = c; g_lpFalloff[1] = l; g_lpFalloff[2] = q; return;
    }
#endif
    for (int i = 0; i < lightIndex; i++) {
        GLenum lt = GL_LIGHT0 + i;
        glLightf(lt, GL_CONSTANT_ATTENUATION,  c);
        glLightf(lt, GL_LINEAR_ATTENUATION,    l);
        glLightf(lt, GL_QUADRATIC_ATTENUATION, q);
    }
}

// Set specular colour on all active lights
void PApplet::lightSpecular(float r, float g, float b) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        g_lpLightSpec[0] = r/colorMaxH; g_lpLightSpec[1] = g/colorMaxS; g_lpLightSpec[2] = b/colorMaxB; return;
    }
#endif
    // Store as pending so subsequent light calls pick it up
    pendingSpecR = lc(r); pendingSpecG = lc(g); pendingSpecB = lc(b);
    // Also apply to any already-created lights
    for (int i = 0; i < lightIndex; i++) {
        GLenum  lt  = GL_LIGHT0 + i;
        GLfloat col[] = { pendingSpecR, pendingSpecG, pendingSpecB, 1.0f };
        glLightfv(lt, GL_SPECULAR, col);
    }
}

void PApplet::normal(float nx, float ny, float nz) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { g_lpNormal[0] = nx; g_lpNormal[1] = ny; g_lpNormal[2] = nz; g_lpHasNormal = true; return; }  // cppmode-api3
#endif
    glNormal3f(nx, ny, nz); }

// =============================================================================
// MATERIAL
// =============================================================================

void PApplet::ambient(float r, float g, float b) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { g_lpMatAmb[0] = lc(r); g_lpMatAmb[1] = lc(g); g_lpMatAmb[2] = lc(b); g_lpMatAmbSet = true; lpLitRefresh(); return; }
#endif
    GLfloat col[] = { lc(r), lc(g), lc(b), 1.0f };
#ifndef __EMSCRIPTEN__
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, col);
#endif
}
void PApplet::ambient(color c) {
    unsigned int v = c.value;
    // the float version divides by the color range, so scale into it (not 0..1)
    ambient((v >> 16 & 0xFF) / 255.f * colorMaxH,
            (v >>  8 & 0xFF) / 255.f * colorMaxS,
            (v       & 0xFF) / 255.f * colorMaxB);
}

void PApplet::emissive(float r, float g, float b) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { g_lpMatEmis[0] = lc(r); g_lpMatEmis[1] = lc(g); g_lpMatEmis[2] = lc(b); lpLitRefresh(); return; }
#endif
    GLfloat col[] = { lc(r), lc(g), lc(b), 1.0f };
#ifndef __EMSCRIPTEN__
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, col);
#endif
}
void PApplet::emissive(color c) {
    unsigned int v = c.value;
    // the float version divides by the color range, so scale into it (not 0..1)
    emissive((v >> 16 & 0xFF) / 255.f * colorMaxH,
             (v >>  8 & 0xFF) / 255.f * colorMaxS,
             (v       & 0xFF) / 255.f * colorMaxB);
}

void PApplet::specular(float r, float g, float b) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { g_lpMatSpec[0] = lc(r); g_lpMatSpec[1] = lc(g); g_lpMatSpec[2] = lc(b); lpLitRefresh(); return; }
#endif
    GLfloat col[] = { lc(r), lc(g), lc(b), 1.0f };
#ifndef __EMSCRIPTEN__
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, col);
#endif
}
void PApplet::specular(color c) {
    unsigned int v = c.value;
    // the float version divides by the color range, so scale into it (not 0..1)
    specular((v >> 16 & 0xFF) / 255.f * colorMaxH,
             (v >>  8 & 0xFF) / 255.f * colorMaxS,
             (v       & 0xFF) / 255.f * colorMaxB);
}

void PApplet::shininess(float s) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { g_lpMatShin = s; lpLitRefresh(); return; }
#endif
#ifndef __EMSCRIPTEN__
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, s);
#endif
}

// =============================================================================
// TEXT
// =============================================================================

// -- Embedded 6x8 bitmap font fallback (ASCII 32-126) -------------------------
static const unsigned char g_font6x8[][6] = {
    {0x00,0x00,0x00,0x00,0x00,0x00},{0x04,0x04,0x04,0x04,0x00,0x04},{0x0A,0x0A,0x00,0x00,0x00,0x00},{0x0A,0x1F,0x0A,0x1F,0x0A,0x00},{0x0E,0x15,0x1C,0x07,0x15,0x0E},{0x19,0x1A,0x02,0x04,0x0B,0x13},{0x08,0x14,0x08,0x15,0x12,0x0D},{0x04,0x04,0x00,0x00,0x00,0x00},
    {0x02,0x04,0x04,0x04,0x04,0x02},{0x08,0x04,0x04,0x04,0x04,0x08},{0x00,0x0A,0x04,0x1F,0x04,0x0A},{0x00,0x04,0x04,0x1F,0x04,0x04},{0x00,0x00,0x00,0x00,0x04,0x08},{0x00,0x00,0x00,0x1F,0x00,0x00},{0x00,0x00,0x00,0x00,0x00,0x04},{0x01,0x02,0x02,0x04,0x08,0x10},
    {0x0E,0x11,0x13,0x15,0x19,0x0E},{0x04,0x0C,0x04,0x04,0x04,0x0E},{0x0E,0x11,0x02,0x04,0x08,0x1F},{0x1F,0x02,0x06,0x01,0x11,0x0E},{0x02,0x06,0x0A,0x1F,0x02,0x02},{0x1F,0x10,0x1E,0x01,0x11,0x0E},{0x06,0x08,0x1E,0x11,0x11,0x0E},{0x1F,0x01,0x02,0x04,0x04,0x04},
    {0x0E,0x11,0x0E,0x11,0x11,0x0E},{0x0E,0x11,0x0F,0x01,0x02,0x0C},{0x00,0x04,0x00,0x00,0x04,0x00},{0x00,0x04,0x00,0x00,0x04,0x08},{0x02,0x04,0x08,0x08,0x04,0x02},{0x00,0x1F,0x00,0x1F,0x00,0x00},{0x08,0x04,0x02,0x02,0x04,0x08},{0x0E,0x11,0x02,0x04,0x00,0x04},
    {0x0E,0x11,0x17,0x15,0x17,0x0E},{0x04,0x0A,0x11,0x1F,0x11,0x11},{0x1E,0x11,0x1E,0x11,0x11,0x1E},{0x0E,0x11,0x10,0x10,0x11,0x0E},{0x1C,0x12,0x11,0x11,0x12,0x1C},{0x1F,0x10,0x1E,0x10,0x10,0x1F},{0x1F,0x10,0x1E,0x10,0x10,0x10},{0x0E,0x11,0x10,0x17,0x11,0x0F},
    {0x11,0x11,0x1F,0x11,0x11,0x11},{0x0E,0x04,0x04,0x04,0x04,0x0E},{0x07,0x02,0x02,0x02,0x12,0x0C},{0x11,0x12,0x1C,0x12,0x11,0x11},{0x10,0x10,0x10,0x10,0x10,0x1F},{0x11,0x1B,0x15,0x11,0x11,0x11},{0x11,0x19,0x15,0x13,0x11,0x11},{0x0E,0x11,0x11,0x11,0x11,0x0E},
    {0x1E,0x11,0x11,0x1E,0x10,0x10},{0x0E,0x11,0x11,0x15,0x12,0x0D},{0x1E,0x11,0x11,0x1E,0x11,0x11},{0x0F,0x10,0x0E,0x01,0x11,0x0E},{0x1F,0x04,0x04,0x04,0x04,0x04},{0x11,0x11,0x11,0x11,0x11,0x0E},{0x11,0x11,0x11,0x0A,0x0A,0x04},{0x11,0x11,0x15,0x15,0x1B,0x11},
    {0x11,0x0A,0x04,0x04,0x0A,0x11},{0x11,0x0A,0x04,0x04,0x04,0x04},{0x1F,0x02,0x04,0x08,0x10,0x1F},{0x0E,0x08,0x08,0x08,0x08,0x0E},{0x10,0x08,0x08,0x04,0x02,0x01},{0x0E,0x02,0x02,0x02,0x02,0x0E},{0x04,0x0A,0x11,0x00,0x00,0x00},{0x00,0x00,0x00,0x00,0x00,0x1F},
    {0x08,0x04,0x00,0x00,0x00,0x00},{0x00,0x0E,0x01,0x0F,0x11,0x0F},{0x10,0x10,0x1E,0x11,0x11,0x1E},{0x00,0x0E,0x10,0x10,0x10,0x0E},{0x01,0x01,0x0F,0x11,0x11,0x0F},{0x00,0x0E,0x11,0x1F,0x10,0x0E},{0x06,0x08,0x1E,0x08,0x08,0x08},{0x00,0x0F,0x11,0x0F,0x01,0x0E},
    {0x10,0x10,0x1E,0x11,0x11,0x11},{0x04,0x00,0x04,0x04,0x04,0x0E},{0x02,0x00,0x02,0x02,0x12,0x0C},{0x10,0x12,0x14,0x1C,0x12,0x11},{0x0C,0x04,0x04,0x04,0x04,0x0E},{0x00,0x1B,0x15,0x15,0x11,0x11},{0x00,0x1E,0x11,0x11,0x11,0x11},{0x00,0x0E,0x11,0x11,0x11,0x0E},
    {0x00,0x1E,0x11,0x1E,0x10,0x10},{0x00,0x0F,0x11,0x0F,0x01,0x01},{0x00,0x16,0x19,0x10,0x10,0x10},{0x00,0x0E,0x10,0x0E,0x01,0x1E},{0x08,0x1F,0x08,0x08,0x08,0x07},{0x00,0x11,0x11,0x11,0x13,0x0D},{0x00,0x11,0x11,0x0A,0x0A,0x04},{0x00,0x11,0x15,0x15,0x1B,0x11},
    {0x00,0x11,0x0A,0x04,0x0A,0x11},{0x00,0x11,0x0A,0x04,0x08,0x10},{0x00,0x1F,0x02,0x04,0x08,0x1F},{0x06,0x04,0x0C,0x04,0x04,0x06},{0x04,0x04,0x04,0x04,0x04,0x04},{0x0C,0x04,0x06,0x04,0x04,0x0C},{0x08,0x15,0x02,0x00,0x00,0x00},
};

// -- stb_truetype font state ---------------------------------------------------
#if PROCESSING_HAS_STB_TRUETYPE
bool PApplet::loadTTFFile(const ::std::string& path) {
    ::std::ifstream f(path, ::std::ios::binary);
    if (!f) return false;
    // Load into local vector first, then move — ensures data pointer is stable
    // before stbtt_InitFont stores a reference into it.
    ::std::vector<unsigned char> tmp(
        (::std::istreambuf_iterator<char>(f)),
         ::std::istreambuf_iterator<char>());
    if (tmp.empty()) return false;
    g_ttf.data = ::std::move(tmp);
    g_ttf.data.shrink_to_fit(); // pin allocation before passing pointer to stbtt
    return stbtt_InitFont(&g_ttf.info, g_ttf.data.data(), 0) != 0;
}

#ifdef PROCESSING_USE_FREETYPE
// Bake ASCII 32..127 with FreeType (hinted) into the same atlas layout the
// stb path uses, so drawing and caching stay unchanged. Returns 0 on failure.
static int cppmodeBakeFreeType(const ::std::vector<unsigned char>& data, float px,
                               unsigned char* bitmap, int W, int H, stbtt_bakedchar* chars) {
    static FT_Library lib = nullptr;
    if (!lib && FT_Init_FreeType(&lib)) { lib = nullptr; return 0; }
    FT_Face face = nullptr;
    if (FT_New_Memory_Face(lib, data.data(), (FT_Long)data.size(), 0, &face)) return 0;
    // textSize is the em size in pixels, as in Java
    if (FT_Set_Char_Size(face, 0, (FT_F26Dot6)(px * 64.0f), 72, 72)) { FT_Done_Face(face); return 0; }
    ::std::memset(bitmap, 0, (size_t)W * (size_t)H);
    int x = 1, y = 1, rowH = 0;
    for (int c = 32; c < 128; c++) {
        stbtt_bakedchar& bc = chars[c - 32];
        bc = stbtt_bakedchar{};
        if (FT_Load_Char(face, (FT_ULong)c, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT)) continue;
        FT_GlyphSlot g = face->glyph;
        int gw = (int)g->bitmap.width, gh = (int)g->bitmap.rows;
        if (x + gw + 1 >= W) { x = 1; y += rowH + 1; rowH = 0; }
        if (y + gh + 1 >= H) { FT_Done_Face(face); return 0; }   // doesn't fit: fall back
        for (int r = 0; r < gh; r++)
            ::std::memcpy(bitmap + (size_t)(y + r) * W + x,
                          g->bitmap.buffer + (ptrdiff_t)r * g->bitmap.pitch, (size_t)gw);
        bc.x0 = (unsigned short)x;        bc.y0 = (unsigned short)y;
        bc.x1 = (unsigned short)(x + gw); bc.y1 = (unsigned short)(y + gh);
        bc.xoff = (float)g->bitmap_left;
        bc.yoff = -(float)g->bitmap_top;
        // unhinted advance, so spacing agrees with textWidth()
        bc.xadvance = (float)g->linearHoriAdvance / 65536.0f;
        x += gw + 1;
        if (gh > rowH) rowH = gh;
    }
    FT_Done_Face(face);
    { static bool said = false; if (!said) { said = true; fprintf(stderr, "[CppMode] text: FreeType glyphs (auto-hinted)\n"); } }
    return 1;
}
#endif

void PApplet::bakeAtlas(float pixelSize) {
    if (!g_ttf.loaded) return;
    // Cache key: rounded to nearest 0.5px to avoid float noise
    int cacheKey = (int)::std::round(pixelSize * 2.0f);
    // Check cache — if already baked this size, just activate it
    auto it = g_ttf.atlasCache.find(cacheKey);
    if (it != g_ttf.atlasCache.end()) {
        g_ttf.current = &it->second;
        g_ttf.texID   = g_ttf.current->texID;
        g_ttf.chars   = g_ttf.current->chars;
        g_ttf.bakeSize = pixelSize;
        return;
    }
    // Not cached — bake a new atlas for this size
    TTFAtlas atlas;
    g_ttf.atlasW = 1024; g_ttf.atlasH = 1024;
    ::std::vector<unsigned char> bitmap(g_ttf.atlasW * g_ttf.atlasH);
    int ret = 0;
#ifdef PROCESSING_USE_FREETYPE
    ret = cppmodeBakeFreeType(g_ttf.data, pixelSize, bitmap.data(), g_ttf.atlasW, g_ttf.atlasH, atlas.chars);
#endif
    if (ret == 0) ret = stbtt_BakeFontBitmap(
        g_ttf.data.data(), 0,
        // textSize is the font's em size (as in Java), but this call wants the
        // full ascent-to-descent height, so convert.
        stbtt_ScaleForMappingEmToPixels(&g_ttf.info, pixelSize) / stbtt_ScaleForPixelHeight(&g_ttf.info, 1.0f),
        bitmap.data(), g_ttf.atlasW, g_ttf.atlasH,
        32, 96, atlas.chars);
    if (ret == 0) { g_ttf.loaded = false; return; }
    glGenTextures(1, &atlas.texID);
    glBindTexture(GL_TEXTURE_2D, atlas.texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ::std::vector<unsigned char> rgba(g_ttf.atlasW * g_ttf.atlasH * 4);
    for (int i = 0; i < g_ttf.atlasW * g_ttf.atlasH; i++) {
        rgba[i*4+0] = rgba[i*4+1] = rgba[i*4+2] = 255;
        rgba[i*4+3] = bitmap[i];
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_ttf.atlasW, g_ttf.atlasH,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    // Store in cache
    g_ttf.atlasCache[cacheKey] = ::std::move(atlas);
    g_ttf.current = &g_ttf.atlasCache[cacheKey];
    g_ttf.texID   = g_ttf.current->texID;
    g_ttf.chars   = g_ttf.current->chars;
    g_ttf.bakeSize = pixelSize;
}
#endif // PROCESSING_HAS_STB_TRUETYPE

// -- Shared text state ---------------------------------------------------------

// -- Bitmap font rendering (fallback) -----------------------------------------
static const int BF_GW = 6;
static const int BF_GH = 8;

void PApplet::drawBitmapStr(float x, float y, const ::std::string& s, int scale) {
    glColor4f(fillR, fillG, fillB, fillA);
    glDisable(GL_TEXTURE_2D);
    float cx = x;
    for (char ch : s) {
        if (ch < 32 || ch > 126) { cx += (BF_GW+1)*scale; continue; }
        const unsigned char* bm = g_font6x8[(unsigned char)ch - 32];
        for (int row = 0; row < BF_GH; row++) {
            unsigned char bits = bm[row];
            for (int col = 0; col < BF_GW; col++) {
                if (bits & (1 << (BF_GW - 1 - col))) {
#ifdef __EMSCRIPTEN__
                    { float px = cx + col*scale, py = y + row*scale;
                    glBegin(GL_TRIANGLES);
                    glVertex2f(px,py); glVertex2f(px+scale,py); glVertex2f(px+scale,py+scale);
                    glVertex2f(px,py); glVertex2f(px+scale,py+scale); glVertex2f(px,py+scale);
                    glEnd(); }
#else
                    glBegin(GL_QUADS);
                    float px = cx + col*scale, py = y + row*scale;
                    glVertex2f(px,        py);
                    glVertex2f(px+scale,  py);
                    glVertex2f(px+scale,  py+scale);
                    glVertex2f(px,        py+scale);
                    glEnd();
#endif
                }
            }
        }
        cx += (BF_GW+1)*scale;
    }
}

float PApplet::bitmapStrWidth(const ::std::string& s, int scale) {
    return static_cast<float>(s.size()) * static_cast<float>(BF_GW+1) * static_cast<float>(scale);
}

// -- TTF rendering -------------------------------------------------------------
#if PROCESSING_HAS_STB_TRUETYPE
float PApplet::ttfStrWidth(const ::std::string& s) {
    float x = 0;
    for (char ch : s) {
        if (ch < 32 || ch > 127) continue;
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&g_ttf.info, ch, &advance, &lsb);
        float sc = stbtt_ScaleForMappingEmToPixels(&g_ttf.info, g_textSize);
        x += advance * sc;
    }
    return x;
}

#ifdef PROCESSING_USE_FREETYPE
static int cppmodeBakeFreeType(const ::std::vector<unsigned char>& data, float px,
                               unsigned char* bitmap, int W, int H, stbtt_bakedchar* chars);
#endif
void PApplet::drawTTFStr(float x, float y, const ::std::string& s) {
    if (!g_ttf.loaded) return;
    if (_backend && _backend->glyph_images()) {
        // libprocessing: draw our own letter images (the same FreeType/stb glyphs
        // the OpenGL path uses) instead of its outline-based text, which comes
        // out soft and dim at small sizes. One white atlas image per font and
        // size, tinted with the fill colour.
        struct LpAtlas { uint64_t img = 0; int w = 0, h = 0; stbtt_bakedchar chars[96]; };
        static ::std::unordered_map<uint64_t, LpAtlas> lpAtlases;
        uint64_t fh = 1469598103934665603ULL;   // FNV-1a over a sample of the font file
        {
            const auto& d = g_ttf.data;
            size_t step = d.size() / 256 + 1;
            for (size_t i = 0; i < d.size(); i += step) { fh ^= d[i]; fh *= 1099511628211ULL; }
            fh ^= (uint64_t)d.size(); fh *= 1099511628211ULL;
        }
        uint64_t key = fh ^ ((uint64_t)::std::llround(g_textSize * 2.0f) * 0x9E3779B97F4A7C15ULL);
        auto it = lpAtlases.find(key);
        if (it == lpAtlases.end()) {
            LpAtlas a;
            const int W = 1024, H = 1024;
            ::std::vector<unsigned char> bm((size_t)W * H);
            int ok = 0;
#ifdef PROCESSING_USE_FREETYPE
            ok = cppmodeBakeFreeType(g_ttf.data, g_textSize, bm.data(), W, H, a.chars);
#endif
            if (ok == 0) {
                ::std::fill(bm.begin(), bm.end(), (unsigned char)0);
                ok = stbtt_BakeFontBitmap(g_ttf.data.data(), 0,
                        stbtt_ScaleForMappingEmToPixels(&g_ttf.info, g_textSize) / stbtt_ScaleForPixelHeight(&g_ttf.info, 1.0f),
                        bm.data(), W, H, 32, 96, a.chars) != 0 ? 1 : 0;
            }
            if (ok) {
                int usedH = 1;
                for (const auto& b : a.chars) if ((int)b.y1 + 1 > usedH) usedH = (int)b.y1 + 1;
                if (usedH > H) usedH = H;
                ::std::vector<unsigned char> rgba((size_t)W * usedH * 4);
                for (size_t i = 0; i < (size_t)W * usedH; i++) {
                    rgba[i*4+0] = rgba[i*4+1] = rgba[i*4+2] = 255;
                    rgba[i*4+3] = bm[i];
                }
                a.w = W; a.h = usedH;
                a.img = _backend->image_create_rgba((uint32_t)W, (uint32_t)usedH, rgba.data());
            }
            it = lpAtlases.emplace(key, a).first;
        }
        const LpAtlas& a = it->second;
        if (!a.img) return;
        // just under full alpha so letters go through the blend pass (smooth edges)
        _backend->image_tint(fillR, fillG, fillB, fillA < 0.996f ? fillA : 0.996f);
        float cx = x;
        for (char ch : s) {
            if ((unsigned char)ch < 32 || (unsigned char)ch >= 128) continue;
            const stbtt_bakedchar& b = a.chars[ch - 32];
            float gw = (float)(b.x1 - b.x0), gh = (float)(b.y1 - b.y0);
            if (gw > 0 && gh > 0)
                _backend->image_region(a.img,
                    ::std::floor(cx + b.xoff + 0.5f), ::std::floor(y + b.yoff + 0.5f), gw, gh,
                    (float)b.x0, (float)b.y0, gw, gh);
            cx += b.xadvance;
        }
        // Always restore tint state after glyph rendering
        if (doTint) _backend->image_tint(tintR, tintG, tintB, tintA);
        else        _backend->image_no_tint();
        return;
    }
    bakeAtlas(g_textSize);
    // BUG FIX: same class of issue found in drawPGraphicsRect() -- if
    // lights() was called earlier this frame, GL_LIGHTING stays globally
    // enabled, and text glyph quads (no normal ever set) get lit/
    // darkened unexpectedly. Text should never be affected by whatever
    // 3D lighting state happens to be active.
    GLboolean wasLighting = glIsEnabled(GL_LIGHTING);
    GLboolean wasDepthTest = glIsEnabled(GL_DEPTH_TEST);
    if (wasLighting) glDisable(GL_LIGHTING);
    if (wasDepthTest) glDisable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_ttf.texID);
    glEnable(GL_BLEND);
    glApplyBlend(g_glBlendMode);
    // Use fill colour modulated by font alpha
    glColor4f(fillR, fillG, fillB, fillA);
    float cx = x;
    for (char ch : s) {
        if ((unsigned char)ch < 32 || (unsigned char)ch >= 128) continue;
        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(g_ttf.chars, g_ttf.atlasW, g_ttf.atlasH,
                           ch - 32, &cx, &y, &q, 1);
#ifdef __EMSCRIPTEN__
        glBegin(GL_TRIANGLES);
        glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
        glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
        glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
        glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
        glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
        glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
        glEnd();
#else
        glBegin(GL_QUADS);
        glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y0);
        glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y0);
        glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y1);
        glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y1);
        glEnd();
#endif
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    if (wasLighting) glEnable(GL_LIGHTING);
    if (wasDepthTest) glEnable(GL_DEPTH_TEST);
}
#endif

// -- Main renderText entry point -----------------------------------------------
float PApplet::getLineWidth(const ::std::string& line) {
#if PROCESSING_HAS_STB_TRUETYPE
    if (g_ttf.loaded) return ttfStrWidth(line);
#endif
    int sc = ::std::max(1,(int)(g_textSize/8.0f));
    return bitmapStrWidth(line, sc);
}

void PApplet::renderText(const ::std::string& msg, float x, float y) {
    if (msg.empty()) return;

    float leading = (g_textLeading > 0) ? g_textLeading : (textAscent() + textDescent()) * 1.275f;

    // Split on \n
    ::std::vector<::std::string> ls;
    ::std::string cur;
    for (char c : msg) { if(c=='\n'){ls.push_back(cur);cur.clear();}else cur+=c; }
    ls.push_back(cur);

    float totalH = (float)ls.size() * leading;
    float yOff = 0;
    // Same vertical rules as Processing's Java PGraphics.text()
    (void)totalH;
    float high = (float)((int)ls.size() - 1) * leading;
    if      (g_textAlignY == TOP_ALIGN)    yOff = textAscent();
    else if (g_textAlignY == CENTER_ALIGN) yOff = (textAscent() - high) * 0.5f;
    else if (g_textAlignY == BOTTOM_ALIGN) yOff = -(textDescent() + high);
    for (int li = 0; li < (int)ls.size(); li++) {
        float lw  = getLineWidth(ls[li]);
        float dx  = x;
        if      (g_textAlignX == RIGHT_ALIGN)  dx = x - lw;
        else if (g_textAlignX == CENTER_ALIGN) dx = x - lw * 0.5f;
        float dy = y + yOff + li * leading;

#if PROCESSING_HAS_STB_TRUETYPE
        if (g_ttf.loaded) { drawTTFStr(dx, dy, ls[li]); continue; }
#endif
        // Bitmap fallback
        int sc = ::std::max(1,(int)(g_textSize/8.0f));
        // Shift so baseline sits at y (bitmap font: ascent = BF_GH-2 rows)
        float ascent = static_cast<float>(BF_GH - 2) * static_cast<float>(sc);
        drawBitmapStr(dx, dy - ascent, ls[li], sc);
    }
}

void PApplet::text(const ::std::string& msg, float x, float y) {
    if (_backend && !(_backend->glyph_images() && g_ttf.loaded)) { _backend->text(msg, x, y); return; }
    renderText(msg, x, y);
}
void PApplet::text(int   v, float x, float y) { text(::std::to_string(v), x, y); }
void PApplet::text(float v, float x, float y) {
    ::std::ostringstream ss; ss << v; text(ss.str(), x, y);
}

// text(str, x, y, w, h) -- word-wrap into a bounding box, matching Processing Java.
// Words are wrapped when they exceed width w. Lines are clipped at height h.
// x,y is top-left corner of the box. Text starts at the baseline of the first line.
void PApplet::text(const ::std::string& msg, float x, float y, float w, float h) {
    if (_backend && !(_backend->glyph_images() && g_ttf.loaded)) {
        // Java boxed text: first baseline one ascent below the box top (TOP/BASELINE)
        bool placed = (g_textAlignY == CENTER_ALIGN || g_textAlignY == BOTTOM_ALIGN);
        _backend->text_box(msg, x, placed ? y : y + textAscent(), w, h);
        return;
    }
    if (msg.empty() || w <= 0) return;
    float leading = (g_textLeading > 0) ? g_textLeading : (textAscent() + textDescent()) * 1.275f;
    float maxY = (h > 0) ? y + h : 1e9f;

    // Split message into tokens: words and explicit newlines
    ::std::vector<::std::string> tokens;
    {
        ::std::string cur;
        for (char c : msg) {
            if (c == '\n') {
                if (!cur.empty()) { tokens.push_back(cur); cur.clear(); }
                tokens.push_back("\n");
            } else if (c == ' ') {
                if (!cur.empty()) { tokens.push_back(cur); cur.clear(); }
            } else {
                cur += c;
            }
        }
        if (!cur.empty()) tokens.push_back(cur);
    }

    // Layout: accumulate words into lines, wrap when line width exceeds w
    // Java boxed text: the first baseline sits one ascent below the box top,
    // and lines are anchored at the box's left edge, middle or right edge.
    float cy = y + textAscent();
    if      (g_textAlignX == CENTER_ALIGN) x += w * 0.5f;
    else if (g_textAlignX == RIGHT_ALIGN)  x += w;
    auto _savedAlignY = g_textAlignY;
    g_textAlignY = BASELINE;   // vertical placement is handled here, not per line
    ::std::vector<::std::pair<::std::string, float>> _boxLines;   // laid out first, drawn at the end
    ::std::string line;

    auto flushLine = [&](){
        if (!line.empty() && cy + g_textSize <= maxY) {
            _boxLines.emplace_back(line, cy);
        }
        line.clear();
        cy += leading;
    };

    for (auto& tok : tokens) {
        if (tok == "\n") {
            // Explicit newline: flush current line and advance
            if (cy + g_textSize <= maxY) _boxLines.emplace_back(line, cy);
            line.clear();
            cy += leading;
            continue;
        }
        // Try adding this word to the current line
        ::std::string candidate = line.empty() ? tok : line + " " + tok;
        float cw = getLineWidth(candidate);

        if (!line.empty() && cw > w) {
            // Word doesn't fit: flush current line, start new line with this word
            if (cy + g_textSize <= maxY) _boxLines.emplace_back(line, cy);
            line.clear();
            cy += leading;
            // If the word itself is wider than w, wrap it character by character
            if (getLineWidth(tok) > w) {
                ::std::string charLine;
                for (char c : tok) {
                    ::std::string test2 = charLine + c;
                    if (!charLine.empty() && getLineWidth(test2) > w) {
                        if (cy + g_textSize <= maxY) _boxLines.emplace_back(charLine, cy);
                        charLine.clear();
                        cy += leading;
                    }
                    charLine += c;
                }
                line = charLine;
            } else {
                line = tok;
            }
        } else if (line.empty() && cw > w) {
            // First word on line is too wide: wrap char by char
            ::std::string charLine;
            for (char c : tok) {
                ::std::string test2 = charLine + c;
                if (!charLine.empty() && getLineWidth(test2) > w) {
                    if (cy + g_textSize <= maxY) _boxLines.emplace_back(charLine, cy);
                    charLine.clear();
                    cy += leading;
                }
                charLine += c;
            }
            line = charLine;
        } else {
            line = candidate;
        }
    }
    // Flush remaining text
    if (!line.empty() && cy + g_textSize <= maxY) {
        _boxLines.emplace_back(line, cy);
    }
    {   // Java: CENTER and BOTTOM place the whole block of lines inside the box height
        float shift = 0;
        float gaps = _boxLines.empty() ? 0.0f : (float)(_boxLines.size() - 1);
        if (h > 0 && _savedAlignY == CENTER_ALIGN)
            shift = (h - (textAscent() + leading * gaps)) * 0.5f;
        else if (h > 0 && _savedAlignY == BOTTOM_ALIGN)
            shift = (y + h - textDescent() - leading * gaps) - (y + textAscent());
        for (auto& bl : _boxLines) renderText(bl.first, x, bl.second + shift);
    }
    g_textAlignY = _savedAlignY;
}

void PApplet::textSize(float size) {
    g_textSize = size;
    if (_backend) _backend->text_size(size);
    g_textLeading = 0;  // Java: textSize() resets the leading
    if (_backend) _backend->text_leading((textAscent() + textDescent()) * 1.275f);
#if PROCESSING_HAS_STB_TRUETYPE
    if (g_ttf.loaded) bakeAtlas(size);
#endif
}

void PApplet::textAlign(int alignX, int alignY) {
    // Map standard constants (LEFT=37, RIGHT=39, CENTER=3) to align constants
    auto mapAlign=[](int a)->int{
        if(a==37||a==20) return LEFT_ALIGN;   // LEFT or LEFT_ALIGN
        if(a==39||a==21) return RIGHT_ALIGN;  // RIGHT or RIGHT_ALIGN
        if(a==3 ||a==25) return CENTER_ALIGN; // CENTER or CENTER_ALIGN
        if(a==101||a==22) return TOP_ALIGN;   // TOP or TOP_ALIGN
        if(a==102||a==23) return BOTTOM_ALIGN;// BOTTOM or BOTTOM_ALIGN
        if(a==0)          return BASELINE;    // BASELINE
        return a;
    };
    g_textAlignX = mapAlign(alignX);
    if(alignY >= 0) g_textAlignY = mapAlign(alignY);
    if (_backend) {
        int h = g_textAlignX == RIGHT_ALIGN ? 2 : g_textAlignX == CENTER_ALIGN ? 1 : 0;
        int v = g_textAlignY == TOP_ALIGN ? 1 : g_textAlignY == CENTER_ALIGN ? 2
              : g_textAlignY == BOTTOM_ALIGN ? 3 : 0;
        _backend->text_align(h, v);
    }
}
void PApplet::textLeading(float v) { g_textLeading = v; if (_backend) _backend->text_leading(v); }
void PApplet::textMode(int) {}

float PApplet::textWidth(const ::std::string& s) {
    if (_backend && !(_backend->glyph_images() && g_ttf.loaded)) { float w = _backend->text_width(s); if (w >= 0) return w; }
    return getLineWidth(s);
}

float PApplet::textAscent() {
#if PROCESSING_HAS_STB_TRUETYPE
    if (g_ttf.loaded) {
        float sc = stbtt_ScaleForMappingEmToPixels(&g_ttf.info, g_textSize);
        // Java Processing: ascent is the height of 'd'
        { int x0,y0,x1,y1; if (stbtt_GetCodepointBox(&g_ttf.info, 'd', &x0,&y0,&x1,&y1)) return y1 * sc; }
        int asc; stbtt_GetFontVMetrics(&g_ttf.info, &asc, nullptr, nullptr);
        return asc * sc;
    }
#endif
    int sc = ::std::max(1,(int)(g_textSize/8.0f));
    return static_cast<float>(BF_GH - 2) * static_cast<float>(sc);
}

float PApplet::textDescent() {
#if PROCESSING_HAS_STB_TRUETYPE
    if (g_ttf.loaded) {
        float sc = stbtt_ScaleForMappingEmToPixels(&g_ttf.info, g_textSize);
        // Java Processing: descent is how far 'p' drops below the baseline
        { int x0,y0,x1,y1; if (stbtt_GetCodepointBox(&g_ttf.info, 'p', &x0,&y0,&x1,&y1)) return ::std::fabs(y0 * sc); }
        int desc; stbtt_GetFontVMetrics(&g_ttf.info, nullptr, &desc, nullptr);
        return ::std::fabs(desc * sc);
    }
#endif
    int sc = ::std::max(1,(int)(g_textSize/8.0f));
    return 2.0f * sc;
}

// =============================================================================
// IMAGE
// =============================================================================

PImage* PApplet::createImage(int w,int h,int mode){
    PImage* img = new PImage(w,h);
    img->format = mode;
    if(mode==ARGB) {
        ::std::fill(img->pixels.begin(),img->pixels.end(),0x00000000);
    } else if(mode==ALPHA) {
        ::std::fill(img->pixels.begin(),img->pixels.end(),0x00000000);
    }
    img->dirty=true;
    return img;
}

// Upload a PImage's CPU pixels to libprocessing the first time it is drawn,
// and again after updatePixels(). Works for loadImage, createImage and URLs.
#ifdef CPPMODE_BACKEND_LIBPROCESSING
static bool lpEnsureImage(ProcessingBackend* be, PImage* img) {
    if (!img || img->width <= 0 || img->height <= 0) return false;
    if (img->lp_image_id && !img->dirty) return true;
    size_t n = (size_t)img->width * (size_t)img->height;
    ::std::vector<unsigned char> rgba(n * 4);
    for (size_t i = 0; i < n; i++) {
        uint32_t c = (uint32_t)img->pixels[i];   // ARGB
        rgba[i*4+0] = (c >> 16) & 0xff;
        rgba[i*4+1] = (c >> 8)  & 0xff;
        rgba[i*4+2] =  c        & 0xff;
        rgba[i*4+3] = (c >> 24) & 0xff;
    }
    img->lp_image_id = be->image_create_rgba((uint32_t)img->width, (uint32_t)img->height, rgba.data());
    img->dirty = false;
    return img->lp_image_id != 0;
}
#else
// OpenGL build: _backend is always null, so this is never reached
template <class B> static bool lpEnsureImage(B*, PImage*) { return false; }
#endif

PImage* PApplet::loadImage(const ::std::string& path){
    // libprocessing: decoded here like OpenGL, uploaded on first draw
    // Handle URLs: download with curl/wget and validate image magic bytes
    if (path.size()>7 && (path.substr(0,7)=="http://" || path.substr(0,8)=="https://")){
#ifdef _WIN32
        ::std::string tempDir = processingEnvironmentVariable("TEMP");
        if (tempDir.empty()) tempDir = "C:\\Temp";
        ::std::string tmp=tempDir+"\\pg_img_";
#else
        ::std::string tmp="/tmp/pg_img_";
#endif
        size_t sl=path.rfind('/');
        ::std::string bn=(sl!=::std::string::npos)?path.substr(sl+1):"img.png";
        { size_t q=bn.find('?'); if(q!=::std::string::npos) bn=bn.substr(0,q); }
        if(bn.empty()||bn.find('.')==::std::string::npos) bn="img.png";
        for(char& c:bn) if(c==':'||c=='*'||c=='<'||c=='>'||c=='|') c='_';
        tmp+=bn;
        auto isValidImg=[&]()->bool{
            FILE* f2=processingOpenFile(tmp.c_str(),"rb"); if(!f2) return false;
            fseek(f2,0,SEEK_END); long sz=ftell(f2); fseek(f2,0,SEEK_SET);
            unsigned char h[4]={}; fread(h,1,4,f2); fclose(f2);
            if(sz<100) return false;
            return (h[0]==0x89&&h[1]=='P')|| // PNG
                   (h[0]==0xFF&&h[1]==0xD8)|| // JPEG
                   (h[0]=='G'&&h[1]=='I')||   // GIF
                   (h[0]=='B'&&h[1]=='M')||   // BMP
                   (h[0]=='R'&&h[1]=='I');    // WEBP
        };
        // Try curl
#ifdef _WIN32
        system(("curl -sL --max-time 15 -o \""+tmp+"\" \""+path+"\"").c_str());
#else
        system(("curl -sL --max-time 15 -o '"+tmp+"' '"+path+"'").c_str());
#endif
        if(!isValidImg()){
            // Try wget as fallback
#ifdef _WIN32
            system(("wget -q -O \""+tmp+"\" \""+path+"\"").c_str());
#else
            system(("wget -q -O '"+tmp+"' '"+path+"'").c_str());
#endif
        }
        if(isValidImg()){
            PImage* r2=loadImage(tmp);
            if(r2&&r2->width>0) return r2;
        }
        fprintf(stderr,"[loadImage] URL download failed: %s\n",path.c_str());
        return nullptr;
    }
    // Search paths: current dir, data/, files/, and sketch subdirs
    // Check PROCESSING_SKETCH_PATH env var set by IDE
    ::std::string _sketchDir = processingEnvironmentVariable("PROCESSING_SKETCH_PATH");
    if (!_sketchDir.empty()) _sketchDir += "/";
    // Also get the directory of the running executable
    ::std::string _exeDir;
    {
        char _buf[4096] = {};
#ifdef _WIN32
        GetModuleFileNameA(nullptr, _buf, sizeof(_buf));
        ::std::string _ep(_buf);
        size_t _sl = _ep.find_last_of("\\\\");
#else
#ifndef __EMSCRIPTEN__
        ssize_t _len = readlink("/proc/self/exe", _buf, sizeof(_buf)-1);
        if (_len > 0) _buf[_len] = 0;
#endif
        ::std::string _ep(_buf);
        size_t _sl = _ep.find_last_of("/");
#endif
        if (_sl != ::std::string::npos) _exeDir = _ep.substr(0, _sl+1);
    }
    ::std::vector<::std::string> tries = {
        path,
        "data/" + path,
        "files/" + path,
        "../data/" + path,
        _sketchDir + path,
        _sketchDir + "data/" + path,
        _exeDir + path,
        _exeDir + "data/" + path,    // in case running from a subdirectory
    };
#ifdef PROCESSING_HAS_STB_IMAGE
    for (auto& p : tries) {
        int w, h, ch;
        unsigned char* data = stbi_load(p.c_str(), &w, &h, &ch, 4);
        if (!data) continue;
        PImage* img = new PImage(w, h);
        for (int i = 0; i < w*h; i++) {
            unsigned char r=data[i*4+0], g=data[i*4+1], b=data[i*4+2], a=data[i*4+3];
            img->pixels[i] = ((unsigned int)a<<24)|((unsigned int)r<<16)|((unsigned int)g<<8)|b;
        }
        stbi_image_free(data);
        img->dirty = true;
        return img;
    }
#endif
    for (auto& p : tries)
        ::std::cerr << "[loadImage] not found: " << p << "\n";
    ::std::cerr << "[loadImage] returning empty image -- check the path above\n";
    return new PImage(); // return empty (not null) so -> calls are safe
}

// ---- cppmode-pg-offscreen: libprocessing createGraphics() / saveFrame() ----
#ifdef CPPMODE_BACKEND_LIBPROCESSING
#define CPPMODE_LP_STYLE_FIELDS(X) X(fillR) X(fillG) X(fillB) X(fillA) X(strokeR) X(strokeG) X(strokeB) X(strokeA) X(strokeW) X(doFill) X(doStroke) X(smoothing) X(currentRectMode) X(currentEllipseMode) X(currentImageMode) X(tintR) X(tintG) X(tintB) X(tintA) X(doTint) X(colorModeVal) X(colorMaxH) X(colorMaxS) X(colorMaxB) X(colorMaxA) X(g_textSize) X(g_textAlignX) X(g_textAlignY) X(g_textLeading)
// cppmode-pg-v5: same style swap the OpenGL beginDraw()/endDraw() do, so pg keeps its own
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
}
bool cppmodeLpEndDraw(PGraphics* pg) {
    PApplet* app = PApplet::g_papplet;
    if (!pg || !app || !app->_backend) return false;
    if (!pg->lp_graphics) return true;  // endDraw() without beginDraw()
    static_cast<LibProcessingBackend*>(app->_backend)->cpp_pg_end(pg->lp_graphics);
    cppmodeLpStyleSave(pg->myStyle, app);          // remember pg's style for next time
    cppmodeLpStyleLoad(pg->_savedMainStyle, app);  // and give the main canvas its own back
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
    else if (ext == ".tga" || ext == ".tif" || ext == "tiff") ok = cppmodeWriteTgaOrTiff(fn.c_str(), ext, w, h, px.data());
    else { if (ext != ".png") fn += ".png"; ok = stbi_write_png(fn.c_str(), w, h, 3, px.data(), w * 3); }
    if (!ok) fprintf(stderr, "saveFrame: could not write %s\n", fn.c_str());
}
// cppmode-pg-v5: filter() and pg.save() for the libprocessing renderer
static void cppmodeLpFilter(PApplet* app, int mode, float param) {
    auto* be = static_cast<LibProcessingBackend*>(app->_backend);
    {   // cppmode-gpu-filter: GPU first, the CPU readback below is the fallback
        int k = mode == GRAY ? 0 : mode == INVERT ? 1 : mode == THRESHOLD ? 2 : mode == POSTERIZE ? 3 : mode == BLUR ? 4 : mode == OPAQUE ? 5 : mode == ERODE ? 6 : mode == DILATE ? 7 : -1;
        float p = param;
        if (k == 4 && p == 0.5f) p = 1.0f;         // filter(BLUR) with no radius: Java's default is 1
        if (k == 3 && (p < 2.0f || p > 255.0f)) k = -1;  // odd posterize levels: let the CPU path deal with it
        if (k >= 0 && be->cpp_gpu_filter(k, p)) return;
    }
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
    else if (ext == ".tga" || ext == ".tif" || ext == "tiff") ok = cppmodeWriteTgaOrTiff(fn.c_str(), ext, w, h, rgb.data());
    else { if (ext != ".png") fn += ".png"; ok = stbi_write_png(fn.c_str(), w, h, 3, rgb.data(), w * 3); }
    if (!ok) fprintf(stderr, "save: could not write %s\n", fn.c_str());
    return ok != 0;
}
void PGraphics::save(const ::std::string& filename) {
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
#else
bool cppmodeLpBeginDraw(PGraphics*) { return false; }
bool cppmodeLpEndDraw(PGraphics*) { return false; }
void PGraphics::save(const ::std::string& filename) {
    fprintf(stderr, "pg.save() works in the libprocessing renderer for now\n"); (void)filename;
}
#endif
PGraphics* PApplet::createGraphics(int w,int h){return new PGraphics(w,h);}
PGraphics* PApplet::createGraphics(int w,int h,int renderer){
    // The renderer argument genuinely matters: a P3D buffer needs
    // beginDraw() to set up a real perspective projection with depth
    // testing enabled, instead of the flat 2D ortho projection (depth
    // range -1..1) used for plain 2D buffers -- without this, any 3D
    // content drawn into the buffer (box(), sphere(), translate(...,z))
    // gets clipped away entirely by the paper-thin depth range, and
    // lighting/depth-testing never activates at all. See PGraphics::
    // beginDraw()'s is3D branch for the actual projection setup.
    return new PGraphics(w, h, renderer == P3D);
}

// ── PImage::uploadTexture ────────────────────────────────────────────────────
void PImage::uploadTexture() {
    if (width <= 0 || height <= 0 || pixels.empty()) return;
    if ((int)pixels.size() < width * height) return;
    ::std::vector<unsigned char> rgba((size_t)width * height * 4);
    for (int i = 0; i < width*height; i++) {
        unsigned int p = pixels[i];
        rgba[i*4+0] = (p>>16)&0xFF;
        rgba[i*4+1] = (p>>8) &0xFF;
        rgba[i*4+2] =  p     &0xFF;
        rgba[i*4+3] = (p>>24)&0xFF;
    }
    if (texID == 0) glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    dirty = false;
}

void PApplet::drawImageRect(PImage& img,float x,float y,float w,float h){
    if(img.width==0||img.height==0) return;
    if(img.dirty) img.uploadTexture();
    if(img.texID==0) return;
    glEnable(GL_BLEND);
    glApplyBlend(g_glBlendMode);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,img.texID);
    glColor4f(doTint?tintR:1.f,doTint?tintG:1.f,doTint?tintB:1.f,doTint?tintA:1.f);
#ifdef __EMSCRIPTEN__
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0,0);glVertex2f(x,y);
    glTexCoord2f(1,0);glVertex2f(x+w,y);
    glTexCoord2f(1,1);glVertex2f(x+w,y+h);
    glTexCoord2f(0,0);glVertex2f(x,y);
    glTexCoord2f(1,1);glVertex2f(x+w,y+h);
    glTexCoord2f(0,1);glVertex2f(x,y+h);
    glEnd();
#else
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex2f(x,y);
    glTexCoord2f(1,0);glVertex2f(x+w,y);
    glTexCoord2f(1,1);glVertex2f(x+w,y+h);
    glTexCoord2f(0,1);glVertex2f(x,y+h);
    glEnd();
#endif
    glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    // Restore white so non-textured drawing after image() looks correct
    glColor4f(1.f,1.f,1.f,1.f);
}

// ── image() canonical implementation ─────────────────────────────────────────
// Single entry point: everything goes through drawImage_impl.
// Takes a raw PImage pointer so no reference/ABI issues across TUs.
void PApplet::drawImage_impl(PImage* img, float x, float y, float w, float h) {
    if (!img || img->width == 0 || img->height == 0) return;
    // Apply imageMode to x,y,w,h
    float dx = x, dy = y, dw = w, dh = h;
    if (currentImageMode == CENTER)  { dx -= dw*0.5f; dy -= dh*0.5f; }
    else if (currentImageMode == CORNERS) { dw = w - x; dh = h - y; dx = x; dy = y; }
    drawImageRect(*img, dx, dy, dw, dh);
}

// ── Public image() entry points ─────────────────────────────────────────────
void PApplet::drawPGraphicsRect(PGraphics& pg, float x, float y, float w, float h){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // cppmode-api4: image(pg) draws the offscreen surface endDraw() left behind
        if (!pg.lp_image_id || pg.width == 0 || pg.height == 0) return;
        if (!doTint) _backend->image_no_tint();
        else _backend->image_tint(tintR, tintG, tintB, tintA);
        if (currentImageMode == CENTER) { x -= w * 0.5f; y -= h * 0.5f; }
        else if (currentImageMode == CORNERS) { w -= x; h -= y; }
        processing_image_scaled(_backend->graphics_id(), pg.lp_image_id, x, y, w, h);
        return; }
#endif
    PDEBUG("drawPGraphicsRect ENTRY: requested x=%.1f y=%.1f w=%.1f h=%.1f | pg.width=%d pg.height=%d | pg.texID=%u\n",
           x, y, w, h, pg.width, pg.height, pg.texID);
    if(pg.width==0||pg.height==0) { PDEBUG("  -> early return: width or height is 0\n"); return; }
    if(pg.texID==0) { PDEBUG("  -> early return: texID is 0\n"); return; }
    // BUG FIX: if lights() was called earlier this frame (e.g. drawing a
    // lit 3D scene on the main canvas before displaying this buffer),
    // GL_LIGHTING/GL_COLOR_MATERIAL/GL_DEPTH_TEST are all still globally
    // enabled at this point -- nothing here ever disabled them. Drawing
    // this textured quad with lighting still active means OpenGL tries
    // to light it like a 3D surface (with no normal ever set for it,
    // since this is meant to be a plain 2D blit), which can darken or
    // otherwise corrupt the displayed colors even though the texture's
    // actual stored content is correct. A 2D image blit should never be
    // affected by whatever 3D lighting state happens to be active.
    GLboolean wasLighting = glIsEnabled(GL_LIGHTING);
    GLboolean wasDepthTest = glIsEnabled(GL_DEPTH_TEST);
    if (wasLighting) glDisable(GL_LIGHTING);
    if (wasDepthTest) glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); glApplyBlend(g_glBlendMode);
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,pg.texID);
    if (doTint) glColor4f(tintR, tintG, tintB, tintA); else glColor4f(1,1,1,1);  // cppmode-pgpixels: tint() applies to image(pg)
#ifdef __EMSCRIPTEN__
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0,0);glVertex2f(x,y);
    glTexCoord2f(1,0);glVertex2f(x+w,y);
    glTexCoord2f(1,1);glVertex2f(x+w,y+h);
    glTexCoord2f(0,0);glVertex2f(x,y);
    glTexCoord2f(1,1);glVertex2f(x+w,y+h);
    glTexCoord2f(0,1);glVertex2f(x,y+h);
    glEnd();
#else
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex2f(x,y);
    glTexCoord2f(1,0);glVertex2f(x+w,y);
    glTexCoord2f(1,1);glVertex2f(x+w,y+h);
    glTexCoord2f(0,1);glVertex2f(x,y+h);
    glEnd();
#endif
    glBindTexture(GL_TEXTURE_2D,0); glDisable(GL_TEXTURE_2D); glDisable(GL_BLEND);
    glColor4f(1,1,1,1);
    if (wasLighting) glEnable(GL_LIGHTING);
    if (wasDepthTest) glEnable(GL_DEPTH_TEST);
}
void PApplet::image(PGraphics& pg, float x, float y){ drawPGraphicsRect(pg,x,y,(float)pg.width,(float)pg.height); }
void PApplet::image(PGraphics& pg, float x, float y, float w, float h){ drawPGraphicsRect(pg,x,y,w,h); }
void PApplet::image(PImage* img, float x, float y) {
    if (!img) return;
    if (_backend && lpEnsureImage(_backend, img)) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
        if (!doTint) _backend->image_no_tint();
        else _backend->image_tint(tintR, tintG, tintB, tintA);
#endif
        if (currentImageMode == CENTER && img->width > 0 && img->height > 0) {
            float iw = (float)img->width, ih = (float)img->height;
            processing_image_scaled(_backend->graphics_id(), img->lp_image_id, x - iw * 0.5f, y - ih * 0.5f, iw, ih); return;
        }
        processing_image(_backend->graphics_id(), img->lp_image_id, x, y); return;
    }
    if(!img || img->width==0 || img->height==0) return;
    drawImage_impl(img, x, y, (float)img->width, (float)img->height);
}
void PApplet::image(PImage* img, float x, float y, float w, float h) {
    if (!img) return;
    if (_backend && lpEnsureImage(_backend, img)) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
        if (!doTint) _backend->image_no_tint();
        else _backend->image_tint(tintR, tintG, tintB, tintA);
#endif
        // imageMode, as in Java: CENTER is the middle, CORNERS gives the opposite corner
        if (currentImageMode == CENTER)  { x -= w * 0.5f; y -= h * 0.5f; }
        if (currentImageMode == CORNERS) { w -= x; h -= y; }
        processing_image_scaled(_backend->graphics_id(), img->lp_image_id, x, y, w, h); return;
    }
    if(!img || img->width==0 || img->height==0) return;
    drawImage_impl(img, x, y, w, h);
}
// Nine-argument form: image(img, dx1,dy1,dx2,dy2, sx1,sy1,sx2,sy2)
void PApplet::image(PImage* img, float dx1,float dy1,float dx2,float dy2,
                               float sx1,float sy1,float sx2,float sy2) {
    if(!img || img->width==0 || img->height==0) return;
    // As in Java: args 3/4 are width/height unless imageMode(CORNERS);
    // source coords are always pixel corners. Resolve to dest corners here.
    if (currentImageMode == CORNER)       { dx2 += dx1; dy2 += dy1; }
    else if (currentImageMode == CENTER)  { float hw = dx2 * 0.5f, hh = dy2 * 0.5f;
                                            dx2 = dx1 + hw; dy2 = dy1 + hh; dx1 -= hw; dy1 -= hh; }
    if (_backend && lpEnsureImage(_backend, img)) {
        if (!doTint) _backend->image_no_tint();
        else _backend->image_tint(tintR, tintG, tintB, tintA);
        _backend->image_region(img->lp_image_id, dx1, dy1, dx2 - dx1, dy2 - dy1,
                               sx1, sy1, sx2 - sx1, sy2 - sy1);
        return;
    }
    if(img->dirty) img->uploadTexture();
    if(img->texID==0) return;
    // Convert source pixel coords to UV [0,1]
    float u1=sx1/img->width, v1=sy1/img->height;
    float u2=sx2/img->width, v2=sy2/img->height;
    glEnable(GL_BLEND); glApplyBlend(g_glBlendMode);
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,img->texID);
    glColor4f(doTint?tintR:1.f,doTint?tintG:1.f,doTint?tintB:1.f,doTint?tintA:1.f);
    glBegin(GL_QUADS);
    glTexCoord2f(u1,v1); glVertex2f(dx1,dy1);
    glTexCoord2f(u2,v1); glVertex2f(dx2,dy1);
    glTexCoord2f(u2,v2); glVertex2f(dx2,dy2);
    glTexCoord2f(u1,v2); glVertex2f(dx1,dy2);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0); glDisable(GL_TEXTURE_2D); glDisable(GL_BLEND);
    glColor4f(1.f,1.f,1.f,1.f);
}

void PApplet::imageMode(int m){currentImageMode=m;}  // resolved in image(), not forwarded
void PApplet::tint(float gray)           {tint(gray, colorMaxA);}
void PApplet::tint(float gray, float a) {
    // tint(gray, alpha) -- both in 0-255 range like Processing Java
    tintR = tintG = tintB = gray / 255.f;
    tintA = a / 255.f;
    doTint = true;
}
void PApplet::tint(float r, float g, float b, float a) {
    // tint(r, g, b, alpha) -- all in 0-255 range
    tintR = r / 255.f;
    tintG = g / 255.f;
    tintB = b / 255.f;
    tintA = a / 255.f;
    doTint = true;
}
void PApplet::tint(float r, float g, float b) { tint(r, g, b, colorMaxA); }
void PApplet::noTint(){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend){_backend->image_no_tint();}
#endif
    doTint=false;}

void PApplet::filter(int mode) { filter(mode, 0.5f); }
void PApplet::filter(int mode, float param) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { cppmodeLpFilter(this, mode, param); return; }  // cppmode-pg-v5
#endif
    int w = winWidth, h = winHeight, total = w * h;
    ::std::vector<unsigned char> buf(total * 4);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    if (mode == GRAY || mode == INVERT || mode == THRESHOLD || mode == OPAQUE || mode == POSTERIZE) {
        for (int i = 0; i < total; i++) {
            int r=buf[i*4],g=buf[i*4+1],b=buf[i*4+2];
            int grey=(r+g+b)/3;
            if      (mode==GRAY)      { buf[i*4]=buf[i*4+1]=buf[i*4+2]=(unsigned char)grey; }
            else if (mode==INVERT)    { buf[i*4]=static_cast<unsigned char>(255-r); buf[i*4+1]=static_cast<unsigned char>(255-g); buf[i*4+2]=static_cast<unsigned char>(255-b); }
            else if (mode==THRESHOLD) { unsigned char t=(grey>param*255)?255:0; buf[i*4]=buf[i*4+1]=buf[i*4+2]=t; }
            else if (mode==OPAQUE)    { buf[i*4+3]=static_cast<unsigned char>(255); }
            else if (mode==POSTERIZE) {
                int levels = ::std::max(2,(int)param);
                auto post = [levels](int v){ return (int)((int)(v*(levels-1)/255.0f+0.5f)*255/(levels-1)); };
             buf[static_cast<size_t>(i)*4u]=static_cast<unsigned char>(post(r)); buf[static_cast<size_t>(i)*4u+1u]=static_cast<unsigned char>(post(g)); buf[static_cast<size_t>(i)*4u+2u]=static_cast<unsigned char>(post(b));
            }
        }
    } else if (mode == BLUR) {
        int radius = ::std::max(1,(int)param);
        ::std::vector<unsigned char> tmp(buf);
        for (int y=0;y<h;y++) for (int x2=0;x2<w;x2++) {
            int sr=0,sg=0,sb=0,sa=0,cnt=0;
            for (int dy=-radius;dy<=radius;dy++) for (int dx=-radius;dx<=radius;dx++) {
                int nx=x2+dx,ny=y+dy;
                if(nx<0||nx>=w||ny<0||ny>=h) continue;
                int j=(ny*w+nx)*4;
                sr+=tmp[j]; sg+=tmp[j+1]; sb+=tmp[j+2]; sa+=tmp[j+3]; cnt++;
            }
            int i=(y*w+x2)*4;
            buf[i]=static_cast<unsigned char>(sr/cnt); buf[i+1]=static_cast<unsigned char>(sg/cnt); buf[i+2]=static_cast<unsigned char>(sb/cnt); buf[i+3]=static_cast<unsigned char>(sa/cnt);
        }
    } else if (mode == ERODE || mode == DILATE) {
        ::std::vector<unsigned char> tmp(buf);
        for (int y=0;y<h;y++) for (int x2=0;x2<w;x2++) {
            int best=-1;
            for (int dy=-1;dy<=1;dy++) for (int dx=-1;dx<=1;dx++) {
                int nx=x2+dx,ny=y+dy;
                if(nx<0||nx>=w||ny<0||ny>=h) continue;
                int j=(ny*w+nx)*4;
                int lum=(tmp[j]+tmp[j+1]+tmp[j+2])/3;
                if(best<0||(mode==ERODE?lum<best:lum>best)) best=lum;
            }
            int i=(y*w+x2)*4;
            buf[i]=buf[i+1]=buf[i+2]=(unsigned char)best;
        }
    }
#ifndef __EMSCRIPTEN__
    glWindowPos2i(0, 0);  // cppmode-pimage-api: set() moves the raster position
    glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
#endif
}
#ifdef CPPMODE_BACKEND_LIBPROCESSING
// cppmode-fastpixels: pixels[] as loadPixels() left it, so updatePixels() uploads only the changed rows
static ::std::vector<unsigned int> g_lpPixSnap;
#endif
void PApplet::loadPixels() {
    int total = winWidth * winHeight;
    pixels.resize(total);
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        // graphics_readback flushes the queued draw commands itself;
        // flushing here too renders an empty second frame and reads that
        ::std::vector<Color> buf(total);
        processing_graphics_readback(_backend->graphics_id(), buf.data(), (uintptr_t)total);
        // cppmode-fastpixels: readback is LinearRgba; pixels[] is sRGB like Java.
        // A table built once replaces pow() per channel (about 0.5M calls a frame at 600x300).
        static unsigned char lut[4097]; static bool lutReady = false;
        if (!lutReady) { lutReady = true;
            for (int k = 0; k <= 4096; k++) { float v = k / 4096.0f;
                v = v <= 0.0031308f ? v * 12.92f : 1.055f * ::std::pow(v, 1.f / 2.4f) - 0.055f;
                lut[k] = (unsigned char)(v * 255.f + 0.5f); } }
        auto idx = [](float v) -> int { v = v < 0 ? 0 : (v > 1 ? 1 : v); return (int)(v * 4096.f + 0.5f); };
        auto ch  = [](float v) -> unsigned { v = v < 0 ? 0 : (v > 1 ? 1 : v); return (unsigned)(v * 255.f + 0.5f); };
        for (int i = 0; i < total; i++) {
            const Color& c = buf[i];
            pixels[i] = (ch(c.a) << 24) | ((unsigned)lut[idx(c.c1)] << 16) | ((unsigned)lut[idx(c.c2)] << 8) | lut[idx(c.c3)];
        }
        g_lpPixSnap.assign(pixels.begin(), pixels.end());   // updatePixels() sends only what changed
        return;
    }
#endif
    // Copy framebuffer into pixels[] (OpenGL path)

    ::std::vector<unsigned char> buf(total * 4);
    glReadPixels(0, 0, winWidth, winHeight, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
    // Convert RGBA + flip Y (GL is bottom-up, Processing is top-down)
    for (int y = 0; y < winHeight; y++) {
        for (int x = 0; x < winWidth; x++) {
            int si = (winHeight - 1 - y) * winWidth + x;
            int di = y * winWidth + x;
            unsigned char r=buf[si*4],g=buf[si*4+1],b=buf[si*4+2],a=buf[si*4+3];
            pixels[di] = (a<<24)|(r<<16)|(g<<8)|b;
        }
    }
}
void PApplet::updatePixels() {
    int total = winWidth * winHeight;
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // cppmode-fastpixels: this used to fall through to glDrawPixels, which does nothing here
        if ((int)pixels.size() < total) return;
        int y0 = 0, y1 = winHeight - 1;
        bool snap = (int)g_lpPixSnap.size() == total;
        if (snap) {
            y0 = -1;
            for (int y = 0; y < winHeight && y0 < 0; y++)
                if (::std::memcmp(&pixels[(size_t)y * winWidth], &g_lpPixSnap[(size_t)y * winWidth], winWidth * sizeof(unsigned int))) y0 = y;
            if (y0 < 0) return;   // nothing changed
            for (int y = winHeight - 1; y >= y0; y--)
                if (::std::memcmp(&pixels[(size_t)y * winWidth], &g_lpPixSnap[(size_t)y * winWidth], winWidth * sizeof(unsigned int))) { y1 = y; break; }
        }
        size_t n = (size_t)(y1 - y0 + 1) * winWidth;
        ::std::vector<Color> buf(n);
        for (size_t k = 0; k < n; k++) {
            unsigned int c = pixels[(size_t)y0 * winWidth + k];
            buf[k] = lp_rgba(((c >> 16) & 255) / 255.0f, ((c >> 8) & 255) / 255.0f, (c & 255) / 255.0f, ((c >> 24) & 255) / 255.0f);
        }
        processing_graphics_update_region(_backend->graphics_id(), 0, (uint32_t)y0, (uint32_t)winWidth, (uint32_t)(y1 - y0 + 1), buf.data(), n);
        if (const char* e = processing_check_error()) fprintf(stderr, "[libprocessing] updatePixels: %s\n", e);
        if (snap) ::std::copy(pixels.begin() + (size_t)y0 * winWidth, pixels.begin() + (size_t)(y1 + 1) * winWidth, g_lpPixSnap.begin() + (size_t)y0 * winWidth);
        else g_lpPixSnap.assign(pixels.begin(), pixels.begin() + total);
        return;
    }
#endif
    ::std::vector<unsigned char> rgba(total * 4);

    // pixels[] is top-to-bottom; glDrawPixels expects bottom-to-top,
    // so flip rows on the way out.
    for (int y = 0; y < winHeight; y++) {
        for (int x = 0; x < winWidth; x++) {
            int si = y * winWidth + x;
            int di = (winHeight - 1 - y) * winWidth + x;
            rgba[di*4 + 0] = (pixels[si] >> 16) & 0xFF;  // R
            rgba[di*4 + 1] = (pixels[si] >>  8) & 0xFF;  // G
            rgba[di*4 + 2] =  pixels[si]        & 0xFF;  // B
            rgba[di*4 + 3] = (pixels[si] >> 24) & 0xFF;  // A
        }
    }
#ifndef __EMSCRIPTEN__
    glWindowPos2i(0, 0);  // cppmode-pimage-api: set() moves the raster position
    glDrawPixels(winWidth, winHeight, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
#else
    GLuint tex; glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,winWidth,winHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.data());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,tex);
    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0,1);glVertex2f(0,0);
    glTexCoord2f(1,1);glVertex2f(winWidth,0);
    glTexCoord2f(1,0);glVertex2f(winWidth,winHeight);
    glTexCoord2f(0,0);glVertex2f(0,winHeight);
    glEnd();
    glDisable(GL_TEXTURE_2D); glDeleteTextures(1,&tex);
#endif
}
// cppmode-lpcopy: canvas pixels for get()/copy()/blend() in the libprocessing renderer
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
color PApplet::get(int x, int y) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // cppmode-lpcopy
        ::std::vector<unsigned int> px; int W = 0, H = 0; float s = 1;
        if (x < 0 || y < 0 || x >= width || y >= height || !cppmodeLpRead(this, px, W, H, s)) return color(0u);
        int X = ::std::min(W - 1, (int)(x * s)), Y = ::std::min(H - 1, (int)(y * s));
        return color(px[(size_t)Y * W + X]);
    }
#endif
    unsigned char p[4];
    glReadPixels(x, winHeight - 1 - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, p);
    return colorVal(p[0], p[1], p[2], p[3]);
}

void PApplet::set(int x, int y, color c) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // cppmode-api3
        if (x < 0 || y < 0 || x >= width || y >= height) return;
        unsigned int v = c.value;
        processing_graphics_set(_backend->graphics_id(), (uint32_t)x, (uint32_t)y,
            lp_rgba(((v >> 16) & 255) / 255.0f, ((v >> 8) & 255) / 255.0f, (v & 255) / 255.0f, ((v >> 24) & 255) / 255.0f));
        return; }
#endif
    unsigned int v = c.value;
    unsigned char p[] = {
        (unsigned char)((v >> 16) & 0xFF),  // R
        (unsigned char)((v >>  8) & 0xFF),  // G
        (unsigned char)( v        & 0xFF),  // B
        (unsigned char)((v >> 24) & 0xFF)   // A
    };
#ifndef __EMSCRIPTEN__
    glWindowPos2i(x, winHeight - 1 - y);
    glDrawPixels(1, 1, GL_RGBA, GL_UNSIGNED_BYTE, p);
#else
    // WASM: single pixel set via texture
    GLuint tex; glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,p);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,tex);
    glColor4f(1,1,1,1);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0,0);glVertex2f((float)x,(float)y);
    glTexCoord2f(1,0);glVertex2f((float)x+1,(float)y);
    glTexCoord2f(1,1);glVertex2f((float)x+1,(float)y+1);
    glTexCoord2f(0,0);glVertex2f((float)x,(float)y);
    glTexCoord2f(1,1);glVertex2f((float)x+1,(float)y+1);
    glTexCoord2f(0,1);glVertex2f((float)x,(float)y+1);
    glEnd();
    glDisable(GL_TEXTURE_2D); glDeleteTextures(1,&tex);
#endif
}

// =============================================================================
// BLEND / CLIP
// =============================================================================

void PApplet::blendMode(int mode) {
    if (_backend) {
        uint8_t lp = 0;
        switch(mode) {
            case BLEND:      lp=0; break;
            case ADD:        lp=1; break;
            case SUBTRACT:   lp=2; break;
            case DARKEST:    lp=3; break;
            case LIGHTEST:   lp=4; break;
            case DIFFERENCE: lp=5; break;
            case MULTIPLY:   lp=7; break;
            case SCREEN:     lp=8; break;
            case REPLACE:    lp=9; break;
            default:         lp=0; break;
        }
        processing_set_blend_mode(_backend->graphics_id(), lp);
        return;
    }
    g_glBlendMode = mode;
    glApplyBlend(mode);
}

void PApplet::clip(float x, float y, float w, float h) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // cppmode-clip: the shader drops pixels outside this rect
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
#endif
    // Scissor coordinates are in GL space (Y=0 at bottom), so flip Y
    glEnable(GL_SCISSOR_TEST);
    glScissor((int)x, winHeight - (int)(y + h), (int)w, (int)h);
}

void PApplet::noClip() {
    if (_backend) { processing_no_clip(_backend->graphics_id()); return; }  /* cppmode-clip */
    glDisable(GL_SCISSOR_TEST);
}

// =============================================================================
// SAVE
// =============================================================================

void PApplet::saveFrame(const ::std::string& filename) {
#ifdef __EMSCRIPTEN__
    // WASM: capture pixels and trigger browser download
    int _sw = logicalW, _sh = logicalH;
    ::std::vector<unsigned char> _spx(_sw * _sh * 4);
    glReadPixels(0, 0, _sw, _sh, GL_RGBA, GL_UNSIGNED_BYTE, _spx.data());
    for (int _sy = 0; _sy < _sh/2; _sy++)
        for (int _sx = 0; _sx < _sw*4; _sx++)
            ::std::swap(_spx[_sy*_sw*4+_sx], _spx[(_sh-1-_sy)*_sw*4+_sx]);
    int pngLen = 0;
    unsigned char* png = stbi_write_png_to_mem(_spx.data(), _sw*4, _sw, _sh, 4, &pngLen);
    if (png && pngLen > 0) {
        // Pass to JS for download
        EM_ASM({
            var arr = new Uint8Array(Module.HEAPU8.buffer, $0, $1);
            var blob = new Blob([arr], {type:'image/png'});
            var url = URL.createObjectURL(blob);
            var a = document.createElement('a');
            a.href = url; a.download = UTF8ToString($2);
            document.body.appendChild(a); a.click();
            document.body.removeChild(a);
            URL.revokeObjectURL(url);
        }, png, pngLen, filename.c_str());
        free(png);
    }
    return;
#endif
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { cppmodeLpSaveFrame(this, filename); return; }  // cppmode-pg-offscreen
#endif
    ::std::string fn = filename;
    size_t pos = fn.find("####");
    if (pos != ::std::string::npos) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%04d", frameCount);
        fn.replace(pos, 4, buf);
    }
    int w = pixelWidth  > 0 ? pixelWidth  : logicalW;
    int h = pixelHeight > 0 ? pixelHeight : logicalH;
    ::std::vector<unsigned char> px(w * h * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, px.data());
    for (int y = 0; y < h / 2; y++)
        for (int x = 0; x < w * 3; x++)
            ::std::swap(px[y*w*3+x], px[(h-1-y)*w*3+x]);
    ::std::string ext = fn.size() > 4 ? fn.substr(fn.size()-4) : "";
    for (auto& c : ext) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    int ok = 0;
    if (ext == ".png")                ok = stbi_write_png(fn.c_str(), w, h, 3, px.data(), w*3);
    else if (ext == ".jpg" || ext == "jpeg") ok = stbi_write_jpg(fn.c_str(), w, h, 3, px.data(), 95);
    else if (ext == ".bmp")           ok = stbi_write_bmp(fn.c_str(), w, h, 3, px.data());
    else if (ext == ".tga" || ext == ".tif" || ext == "tiff")
                                      ok = cppmodeWriteTgaOrTiff(fn.c_str(), ext, w, h, px.data());
    else { fn += ".png"; ok = stbi_write_png(fn.c_str(), w, h, 3, px.data(), w*3); }
    if (!ok) ::std::cerr << "saveFrame: could not write " << fn << "\n";  // cppmode-savefile: quiet on success, like Java
}
void PApplet::save(const ::std::string& fn){saveFrame(fn);}

// =============================================================================
// GLFW CALLBACKS
// =============================================================================

static void cursor_pos_cb(GLFWwindow*, double x, double y) {
    auto* p = PApplet::g_papplet; if(!p) return;
    if (p->_backend) p->_backend->input_mouse_move((float)x,(float)y);
    p->mouseInWindow = true;
    p->mouseDX += (float)x - p->mouseX;
    p->mouseDY += (float)y - p->mouseY;
    p->pmouseX = p->mouseX;
    p->pmouseY = p->mouseY;
    p->mouseX  = (float)x;
    p->mouseY  = (float)y;
    { auto [sh,ct,al,me] = ::std::make_tuple(
          (p->g_currentMods&GLFW_MOD_SHIFT)!=0,
          (p->g_currentMods&GLFW_MOD_CONTROL)!=0,
          (p->g_currentMods&GLFW_MOD_ALT)!=0,
          (p->g_currentMods&GLFW_MOD_SUPER)!=0);
      int act = p->_mousePressed ? MouseEvent::DRAG : MouseEvent::MOVE;
      MouseEvent me2{(float)p->mouseX, (float)p->mouseY, p->mouseButton, 0, sh, ct, al, me, act};
      if (!p->_setupDone) return;  // cppmode-events: no sketch callbacks before setup() has run
      if (p->_mousePressed) { p->mouseWasPressed = false; p->mouseDragged(); p->mouseDragged(me2); }  // a drag cancels mouseClicked
      else                  { p->mouseMoved();   p->mouseMoved(me2);   }
    }
}
static void mouse_btn_cb(GLFWwindow*, int btn, int action, int mods) {
    auto* p = PApplet::g_papplet; if(!p) return;
    p->g_currentMods = mods;
    if (p->_backend) p->_backend->input_mouse_button((uint8_t)btn, action == GLFW_PRESS);
    if (action == GLFW_PRESS) {
        p->_mousePressed = true;
        if(btn>=0&&btn<8) p->mouseButtons[btn]=true;
        { int mb=(btn==GLFW_MOUSE_BUTTON_LEFT)?37:(btn==GLFW_MOUSE_BUTTON_RIGHT)?39:3;
          if(mb<128) p->mouseDown[mb]=true; }
        if      (btn == GLFW_MOUSE_BUTTON_LEFT)   p->mouseButton = LEFT;
        else if (btn == GLFW_MOUSE_BUTTON_RIGHT)  p->mouseButton = RIGHT;
        else                                       p->mouseButton = CENTER;
        p->_eventDrewSomething=true;
        { auto sh=(p->g_currentMods&GLFW_MOD_SHIFT)!=0;
          auto ct=(p->g_currentMods&GLFW_MOD_CONTROL)!=0;
          auto al=(p->g_currentMods&GLFW_MOD_ALT)!=0;
          auto me=(p->g_currentMods&GLFW_MOD_SUPER)!=0;
          MouseEvent ev{(float)p->mouseX, (float)p->mouseY, p->mouseButton, 1, sh, ct, al, me, MouseEvent::PRESS};
          if (p->_setupDone) { p->mousePressed(); p->mousePressed(ev); }  // cppmode-events
        }
        p->mouseWasPressed = true;
    } else if (action == GLFW_RELEASE) {
        if(btn>=0&&btn<8) p->mouseButtons[btn]=false;
        { int mb=(btn==GLFW_MOUSE_BUTTON_LEFT)?37:(btn==GLFW_MOUSE_BUTTON_RIGHT)?39:3;
          if(mb<128) p->mouseDown[mb]=false; }
        bool anyMouse=false;
        for(int i=0;i<8;i++) if(p->mouseButtons[i]){anyMouse=true;break;}
        p->_mousePressed=anyMouse;
        p->mouseButton = p->mouseButtons[GLFW_MOUSE_BUTTON_LEFT]   ? LEFT  :
                         p->mouseButtons[GLFW_MOUSE_BUTTON_RIGHT]  ? RIGHT :
                         p->mouseButtons[GLFW_MOUSE_BUTTON_MIDDLE] ? CENTER : -1;
        { auto sh=(p->g_currentMods&GLFW_MOD_SHIFT)!=0;
          auto ct=(p->g_currentMods&GLFW_MOD_CONTROL)!=0;
          auto al=(p->g_currentMods&GLFW_MOD_ALT)!=0;
          auto me=(p->g_currentMods&GLFW_MOD_SUPER)!=0;
          MouseEvent ev{(float)p->mouseX, (float)p->mouseY, p->mouseButton, 1, sh, ct, al, me, MouseEvent::RELEASE};
          if (p->_setupDone) { p->mouseReleased(); p->mouseReleased(ev); }
          if(p->mouseWasPressed && p->_setupDone) {
              MouseEvent ce{(float)p->mouseX, (float)p->mouseY, p->mouseButton, 1, sh, ct, al, me, MouseEvent::CLICK};
              p->mouseClicked(); p->mouseClicked(ce);
          }
        }
        p->mouseWasPressed=false;
    }
}
static void scroll_cb(GLFWwindow*,double xoffset,double yoffset){
    auto* p = PApplet::g_papplet; if(!p) return;
    if (p->_backend) p->_backend->input_scroll((float)xoffset, (float)yoffset);
    int delta = (int)-yoffset;  // cppmode-events: Java counts wheel-down as positive; GLFW is the opposite
    if (!p->_setupDone) return;
    if(p->_onMouseWheel) p->_onMouseWheel(delta);
    p->mouseWheel(delta);
    { auto sh=(p->g_currentMods&GLFW_MOD_SHIFT)!=0;
      auto ct=(p->g_currentMods&GLFW_MOD_CONTROL)!=0;
      auto al=(p->g_currentMods&GLFW_MOD_ALT)!=0;
      auto me=(p->g_currentMods&GLFW_MOD_SUPER)!=0;
      MouseEvent ev{(float)p->mouseX, (float)p->mouseY, p->mouseButton, delta, sh, ct, al, me, MouseEvent::WHEEL};
      p->mouseWheel(ev);
    }
}

static void char_cb(GLFWwindow*, unsigned int codepoint) {
    auto* p = PApplet::g_papplet; if(!p) return;
    if (p->_backend) p->_backend->input_char(0, codepoint);
    // char_cb fires after key_cb for printable keys, with the correct
    // Unicode character (shift/caps/layout all applied).
    if (codepoint < 128) {
        p->key = (char16_t)codepoint;
    }
    // Fire deferred keyPressed() now that p->key has the correct char value
    if (p->g_pendingKeyPressed) {
        p->g_pendingKeyPressed = false;
        p->_eventDrewSomething=true;
        if (p->_setupDone) { p->keyPressed(); p->keyPressed(KeyEvent{p->key, p->keyCode, (p->g_currentMods&GLFW_MOD_SHIFT)!=0, (p->g_currentMods&GLFW_MOD_CONTROL)!=0, (p->g_currentMods&GLFW_MOD_ALT)!=0, (p->g_currentMods&GLFW_MOD_SUPER)!=0, KeyEvent::PRESS}); }
    }
    // keyTyped() -- Processing standard: only printable chars, no action keys
    // Action keys (Ctrl, Shift, Alt, etc.) never reach char_cb, so this is correct.
    if (p->_setupDone) { p->keyTyped(); p->keyTyped(KeyEvent{p->key, p->keyCode, (p->g_currentMods&GLFW_MOD_SHIFT)!=0, (p->g_currentMods&GLFW_MOD_CONTROL)!=0, (p->g_currentMods&GLFW_MOD_ALT)!=0, (p->g_currentMods&GLFW_MOD_SUPER)!=0, KeyEvent::TYPE}); }
}

// Translate GLFW key code to Java KeyEvent.VK_* value.
// The Processing reference says keyCode matches Java's KeyEvent constants.
static int glfw_to_java_keycode(int k) {
    switch (k) {
        // Arrow / navigation
        case GLFW_KEY_UP:        return 38;
        case GLFW_KEY_DOWN:      return 40;
        case GLFW_KEY_LEFT:      return 37;
        case GLFW_KEY_RIGHT:     return 39;
        case GLFW_KEY_HOME:      return 36;
        case GLFW_KEY_END:       return 35;
        case GLFW_KEY_PAGE_UP:   return 33;
        case GLFW_KEY_PAGE_DOWN: return 34;
        // Modifiers
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:   return 16;
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL: return 17;
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:     return 18;
        case GLFW_KEY_LEFT_SUPER:
        case GLFW_KEY_RIGHT_SUPER:   return 157;  // VK_META
        // ASCII control (key is set directly, not CODED, but keyCode is still set)
        case GLFW_KEY_BACKSPACE: return 8;
        case GLFW_KEY_TAB:       return 9;
        case GLFW_KEY_ENTER:
        case GLFW_KEY_KP_ENTER:  return 10;       // ENTER (also fires RETURN=13 via key)
        case GLFW_KEY_ESCAPE:    return 27;
        case GLFW_KEY_DELETE:    return 127;
        case GLFW_KEY_INSERT:    return 155;
        // Function keys
        case GLFW_KEY_F1:  return 112; case GLFW_KEY_F2:  return 113;
        case GLFW_KEY_F3:  return 114; case GLFW_KEY_F4:  return 115;
        case GLFW_KEY_F5:  return 116; case GLFW_KEY_F6:  return 117;
        case GLFW_KEY_F7:  return 118; case GLFW_KEY_F8:  return 119;
        case GLFW_KEY_F9:  return 120; case GLFW_KEY_F10: return 121;
        case GLFW_KEY_F11: return 122; case GLFW_KEY_F12: return 123;
        // Letter keys: GLFW A=65..Z=90 matches Java VK_A=65..VK_Z=90
        // (GLFW uses uppercase, Java uses uppercase -- same values)
        default:
            if (k >= GLFW_KEY_A && k <= GLFW_KEY_Z) return k;  // 65..90 match
            if (k >= GLFW_KEY_0 && k <= GLFW_KEY_9) return k - GLFW_KEY_0 + 48; // 48..57
            return k; // best effort for anything else
    }
}

// Current modifier state -- set from key_cb mods parameter (reliable on Windows)

// Map GLFW keycode -> Processing/Java keyCode constant
static int glfw_to_processing_keycode(int k) {
    if(k >= GLFW_KEY_A && k <= GLFW_KEY_Z) return 'A' + (k - GLFW_KEY_A); // 65-90
    if(k >= GLFW_KEY_0 && k <= GLFW_KEY_9) return '0' + (k - GLFW_KEY_0); // 48-57
    switch(k) {
        case GLFW_KEY_UP:           return 38;
        case GLFW_KEY_DOWN:         return 40;
        case GLFW_KEY_LEFT:         return 37;
        case GLFW_KEY_RIGHT:        return 39;
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:  return 16;  // SHIFT
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:return 17;  // CONTROL
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:    return 18;  // ALT
        case GLFW_KEY_SPACE:        return 32;
        case GLFW_KEY_ENTER:
        case GLFW_KEY_KP_ENTER:     return 10;
        case GLFW_KEY_BACKSPACE:    return 8;
        case GLFW_KEY_TAB:          return 9;
        case GLFW_KEY_ESCAPE:       return 27;
        case GLFW_KEY_DELETE:       return 127;
        case GLFW_KEY_HOME:         return 36;
        case GLFW_KEY_END:          return 35;
        case GLFW_KEY_PAGE_UP:      return 33;
        case GLFW_KEY_PAGE_DOWN:    return 34;
        case GLFW_KEY_INSERT:       return 155;
        case GLFW_KEY_F1:           return 112;
        case GLFW_KEY_F2:           return 113;
        case GLFW_KEY_F3:           return 114;
        case GLFW_KEY_F4:           return 115;
        case GLFW_KEY_F5:           return 116;
        case GLFW_KEY_F6:           return 117;
        case GLFW_KEY_F7:           return 118;
        case GLFW_KEY_F8:           return 119;
        case GLFW_KEY_F9:           return 120;
        case GLFW_KEY_F10:          return 121;
        case GLFW_KEY_F11:          return 122;
        case GLFW_KEY_F12:          return 123;
        default:                    return -1;
    }
}

// Scancode-based press tracking for Bug B (GLFW #2417 Windows virtual key mismatch)
static ::std::unordered_set<int> s_pressedScancodes;

// Translate GLFW key to Processing key/keyCode atomically
static void translate_glfw_key(int k, char16_t* outKey, int* outKeyCode) {
    *outKeyCode = glfw_to_java_keycode(k);
    switch (k) {
        case GLFW_KEY_BACKSPACE: *outKey = 8;   return;
        case GLFW_KEY_TAB:       *outKey = 9;   return;
        case GLFW_KEY_ENTER:
        case GLFW_KEY_KP_ENTER:  *outKey = 10;  return;
        case GLFW_KEY_ESCAPE:    *outKey = 27;  return;
        case GLFW_KEY_SPACE:     *outKey = 32;  return;
        case GLFW_KEY_DELETE:    *outKey = 127; return;
        case GLFW_KEY_UP: case GLFW_KEY_DOWN:
        case GLFW_KEY_LEFT: case GLFW_KEY_RIGHT:
        case GLFW_KEY_HOME: case GLFW_KEY_END:
        case GLFW_KEY_PAGE_UP: case GLFW_KEY_PAGE_DOWN:
        case GLFW_KEY_LEFT_SHIFT: case GLFW_KEY_RIGHT_SHIFT:
        case GLFW_KEY_LEFT_CONTROL: case GLFW_KEY_RIGHT_CONTROL:
        case GLFW_KEY_LEFT_ALT: case GLFW_KEY_RIGHT_ALT:
        case GLFW_KEY_LEFT_SUPER: case GLFW_KEY_RIGHT_SUPER:
        case GLFW_KEY_INSERT: case GLFW_KEY_CAPS_LOCK:
        case GLFW_KEY_F1: case GLFW_KEY_F2: case GLFW_KEY_F3:
        case GLFW_KEY_F4: case GLFW_KEY_F5: case GLFW_KEY_F6:
        case GLFW_KEY_F7: case GLFW_KEY_F8: case GLFW_KEY_F9:
        case GLFW_KEY_F10: case GLFW_KEY_F11: case GLFW_KEY_F12:
            *outKey = CODED; return;
        default:
            if (k >= GLFW_KEY_A && k <= GLFW_KEY_Z)
                *outKey = (char16_t)('a' + (k - GLFW_KEY_A));
            else
                *outKey = (char16_t)k;
            return;
    }
}

static void key_cb(GLFWwindow* w, int k, int scancode, int action, int mods) {
    auto* p = PApplet::g_papplet; if(!p) return;
    p->g_currentMods = mods; // capture before callbacks fire
    if (p->_backend) p->_backend->input_key((uint32_t)k, action != GLFW_RELEASE);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (action == GLFW_PRESS) s_pressedScancodes.insert(scancode);
        p->_keyPressed = true;
        if(k>=0&&k<349) p->keys[k]=true;
        { int pk=glfw_to_processing_keycode(k); if(pk>=0&&pk<256) {
            p->keysDown[pk]=true;
            // Also store lowercase so K['w'] and K['W'] both work
            if(pk>='A'&&pk<='Z') p->keysDown[pk+32]=true;
            if(pk>='a'&&pk<='z') p->keysDown[pk-32]=true;
          } }
        p->g_pendingKeyPressed = false; // reset for each new p->key event

        // Translate GLFW code -> Java KeyEvent.VK_* value (Processing reference standard)
        p->keyCode = glfw_to_java_keycode(k);

        // Set p->key=CODED for all non-printable/special keys.
        // ASCII keys that Processing handles via p->key directly (not CODED):
        //   BACKSPACE(8), TAB(9), ENTER(10), ESC(27), DELETE(127)
        // Everything else that has no ASCII representation gets CODED.
        switch (k) {
            case GLFW_KEY_BACKSPACE: p->key = (char16_t)8;   break;
            case GLFW_KEY_TAB:       p->key = (char16_t)9;   break;
            case GLFW_KEY_ENTER:
            case GLFW_KEY_KP_ENTER:  p->key = (char16_t)10;  break;
            case GLFW_KEY_ESCAPE:    p->key = (char16_t)27;  break;
            case GLFW_KEY_SPACE:     p->key = (char16_t)32;  break;
            case GLFW_KEY_DELETE:    p->key = (char16_t)127; break;
            // All other special keys set p->key=CODED
            case GLFW_KEY_UP: case GLFW_KEY_DOWN:
            case GLFW_KEY_LEFT: case GLFW_KEY_RIGHT:
            case GLFW_KEY_HOME: case GLFW_KEY_END:
            case GLFW_KEY_PAGE_UP: case GLFW_KEY_PAGE_DOWN:
            case GLFW_KEY_LEFT_SHIFT: case GLFW_KEY_RIGHT_SHIFT:
            case GLFW_KEY_LEFT_CONTROL: case GLFW_KEY_RIGHT_CONTROL:
            case GLFW_KEY_LEFT_ALT: case GLFW_KEY_RIGHT_ALT:
            case GLFW_KEY_LEFT_SUPER: case GLFW_KEY_RIGHT_SUPER:
            case GLFW_KEY_INSERT: case GLFW_KEY_CAPS_LOCK:
            case GLFW_KEY_F1: case GLFW_KEY_F2: case GLFW_KEY_F3:
            case GLFW_KEY_F4: case GLFW_KEY_F5: case GLFW_KEY_F6:
            case GLFW_KEY_F7: case GLFW_KEY_F8: case GLFW_KEY_F9:
            case GLFW_KEY_F10: case GLFW_KEY_F11: case GLFW_KEY_F12:
                p->key = CODED; // no (char) cast -- key is now int, casting to char would truncate 0xFFFF back down to the original bug
                break;
            default:
                // Printable p->key: if a modifier (Ctrl/Alt) is held, char_cb will
                // NOT fire on Windows for Ctrl+letter combos. Fire immediately.
                // If no modifier, defer to char_cb so p->key gets the correct char.
                if (mods & (GLFW_MOD_CONTROL | GLFW_MOD_ALT | GLFW_MOD_SUPER)) {
                    // Ctrl/Alt/Meta combos: char_cb won't fire.
                    // Set p->key to the lowercase letter and fire immediately.
                    if (k >= GLFW_KEY_A && k <= GLFW_KEY_Z)
                        p->key = (char16_t)('a' + (k - GLFW_KEY_A));
                    p->g_pendingKeyPressed = false; // fire immediately
                } else {
                    // Plain printable p->key (possibly with Shift): defer to char_cb
                    // so p->key gets the correct shifted/layout-aware character.
                    p->g_pendingKeyPressed = true;
                }
                break;
        }

        // Fire keyPressed() now unless deferred to char_cb
        if (!p->g_pendingKeyPressed) {
            if(action==GLFW_PRESS) p->_eventDrewSomething=true;
            if (p->_setupDone) { p->keyPressed(); p->keyPressed(KeyEvent{p->key, p->keyCode, (p->g_currentMods&GLFW_MOD_SHIFT)!=0, (p->g_currentMods&GLFW_MOD_CONTROL)!=0, (p->g_currentMods&GLFW_MOD_ALT)!=0, (p->g_currentMods&GLFW_MOD_SUPER)!=0, KeyEvent::PRESS}); }
            // Java Processing: ESC closes the sketch unless keyPressed() set p->key=0
            if (p->key == (char16_t)27 && p->gWindow)
                glfwSetWindowShouldClose(p->gWindow, GLFW_TRUE);
        }

    } else if (action == GLFW_RELEASE) {
        // Bug B guard: drop phantom releases with no matching press scancode
        if (s_pressedScancodes.find(scancode) == s_pressedScancodes.end()) return;
        s_pressedScancodes.erase(scancode);
        if(k>=0&&k<349) p->keys[k]=false;
        { int pk=glfw_to_processing_keycode(k); if(pk>=0&&pk<256) {
            p->keysDown[pk]=false;
            if(pk>='A'&&pk<='Z') p->keysDown[pk+32]=false;
            if(pk>='a'&&pk<='z') p->keysDown[pk-32]=false;
          } }
        bool anyHeld=false; for(int i=0;i<349;i++) if(p->keys[i]){anyHeld=true;break;}
        p->_keyPressed=anyHeld;
        p->g_currentMods = mods;
        // Fix A: translate key/keyCode atomically from k, not from stale globals
        char16_t relKey; int relKeyCode;
        translate_glfw_key(k, &relKey, &relKeyCode);
        p->key = relKey; p->keyCode = relKeyCode;
        if (p->_setupDone) { p->keyReleased(); p->keyReleased(KeyEvent{p->key, p->keyCode, (p->g_currentMods&GLFW_MOD_SHIFT)!=0, (p->g_currentMods&GLFW_MOD_CONTROL)!=0, (p->g_currentMods&GLFW_MOD_ALT)!=0, (p->g_currentMods&GLFW_MOD_SUPER)!=0, KeyEvent::RELEASE}); }
    }
}
static void focus_cb(GLFWwindow*,int f){
    auto* p = PApplet::g_papplet; if(!p) return;
    p->focused = (f == GLFW_TRUE);
    if (!p->focused) p->g_pendingKeyPressed = false; // clear pending if focus lost
}
static void winpos_cb(GLFWwindow*,int,int){
    auto* p = PApplet::g_papplet; if(!p) return;if(p->_onWindowMoved)p->_onWindowMoved();}
static void winsize_cb(GLFWwindow*,int lw,int lh){
    auto* p = PApplet::g_papplet; if(!p) return;
    if(!lw||!lh||p->_inWinsizeCb)return;
    p->_inWinsizeCb = true;
    {
        int fbw=0, fbh=0;
        if (p->gWindow) glfwGetFramebufferSize(p->gWindow, &fbw, &fbh);
        PDEBUG("winsize_cb FIRED: callback-reported lw=%d lh=%d | isResizable=%d setupDone=%d\n",
               lw, lh, p->isResizable, p->_setupDone);
        PDEBUG("  BEFORE update: logicalW=%d logicalH=%d winWidth=%d winHeight=%d realFbSize=(%d,%d)\n",
               p->logicalW, p->logicalH, p->winWidth, p->winHeight, fbw, fbh);
    }
    if(p->_setupDone){
        if(p->isResizable){
            // Update logical size for resizable sketches so width/height reflect new size
            p->winWidth=lw; p->winHeight=lh;
            p->logicalW=lw; p->logicalH=lh;
        } else {
            PDEBUG("  isResizable=false -- logicalW/logicalH NOT updated despite a real "
                   "window-size-change event (lw=%d lh=%d). If the OS/WM forced an actual "
                   "geometry change anyway (common with tiling WMs like i3, which can "
                   "override an app's requested size), logicalW/H now silently disagree "
                   "with the window's REAL size -- everything drawn using logical "
                   "coordinates will appear scaled/contained relative to the actual "
                   "window, since the viewport tracks real size but the projection still "
                   "uses the stale logical one.\n");
        }
        if(p->_onWindowResized)p->_onWindowResized();
    }
    {
        int fbw=0, fbh=0;
        if (p->gWindow) glfwGetFramebufferSize(p->gWindow, &fbw, &fbh);
        PDEBUG("  AFTER update: logicalW=%d logicalH=%d winWidth=%d winHeight=%d realFbSize=(%d,%d)\n",
               p->logicalW, p->logicalH, p->winWidth, p->winHeight, fbw, fbh);
    }
    // p->setProjection uses p->logicalW/H for ortho, actual size for viewport.
    p->setProjection(p->winWidth, p->winHeight);
    // Only trigger redraw if size actually changed from last draw.
    // This prevents flashing from rapid i3 resize events.
    static int _lastDrawW = 0, _lastDrawH = 0;
    if(p->_setupDone && !p->looping) {
        if(lw != _lastDrawW || lh != _lastDrawH) {
            _lastDrawW = lw; _lastDrawH = lh;
            p->redrawOnce = true;
        }
    }
    p->_inWinsizeCb = false;
}
static void fbsize_cb(GLFWwindow*,int fw,int fh){
    auto* p = PApplet::g_papplet; if(!p) return;
    if(!fw||!fh)return;
    p->pixelWidth=fw;p->pixelHeight=fh;
}

// =============================================================================
// RUN
// =============================================================================


// Called from main() before run() if --debug flag is present
void PApplet::enableDebugConsole(){_doEnableDebugConsole();}

void PApplet::run(){
    PDEBUG("CppMode build stamp: %s\n", PROCESSING_BUILD_STAMP);
    PDEBUG("Debug mode ON -- PApplet::run() starting\n");
    g_papplet = this;
#ifdef _WIN32
    // CRITICAL: call sync_with_stdio before ANY iostream use on Windows.
    // MinGW 14+ has a bug where ios_base::_M_init crashes when the first
    // std::cerr/cout use happens without a console attached (-mwindows).
    // sync_with_stdio(false) bypasses the broken locale init path.
    // DO NOT add any std::cerr/cout calls before PApplet::run() is called.
    ::std::ios_base::sync_with_stdio(false);
    ::std::cin.tie(nullptr);
#endif
    signal(SIGTERM, [](int){ if(PApplet::g_papplet && PApplet::g_papplet->gWindow) glfwSetWindowShouldClose(PApplet::g_papplet->gWindow, GLFW_TRUE); });
#ifndef _WIN32
    // SIGHUP is POSIX-only -- Windows has no controlling-terminal/session
    // concept, so there's no equivalent signal to catch here at all.
    signal(SIGHUP,  [](int){ if(PApplet::g_papplet && PApplet::g_papplet->gWindow) glfwSetWindowShouldClose(PApplet::g_papplet->gWindow, GLFW_TRUE); });
#endif
    // Write directly to a file since -mwindows kills stderr on Windows
    setvbuf(stdout, nullptr, _IONBF, 0);
    ::std::srand((unsigned)::std::time(nullptr));
    initPerlin(0); // initialize noise table with default seed

#if defined(__APPLE__)
    // Make this process a foreground app so GLFW can create windows.
    // Required when launched as a subprocess from the Processing IDE.
    // Uses ObjC runtime directly -- no .m file needed.
    {
        void* appkit = dlopen("/System/Library/Frameworks/AppKit.framework/AppKit", RTLD_LAZY);
        if (appkit) {
            // id NSApp = [NSApplication sharedApplication]
            typedef void* (*id_imp)(void*, void*);
            typedef void  (*void_imp)(void*, void*, long);
            typedef void  (*void_imp2)(void*, void*, bool);
            void* cls = (void*)dlsym(RTLD_DEFAULT, "OBJC_CLASS_$_NSApplication");
            if (cls) {
                void* sel_shared  = sel_registerName("sharedApplication");
                void* sel_policy  = sel_registerName("setActivationPolicy:");
                void* sel_activate = sel_registerName("activateIgnoringOtherApps:");
                id_imp  msgSend   = (id_imp) objc_msgSend;
                void*   app       = msgSend(cls, (void*)sel_shared);
                if (app) {
                    // NSApplicationActivationPolicyRegular = 0
                    ((void_imp)objc_msgSend)(app, (void*)sel_policy, 0L);
                    ((void_imp2)objc_msgSend)(app, (void*)sel_activate, true);
                }
            }
        }
    }
    glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif
    if(!glfwInit()){
        fprintf(stderr, "[ERR] glfwInit() failed. Make sure libglfw3.dll is next to ide.exe\n");
#ifdef _WIN32
        MessageBoxA(NULL, "glfwInit() failed.\nMake sure libglfw3.dll and glew32.dll are next to ide.exe", "processing-cpp Error", MB_OK|MB_ICONERROR);
#endif
        return;
    }
    GLFWmonitor* mon=glfwGetPrimaryMonitor();
    if(mon){const GLFWvidmode* vm=glfwGetVideoMode(mon);displayWidth=vm->width;displayHeight=vm->height;}

    // Reset all style state to defaults BEFORE this->setup()
    doFill=true;doStroke=true;
    fillR=1;fillG=1;fillB=1;fillA=1;
    strokeR=0;strokeG=0;strokeB=0;strokeA=1;strokeW=1;
    colorModeVal=RGB;
    colorMaxH=255;colorMaxS=255;colorMaxB=255;colorMaxA=255;
    currentRectMode=CORNER;currentEllipseMode=CENTER;currentImageMode=CORNER;
    doTint=false;tintR=1;tintG=1;tintB=1;tintA=1;
    lightsEnabled=false;lightIndex=0;
    looping=true;
    bgR=0.8f;bgG=0.8f;bgB=0.8f;bgA=1.0f;
    bgR=0.8f;bgG=0.8f;bgB=0.8f;bgA=1.0f; // Processing default gray

    this->settings();

#ifdef CPPMODE_BACKEND_LIBPROCESSING
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#else
    glfwWindowHint(GLFW_SAMPLES,4);
    glfwWindowHint(GLFW_STENCIL_BITS,8);
#endif
    glfwWindowHint(GLFW_RESIZABLE,isResizable?GLFW_TRUE:GLFW_FALSE);
    glfwWindowHint(GLFW_STENCIL_BITS,8);
    // cppmode-fscreate: fullScreen() in settings() creates the window on the monitor directly, so
    // tiling window managers (i3, sway) never tile a normal window first and then resize it.
    {
        GLFWmonitor* fsMon = (g_fullScreenPending && g_fsDisplay >= 0) ? cppmodeFsMonitor() : nullptr;  // cppmode-fsdisplay
        const GLFWvidmode* fsMode = fsMon ? glfwGetVideoMode(fsMon) : nullptr;
        if (fsMode) {
            glfwWindowHint(GLFW_REFRESH_RATE, fsMode->refreshRate);
            gWindow = glfwCreateWindow(fsMode->width, fsMode->height, g_sketchName.c_str(), fsMon, nullptr);
            g_fullScreenPending = false;
        } else {
            gWindow = glfwCreateWindow(winWidth, winHeight, g_sketchName.c_str(), nullptr, nullptr);
        }
    }
    if(!gWindow){
        const char* err="unknown"; (void)err;
        fprintf(stderr,"[ERR] glfwCreateWindow() failed: %s\n", err?err:"unknown");
#ifdef _WIN32
        MessageBoxA(NULL,
            "Failed to create OpenGL window.\n\n"
            "This usually means:\n"
            "  - Your GPU driver does not support OpenGL 3.3\n"
            "  - glfw3.dll or glew32.dll is missing next to the sketch exe\n\n"
            "Try updating your GPU drivers.\n"
            "Intel: https://www.intel.com/content/www/us/en/download-center/home.html\n"
            "AMD:   https://www.amd.com/en/support\n"
            "NVIDIA: https://www.nvidia.com/drivers",
            "processing-cpp: Window Creation Failed", MB_OK|MB_ICONERROR);
#endif
        glfwTerminate();
        return;
    }
    if (g_fullScreenPending) { g_fullScreenPending = false; cppmodeGoFullScreen(gWindow); }  // fullScreen() in settings()
    // Prevent freeze when dragging title bar on Windows
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    {
        _backend = new LibProcessingBackend();
#if defined(_WIN32)
        _backend->init((uint64_t)(uintptr_t)glfwGetWin32Window(gWindow), 0,
                       (uint32_t)winWidth,(uint32_t)winHeight,1.0f);
#elif defined(__APPLE__)
        _backend->init((uint64_t)(uintptr_t)glfwGetCocoaWindow(gWindow), 0,
                       (uint32_t)winWidth,(uint32_t)winHeight,1.0f);
#else
        const char* wl = getenv("WAYLAND_DISPLAY");
        if (wl && wl[0]) {
            _backend->init((uint64_t)glfwGetWaylandWindow(gWindow),
                           (uint64_t)glfwGetWaylandDisplay(),
                           (uint32_t)winWidth,(uint32_t)winHeight,1.0f);
        } else {
            _backend->init((uint64_t)glfwGetX11Window(gWindow),
                           (uint64_t)(uintptr_t)glfwGetX11Display(),
                           (uint32_t)winWidth,(uint32_t)winHeight,1.0f);
        }
#endif
        // size() ran before the backend existed; apply P3D's camera now,
        // before setup() so a camera() call there still wins
        if (defaultP3D) {
            processing_mode_3d(_backend->graphics_id());
            camera();  // Java's default: eye at (width/2, height/2, cameraZ) looking at the middle
            perspective();  // Java's default projection; Processing's y points down
        }
        if (g_lpMsaaWanted >= 0)  // smooth()/noSmooth() called before the window existed
            processing_graphics_set_msaa(_backend->graphics_id(), (uint32_t)g_lpMsaaWanted);
    }
#endif
    glfwSetWindowRefreshCallback(gWindow,[](GLFWwindow* w){
        // Originally just glClear()+swap "to prevent freeze when dragging the
        // title bar on Windows" -- but this unconditionally WIPES the screen
        // on every OS/WM-triggered refresh event (resize, focus change,
        // workspace switch, tiling re-layout, etc.), with no attempt to
        // redraw actual sketch content. On tiling WMs like i3 that send
        // refresh events frequently, this is what made noLoop() sketches
        // (which never swap again after their one real draw) appear to
        // "show the image, then go blank a few frames later" -- the WM's
        // very next refresh event was clearing it out from under them.
        // Restoring from the persist FBO (the last frame we know is
        // correct) instead of blank-clearing keeps the window responsive
        // during the same blocking scenarios (e.g. title-bar drag) while
        // never destroying real content.
        auto* p = PApplet::g_papplet;
        if (p && p->persistFBO) {
            p->restoreFromPersist();
        } else {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        }
        glfwSwapBuffers(w);
    });
#ifndef CPPMODE_BACKEND_LIBPROCESSING
    glfwMakeContextCurrent(gWindow);
    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    bool glewOk = (glewErr == GLEW_OK);
#if defined(__linux__) && defined(GLEW_ERROR_NO_GLX_DISPLAY)
    // A GLX-built GLEW reports NO_GLX_DISPLAY under native Wayland (GLFW uses EGL there),
    // but the core GL functions are already loaded by then, so it's safe to carry on.
    if (!glewOk && glewErr == GLEW_ERROR_NO_GLX_DISPLAY && getenv("WAYLAND_DISPLAY") != nullptr)
        glewOk = true;
#endif
    if(!glewOk){

#ifdef _WIN32
        char msg[256]; snprintf(msg,sizeof(msg),"glewInit() failed: %s", glewGetErrorString(glewErr));
        MessageBoxA(NULL, msg, "processing-cpp Error", MB_OK|MB_ICONERROR);
#else
        fprintf(stderr, "[ERR] glewInit() failed: %s\n", glewGetErrorString(glewErr));
#endif
        glfwDestroyWindow(gWindow); glfwTerminate(); return;
    }
#endif

#ifndef CPPMODE_BACKEND_LIBPROCESSING
    // Reset cached Phong shader so it recompiles fresh with current source
    if(phongProg){glDeleteProgram(phongProg);phongProg=0;}
    glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    // Don't call setProjection here -- let size() in this->setup() do it
    // with the correct dimensions. Calling it now with winWidth=640,winHeight=480
    // (defaults) would set the wrong ortho before this->setup() changes the size.
    {int fw,fh;glfwGetFramebufferSize(gWindow,&fw,&fh);pixelWidth=fw;pixelHeight=fh;fbW=fw>0?fw:logicalW;fbH=fh>0?fh:logicalH;}

    // Enable sticky keys/buttons: GLFW will keep state as PRESSED until polled,
    // Clear both buffers once at startup so the sketch starts with
    // a known clean state (no GPU garbage in either buffer).
    glClearColor(0.8f,0.8f,0.8f,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(gWindow);
    // preventing missed inputs on Windows where events can arrive between polls.
    glfwSetInputMode(gWindow, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(gWindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
#endif
    glfwSetCursorPosCallback(gWindow,   cursor_pos_cb);
    glfwSetMouseButtonCallback(gWindow, mouse_btn_cb);
    glfwSetScrollCallback(gWindow,      scroll_cb);
    glfwSetKeyCallback(gWindow,         key_cb);
    glfwSetCharCallback(gWindow,        char_cb);
    glfwSetFramebufferSizeCallback(gWindow,fbsize_cb);
    glfwSetWindowSizeCallback(gWindow,  winsize_cb);
    glfwSetWindowFocusCallback(gWindow, focus_cb);
    glfwSetWindowPosCallback(gWindow,   winpos_cb);
    focused=(glfwGetWindowAttrib(gWindow,GLFW_FOCUSED)==GLFW_TRUE);

    // Auto-load default.ttf from project root as the default font
    // (matches Processing Java's "a generic sans-serif font will be used")
    // Try to load default.ttf from several common locations
    {
        ::std::string _homeDir;
#ifdef _WIN32
        _homeDir = processingEnvironmentVariable("USERPROFILE");
#else
        _homeDir = processingEnvironmentVariable("HOME");
#endif
        ::std::string _modePath;
        _modePath = processingEnvironmentVariable("PROCESSING_MODE_PATH");
        if (!_modePath.empty()) _modePath += "/";

        // Font name used by Processing4
        const ::std::string _font = "ProcessingSansPro-Regular.ttf";

        // Check Documents/Processing on Windows (user sketchbook)
        ::std::string _docsPath;
#ifdef _WIN32
        ::std::string userProfile = processingEnvironmentVariable("USERPROFILE");
        if (!userProfile.empty()) _docsPath = userProfile + "/Documents/Processing/";
#endif

        if (!tryLoadTTF("fonts/" + _font, g_textSize) &&
            !tryLoadTTF(_modePath + "fonts/" + _font, g_textSize) &&
            !tryLoadTTF(_modePath + _font, g_textSize) &&
            !tryLoadTTF(_docsPath + "modes/CppMode/fonts/" + _font, g_textSize) &&
            // Windows: Processing4 install locations
            !tryLoadTTF("C:/Program Files/Processing/core/library/" + _font, g_textSize) &&
            !tryLoadTTF("C:/Program Files (x86)/Processing/core/library/" + _font, g_textSize) &&
            !tryLoadTTF("C:/Program Files/processing-4.3/core/library/" + _font, g_textSize) &&
            !tryLoadTTF(_homeDir + "/AppData/Local/Programs/Processing/core/library/" + _font, g_textSize) &&
            // Windows: dev build
            !tryLoadTTF(_homeDir + "/Projects/processing4/core/src/font/" + _font, g_textSize) &&
            // Linux: installed Processing4
            !tryLoadTTF("/usr/lib/processing4/core/library/" + _font, g_textSize) &&
            !tryLoadTTF("/usr/share/processing4/core/library/" + _font, g_textSize) &&
            !tryLoadTTF("/opt/processing4/core/library/" + _font, g_textSize) &&
            !tryLoadTTF("/opt/processing/core/library/" + _font, g_textSize) &&
            !tryLoadTTF(_homeDir + "/.local/share/processing4/core/library/" + _font, g_textSize) &&
            !tryLoadTTF(_homeDir + "/processing-4.3/core/library/" + _font, g_textSize) &&
            !tryLoadTTF(_homeDir + "/Projects/processing4/core/src/font/" + _font, g_textSize) &&
            // macOS
            !tryLoadTTF("/Applications/Processing.app/Contents/Java/core/library/" + _font, g_textSize) &&
            !tryLoadTTF(_homeDir + "/Applications/Processing.app/Contents/Java/core/library/" + _font, g_textSize)) {
            ::std::cerr << "[font] ProcessingSansPro-Regular.ttf not found -- using bitmap fallback\n";
        }
    }

    glfwFocusWindow(gWindow);

    // Settle loop: poll+swap several times BEFORE this->setup() runs so i3/tiling WMs
    // fully resize the window first. winsize_cb fires during these polls and
    // sets the correct viewport. After settling, this->setup() and this->draw() use the
    // correct dimensions from the start -- no shifted first frame.
    // Initialize the framebuffer
    glClearColor(0.8f,0.8f,0.8f,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(gWindow);
#ifndef _WIN32
    // Settle loop for tiling WMs (i3 etc) -- not needed on Windows
    for (int _settle = 0; _settle < 5; _settle++) {
        glClearColor(0.8f,0.8f,0.8f,1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        glfwSwapBuffers(gWindow);
    }
#endif
    _setupDone = true; // enable winsize_cb to update projection after settle
    if (!defaultP3D) setProjection(winWidth, winHeight);

    this->setup();
    // Real Processing's actual default is smooth(2) -- antialiased --
    // even for plain 2D sketches (P2D/P3D both default to smooth(2); the
    // legacy JAVA2D software renderer defaults to smooth(3)/bicubic).
    // This previously UNCONDITIONALLY disabled MSAA for every 2D sketch,
    // on the assumption that 2D wants "crisp" pixels by default -- that's
    // backwards from real Processing's actual behavior, and it's why
    // PGraphics buffers (and the main canvas) never looked antialiased
    // even after wiring up multisample framebuffers: GL_MULTISAMPLE was
    // globally disabled the whole time regardless of framebuffer setup.
    // Only disable smoothing if the sketch explicitly called noSmooth()
    // (smoothing==false) BEFORE this point -- otherwise leave the
    // defaults smooth()/setup() already enabled (smooth() runs earlier in
    // PApplet::run(), before this->setup(), as part of normal startup).
    if (!defaultP3D && !smoothing) {
        glDisable(GL_MULTISAMPLE);
        glDisable(GL_POINT_SMOOTH);
        glDisable(GL_LINE_SMOOTH);
    }
    if (_wireCallbacksFn) _wireCallbacksFn();

    if (!defaultP3D) {
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    }

    // If this->setup() called noLoop(): run this->draw() once then swap.
    // Static sketches have an empty this->draw() -- no-op.
    // Structured noLoop sketches (like Pie Chart) need this->draw() to show content.
    if (!looping) {
        if (defaultP3D) {
            // P3D static sketch: this->setup() already drew everything with the correct
            // camera. Just apply projection state for the this->draw() call (which may
            // be empty for static sketches, or may draw content for structured ones).
            glViewport(0, 0, fbW, fbH);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glDisable(GL_CULL_FACE);
            glFrontFace(GL_CW);
                    // Do NOT clear -- this->setup() already drew to the back buffer.
            // Only clear if this->draw() will redraw everything (i.e. calls background()).
            applyDefaultCamera();
        } else {
            setProjection(logicalW, logicalH);
        }
        ++frameCount; { /* cppmode-measured-fps */ static auto _cppT = std::chrono::steady_clock::now(); auto _cppN = std::chrono::steady_clock::now(); double _cppD = std::chrono::duration<double>(_cppN - _cppT).count(); _cppT = _cppN; if (frameCount > 1 && _cppD > 0) { float _cppR = (float)(1.0 / _cppD); _frameRate = frameCount == 2 ? _cppR : _frameRate * 0.9f + _cppR * 0.1f; } }; this->draw();
                mouseDX = 0; mouseDY = 0;  // reset delta each frame
        flushPoints(); // flush any pending points before swap
        saveToPersist(); // save back buffer before swap for next frame restore
        glfwSwapBuffers(gWindow);
        // For noLoop() sketches, this is the ONLY draw that will ever happen --
        // the main loop below never swaps again (looping||redrawOnce stays
        // false forever after this point). With double buffering, a single
        // swap leaves the correct image in only ONE of the two buffers; the
        // other buffer still holds stale content from the settle loop above.
        // If anything outside our control (window manager/compositor on
        // tiling WMs like i3, focus changes, etc.) forces an implicit buffer
        // flip later, that stale buffer becomes visible and the sketch
        // appears to "go blank" after a few frames. Restoring from the
        // persist FBO we just saved and swapping again primes BOTH buffers
        // with the correct content, so no swap-parity assumption is needed.
        restoreFromPersist();
        glfwSwapBuffers(gWindow);
    }

    redrawOnce = looping;
    auto last=::std::chrono::steady_clock::now();
#ifndef __EMSCRIPTEN__
    // Drain Windows message queue before entering main loop.
    // On Windows, a WM_QUIT from a previous sketch run can be in the queue.
    // Poll multiple times to flush it, then forcibly clear the close flag.
    for(int _flush=0; _flush<10; _flush++) glfwPollEvents();
    glfwSetWindowShouldClose(gWindow, GLFW_FALSE);
    if (g_exitRequested) glfwSetWindowShouldClose(gWindow, GLFW_TRUE);  // exit() in setup()
    // For looping sketches: clear both buffers to bgColor so the
    // front-to-back blit starts from the correct background color.
    // For static sketches (noLoop): don't clear - preserve what this->setup() drew.
    if(looping){
        for(int _b=0;_b<2;_b++){
            glClearColor(bgR,bgG,bgB,bgA);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            glfwSwapBuffers(gWindow);
        }
    }
#endif
#ifndef __EMSCRIPTEN__
    while(!glfwWindowShouldClose(gWindow)&&(!_backend||!_backend->should_close())){

        if(looping||redrawOnce){
            redrawOnce=false;

            if(defaultP3D){
                glViewport(0, 0, fbW, fbH);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                glDisable(GL_CULL_FACE);
                glFrontFace(GL_CW);
                            // Do NOT clear color here -- the sketch calls background() itself.
                // Clearing color would erase anything drawn in this->setup().
                flushPoints(); // flush any pending points from previous frame
            glClear(GL_DEPTH_BUFFER_BIT);
                // Auto-apply the default Processing camera BEFORE this->draw() --
                // matches Java Processing behaviour exactly. The sketch can
                // override by calling camera() / perspective() itself.
                // This means lights() called anywhere in this->draw() will always
                // see the camera matrix and lock correctly in eye space.
                applyDefaultCamera();
            } else {
                // 2D mode: reset viewport and projection every frame.
                // Use actual framebuffer size for viewport (handles HiDPI/Retina
                // where framebuffer may be 2x logical size) but logical
                // winWidth/winHeight for the ortho matrix so sketch pixel
                // coordinates map 1:1 regardless of DPI scaling.
                //
                // BUG FIX: glLoadIdentity() alone only resets the CONTENT of
                // whatever's currently on top of the matrix stack -- it does
                // nothing about the stack's DEPTH. If a previous frame left
                // an unbalanced glPushMatrix() somewhere (e.g. a PGraphics
                // buffer's beginDraw() that was abandoned without a matching
                // endDraw(), which can happen if a sketch reassigns a
                // PGraphics* pointer to a new buffer without ever calling
                // endDraw()/delete on the old one), every subsequent
                // glLoadIdentity() still operates one level too deep on the
                // stack, and main-canvas drawing for the REST OF THE PROGRAM
                // silently inherits that leftover transform -- exactly
                // matching "the same line(...) call lands in a different
                // place depending on what ran before it." Fully unwinding
                // the stack to its base depth every frame, before doing
                // anything else, makes the main render loop resilient to
                // ANY code (ours or the sketch's) that fails to balance its
                // own push/pop calls, rather than silently accumulating
                // corruption frame after frame.
                {
                    // Pop down to stack depth 1 (the base level) rather
                    // than a blind fixed-count loop -- popping past the
                    // bottom of the stack is ALSO undefined behavior,
                    // exactly the same category of bug as overflowing it,
                    // so we query the actual current depth first.
                    GLint depth = 0;
                    glMatrixMode(GL_PROJECTION);
                    glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &depth);
                    while (depth > 1) { glPopMatrix(); depth--; }
                    glLoadIdentity();

                    glMatrixMode(GL_MODELVIEW);
                    glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &depth);
                    while (depth > 1) { glPopMatrix(); depth--; }
                    glLoadIdentity();
                }

                glViewport(0, 0, fbW, fbH);
                glMatrixMode(GL_PROJECTION); glLoadIdentity();
                glOrtho(0, logicalW, logicalH, 0, -1, 1);
                glMatrixMode(GL_MODELVIEW); glLoadIdentity();
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_LIGHTING);
                // Restore previous frame content from persist FBO.
                // Sketches that call background() will overwrite this.
                // Sketches that don't (like Continuous Lines) preserve their drawn content.
                if(!_backgroundCalledThisFrame) restoreFromPersist();
                glClear(GL_DEPTH_BUFFER_BIT);
            }

            // Reset all light slots each frame so lights from the previous
            // frame don't accumulate. The sketch re-establishes its lights
            // in each this->draw() call.
            for (int _li = 0; _li < 8; _li++) {
                glDisable(GL_LIGHT0 + _li);
                // Reset each slot to black so disabled lights contribute nothing
                GLfloat black[] = { 0.f, 0.f, 0.f, 1.f };
                glLightfv(GL_LIGHT0+_li, GL_DIFFUSE,  black);
                glLightfv(GL_LIGHT0+_li, GL_AMBIENT,  black);
                glLightfv(GL_LIGHT0+_li, GL_SPECULAR, black);
            }
            glDisable(GL_LIGHTING);
            glDisable(GL_COLOR_MATERIAL);
            // No global ambient by default (Java Processing uses 0 unless ambientLight() called)
            GLfloat noAmb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noAmb);
            lightsEnabled = false;
            lightIndex    = 0;
            pendingSpecR=0; pendingSpecG=0; pendingSpecB=0;
            for(int _li=0;_li<8;_li++){lightConcentration[_li]=0;lightCutoffCos[_li]=-1;}
            // Reset material to neutral so non-lit objects look correct
            GLfloat matWhite[] = { 0.8f, 0.8f, 0.8f, 1.0f };
            GLfloat matBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };
#ifndef __EMSCRIPTEN__
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matBlack);
#endif
#ifndef __EMSCRIPTEN__
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matWhite);
#endif
#ifndef __EMSCRIPTEN__
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matBlack);
#endif
#ifndef __EMSCRIPTEN__
            glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
#endif
            // Reset specular material to none
            GLfloat noSpec[] = {0,0,0,1};
#ifndef __EMSCRIPTEN__
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, noSpec);
#endif
#ifndef __EMSCRIPTEN__
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noSpec);
#endif

            // Draw guard: handle different sketch types
            if (!looping && _staticSketchSetup) {
                // Static sketch: reset style and redraw
                doFill=true; doStroke=true;
                fillR=1; fillG=1; fillB=1; fillA=1;
                strokeR=0; strokeG=0; strokeB=0; strokeA=1; strokeW=1;
                colorModeVal=RGB;
                colorMaxH=255; colorMaxS=255; colorMaxB=255; colorMaxA=255;
                lightsEnabled=false; lightIndex=0;
                _staticSketchSetup();
            } else {
                // Run this->draw() every frame -- no mouseInWindow guard.
                // The Hue sketch first-frame issue is handled by the settle loop
                // ensuring mouseX/mouseY are 0 which is acceptable.
                // Reset tint state each frame (Processing Java behavior)
                doTint=false; tintR=1; tintG=1; tintB=1; tintA=1;
                glGetError();
                _backgroundCalledThisFrame = false;
                if (_backend) { lpLightsReset(_backend->graphics_id()); lpFrameGeosFree(); _backend->begin_draw(); }
                ++frameCount; { /* cppmode-measured-fps */ static auto _cppT = std::chrono::steady_clock::now(); auto _cppN = std::chrono::steady_clock::now(); double _cppD = std::chrono::duration<double>(_cppN - _cppT).count(); _cppT = _cppN; if (frameCount > 1 && _cppD > 0) { float _cppR = (float)(1.0 / _cppD); _frameRate = frameCount == 2 ? _cppR : _frameRate * 0.9f + _cppR * 0.1f; } }; this->draw(); pmouseX=mouseX; pmouseY=mouseY; mouseDX=0; mouseDY=0;
                if (_backend) _backend->end_draw();
                else glGetError();
            }

            // Only save persist FBO if sketch uses persistence (no background() call)
            if(!_backgroundCalledThisFrame) saveToPersist();
            glfwSwapBuffers(gWindow);
        }
        glfwPollEvents();
        if (PApplet::g_papplet && PApplet::g_papplet->_backend)
            PApplet::g_papplet->_backend->input_flush();
        cppmodeDeliverSelections();  // cppmode-select: finished file dialogs call their callback here
        // end_draw() calls app.update() -- no separate poll_events() needed
        // Only re-save persist if an event callback drew something.
        // Checked via _eventDrewSomething flag set in keyPressed/mousePressed.
        if(persistFBO && _eventDrewSomething) { saveToPersist(); _eventDrewSomething=false; }

        // Frame timing -- matches Processing Java's model exactly:
        // sleep the remainder of the target frame time, then measure
        // the full wall-clock frame duration (draw + swap + sleep) and
        // report it as _targetFrameRate.
        auto now = ::std::chrono::steady_clock::now();
        double elapsed = ::std::chrono::duration<double>(now - last).count();
        double sl = targetFrameTime - elapsed;
        if (sl > 0) {
            #ifdef _WIN32
            Sleep((DWORD)(sl * 1000.0));
            #else
            ::std::this_thread::sleep_for(::std::chrono::duration<double>(sl));
            #endif
        }
        auto frameEnd = ::std::chrono::steady_clock::now();
        double fullFrame = ::std::chrono::duration<double>(frameEnd - last).count();
        if (fullFrame > 0) _targetFrameRate = (float)(1.0 / fullFrame);
        deltaTime = (float)fullFrame;
        last = frameEnd;
    }
    if(phongProg){glDeleteProgram(phongProg);phongProg=0;}
    if (_backend) { delete _backend; _backend = nullptr; }
    if(gWindow){glfwDestroyWindow(gWindow);gWindow=nullptr;}
    glfwTerminate();
#else
    emscripten_set_main_loop_arg([](void* arg){
        auto* p = static_cast<PApplet*>(arg);
        if(p->looping||p->redrawOnce){
            p->redrawOnce=false;
            if(p->defaultP3D){
                glViewport(0, 0, p->winWidth, p->winHeight);
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                glDisable(GL_CULL_FACE);
                glFrontFace(GL_CW);
                            glClear(GL_DEPTH_BUFFER_BIT);
                p->applyDefaultCamera();
            } else {
                glViewport(0, 0, p->winWidth, p->winHeight);
                glMatrixMode(GL_PROJECTION); glLoadIdentity();
                glOrtho(0, p->logicalW, p->logicalH, 0, -1, 1);
                glMatrixMode(GL_MODELVIEW); glLoadIdentity();
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_LIGHTING);
            }
            // reset lights each frame
            for(int _li=0;_li<8;_li++) glDisable(GL_LIGHT0+_li);
            glDisable(GL_LIGHTING);
            glDisable(GL_COLOR_MATERIAL);
            p->lightsEnabled=false; p->lightIndex=0;
            p->_backgroundCalledThisFrame=false;
            ++p->frameCount; { /* cppmode-measured-fps */ static auto _cppT = std::chrono::steady_clock::now(); auto _cppN = std::chrono::steady_clock::now(); double _cppD = std::chrono::duration<double>(_cppN - _cppT).count(); _cppT = _cppN; if (p->frameCount > 1 && _cppD > 0) { float _cppR = (float)(1.0 / _cppD); p->_frameRate = p->frameCount == 2 ? _cppR : p->_frameRate * 0.9f + _cppR * 0.1f; } };
            p->draw();
            p->pmouseX=p->mouseX; p->pmouseY=p->mouseY;
            p->mouseDX=0; p->mouseDY=0;
        }
    }, this, 0, 1);
#endif
}

// =============================================================================
// PGRAPHICS EXTENDED METHOD IMPLEMENTATIONS
// All methods call through to PApplet after beginDraw() has set up context.
// =============================================================================

void PGraphics::stroke(float g, float a)               { if(PApplet::g_papplet) PApplet::g_papplet->stroke(g,a); }
void PGraphics::stroke(float r, float g, float b, float a){ if(PApplet::g_papplet) PApplet::g_papplet->stroke(r,g,b,a); }
void PGraphics::stroke(color c)                        { if(PApplet::g_papplet) PApplet::g_papplet->stroke(c); }
void PGraphics::fill(float g, float a)                 { if(PApplet::g_papplet) PApplet::g_papplet->fill(g,a); }
void PGraphics::fill(color c)                          { if(PApplet::g_papplet) PApplet::g_papplet->fill(c); }
void PGraphics::background(color c)                    { if(PApplet::g_papplet) PApplet::g_papplet->background(c); }
void PGraphics::beginShape(int kind)                   { if(PApplet::g_papplet) PApplet::g_papplet->beginShape(kind); }
void PGraphics::vertex(float x, float y, float z)     { if(PApplet::g_papplet) PApplet::g_papplet->vertex(x,y,z); }
void PGraphics::camera()                               { if(PApplet::g_papplet) PApplet::g_papplet->camera(); }
void PGraphics::camera(float ex,float ey,float ez,float cx,float cy,float cz,float ux,float uy,float uz)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->camera(ex,ey,ez,cx,cy,cz,ux,uy,uz); }
void PGraphics::perspective()                          { if(PApplet::g_papplet) PApplet::g_papplet->perspective(); }
void PGraphics::perspective(float fov,float aspect,float zNear,float zFar)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->perspective(fov,aspect,zNear,zFar); }
void PGraphics::ortho()                                { if(PApplet::g_papplet) PApplet::g_papplet->ortho(); }
void PGraphics::ortho(float l,float r,float b,float t,float n,float f)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->ortho(l,r,b,t,n,f); }
void PGraphics::bezier(float x1,float y1,float cx1,float cy1,float cx2,float cy2,float x2,float y2)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->bezier(x1,y1,cx1,cy1,cx2,cy2,x2,y2); }
void PGraphics::curve(float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->curve(x0,y0,x1,y1,x2,y2,x3,y3); }
void PGraphics::bezierVertex(float cx1,float cy1,float cx2,float cy2,float x,float y)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->bezierVertex(cx1,cy1,cx2,cy2,x,y); }
void PGraphics::curveVertex(float x, float y)          { if(PApplet::g_papplet) PApplet::g_papplet->curveVertex(x,y); }
void PGraphics::image(PImage* img,float x,float y)    { if(PApplet::g_papplet) PApplet::g_papplet->image(img,x,y); }
void PGraphics::image(PImage* img,float x,float y,float w,float h)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->image(img,x,y,w,h); }
void PGraphics::tint(float gray)                       { if(PApplet::g_papplet) PApplet::g_papplet->tint(gray); }
void PGraphics::tint(float gray,float a)               { if(PApplet::g_papplet) PApplet::g_papplet->tint(gray,a); }
void PGraphics::tint(float r,float g,float b,float a)  { if(PApplet::g_papplet) PApplet::g_papplet->tint(r,g,b,a); }
void PGraphics::noTint()                               { if(PApplet::g_papplet) PApplet::g_papplet->noTint(); }
void PGraphics::colorMode(int mode,float mx)           { if(PApplet::g_papplet) PApplet::g_papplet->colorMode(mode,mx); }
void PGraphics::colorMode(int mode,float mH,float mS,float mB,float mA)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->colorMode(mode,mH,mS,mB,mA); }
void PGraphics::textLeading(float v)                   { if(PApplet::g_papplet) PApplet::g_papplet->textLeading(v); }
float PGraphics::textWidth(const ::std::string& s)       { return PApplet::g_papplet ? PApplet::g_papplet->textWidth(s) : 0; }
void PGraphics::push()                                 { if(PApplet::g_papplet) PApplet::g_papplet->push(); }
void PGraphics::pop()                                  { if(PApplet::g_papplet) PApplet::g_papplet->pop(); }
void PGraphics::scale(float sx,float sy)               { if(PApplet::g_papplet) PApplet::g_papplet->scale(sx,sy); }
void PGraphics::rect(float x,float y,float w,float h,float r)                     { if(PApplet::g_papplet) PApplet::g_papplet->rect(x,y,w,h,r); }
void PGraphics::resetMatrix()                          { if(PApplet::g_papplet) PApplet::g_papplet->resetMatrix(); }
void PGraphics::shearX(float a)                        { if(PApplet::g_papplet) PApplet::g_papplet->shearX(a); }
void PGraphics::shearY(float a)                        { if(PApplet::g_papplet) PApplet::g_papplet->shearY(a); }
void PGraphics::normal(float nx,float ny,float nz)     { if(PApplet::g_papplet) PApplet::g_papplet->normal(nx,ny,nz); }
void PGraphics::shininess(float s)                     { if(PApplet::g_papplet) PApplet::g_papplet->shininess(s); }
void PGraphics::specular(float r,float g,float b)      { if(PApplet::g_papplet) PApplet::g_papplet->specular(r,g,b); }
void PGraphics::emissive(float r,float g,float b)      { if(PApplet::g_papplet) PApplet::g_papplet->emissive(r,g,b); }
void PGraphics::ambient(float r,float g,float b)       { if(PApplet::g_papplet) PApplet::g_papplet->ambient(r,g,b); }
void PGraphics::rectMode(int m)                        { if(PApplet::g_papplet) PApplet::g_papplet->rectMode(m); }
void PGraphics::ellipseMode(int m)                     { if(PApplet::g_papplet) PApplet::g_papplet->ellipseMode(m); }
void PGraphics::imageMode(int m)                       { if(PApplet::g_papplet) PApplet::g_papplet->imageMode(m); }
void PGraphics::noSmooth()                             { if(PApplet::g_papplet) PApplet::g_papplet->noSmooth(); }
void PGraphics::smooth()                               { if(PApplet::g_papplet) PApplet::g_papplet->smooth(); }
void PGraphics::circle(float x,float y,float d)        { if(PApplet::g_papplet) PApplet::g_papplet->circle(x,y,d); }
void PGraphics::square(float x,float y,float s)        { if(PApplet::g_papplet) PApplet::g_papplet->square(x,y,s); }
void PGraphics::quad(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->quad(x1,y1,x2,y2,x3,y3,x4,y4); }
void PGraphics::arc(float cx,float cy,float w,float h,float sa,float ea)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->arc(cx,cy,w,h,sa,ea); }
void PGraphics::arc(float cx,float cy,float w,float h,float sa,float ea,int mode)
                                                       { if(PApplet::g_papplet) PApplet::g_papplet->arc(cx,cy,w,h,sa,ea,mode); }
void PGraphics::blendMode(int mode)                    { if(PApplet::g_papplet) PApplet::g_papplet->blendMode(mode); }
void PGraphics::clip(float x,float y,float w,float h)  { if(PApplet::g_papplet) PApplet::g_papplet->clip(x,y,w,h); }
void PGraphics::noClip()                               { if(PApplet::g_papplet) PApplet::g_papplet->noClip(); }
// cppmode-pgpixels: in OpenGL these read and write the buffer's own framebuffer. They used
// to go to the main canvas, so pg.loadPixels()/pg.get() returned the window's pixels.
// libprocessing keeps the old routing (its beginDraw() swaps the canvas).
static bool pgUseGl(PGraphics* g) { return PApplet::g_papplet && !PApplet::g_papplet->_backend && g->fbo && g->width > 0 && g->height > 0; }
struct PgFboScope {
    GLint prev = 0;
    explicit PgFboScope(GLuint fbo) { glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev); glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
    ~PgFboScope() { glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)prev); }
};
static unsigned int pgArgb(const unsigned char* p) { return ((unsigned)p[3] << 24) | ((unsigned)p[0] << 16) | ((unsigned)p[1] << 8) | p[2]; }
void PGraphics::loadPixels() {
    if (!pgUseGl(this)) { if(PApplet::g_papplet) PApplet::g_papplet->loadPixels(); return; }
    ::std::vector<unsigned char> b((size_t)width * height * 4);
    { PgFboScope s(fbo); glPixelStorei(GL_PACK_ALIGNMENT, 1); glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, b.data()); }
    pixels.resize((size_t)width * height);
    for (size_t i = 0; i < (size_t)width * height; i++)   // the buffer is drawn y-down, so row 0 is the top
        pixels[i] = pgArgb(&b[i * 4]);
}
void PGraphics::updatePixels() {
    if (!pgUseGl(this)) { if(PApplet::g_papplet) PApplet::g_papplet->updatePixels(); return; }
    if ((int)pixels.size() < width * height) return;
    ::std::vector<unsigned char> b((size_t)width * height * 4);
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            unsigned int v = pixels[(size_t)y * width + x]; unsigned char* p = &b[((size_t)y * width + x) * 4];
            p[0] = (v >> 16) & 255; p[1] = (v >> 8) & 255; p[2] = v & 255; p[3] = (v >> 24) & 255;
        }
    PgFboScope s(fbo);
    GLboolean bl = glIsEnabled(GL_BLEND), dt = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_BLEND); glDisable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); glWindowPos2i(0, 0);
    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, b.data());
    if (bl) glEnable(GL_BLEND); if (dt) glEnable(GL_DEPTH_TEST);
}
color PGraphics::get(int x,int y) {
    if (!pgUseGl(this)) return PApplet::g_papplet ? PApplet::g_papplet->get(x,y) : color(0);
    if (x < 0 || y < 0 || x >= width || y >= height) return color(0);
    unsigned char p[4] = {0, 0, 0, 0};
    { PgFboScope s(fbo); glPixelStorei(GL_PACK_ALIGNMENT, 1); glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, p); }
    color r; r.value = pgArgb(p); return r;
}
void PGraphics::set(int x,int y,color c) {
    if (!pgUseGl(this)) { if(PApplet::g_papplet) PApplet::g_papplet->set(x,y,c); return; }
    if (x < 0 || y < 0 || x >= width || y >= height) return;
    unsigned int v = c.value; unsigned char p[4] = {(unsigned char)((v >> 16) & 255), (unsigned char)((v >> 8) & 255), (unsigned char)(v & 255), (unsigned char)((v >> 24) & 255)};
    PgFboScope s(fbo);
    GLboolean bl = glIsEnabled(GL_BLEND); glDisable(GL_BLEND);
    glWindowPos2i(x, y); glDrawPixels(1, 1, GL_RGBA, GL_UNSIGNED_BYTE, p);
    if (bl) glEnable(GL_BLEND);
}
PImage* PGraphics::get(int x, int y, int w, int h) { loadPixels(); return PImage::get(x, y, w, h); }
PImage* PGraphics::get() { loadPixels(); return PImage::get(0, 0, width, height); }

// =============================================================================
// JSON IMPLEMENTATION
// =============================================================================

static void skipWS(const ::std::string& s, size_t& i){
    while(i<s.size()&&(s[i]==' '||s[i]=='\t'||s[i]=='\n'||s[i]=='\r'))i++;
}
static ::std::string parseString(const ::std::string& s, size_t& i){
    i++;
    ::std::string r;
    while(i<s.size()&&s[i]!='"'){
        if(s[i]=='\\'&&i+1<s.size()){i++;
            switch(s[i]){case '"':r+='"';break;case '\\':r+='\\';break;case '/':r+='/';break;
                         case 'n':r+='\n';break;case 't':r+='\t';break;case 'r':r+='\r';break;default:r+=s[i];break;}
        } else r+=s[i];
        i++;
    }
    if(i<s.size())i++;
    return r;
}
static JSONValue parseJSONValue(const ::std::string& s, size_t& i);
static JSONObjectMap parseJSONObj(const ::std::string& s, size_t& i){
    JSONObjectMap obj; i++;
    skipWS(s,i);
    while(i<s.size()&&s[i]!='}'){
        skipWS(s,i); if(s[i]!='"')break;
        ::std::string key=parseString(s,i);
        skipWS(s,i); if(i<s.size()&&s[i]==':')i++;
        skipWS(s,i);
        obj[key]=parseJSONValue(s,i);
        skipWS(s,i); if(i<s.size()&&s[i]==',')i++;
        skipWS(s,i);
    }
    if(i<s.size())i++;
    return obj;
}
static JSONArrayVec parseJSONArr(const ::std::string& s, size_t& i){
    JSONArrayVec arr; i++;
    skipWS(s,i);
    while(i<s.size()&&s[i]!=']'){
        skipWS(s,i);
        arr.push_back(parseJSONValue(s,i));
        skipWS(s,i); if(i<s.size()&&s[i]==',')i++;
        skipWS(s,i);
    }
    if(i<s.size())i++;
    return arr;
}
static JSONValue parseJSONValue(const ::std::string& s, size_t& i){
    skipWS(s,i);
    if(i>=s.size()) return JSONValue();
    if(s[i]=='"')  return JSONValue(parseString(s,i));
    if(s[i]=='{')  return JSONValue(parseJSONObj(s,i));
    if(s[i]=='[')  return JSONValue(parseJSONArr(s,i));
    if(s.substr(i,4)=="null") { i+=4; return JSONValue(); }
    if(s.substr(i,4)=="true") { i+=4; return JSONValue(true); }
    if(s.substr(i,5)=="false"){ i+=5; return JSONValue(false); }
    size_t start=i;
    bool isFloat=false;
    if(i<s.size()&&s[i]=='-')i++;
    while(i<s.size()&&(::std::isdigit(s[i])||s[i]=='.'||s[i]=='e'||s[i]=='E'||s[i]=='+'||s[i]=='-')){
        if(s[i]=='.'||s[i]=='e'||s[i]=='E')isFloat=true; i++;
    }
    ::std::string num=s.substr(start,i-start);
    double d=0; try { d=::std::stod(num); } catch(...) { return JSONValue(); }  // cppmode-json: bad numbers no longer throw
    if(isFloat || d>2147483647.0 || d<-2147483648.0) return JSONValue(d);
    return JSONValue((int)d);
}

JSONValue PApplet::parseJSON(const ::std::string& src){ size_t i=0; return parseJSONValue(src,i); }

::std::string PApplet::toJSONString(const JSONValue& v, int indent){
    ::std::string pad(indent*2,' ');
    ::std::string pad2((indent+1)*2,' ');
    switch(v.type){
        case JSONValue::NULL_T:   return "null";
        case JSONValue::BOOL_T:   return v.b?"true":"false";
        case JSONValue::INT_T:    return ::std::to_string((int)v.n);
        case JSONValue::FLOAT_T:  { ::std::ostringstream ss; ss<<v.n; return ss.str(); }
        case JSONValue::STRING_T: {  // cppmode-json: escape quotes, backslashes and control characters
            ::std::string r="\""; for(unsigned char ch : v.s){
                if(ch=='"') r+="\\\""; else if(ch=='\\') r+="\\\\"; else if(ch=='\n') r+="\\n"; else if(ch=='\r') r+="\\r"; else if(ch=='\t') r+="\\t";
                else if(ch<0x20){ char u[8]; snprintf(u,sizeof u,"\\u%04x",ch); r+=u; } else r+=(char)ch; }
            return r+"\""; }
        case JSONValue::ARRAY_T: {
            if(v.arr->empty())return "[]";
            ::std::string r="[\n";
            for(size_t i=0;i<v.arr->size();i++){
                r+=pad2+toJSONString((*v.arr)[i],indent+1);
                if(i+1<v.arr->size())r+=",";
                r+="\n";
            }
            return r+pad+"]";
        }
        case JSONValue::OBJECT_T: {
            if(v.obj->empty())return "{}";
            ::std::string r="{\n";
            size_t n=0;
            for(auto& p:*v.obj){
                r+=pad2+"\""+p.first+"\": "+toJSONString(p.second,indent+1);
                if(++n<v.obj->size())r+=",";
                r+="\n";
            }
            return r+pad+"}";
        }
    }
    return "null";
}

::std::string JSONValue::toString() const { return PApplet::toJSONString(*this, 0); }
::std::string JSONValue::format(int indent) const {
    if (indent >= 0) return PApplet::toJSONString(*this, 0);
    ::std::string s = PApplet::toJSONString(*this, 0), r; bool inStr = false, esc = false;  // one line: drop layout whitespace outside strings
    for (char ch : s) { if (inStr) { r += ch; if (esc) esc = false; else if (ch == '\\') esc = true; else if (ch == '"') inStr = false; }
                        else if (ch == '"') { inStr = true; r += ch; } else if (ch != '\n' && ch != ' ') r += ch; }
    return r;
}

static ::std::string readFileString(const ::std::string& path){
    ::std::ifstream f(path); if(!f)return "";
    return ::std::string((::std::istreambuf_iterator<char>(f)),::std::istreambuf_iterator<char>());
}

JSONValue PApplet::loadJSONObject(const ::std::string& path){ return parseJSON(readFileString(path)); }
JSONValue PApplet::loadJSONArray(const ::std::string& path) { return parseJSON(readFileString(path)); }
bool PApplet::saveJSONObject(const ::std::string& path,const JSONValue& v,int indent){
    ::std::ofstream f(path); if(!f)return false; f<<toJSONString(v,indent); return true;
}
bool PApplet::saveJSONArray(const ::std::string& path,const JSONValue& v,int indent){
    return saveJSONObject(path,v,indent);
}

// =============================================================================
// XML IMPLEMENTATION
// =============================================================================

static void xmlSkipWS(const ::std::string& s,size_t& i){ while(i<s.size()&&::std::isspace(s[i]))i++; }

static XML parseXMLNode(const ::std::string& s, size_t& i){
    XML node;
    xmlSkipWS(s,i);
    if(i>=s.size()||s[i]!='<')return node;
    i++;
    if(i<s.size()&&s[i]=='?'){ while(i<s.size()&&!(s[i]=='>'||s.substr(i,2)=="?>"))i++; if(i<s.size())i++; return node; }
    if(i<s.size()&&s[i]=='!'){ while(i<s.size()&&s[i]!='>')i++; i++; return node; }
    while(i<s.size()&&!::std::isspace(s[i])&&s[i]!='>'&&s[i]!='/')node.name+=s[i++];
    xmlSkipWS(s,i);
    while(i<s.size()&&s[i]!='>'&&s[i]!='/'){
        ::std::string key; while(i<s.size()&&!::std::isspace(s[i])&&s[i]!='='&&s[i]!='>'&&s[i]!='/')key+=s[i++];
        xmlSkipWS(s,i);
        if(i<s.size()&&s[i]=='='){i++;xmlSkipWS(s,i);
            char q=s[i++]; ::std::string val;
            while(i<s.size()&&s[i]!=q)val+=s[i++]; if(i<s.size())i++;
            node.attributes[key]=val;
        }
        xmlSkipWS(s,i);
    }
    if(i<s.size()&&s[i]=='/'){i++;if(i<s.size())i++;return node;}
    if(i<s.size())i++;
    while(i<s.size()){
        xmlSkipWS(s,i);
        if(i+1<s.size()&&s[i]=='<'&&s[i+1]=='/'){
            i++; while(i<s.size()&&s[i]!='>')i++; if(i<s.size())i++; break;
        }
        if(i<s.size()&&s[i]=='<'){ XML child=parseXMLNode(s,i); if(!child.name.empty())node.children.push_back(child); }
        else { ::std::string text; while(i<s.size()&&s[i]!='<')text+=s[i++]; auto t=text;t.erase(0,t.find_first_not_of(" \t\n\r"));t.erase(t.find_last_not_of(" \t\n\r")+1);if(!t.empty())node.content+=t; }
    }
    return node;
}

XML PApplet::parseXML(const ::std::string& src){ size_t i=0; return parseXMLNode(src,i); }
XML PApplet::loadXML(const ::std::string& path){ return parseXML(readFileString(path)); }

::std::string XML::toString(int indent) const {
    ::std::string pad(indent*2,' ');
    ::std::string r=pad+"<"+name;
    for(auto& a:attributes)r+=" "+a.first+"=\""+a.second+"\"";
    if(children.empty()&&content.empty()){r+="/>\n";return r;}
    r+=">";
    if(!content.empty())r+=content;
    if(!children.empty()){r+="\n";for(auto& c:children)r+=c.toString(indent+1);r+=pad;}
    r+="</"+name+">\n";
    return r;
}
bool PApplet::saveXML(const ::std::string& path,const XML& x){ ::std::ofstream f(path);if(!f)return false;f<<x.toString();return true; }

// =============================================================================
// TABLE IMPLEMENTATION
// =============================================================================

// cppmode-table: CSV fields can be quoted ("a, b" and "" for a quote), like Java reads and writes them
static ::std::vector<::std::string> cppmodeSplitCsv(const ::std::string& line, char d) {
    ::std::vector<::std::string> out; ::std::string cur; bool q=false;
    for (size_t i=0;i<line.size();i++) { char ch=line[i];
        if (q) { if (ch=='"') { if (i+1<line.size()&&line[i+1]=='"') { cur+='"'; i++; } else q=false; } else cur+=ch; }
        else if (ch=='"') q=true; else if (ch==d) { out.push_back(cur); cur.clear(); } else if (ch!='\r') cur+=ch; }
    out.push_back(cur); return out;
}
static ::std::string cppmodeCsvField(const ::std::string& s, char d) {
    if (s.find(d)==::std::string::npos && s.find('"')==::std::string::npos && s.find('\n')==::std::string::npos) return s;
    ::std::string r="\""; for (char ch : s) { if (ch=='"') r+="\"\""; else r+=ch; } return r+"\"";
}
Table* PApplet::loadTable(const ::std::string& path,const ::std::string& options){
    Table* t=new Table();
    bool hasHeader=options.find("header")!=::std::string::npos;
    char delim=(path.find(".tsv")!=::std::string::npos||options.find("tsv")!=::std::string::npos)?'\t':',';
    auto lines=loadStrings(path);
    if(lines.empty())return t;
    int start=0;
    if(hasHeader){
        for(auto& c:cppmodeSplitCsv(lines[0],delim))t->addColumn(c);
        start=1;
    }
    for(int i=start;i<(int)lines.size();i++){
        if(lines[i].empty()&&i==(int)lines.size()-1) break;
        auto cells=cppmodeSplitCsv(lines[i],delim);
        while(t->getColumnCount()<(int)cells.size()) t->addColumn("");
        auto& row=t->addRowCells();
        for(int j=0;j<(int)cells.size();j++)row[j]=cells[j];
    }
    return t;
}
bool PApplet::saveTable(const ::std::string& path,const Table& t,const ::std::string& ext){
    ::std::ofstream f(path);if(!f)return false;
    char delim=(ext=="tsv"||path.find(".tsv")!=::std::string::npos)?'\t':',';
    bool named=false; for(auto& c:t.columns) if(!c.empty()) named=true;
    if(named){for(size_t i=0;i<t.columns.size();i++){if(i)f<<delim;f<<cppmodeCsvField(t.columns[i],delim);}f<<"\n";}
    for(auto& row:t.rowData){for(size_t i=0;i<row.size();i++){if(i)f<<delim;f<<cppmodeCsvField(row[i],delim);}f<<"\n";}
    return true;
}

// =============================================================================
// PSHAPE IMPLEMENTATION
// =============================================================================
void PApplet::shapeMode(int mode){ shapeDrawMode=mode; }
PShape PApplet::createShape(int kind){ return PShape(kind); }
// cppmode-pshapeprim: primitives keep kind + numbers, drawn by drawPShape through
// rect(), ellipse(), arc() and friends, so both renderers draw them
PShape PApplet::makePrimShape(int kind, ::std::vector<float> p) {
    PShape s(kind);
    s.prim = kind;
    s.params = ::std::move(p);
    // like Java, a primitive takes the sketch's fill and stroke when it is created
    s.hasFill = doFill;   s.fillR = fillR; s.fillG = fillG; s.fillB = fillB; s.fillA = fillA;
    s.hasStroke = doStroke; s.strokeR = strokeR; s.strokeG = strokeG; s.strokeB = strokeB; s.strokeA = strokeA;
    s.strokeW = strokeW;
    s.styleEnabled = true;
    return s;
}
PShape PApplet::createShape(int kind, float a, float b, float c, float d) { return makePrimShape(kind, {a, b, c, d}); }
PShape PApplet::createShape(int kind, float a) { return makePrimShape(kind, {a}); }
PShape PApplet::createShape(int kind, float a, float b) { return makePrimShape(kind, {a, b}); }
PShape PApplet::createShape(int kind, float a, float b, float c) { return makePrimShape(kind, {a, b, c}); }
PShape PApplet::createShape(int kind, float a, float b, float c, float d, float e, float f) {
    return makePrimShape(kind, {a, b, c, d, e, f}); }
PShape PApplet::createShape(int kind, float a, float b, float c, float d, float e, float f, float g) {
    return makePrimShape(kind, {a, b, c, d, e, f, g}); }
PShape PApplet::createShape(int kind, float a, float b, float c, float d, float e, float f, float g, float hh) {
    return makePrimShape(kind, {a, b, c, d, e, f, g, hh}); }

// ── Minimal SVG loader ────────────────────────────────────────────────────────
// Parses basic SVG shapes (path, rect, circle, ellipse, polygon, polyline, line)
// into PShape children for rendering. Handles fill/stroke from style attributes.

static float svgParseFloat(const ::std::string& s, size_t& i){
    while(i<s.size()&&(s[i]==' '||s[i]==','))i++;
    size_t j=i;
    if(j<s.size()&&(s[j]=='-'||s[j]=='+'))j++;
    while(j<s.size()&&(isdigit(s[j])||s[j]=='.'||s[j]=='e'||s[j]=='E'||((s[j]=='-'||s[j]=='+')&&j>i&&(s[j-1]=='e'||s[j-1]=='E'))))j++;
    float v=0; try{v=::std::stof(s.substr(i,j-i));}catch(...){}
    i=j; return v;
}
static uint32_t svgParseColor(const ::std::string& s){
    if(s.empty()||s=="none") return 0;
    if(s[0]=='#'){
        ::std::string h=s.substr(1);
        if(h.size()==3)h={h[0],h[0],h[1],h[1],h[2],h[2]};
        uint32_t v=0; try{v=(uint32_t)::std::stoul(h,nullptr,16);}catch(...){}
        return v|0xFF000000u;
    }
    // Named colors (common subset)
    static const ::std::unordered_map<::std::string,uint32_t> nc={
        {"black",0xFF000000},{"white",0xFFFFFFFF},{"red",0xFF0000FF},
        {"green",0xFF008000},{"blue",0xFF0000FF},{"none",0},
        {"gray",0xFF808080},{"grey",0xFF808080},{"yellow",0xFFFFFF00},
    };
    auto it=nc.find(s); return it!=nc.end()?it->second:0xFF000000;
}
static ::std::string svgAttr(const ::std::string& tag, const ::std::string& attr){
    // Find attr="value" -- whole-word match to avoid "id=" matching "d="
    size_t p=0;
    ::std::string needle=attr+"=";
    while(true){
        p=tag.find(needle,p);
        if(p==::std::string::npos) return "";
        // Must be preceded by space, tab, or start (whole attribute name)
        bool ok=(p==0||tag[p-1]==' '||tag[p-1]==9||tag[p-1]==58); // 58=':'
        if(ok) break;
        p++;
    }
    p+=needle.size();
    if(p>=tag.size()) return "";
    char q=tag[p++];
    if(q!=34&&q!=39) return ""; // 34='"', 39="'"
    size_t e=tag.find(q,p);
    return e==::std::string::npos?tag.substr(p):tag.substr(p,e-p);
}
static void svgApplyStyle(PShape& sh, const ::std::string& tag){
    // Parse style="..." or individual fill/stroke/stroke-width attrs
    ::std::string style=svgAttr(tag,"style");
    auto getVal=[&](const ::std::string& k)->::std::string{
        // from inline style
        size_t p=style.find(k+":"); if(p!=::std::string::npos){
            p+=k.size()+1; while(p<style.size()&&style[p]==' ')p++;
            size_t e=style.find(';',p); return style.substr(p,e==::std::string::npos?::std::string::npos:e-p);
        }
        // from attribute
        return svgAttr(tag,k);
    };
    ::std::string fs=getVal("fill"),ss=getVal("stroke"),sw=getVal("stroke-width"),op=getVal("opacity"),fo=getVal("fill-opacity");
    float alpha=1.0f;
    if(!op.empty())try{alpha=::std::stof(op);}catch(...){}
    float falpha=alpha;
    if(!fo.empty())try{falpha=::std::stof(fo)*alpha;}catch(...){}
    if(!fs.empty()&&fs!="none"){
        uint32_t c=svgParseColor(fs);
        sh.setFill(((c>>16)&0xFF)/255.f,((c>>8)&0xFF)/255.f,(c&0xFF)/255.f,falpha);
        sh.hasFill=true;
    } else if(fs=="none"){sh.hasFill=false;}
    if(!ss.empty()&&ss!="none"){
        uint32_t c=svgParseColor(ss);
        sh.setStroke(((c>>16)&0xFF)/255.f,((c>>8)&0xFF)/255.f,(c&0xFF)/255.f,alpha);
        sh.hasStroke=true;
    }
    if(!sw.empty())try{sh.strokeW=::std::stof(sw);}catch(...){}
}

// Parse SVG path 'd' attribute into vertices
static PShape svgParsePath(const ::std::string& tag){
    PShape sh; sh.kind=-1; sh.hasFill=true; sh.hasStroke=false;
    svgApplyStyle(sh,tag);
    ::std::string d=svgAttr(tag,"d");
    if(d.empty())return sh;
    float cx=0,cy=0,sx=0,sy=0; // current pos and subpath start
    float lcx2=0,lcy2=0; // last control point (for S/T reflection)
    char lastCmd=0;
    size_t i=0;
    auto nextF=[&]()->float{
        while(i<d.size()&&(d[i]==' '||d[i]==','||d[i]=='\t'||d[i]=='\n'||d[i]=='\r'))i++;
        return svgParseFloat(d,i);
    };
    int maxIter=100000, iter=0;
    while(i<d.size()&&iter++<maxIter){
        while(i<d.size()&&(d[i]==' '||d[i]==','||d[i]=='\t'||d[i]=='\n'||d[i]=='\r'))i++;
        if(i>=d.size())break;
        char cmd=d[i];
        bool isCmd=isalpha((unsigned char)cmd);
        if(isCmd){i++;lastCmd=cmd;}
        else cmd=lastCmd;
        if(!cmd)break;
        bool rel=islower((unsigned char)cmd);
        char C=(char)toupper((unsigned char)cmd);

        if(C=='M'){
            float x=nextF()+(rel?cx:0),y=nextF()+(rel?cy:0);
            // Track where each subpath starts
            sh.subpathStarts.push_back((int)sh.verts.size());
            sh.verts.push_back({x,y,0,0,0}); cx=x;cy=y; sx=cx;sy=cy;
            lastCmd=rel?'l':'L';
        } else if(C=='L'){
            float x=nextF()+(rel?cx:0),y=nextF()+(rel?cy:0);
            sh.verts.push_back({x,y,0,0,0}); cx=x;cy=y;
        } else if(C=='H'){
            float x=nextF()+(rel?cx:0);
            sh.verts.push_back({x,cy,0,0,0}); cx=x;
        } else if(C=='V'){
            float y=nextF()+(rel?cy:0);
            sh.verts.push_back({cx,y,0,0,0}); cy=y;
        } else if(C=='C'){
            float x1=nextF()+(rel?cx:0),y1=nextF()+(rel?cy:0);
            float x2=nextF()+(rel?cx:0),y2=nextF()+(rel?cy:0);
            float x=nextF()+(rel?cx:0),y=nextF()+(rel?cy:0);
            {
                // Adaptive segments: based on chord length of control polygon
                float dx1=x1-cx,dy1=y1-cy,dx2=x2-x1,dy2=y2-y1,dx3=x-x2,dy3=y-y2;
                float clen=::std::sqrt(dx1*dx1+dy1*dy1)+::std::sqrt(dx2*dx2+dy2*dy2)+::std::sqrt(dx3*dx3+dy3*dy3);
                int seg=::std::max(2,(int)(clen/2));if(seg>128)seg=128;
                for(int s2=1;s2<=seg;s2++){
                    float t=s2/(float)seg,u=1-t;
                    sh.verts.push_back({u*u*u*cx+3*u*u*t*x1+3*u*t*t*x2+t*t*t*x,
                                        u*u*u*cy+3*u*u*t*y1+3*u*t*t*y2+t*t*t*y,0,0,0});
                }
            }
            lcx2=x2;lcy2=y2; cx=x;cy=y;
        } else if(C=='S'){
            // Smooth cubic: reflect last C's second control point
            float x1=2*cx-lcx2,y1=2*cy-lcy2;
            float x2=nextF()+(rel?cx:0),y2=nextF()+(rel?cy:0);
            float x=nextF()+(rel?cx:0),y=nextF()+(rel?cy:0);
            for(int s2=1;s2<=20;s2++){
                float t=s2/20.f,u=1-t;
                float bx=u*u*u*cx+3*u*u*t*x1+3*u*t*t*x2+t*t*t*x;
                float by=u*u*u*cy+3*u*u*t*y1+3*u*t*t*y2+t*t*t*y;
                sh.verts.push_back({bx,by,0,0,0});
            }
            lcx2=x2;lcy2=y2; cx=x;cy=y;
        } else if(C=='Q'){
            float x1=nextF()+(rel?cx:0),y1=nextF()+(rel?cy:0);
            float x=nextF()+(rel?cx:0),y=nextF()+(rel?cy:0);
            for(int s2=1;s2<=20;s2++){
                float t=s2/20.f,u=1-t;
                sh.verts.push_back({u*u*cx+2*u*t*x1+t*t*x,u*u*cy+2*u*t*y1+t*t*y,0,0,0});
            }
            lcx2=x1;lcy2=y1; cx=x;cy=y;
        } else if(C=='T'){
            // Smooth quadratic: reflect last Q's control point
            float x1=2*cx-lcx2,y1=2*cy-lcy2;
            float x=nextF()+(rel?cx:0),y=nextF()+(rel?cy:0);
            for(int s2=1;s2<=20;s2++){
                float t=s2/20.f,u=1-t;
                sh.verts.push_back({u*u*cx+2*u*t*x1+t*t*x,u*u*cy+2*u*t*y1+t*t*y,0,0,0});
            }
            lcx2=x1;lcy2=y1; cx=x;cy=y;
        } else if(C=='A'){
            float rx=nextF(),ry=nextF();
            nextF();nextF();nextF(); // x-rot, large-arc, sweep
            float ex=nextF()+(rel?cx:0),ey=nextF()+(rel?cy:0);
            // Approximate arc with line to endpoint
            {
                // Approximate arc chord length for adaptive segments
                float adx=ex-cx,ady=ey-cy;
                float chord=::std::sqrt(adx*adx+ady*ady);
                // rx,ry give the arc radius -- use larger for segment count
                float r2=::std::max(rx,ry);
                int arcSeg=::std::max(4,(int)(chord/2));
                if(r2>0){int byRadius=(int)(r2*0.5f);if(byRadius>arcSeg)arcSeg=byRadius;}
                if(arcSeg>96)arcSeg=96;
                for(int s2=1;s2<=arcSeg;s2++){
                    float t=s2/(float)arcSeg;
                    sh.verts.push_back({cx+(ex-cx)*t, cy+(ey-cy)*t,0,0,0});
                }
            }
            cx=ex;cy=ey;
        } else if(C=='Z'){
            sh.closed=true;
            sh.verts.push_back({sx,sy,0,0,0});
            cx=sx;cy=sy;
        } else {
            // Unknown -- skip to next alpha or end
            while(i<d.size()&&!isalpha((unsigned char)d[i])&&d[i]!=' ')i++;
            if(i<d.size()&&!isalpha((unsigned char)d[i]))i++;
        }
    }
    return sh;
}

static PShape* svgLoad(const ::std::string& path){
    // Search paths
    ::std::vector<::std::string> tries={path,"data/"+path,"files/"+path};
    ::std::string found;
    for(auto& t:tries){FILE* f=processingOpenFile(t.c_str(),"r");if(f){fclose(f);found=t;break;}}
    if(found.empty()){::std::cerr<<"loadShape: file not found: "<<path<<"\n";return new PShape();}

    ::std::ifstream f(found);
    ::std::string xml((::std::istreambuf_iterator<char>(f)),::std::istreambuf_iterator<char>());

    PShape* root=new PShape();
    root->hasFill=false;
    // Extract viewBox for scaling
    float vbW=0,vbH=0;
    size_t vbp=xml.find("viewBox=");
    if(vbp!=::std::string::npos){
        vbp+=9;size_t i2=vbp;
        svgParseFloat(xml,i2);svgParseFloat(xml,i2); // minX minY
        vbW=svgParseFloat(xml,i2);vbH=svgParseFloat(xml,i2);
    }

    // Parse all shape elements
    size_t p=0;
    int maxTags=20000, tagCount=0;
    ::std::string currentGroupId; // id from nearest enclosing <g id="...">
    ::std::vector<::std::string> groupIdStack; // stack for nested groups
    while(p<xml.size() && tagCount++<maxTags){
        size_t lt=xml.find('<',p); if(lt==::std::string::npos)break;
        // Skip comments <!-- ... -->
        if(lt+3<xml.size() && xml.substr(lt,4)=="<!--"){
            size_t end=xml.find("-->",lt+4);
            p=(end==::std::string::npos)?xml.size():end+3; continue;
        }
        // Skip DOCTYPE and CDATA <! ... > (may contain > inside [...])
        if(lt+1<xml.size() && xml[lt+1]=='!'){
            // Find matching > accounting for [] nesting
            size_t i2=lt+2; int brackets=0;
            while(i2<xml.size()){
                if(xml[i2]=='[') brackets++;
                else if(xml[i2]==']') brackets--;
                else if(xml[i2]=='>' && brackets<=0){i2++;break;}
                i2++;
            }
            p=i2; continue;
        }
                // Find closing > -- skip quoted attribute values
        size_t gt=lt+1;
        {
            bool inQ=false; char qc=0;
            while(gt<xml.size()){
                char c3=xml[gt];
                if(!inQ && (c3==34||c3==39)){ inQ=true; qc=c3; gt++; }
                else if(inQ && c3==qc){ inQ=false; gt++; }
                else if(!inQ && c3==62){ break; }
                else gt++;
            }
        }
        if(gt>=xml.size())break;
        ::std::string tag=xml.substr(lt+1,gt-lt-1);
        p=gt+1;
        if(tag.empty()||tag[0]=='?')continue;
        if(tag[0]=='/'){
            // Closing tag
            ::std::string ctag=tag.substr(1);
            size_t csp=ctag.find_first_of(" /\t");
            if(csp!=::std::string::npos) ctag=ctag.substr(0,csp);
            if(ctag=="g"&&!groupIdStack.empty()){
                groupIdStack.pop_back();
                currentGroupId=groupIdStack.empty()?"":groupIdStack.back();
            }
            continue;
        }
        // Remove newlines/tabs from tag for easier parsing
        for(size_t ci2=0;ci2<tag.size();ci2++) if(tag[ci2]==10||tag[ci2]==13||tag[ci2]==9) tag[ci2]=' ';
        // get element name
        size_t sp=tag.find_first_of(" /");
        ::std::string elem=(sp==::std::string::npos)?tag:tag.substr(0,sp);

        PShape child;
        if(elem=="path"){
            child=svgParsePath(tag);
        } else if(elem=="rect"){
            float x=0,y=0,w=0,h=0,rx=0,ry=0;
            auto a=[&](const ::std::string& k)->float{::std::string v=svgAttr(tag,k);if(v.empty())return 0;try{return ::std::stof(v);}catch(...){return 0;}};
            x=a("x");y=a("y");w=a("width");h=a("height");rx=a("rx");ry=a("ry");
            child.kind=-1; svgApplyStyle(child,tag);
            child.verts.push_back({x,y,0,0,0});child.verts.push_back({x+w,y,0,0,0});
            child.verts.push_back({x+w,y+h,0,0,0});child.verts.push_back({x,y+h,0,0,0});
            child.closed=true;
        } else if(elem=="circle"||elem=="ellipse"){
            float cx2=0,cy2=0,rx=0,ry=0;
            auto a=[&](const ::std::string& k)->float{::std::string v=svgAttr(tag,k);if(v.empty())return 0;try{return ::std::stof(v);}catch(...){return 0;}};
            cx2=a("cx");cy2=a("cy");
            if(elem=="circle"){rx=ry=a("r");}else{rx=a("rx");ry=a("ry");}
            child.kind=-1; svgApplyStyle(child,tag);
            int seg=32;
            for(int s2=0;s2<=seg;s2++){
                float t=s2*TWO_PI/seg;
                child.verts.push_back({cx2+cos(t)*rx,cy2+sin(t)*ry,0,0,0});
            }
            child.closed=true;
        } else if(elem=="polygon"||elem=="polyline"){
            child.kind=-1; svgApplyStyle(child,tag);
            child.closed=(elem=="polygon");
            ::std::string pts=svgAttr(tag,"points"); size_t pi=0;
            while(pi<pts.size()){
                while(pi<pts.size()&&(pts[pi]==' '||pts[pi]==','))pi++;
                if(pi>=pts.size())break;
                float x=svgParseFloat(pts,pi),y=svgParseFloat(pts,pi);
                child.verts.push_back({x,y,0,0,0});
            }
        } else if(elem=="line"){
            child.kind=LINES; svgApplyStyle(child,tag);
            auto a=[&](const ::std::string& k)->float{::std::string v=svgAttr(tag,k);if(v.empty())return 0;try{return ::std::stof(v);}catch(...){return 0;}};
            child.verts.push_back({a("x1"),a("y1"),0,0,0});
            child.verts.push_back({a("x2"),a("y2"),0,0,0});
        } else if(elem=="defs"||elem=="title"||elem=="desc"){
            continue;
        } else if(elem=="svg"){
            continue;
        } else if(elem=="g"){
            // Push group id onto stack so child shapes inherit it
            ::std::string gid=svgAttr(tag,"id");
            if(gid.empty()) gid=svgAttr(tag,"inkscape:label");
            groupIdStack.push_back(gid);
            currentGroupId=gid.empty()?currentGroupId:gid;
            continue;
        } else continue;

        {
            ::std::string sid = svgAttr(tag,"id");
            if(sid.empty()) sid = svgAttr(tag,"inkscape:label");
            if(sid.empty()) sid = currentGroupId;
            child.name = sid;
            // Always add child so getChildCount() matches Java Processing
            root->children.push_back(child);
        }
        // Also handle <g id="..."> groups by storing named empty shapes
        // so getChild can find them even if they hold no direct verts
    }

    // Store viewBox dimensions on root shape for reference
    // Do NOT normalize coordinates -- keep raw SVG space so shape(s,x,y) works
    // with sketch-supplied offsets. shape(s,x,y,w,h) will scale via glScalef.
    if(vbW>0&&vbH>0){
        root->verts.push_back({vbW,vbH,0,0,0}); // store vbW,vbH as sentinel in root
    }
    root->computeBounds();
    return root;
}

// ── OBJ Loader ───────────────────────────────────────────────────────────────

// Per-vertex data for OBJ (stored flat in child PShape)
struct ObjVertex {
    float x,y,z;   // position
    float nx,ny,nz; // normal
    float u,v;     // texcoord
};

// Parse MTL file, return map of material name -> texture PImage*
// cppmode-pshapeobj: under libprocessing there is no GL context, so an OBJ material's
// texture is kept as a file path under a placeholder id instead of a GL texture
static ::std::unordered_map<GLuint, ::std::string> g_objTexPath;
static ::std::unordered_map<::std::string,GLuint> objLoadMtl(const ::std::string& mtlPath){
    ::std::unordered_map<::std::string,GLuint> mats;
    ::std::ifstream f(mtlPath);
    if(!f.is_open()) return mats;
    ::std::string curMat, line;
    while(::std::getline(f,line)){
        if(line.empty()||line[0]=='#') continue;
        ::std::istringstream ss(line);
        ::std::string tok; ss>>tok;
        if(tok=="newmtl"){ ss>>curMat; }
        else if((tok=="map_Kd"||tok=="map_Ka")&&!curMat.empty()&&mats.find(curMat)==mats.end()){
            ::std::string texFile; ss>>texFile;
            // Try relative to obj dir
            ::std::vector<::std::string> tries={
                _s_objDir+texFile, _s_objDir+"data/"+texFile,
                "data/"+texFile, texFile
            };
            for(auto& t:tries){
                #ifdef PROCESSING_HAS_STB_IMAGE
                int w2,h2,ch;
                unsigned char* d=stbi_load(t.c_str(),&w2,&h2,&ch,4);
                if(d){
                    if (PApplet::g_papplet && PApplet::g_papplet->_backend) {  // cppmode-pshapeobj
                        static GLuint s_nextId = 0x70000000u;
                        GLuint id = ++s_nextId;
                        g_objTexPath[id] = t; mats[curMat] = id;
                        stbi_image_free(d); break;
                    }
                    GLuint tex;
                    glGenTextures(1,&tex);
                    glBindTexture(GL_TEXTURE_2D,tex);
                    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w2,h2,0,GL_RGBA,GL_UNSIGNED_BYTE,d);
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
                    glBindTexture(GL_TEXTURE_2D,0);
                    stbi_image_free(d);
                    mats[curMat]=tex;
                                    break;
                }
                #endif
            }
        }
    }
    return mats;
}

static PShape* objLoad(const ::std::string& path){
    ::std::vector<::std::string> tries={path,"data/"+path,"files/"+path};
    ::std::string found;
    for(auto& t:tries){FILE* f2=processingOpenFile(t.c_str(),"r");if(f2){fclose(f2);found=t;break;}}
    if(found.empty()){::std::cerr<<"loadShape: OBJ not found: "<<path<<"\n";return new PShape();}

    // Get directory for relative texture paths
    size_t slash=found.find_last_of("/\\");
    _s_objDir=(slash==::std::string::npos)?"":found.substr(0,slash+1);

    ::std::vector<::std::array<float,3>> vp,vn;
    ::std::vector<::std::array<float,2>> vt;
    ::std::unordered_map<::std::string,GLuint> mats; // material name -> GL texture

    PShape* root=new PShape();
    root->name="__obj__";

    // Current group state
    struct Group {
        ::std::string name, matName;
        GLuint texId=0;
        ::std::vector<ObjVertex> verts;
    };
    ::std::vector<Group> groups;
    Group* cur=nullptr;

    auto getGroup=[&]()->Group*{
        if(!cur){groups.push_back({});cur=&groups.back();}
        return cur;
    };

    ::std::ifstream f(found);
    ::std::string line;
    while(::std::getline(f,line)){
        // Strip \r
        if(!line.empty()&&line.back()=='\r') line.pop_back();
        if(line.empty()||line[0]=='#') continue;
        ::std::istringstream ss(line);
        ::std::string tok; ss>>tok;
        if(tok=="mtllib"){
            ::std::string mtlFile; ss>>mtlFile;
            ::std::vector<::std::string> mtlTries={_s_objDir+mtlFile,"data/"+mtlFile,mtlFile};
            for(auto& t:mtlTries){
                ::std::ifstream test(t);
                if(test.is_open()){mats=objLoadMtl(t);break;}
            }
        } else if(tok=="v"){
            float x,y,z; ss>>x>>y>>z; vp.push_back({x,y,z});
        } else if(tok=="vt"){
            float u,v2=0; ss>>u>>v2; vt.push_back({u,v2});
        } else if(tok=="vn"){
            float x,y,z; ss>>x>>y>>z; vn.push_back({x,y,z});
        } else if(tok=="o"||tok=="g"){
            groups.push_back({}); cur=&groups.back(); ss>>cur->name;
        } else if(tok=="usemtl"){
            ::std::string mat; ss>>mat;
            if(!cur){groups.push_back({});cur=&groups.back();}
            // Start new group for new material
            if(!cur->verts.empty()){groups.push_back({});cur=&groups.back();}
            cur->matName=mat;
            cur->texId=mats.count(mat)?mats[mat]:0;
        } else if(tok=="f"){
            ::std::vector<::std::array<int,3>> face;
            ::std::string fv;
            while(ss>>fv){
                ::std::array<int,3> idx={0,0,0};
                ::std::istringstream fs(fv);
                ::std::string part; int fi=0;
                while(::std::getline(fs,part,'/')&&fi<3){
                    if(!part.empty()) try{idx[fi]=::std::stoi(part);}catch(...){}
                    fi++;
                }
                face.push_back(idx);
            }
            if(face.size()<3) continue;
            Group* g=getGroup();
            for(size_t fi=1;fi+1<face.size();fi++){
                for(int k=0;k<3;k++){
                    auto& fc=(k==0)?face[0]:(k==1)?face[fi]:face[fi+1];
                    ObjVertex ov={};
                    int vi=fc[0],ti=fc[1],ni=fc[2];
                    if(vi>0&&vi<=(int)vp.size()){ov.x=vp[vi-1][0];ov.y=vp[vi-1][1];ov.z=vp[vi-1][2];}
                    else if(vi<0&&(int)vp.size()+vi>=0){auto& p=vp[vp.size()+vi];ov.x=p[0];ov.y=p[1];ov.z=p[2];}
                    if(ti>0&&ti<=(int)vt.size()){ov.u=vt[ti-1][0];ov.v=1.0f-vt[ti-1][1];} // flip V
                    if(ni>0&&ni<=(int)vn.size()){ov.nx=vn[ni-1][0];ov.ny=vn[ni-1][1];ov.nz=vn[ni-1][2];}
                    g->verts.push_back(ov);
                }
            }
        }
    }

    // Convert groups to PShape children
    for(auto& g:groups){
        if(g.verts.empty()) continue;
        PShape child;
        child.kind=TRIANGLES;
        child.hasFill=true; child.hasStroke=false;
        child.fillR=0.8f;child.fillG=0.8f;child.fillB=0.8f;child.fillA=1.0f;
        child.name=g.matName;
        // Pack ObjVertex into PShape::Vertex (x,y,z) + subpathStarts for normals + verts.z4,z5 for uv
        // Store as flat arrays via a different approach:
        // verts: position (x,y,z), normal (nx via u field, ny via v field... messy)
        // Better: pack into verts with a 5-float struct matching PShape::Vertex
        // PShape::Vertex has {x,y,z,u,v} -- repurpose u,v for texcoords
        // Store normals separately in subpathStarts encoded as 3 floats packed per vertex
        for(auto& ov:g.verts){
            child.verts.push_back({ov.x,ov.y,ov.z,ov.u,ov.v});
        }
        // Store normals as extra data: pack 3 floats per vertex into children[0].verts
        // Use a sibling child marked as normals store
        PShape norms;
        norms.kind=-999; // marker for normal data
        for(auto& ov:g.verts) norms.verts.push_back({ov.nx,ov.ny,ov.nz,0,0});
        child.children.push_back(norms);
        // Store texture ID in strokeR (reuse float field)
        // Better: store as a proper field -- use hasTex + texID on PShape
        // We'll use the existing texId field if it exists, otherwise pack into a child name
        child.texId=g.texId;
        root->children.push_back(child);
    }

    return root;
}

PShape* PApplet::loadShape(const ::std::string& path){
    auto ext=[&](const ::std::string& e)->bool{
        return path.size()>=e.size()&&path.substr(path.size()-e.size())==e;
    };
    if(ext(".svg")||ext(".SVG")) return svgLoad(path);
    if(ext(".obj")||ext(".OBJ")) return objLoad(path);
    ::std::cerr<<"loadShape: unsupported format: "<<path<<"\n";
    return new PShape();
}

// cppmode-sentmat: a shape's current size includes what groups did to it (Java sizes from that)
static void cppGeoScale(const PShape& s, float& sx, float& sy) {
    sx = sy = 1;
    if (!s.hasGeo) return;
    sx = ::std::sqrt(s.geo[0] * s.geo[0] + s.geo[1] * s.geo[1] + s.geo[2] * s.geo[2]);
    sy = ::std::sqrt(s.geo[4] * s.geo[4] + s.geo[5] * s.geo[5] + s.geo[6] * s.geo[6]);
    if (sx <= 0) sx = 1;
    if (sy <= 0) sy = 1;
}
// Java's PShapes have their own default material (specular 128 gray, shininess 1, no emission),
// separate from the sketch's; a front light under lightSpecular() shows it as a wash.
// The sketch's material comes back after the shape, on every way out.
struct CppMat { float spec[3]; float shin; float emis[3]; };
static CppMat cppGetMat() {
    CppMat m{};
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (PApplet::g_papplet && PApplet::g_papplet->_backend) {
        for (int i = 0; i < 3; i++) { m.spec[i] = g_lpMatSpec[i]; m.emis[i] = g_lpMatEmis[i]; }
        m.shin = g_lpMatShin;
        return m;
    }
#endif
#ifndef __EMSCRIPTEN__
    float f[4];
    glGetMaterialfv(GL_FRONT, GL_SPECULAR, f); for (int i = 0; i < 3; i++) m.spec[i] = f[i];
    glGetMaterialfv(GL_FRONT, GL_SHININESS, f); m.shin = f[0];
    glGetMaterialfv(GL_FRONT, GL_EMISSION, f); for (int i = 0; i < 3; i++) m.emis[i] = f[i];
#endif
    return m;
}
static void cppSetMat(const CppMat& m) {
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (PApplet::g_papplet && PApplet::g_papplet->_backend) {
        for (int i = 0; i < 3; i++) { g_lpMatSpec[i] = m.spec[i]; g_lpMatEmis[i] = m.emis[i]; }
        g_lpMatShin = m.shin;
        lpLitRefresh();
        return;
    }
#endif
#ifndef __EMSCRIPTEN__
    float s4[4] = {m.spec[0], m.spec[1], m.spec[2], 1}, e4[4] = {m.emis[0], m.emis[1], m.emis[2], 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s4);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m.shin);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, e4);
#endif
}
struct CppPShapeMat {
    CppMat saved;
    CppPShapeMat() : saved(cppGetMat()) { CppMat ps{{0.5f, 0.5f, 0.5f}, 1.0f, {0, 0, 0}}; cppSetMat(ps); }
    ~CppPShapeMat() { cppSetMat(saved); }
};
void PApplet::drawPShape(const PShape& s,float x,float y,float w,float h,bool parentStyleEnabled){
    if(!s.visible)return;
    CppPShapeMat pshapeMaterial;  // PShapes use their own material
    if (s.prim) {  // cppmode-pshapeprim
        bool se = s.styleEnabled && parentStyleEnabled;
        pushMatrix();
        translate(x, y);
        if (s.hasGeo) { if (_backend) _backend->apply_matrix(s.geo); else glMultMatrixf(s.geo); }
        if (s.hasMtx) { if (_backend) _backend->apply_matrix(s.mtx); else glMultMatrixf(s.mtx); }
        pushStyle();
        if (se) {
            colorMode(RGB, 1, 1, 1, 1);
            if (s.hasFill) fill(s.fillR, s.fillG, s.fillB, s.fillA); else noFill();
            if (s.hasStroke) { stroke(s.strokeR, s.strokeG, s.strokeB, s.strokeA); strokeWeight(s.strokeW); } else noStroke();
        }
        rectMode(CORNER); ellipseMode(CENTER);
        const auto& p = s.params;
        auto P = [&](size_t i) { return i < p.size() ? p[i] : 0.0f; };
        switch (s.prim) {
            case RECT:     rect(P(0), P(1), P(2), P(3)); break;
            case ELLIPSE:  ellipse(P(0), P(1), P(2), P(3)); break;
            case ARC:      if (p.size() >= 7) arc(P(0), P(1), P(2), P(3), P(4), P(5), (int)P(6));
                           else arc(P(0), P(1), P(2), P(3), P(4), P(5)); break;
            case TRIANGLE: triangle(P(0), P(1), P(2), P(3), P(4), P(5)); break;
            case QUAD:     quad(P(0), P(1), P(2), P(3), P(4), P(5), P(6), P(7)); break;
            case LINE:     line(P(0), P(1), P(2), P(3)); break;
            case POINT:    point(P(0), P(1)); break;
            case BOX:      if (p.size() >= 3) box(P(0), P(1), P(2)); else box(P(0)); break;
            case SPHERE:   sphere(P(0)); break;
            default: break;
        }
        popStyle();
        popMatrix();
        return;
    }
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // cppmode-pshape: replay through the drawing calls libprocessing already handles
        bool se = s.styleEnabled && parentStyleEnabled;
        pushMatrix();
        translate(x, y);
        if (w > 0 && h > 0) {
            // root of a loaded SVG keeps its viewBox in verts[0]
            float vbW = 1, vbH = 1;
            if (!s.verts.empty() && !s.children.empty() && s.verts[0].z == 0) { vbW = s.verts[0].x; vbH = s.verts[0].y; }
            if (vbW > 1) scale(w / vbW, h / vbH);
            else if (s.width > 0 && s.height > 0) { float gx, gy; cppGeoScale(s, gx, gy); scale(w / (s.width * gx), h / (s.height * gy)); }  // cppmode-pshapesize
        }
        if (s.hasGeo) _backend->apply_matrix(s.geo);
        if (s.hasMtx && s.children.empty()) _backend->apply_matrix(s.mtx);  // cppmode-pshapematrix
        for (size_t ci = 0; ci < s.children.size(); ci++) drawPShape(s.childAt(ci), 0, 0, -1, -1, se);  // cppmode-javashare
        int n = (int)s.verts.size();
        bool isMesh = (s.kind == TRIANGLES);
        if (n >= (isMesh ? 3 : 2)) {
            // the shape's own style when it has one; the sketch's style comes back after
            float sf[4] = {fillR, fillG, fillB, fillA}, ss[4] = {strokeR, strokeG, strokeB, strokeA}, sw = strokeW;
            bool sdf = doFill, sds = doStroke;
            auto apply = [&]() {
                if (doFill) { _backend->fill(fillR, fillG, fillB, fillA); lpLitFillChanged(fillR, fillG, fillB, fillA); }
                else _backend->no_fill();
                if (doStroke) { _backend->stroke(strokeR, strokeG, strokeB, strokeA); _backend->stroke_weight(strokeW); }
                else _backend->no_stroke();
            };
            if (se) {
                doFill = s.hasFill; doStroke = s.hasStroke;
                fillR = s.fillR; fillG = s.fillG; fillB = s.fillB; fillA = s.fillA;
                strokeR = s.strokeR; strokeG = s.strokeG; strokeB = s.strokeB; strokeA = s.strokeA; strokeW = s.strokeW;
            }
            if (isMesh) doStroke = false;  // OBJ meshes draw no outline, like the OpenGL path
            apply();
            if (isMesh) {
                // cppmode-pshapeobj: the OBJ's own normals (smooth shading) and texture
                const PShape* normStore = nullptr;
                for (auto& c2 : s.children) if (c2.kind == -999) { normStore = &c2; break; }
                PImage* tex = nullptr;
                if (s.texId) {
                    auto it = g_objTexPath.find(s.texId);
                    if (it != g_objTexPath.end()) {
                        static ::std::unordered_map<::std::string, PImage*> s_objTex;
                        auto ti = s_objTex.find(it->second);
                        if (ti == s_objTex.end()) ti = s_objTex.emplace(it->second, loadImage(it->second)).first;
                        tex = ti->second;
                        if (tex && tex->width <= 0) tex = nullptr;
                    }
                }
                int savedTexMode = textureModeVal;
                beginShape(TRIANGLES);
                if (tex) { textureMode(NORMAL); texture(*tex); }
                for (int vi = 0; vi < n; vi++) {
                    if (normStore && vi < (int)normStore->verts.size())
                        normal(normStore->verts[vi].x, normStore->verts[vi].y, normStore->verts[vi].z);
                    const auto& v = s.verts[vi];
                    if (tex) vertex(v.x, v.y, v.z, v.u, v.v); else vertex(v.x, v.y, v.z);
                }
                endShape();
                textureMode(savedTexMode);
            } else {
                // first subpath is the shape, the rest are contours: islands and holes both work
                ::std::vector<int> starts = s.subpathStarts;
                if (starts.empty() || starts[0] != 0) starts.insert(starts.begin(), 0);
                beginShape();
                for (size_t si = 0; si < starts.size(); si++) {
                    int from = starts[si], to = si + 1 < starts.size() ? starts[si + 1] : n;
                    if (si > 0) beginContour();
                    for (int vi = from; vi < to; vi++) vertex(s.verts[vi].x, s.verts[vi].y, s.verts[vi].z);
                    if (si > 0) endContour();
                }
                if (s.closed) endShape(CLOSE); else endShape();
            }
            fillR = sf[0]; fillG = sf[1]; fillB = sf[2]; fillA = sf[3];
            strokeR = ss[0]; strokeG = ss[1]; strokeB = ss[2]; strokeA = ss[3]; strokeW = sw;
            doFill = sdf; doStroke = sds;
            apply();
        }
        popMatrix();
        return;
    }
#endif
    bool se = s.styleEnabled && parentStyleEnabled;
    glPushMatrix();
    glTranslatef(x,y,0);
    if(w>0&&h>0){
        // If root has viewBox stored as sentinel, scale to fit w,h
        float vbW=1,vbH=1;
        if(!s.verts.empty() && s.children.empty()==false && s.verts[0].z==0){
            vbW=s.verts[0].x; vbH=s.verts[0].y;
        }
        if(vbW>1) glScalef(w/vbW,h/vbH,1);
        // cppmode-pshapesize: like Java, the shape ends up w x h, so scale by w / width
        else if(s.width>0&&s.height>0){ float gx,gy; cppGeoScale(s,gx,gy); glScalef(w/(s.width*gx),h/(s.height*gy),1); }
    }
    if(s.hasGeo) glMultMatrixf(s.geo);
    if(s.hasMtx && s.children.empty()) glMultMatrixf(s.mtx);  // cppmode-pshapematrix (a group's is in its children)
    for(size_t ci=0;ci<s.children.size();ci++) drawPShape(s.childAt(ci),0,0,-1,-1,se);  // cppmode-javashare
    if(!s.verts.empty()){
        bool useFill   = se ? s.hasFill  : doFill;
        bool useStroke = se ? s.hasStroke : doStroke;
        int n=(int)s.verts.size();

        if(useFill && n>=3){
            if(se) glColor4f(s.fillR,s.fillG,s.fillB,s.fillA);
            else   applyFill();
            // OBJ triangles: per-vertex normals + texture
            if(s.kind==TRIANGLES){
                const PShape* normStore=nullptr;
                for(auto& c:s.children) if(c.kind==-999){normStore=&c;break;}
                bool hasTex=(s.texId!=0);
                if(hasTex){
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D,s.texId);
                    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
                    glColor4f(1,1,1,1);
                }
                glBegin(GL_TRIANGLES);
                for(int vi=0;vi<n;vi++){
                    if(normStore&&vi<(int)normStore->verts.size()){
                        glNormal3f(normStore->verts[vi].x,normStore->verts[vi].y,normStore->verts[vi].z);
                    } else if(vi%3==0&&vi+2<n){
                        float ax=s.verts[vi+1].x-s.verts[vi].x,ay=s.verts[vi+1].y-s.verts[vi].y,az=s.verts[vi+1].z-s.verts[vi].z;
                        float bx=s.verts[vi+2].x-s.verts[vi].x,by=s.verts[vi+2].y-s.verts[vi].y,bz=s.verts[vi+2].z-s.verts[vi].z;
                        float nx2=ay*bz-az*by,ny2=az*bx-ax*bz,nz2=ax*by-ay*bx;
                        float len=::std::sqrt(nx2*nx2+ny2*ny2+nz2*nz2);
                        if(len>0){nx2/=len;ny2/=len;nz2/=len;}
                        glNormal3f(nx2,ny2,nz2);
                    }
                    if(hasTex) glTexCoord2f(s.verts[vi].u,s.verts[vi].v);
                    glVertex3f(s.verts[vi].x,s.verts[vi].y,s.verts[vi].z);
                }
                glEnd();
                if(hasTex){glDisable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D,0);}
            } else {
            glEnable(GL_BLEND);
            glApplyBlend(g_glBlendMode);
            // Use stencil with evenodd rule -- draw each closed subpath separately
            // so multiple subpaths (e.g. Michigan's two peninsulas) each fill correctly
            glEnable(GL_STENCIL_TEST);
            glClear(GL_STENCIL_BUFFER_BIT);
            glStencilFunc(GL_ALWAYS,0,~0);
            glStencilOp(GL_KEEP,GL_KEEP,GL_INVERT);
            glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
            glDisable(GL_CULL_FACE);
            // Draw each subpath as its own triangle fan into stencil
            // Uses subpathStarts recorded during path parsing (one entry per M command)
            {
                auto drawFan=[&](int from, int to){
                    if(to-from<3) return;
                    glBegin(GL_TRIANGLE_FAN);
                    for(int vi=from;vi<to;vi++)
                        glVertex3f(s.verts[vi].x,s.verts[vi].y,s.verts[vi].z);
                    glVertex3f(s.verts[from].x,s.verts[from].y,s.verts[from].z); // close
                    glEnd();
                };
                if(s.subpathStarts.size()<=1){
                    // Single subpath
                    drawFan(0,(int)s.verts.size());
                } else {
                    // Multiple subpaths (e.g. Michigan's two peninsulas)
                    for(int si=0;si<(int)s.subpathStarts.size();si++){
                        int from=s.subpathStarts[si];
                        int to=(si+1<(int)s.subpathStarts.size())?s.subpathStarts[si+1]:(int)s.verts.size();
                        drawFan(from,to);
                    }
                }
            }
            glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
            glStencilFunc(GL_NOTEQUAL,0,~0);
            glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
            float minx=s.verts[0].x,maxx=minx,miny=s.verts[0].y,maxy=miny;
            for(auto& v:s.verts){minx=::std::min(minx,v.x);maxx=::std::max(maxx,v.x);miny=::std::min(miny,v.y);maxy=::std::max(maxy,v.y);}
#ifdef __EMSCRIPTEN__
            glBegin(GL_TRIANGLES);
            glVertex3f(minx,miny,0);glVertex3f(maxx,miny,0);glVertex3f(maxx,maxy,0);
            glVertex3f(minx,miny,0);glVertex3f(maxx,maxy,0);glVertex3f(minx,maxy,0);
            glEnd();
#else
            glBegin(GL_QUADS);
            glVertex3f(minx,miny,0);glVertex3f(maxx,miny,0);
            glVertex3f(maxx,maxy,0);glVertex3f(minx,maxy,0);
            glEnd();
#endif
            glDisable(GL_STENCIL_TEST);
            glDisable(GL_BLEND);
            } // end else (non-TRIANGLES path)
        }
        if(useStroke && s.kind!=TRIANGLES){
            if(se) glColor4f(s.strokeR,s.strokeG,s.strokeB,s.strokeA);
            else   applyStroke();
            glLineWidth(se?s.strokeW:strokeW);
            glBegin(s.closed?GL_LINE_LOOP:GL_LINE_STRIP);
            for(auto& v:s.verts) glVertex3f(v.x,v.y,v.z);
            glEnd();
            if(!se) restoreLighting();
        }
    }
    glPopMatrix();
}
// cppmode-shapemode: Java's shapeMode() for where and how big shape() draws
static void lpShapeBounds(const PShape& s) {  // cppmode-modemap
    if (s.width > 0 || s.height > 0 || s.verts.empty()) return;
    float minx = s.verts[0].x, maxx = minx, miny = s.verts[0].y, maxy = miny;
    for (const auto& v : s.verts) { minx = ::std::min(minx, v.x); maxx = ::std::max(maxx, v.x);
                                    miny = ::std::min(miny, v.y); maxy = ::std::max(maxy, v.y); }
    const_cast<PShape&>(s).width = maxx - minx; const_cast<PShape&>(s).height = maxy - miny;
}
void PApplet::shape(const PShape& s,float x,float y) {
    lpShapeBounds(s);
    if (shapeDrawMode == CENTER) { x -= s.width * 0.5f; y -= s.height * 0.5f; }
    drawPShape(s, x, y);
}
void PApplet::shape(const PShape& s,float x,float y,float w,float h) {
    lpShapeBounds(s);
    if (shapeDrawMode == CORNERS)     { w -= x; h -= y; }
    else if (shapeDrawMode == CENTER) { x -= w * 0.5f; y -= h * 0.5f; }
    drawPShape(s, x, y, w, h);
}

// =============================================================================
// PFONT / TYPOGRAPHY
// =============================================================================
// Internal helper -- try to load a TTF by path
static ::std::string g_lastTTFPath;   // file behind the active OpenGL font
bool PApplet::tryLoadTTF(const ::std::string& path, float size) {
#if PROCESSING_HAS_STB_TRUETYPE
    if (loadTTFFile(path)) {
        g_ttf.loaded = true;
        g_lastTTFPath = path;
        g_textSize   = size;
        // bakeAtlas deferred to first text() call — GL context not ready yet
        g_ttf.bakeSize = -1.0f; // force bake on next text() call
        return true;
    }
#endif
    return false;
}

// loadFont -- loads a .ttf file; falls back to bitmap font gracefully
PFont PApplet::loadFont(const ::std::string& filename) {
    PFont f(filename, g_textSize);
    tryLoadTTF(filename, g_textSize);
    return f;
}

// createFont -- creates font from a name/path and size
static ::std::vector<PFont>* _fontPool_ptr = nullptr;
static ::std::vector<PFont>& _fontPool = *(_fontPool_ptr ? _fontPool_ptr : (_fontPool_ptr = new ::std::vector<PFont>()));
PFont* PApplet::createFont(const ::std::string& name, float size, bool /*smooth*/) {
    PFont f(name, size);
    // Try as file path first, then common system paths
    // Strip extension from name if already present, to avoid double extensions
    ::std::string baseName = name;
    if (baseName.size() > 4 && (baseName.substr(baseName.size()-4) == ".ttf"
                              || baseName.substr(baseName.size()-4) == ".otf")) {
        // name already has extension -- use as-is for first attempt
    } else {
        baseName = ""; // will build paths with extensions below
    }

    ::std::string nameNoExt = name;
    if (nameNoExt.size() > 4 && (nameNoExt.substr(nameNoExt.size()-4) == ".ttf"
                               || nameNoExt.substr(nameNoExt.size()-4) == ".otf"))
        nameNoExt = nameNoExt.substr(0, nameNoExt.size()-4);

    ::std::vector<::std::string> paths = {
        // Try exact name first (may already have extension)
        name,
        // Try adding extensions if not already present
        nameNoExt + ".ttf",
        nameNoExt + ".otf",
        // Sketch data/ folder
        ::std::string("data/") + name,
        ::std::string("data/") + nameNoExt + ".ttf",
        ::std::string("data/") + nameNoExt + ".otf",
        // Sketch fonts/ folder
        ::std::string("fonts/") + name,
        ::std::string("fonts/") + nameNoExt + ".ttf",
        ::std::string("fonts/") + nameNoExt + ".otf",
        // Linux system font directories
        ::std::string("/usr/share/fonts/truetype/") + nameNoExt + ".ttf",
        ::std::string("/usr/share/fonts/opentype/") + nameNoExt + ".otf",
        ::std::string("/usr/share/fonts/TTF/") + nameNoExt + ".ttf",
        ::std::string("/usr/share/fonts/OTF/") + nameNoExt + ".otf",
        // Windows system fonts
        ::std::string("C:/Windows/Fonts/") + name,
        ::std::string("C:/Windows/Fonts/") + nameNoExt + ".ttf",
        ::std::string("C:/Windows/Fonts/") + nameNoExt + ".otf",
        // macOS
        ::std::string("/Library/Fonts/") + name,
        ::std::string("/System/Library/Fonts/") + name,
    };

    // cppmode-fontname: ask fontconfig for an installed font by name, the way Java finds
    // system fonts ("DejaVu Sans", "DejaVu Sans Bold", "Arial"). fc-match always answers
    // with *some* font, so only take it when its family or full name is what was asked.
    #ifndef _WIN32
    {
        auto norm = [](::std::string s) { ::std::string o; for (char c : s) if (c != ' ' && c != '-' && c != '_') o += (char)tolower((unsigned char)c); return o; };
        auto fcmatch = [&](const ::std::string& pattern) -> ::std::string {
            ::std::string q; for (char c : pattern) { if (c == '\'') q += "'\\''"; else q += c; }
            FILE* p = processingOpenPipe(("fc-match -f '%{family}|%{fullname}|%{file}' '" + q + "' 2>/dev/null").c_str(), "r");
            if (!p) return "";
            char buf[1024]; ::std::string out; while (fgets(buf, sizeof buf, p)) out += buf;
            processingClosePipe(p);
            size_t a = out.find('|'), b = out.find('|', a == ::std::string::npos ? 0 : a + 1);
            if (a == ::std::string::npos || b == ::std::string::npos) return "";
            ::std::string fams = out.substr(0, a) + "," + out.substr(a + 1, b - a - 1) + ",", file = out.substr(b + 1);
            while (!file.empty() && (file.back() == '\n' || file.back() == '\r')) file.pop_back();
            ::std::string want = norm(nameNoExt);
            for (size_t s = 0, e; (e = fams.find(',', s)) != ::std::string::npos; s = e + 1)
                if (norm(fams.substr(s, e - s)) == want) return file;
            return "";
        };
        ::std::string hit = fcmatch(nameNoExt);
        if (hit.empty()) {  // "DejaVu Sans Bold" -> family "DejaVu Sans", style "Bold"
            size_t sp = nameNoExt.find_last_of(" -");
            if (sp != ::std::string::npos) hit = fcmatch(nameNoExt.substr(0, sp) + ":style=" + nameNoExt.substr(sp + 1));
        }
        if (!hit.empty()) paths.insert(paths.begin(), hit);
    }
    #endif
    // Also search system font dirs recursively (Linux: fc-list output)
    #ifndef _WIN32
    {
        FILE* fc = processingOpenPipe(("fc-list : file | grep -i '" + nameNoExt + "' | head -5").c_str(), "r");
        if (fc) {
            char buf[512];
            while (fgets(buf, sizeof(buf), fc)) {
                ::std::string line(buf);
                // fc-list format: /path/to/font.ttf: Family:style
                size_t colon = line.find(':');
                if (colon != ::std::string::npos) {
                    ::std::string fpath = line.substr(0, colon);
                    // strip whitespace
                    fpath.erase(0, fpath.find_first_not_of(" \t"));
                    fpath.erase(fpath.find_last_not_of(" \t\n\r") + 1);
                    if (!fpath.empty()) paths.push_back(fpath);
                }
            }
            processingClosePipe(fc);
        }
    }
    #endif

    bool found = false;
    for (auto& p : paths) {
        if (tryLoadTTF(p, size)) { found = true; break; }
    }
    if (found) { f.path = g_lastTTFPath; f.loaded = true; }
    if (!found) {
        ::std::cerr << "[font] Could not find font: " << name << "\n";
        ::std::cerr << "[font] Put the .ttf file in your sketch's data/ folder\n";
    }
    _fontPool.push_back(::std::move(f)); return &_fontPool.back();
}

// textFont -- switch to a previously loaded font
void PApplet::textFont(const PFont& font) {
    currentFont = font;
    const ::std::string& fp = font.path.empty() ? font.name : font.path;
    tryLoadTTF(fp, font.size);
    if (_backend) _backend->text_font_file(fp);
    textSize(font.size);   // also resets leading, as in Java
}
void PApplet::textFont(const PFont& font, float size) {
    currentFont      = font;
    currentFont.size = size;
    g_textSize       = size;
    tryLoadTTF(font.path.empty() ? font.name : font.path, size);
    if (_backend) { _backend->text_font_file(font.path.empty() ? font.name : font.path); _backend->text_size(size); }
}

// =============================================================================
// TEXTURE
// =============================================================================
void PApplet::textureMode(int mode){ textureModeVal=mode; }
void PApplet::textureWrap(int mode){
    textureWrapVal=mode;
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend){ uint8_t w = mode == REPEAT ? 1 : 0; if (g_lpTexImg) processing_image_set_sampler(g_lpTexImg, 0, w, w); return; }
#endif
    GLint wrap=mode==REPEAT?GL_REPEAT:GL_CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);
}
void PApplet::texture(PImage& img){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if(_backend){
        bool fresh = !img.lp_image_id || img.dirty;
        if (!lpEnsureImage(_backend, &img)) { g_lpTexImg = 0; return; }
        g_lpTexImg = img.lp_image_id;
        g_lpTexW = img.width > 0 ? (float)img.width : 1.0f;
        g_lpTexH = img.height > 0 ? (float)img.height : 1.0f;
        if (fresh) {
            uint8_t w = textureWrapVal == REPEAT ? 1 : 0;
            processing_image_set_sampler(g_lpTexImg, 0, w, w);
        }
        static ::std::map<uint64_t, bool> alphaOf;
        auto it = alphaOf.find(g_lpTexImg);
        if (it == alphaOf.end()) {
            bool a = false;
            for (unsigned int c : img.pixels) if ((c >> 24) != 0xff) { a = true; break; }
            it = alphaOf.emplace(g_lpTexImg, a).first;
        }
        g_lpTexAlpha = it->second;
        return; }
#endif
    if(img.dirty)img.uploadTexture();
    if(img.texID){ glEnable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D,img.texID); }
}

// =============================================================================
// FILE / IO HELPERS
// =============================================================================

BufferedReader* PApplet::createReader(const ::std::string& path){ return new BufferedReader(path); }
PrintWriter* PApplet::createWriter(const ::std::string& path){ return new PrintWriter(path); }

// cppmode-select: native file dialogs. Linux: zenity, then kdialog. macOS: osascript.
// Windows: PowerShell + Windows Forms. Returns false when cancelled or no dialog tool exists.
bool File::exists() const { struct stat st; return !path.empty() && stat(path.c_str(), &st) == 0; }
bool File::isDirectory() const { struct stat st; return !path.empty() && stat(path.c_str(), &st) == 0 && (st.st_mode & S_IFDIR); }
static ::std::string cppmodeShellQuote(const ::std::string& s) {
#ifdef _WIN32
    ::std::string r = "'"; for (char ch : s) { if (ch == '\'') r += "''"; else if (ch == '"') r += "`\""; else r += ch; } return r + "'";
#else
    ::std::string r = "'"; for (char ch : s) { if (ch == '\'') r += "'\\''"; else r += ch; } return r + "'";
#endif
}
static bool cppmodeRunDialog(const ::std::string& cmd, ::std::string& out) {
    FILE* p = processingOpenPipe(cmd.c_str(), "r"); if (!p) return false;
    char buf[4096]; ::std::string r;
    while (fgets(buf, sizeof buf, p)) r += buf;
    int rc = processingClosePipe(p);
    while (!r.empty() && (r.back() == '\n' || r.back() == '\r')) r.pop_back();
    out = r;
    return rc == 0 && !r.empty();
}
// kind: 0 open file, 1 save file, 2 folder
static bool cppmodeChoose(int kind, const ::std::string& prompt, ::std::string& out) {
#ifdef __EMSCRIPTEN__
    (void)kind; (void)prompt; (void)out; return false;
#elif defined(_WIN32)
    ::std::string ps;
    if (kind == 2) ps = "Add-Type -AssemblyName System.Windows.Forms; $d=New-Object System.Windows.Forms.FolderBrowserDialog; $d.Description=" + cppmodeShellQuote(prompt) + "; if($d.ShowDialog() -eq 'OK'){$d.SelectedPath}";
    else ps = ::std::string("Add-Type -AssemblyName System.Windows.Forms; $d=New-Object System.Windows.Forms.") + (kind == 1 ? "SaveFileDialog" : "OpenFileDialog") + "; $d.Title=" + cppmodeShellQuote(prompt) + "; if($d.ShowDialog() -eq 'OK'){$d.FileName}";
    return cppmodeRunDialog("powershell -NoProfile -STA -Command \"" + ps + "\"", out);
#elif defined(__APPLE__)
    ::std::string as = kind == 2 ? "choose folder" : kind == 1 ? "choose file name" : "choose file";
    ::std::string pr = prompt; for (auto& ch : pr) if (ch == '"') ch = '\'';
    if (!pr.empty()) as += " with prompt \"" + pr + "\"";
    return cppmodeRunDialog("osascript -e " + cppmodeShellQuote("POSIX path of (" + as + ")") + " 2>/dev/null", out);
#else
    ::std::string t = cppmodeShellQuote(prompt.empty() ? ::std::string("Select") : prompt);
    ::std::string z = ::std::string("zenity --file-selection") + (kind == 1 ? " --save --confirm-overwrite" : kind == 2 ? " --directory" : "") + " --title=" + t + " 2>/dev/null";
    if (cppmodeRunDialog("command -v zenity >/dev/null 2>&1 && " + z, out)) return true;
    ::std::string k = ::std::string("kdialog ") + (kind == 1 ? "--getsavefilename ." : kind == 2 ? "--getexistingdirectory ." : "--getopenfilename .") + " --title " + t + " 2>/dev/null";
    return cppmodeRunDialog("command -v kdialog >/dev/null 2>&1 && " + k, out);
#endif
}
static bool cppmodeIsCallbackName(const ::std::string& s) {
    if (s.empty() || !(::std::isalpha((unsigned char)s[0]) || s[0] == '_')) return false;
    for (char ch : s) if (!(::std::isalnum((unsigned char)ch) || ch == '_')) return false;
    return true;
}
struct CppmodeSelection { bool folder; ::std::string path; };
static ::std::mutex g_selMutex;
static ::std::vector<CppmodeSelection> g_selDone;
static void cppmodeSelectAsync(int kind, const ::std::string& prompt) {
#if defined(__EMSCRIPTEN__) && !defined(__EMSCRIPTEN_PTHREADS__)
    // cppmode-webselect: no file dialogs or threads in the browser; report a cancel right away
    { ::std::lock_guard<::std::mutex> l(g_selMutex); g_selDone.push_back({kind == 2, ::std::string()}); (void)prompt; return; }
#endif
    ::std::thread([kind, prompt] {
        ::std::string p; if (!cppmodeChoose(kind, prompt, p)) p.clear();
        ::std::lock_guard<::std::mutex> l(g_selMutex); g_selDone.push_back({kind == 2, p});
    }).detach();
}
void PApplet::cppmodeDeliverSelections() {
    ::std::vector<CppmodeSelection> done;
    { ::std::lock_guard<::std::mutex> l(g_selMutex); done.swap(g_selDone); }
    for (auto& s : done) {
        File f(s.path);
        if (s.folder) { folderSelected(f); folderSelected(s.path.empty() ? nullptr : &f); }
        else          { fileSelected(f);   fileSelected(s.path.empty() ? nullptr : &f); }
    }
}
::std::string PApplet::selectInput(const ::std::string& prompt, const ::std::string& cb) {
    if (cppmodeIsCallbackName(cb)) { cppmodeSelectAsync(0, prompt); return ""; }
    ::std::string p; return cppmodeChoose(0, prompt, p) ? p : "";
}
::std::string PApplet::selectOutput(const ::std::string& prompt, const ::std::string& cb) {
    if (cppmodeIsCallbackName(cb)) { cppmodeSelectAsync(1, prompt); return ""; }
    ::std::string p; return cppmodeChoose(1, prompt, p) ? p : "";
}
::std::string PApplet::selectFolder(const ::std::string& prompt, const ::std::string& cb) {
    if (cppmodeIsCallbackName(cb)) { cppmodeSelectAsync(2, prompt); return ""; }
    ::std::string p; return cppmodeChoose(2, prompt, p) ? p : "";
}

PImage* PApplet::requestImage(const ::std::string& path){
    // Create a placeholder: width=-1 means "still loading", width=0 means "failed"
    // We use a heap PImage and fill it in from a background thread.
    // The sketch checks img->width != 0 && img->width != -1 to detect completion.
    PImage* img = new PImage();
    img->width  = -1;  // sentinel: loading in progress
    img->height = -1;
    ::std::thread([img, path]{
        // Resolve search paths same as loadImage
        // Check PROCESSING_SKETCH_PATH env var set by IDE
    ::std::string _sketchDir = processingEnvironmentVariable("PROCESSING_SKETCH_PATH");
    if (!_sketchDir.empty()) _sketchDir += "/";
    // Also get the directory of the running executable
    ::std::string _exeDir;
    {
        char _buf[4096] = {};
#ifdef _WIN32
        GetModuleFileNameA(nullptr, _buf, sizeof(_buf));
        ::std::string _ep(_buf);
        size_t _sl = _ep.find_last_of("\\\\");
#else
#ifndef __EMSCRIPTEN__
        ssize_t _len = readlink("/proc/self/exe", _buf, sizeof(_buf)-1);
        if (_len > 0) _buf[_len] = 0;
#endif
        ::std::string _ep(_buf);
        size_t _sl = _ep.find_last_of("/");
#endif
        if (_sl != ::std::string::npos) _exeDir = _ep.substr(0, _sl+1);
    }
    ::std::vector<::std::string> tries = {
            path,
            "data/" + path,
            "files/" + path,
        };
        ::std::string found;
        for (auto& t : tries) {
            FILE* f = processingOpenFile(t.c_str(), "rb");
            if (f) { fclose(f); found = t; break; }
        }
        if (found.empty()) {
            // Not found: mark as failed (width=0)
            img->width  = 0;
            img->height = 0;
            ::std::cerr << "requestImage: file not found: " << path << "\n";
            return;
        }
#ifdef PROCESSING_HAS_STB_IMAGE
        int w=0, h=0, ch=0;
        unsigned char* data = stbi_load(found.c_str(), &w, &h, &ch, 4);
        if (!data || w<=0 || h<=0) {
            img->width  = 0;
            img->height = 0;
            ::std::cerr << "requestImage: failed to decode: " << path << "\n";
            return;
        }
        img->pixels.resize((size_t)w * h);
        for (int i = 0; i < w*h; i++) {
            unsigned char r=data[i*4+0], g=data[i*4+1],
                          b=data[i*4+2], a=data[i*4+3];
            img->pixels[i] = ((unsigned int)a<<24)|((unsigned int)r<<16)|
                              ((unsigned int)g<<8)|(unsigned int)b;
        }
        stbi_image_free(data);
        img->dirty  = true;
        // Write width/height last so the sketch sees a consistent state
        img->height = h;
        img->width  = w;   // width != -1 signals "done"
#else
        img->width  = 0;
        img->height = 0;
        ::std::cerr << "requestImage: rebuild with -DPROCESSING_HAS_STB_IMAGE: " << path << "\n";
#endif
    }).detach();
    return img;
}

// =============================================================================
// PSHADER IMPLEMENTATION
// =============================================================================

// cppmode-lpshader: shaders in the libprocessing renderer. libprocessing takes WGSL/WESL, not GLSL:
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
    s->lp_shader = processing_shader_create(src.c_str());  // source, not path: the fork has no asset root here
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

static ::std::string readShaderFile(const ::std::string& path){
    ::std::ifstream f(path); if(!f)return "";
    return ::std::string((::std::istreambuf_iterator<char>(f)),::std::istreambuf_iterator<char>());
}

PShader* PApplet::loadShader(const ::std::string& fragPath,const ::std::string& vertPath){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { (void)vertPath; return cppmodeLpLoadShader(fragPath); }  // cppmode-lpshader
#endif
    ::std::string fSrc=readShaderFile(fragPath);
    ::std::string vSrc=vertPath.empty()?
        "#version 120\nvoid main(){gl_Position=ftransform();gl_TexCoord[0]=gl_MultiTexCoord0;gl_FrontColor=gl_Color;}":
        readShaderFile(vertPath);
    if(fSrc.empty()){::std::cerr<<"loadShader: could not read "<<fragPath<<"\n";return nullptr;}
    PShader* s=new PShader(vSrc,fSrc);
    s->compile();
    return s;
}
void PApplet::shader(PShader& s){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {  // cppmode-lpshader
        if (s.lp_material) processing_material(_backend->graphics_id(), s.lp_material);
        else if (s.lp_filter) fprintf(stderr, "shader(): that's a filter shader (processing::filter), use filter(shader) instead\n");
        activeShader = &s; return;
    }
#endif
    s.bind(); activeShader=&s; }
void PApplet::resetShader(){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { processing_reset_material(_backend->graphics_id()); activeShader = nullptr; return; }  // cppmode-lpshader
#endif
    glUseProgram(0); activeShader=nullptr; }
void PApplet::filter(PShader& s){  // cppmode-lpshader
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) {
        if (!s.lp_filter) { fprintf(stderr, "filter(shader): not a filter shader (it needs import processing::filter)\n"); return; }
        processing_graphics_apply_filter(_backend->graphics_id(), s.lp_filter);
        if (const char* e = processing_check_error()) fprintf(stderr, "filter(shader): %s\n", e);
        return;
    }
#endif
    static bool warned = false;
    if (!warned) { fprintf(stderr, "filter(shader) works in the libprocessing renderer for now\n"); warned = true; }
    (void)s;
}

// =============================================================================
// DISPLAY blend() and copy()
// =============================================================================

void PApplet::blend(int sx,int sy,int sw,int sh,int dx,int dy,int dw,int dh,int mode){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { cppmodeLpBlit(this, sx, sy, sw, sh, dx, dy, dw, dh, mode, false); return; }  // cppmode-lpcopy
#endif
    ::std::vector<unsigned char> src(sw*sh*4);
    glReadPixels(sx,winHeight-(sy+sh),sw,sh,GL_RGBA,GL_UNSIGNED_BYTE,src.data());
    ::std::vector<unsigned char> dst(dw*dh*4);
    for(int y=0;y<dh;y++) for(int x=0;x<dw;x++){
        int srcX=(int)(x*(float)sw/dw), srcY=(int)(y*(float)sh/dh);
        for(int c=0;c<4;c++) dst[(y*dw+x)*4+c]=src[(srcY*sw+srcX)*4+c];
    }
    GLenum sfact=GL_SRC_ALPHA,dfact=GL_ONE_MINUS_SRC_ALPHA;
    switch(mode){
        case ADD:     sfact=GL_SRC_ALPHA;dfact=GL_ONE;break;
        case MULTIPLY:sfact=GL_DST_COLOR;dfact=GL_ZERO;break;
        default: break;
    }
    glEnable(GL_BLEND);glBlendFunc(sfact,dfact);
    glWindowPos2i(dx,winHeight-(dy+dh));
    glDrawPixels(dw,dh,GL_RGBA,GL_UNSIGNED_BYTE,dst.data());
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void PApplet::copy(int sx,int sy,int sw,int sh,int dx,int dy,int dw,int dh){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (_backend) { cppmodeLpBlit(this, sx, sy, sw, sh, dx, dy, dw, dh, BLEND, true); return; }  // cppmode-lpcopy: copy replaces, like Java
#endif
    blend(sx,sy,sw,sh,dx,dy,dw,dh,BLEND);
}

// =============================================================================
// getRegion
// =============================================================================

PImage getRegion(int x,int y,int w,int h){
#ifdef CPPMODE_BACKEND_LIBPROCESSING
    if (PApplet::g_papplet && PApplet::g_papplet->_backend) {  // cppmode-lpcopy
        PImage img(w > 0 ? w : 1, h > 0 ? h : 1);
        ::std::vector<unsigned int> px; int W = 0, H = 0; float s = 1;
        if (w <= 0 || h <= 0 || !cppmodeLpRead(PApplet::g_papplet, px, W, H, s)) return img;
        for (int iy = 0; iy < h; iy++) for (int ix = 0; ix < w; ix++) {
            int X = (int)((x + ix) * s), Y = (int)((y + iy) * s);
            img.pixels[(size_t)iy * w + ix] = (X >= 0 && Y >= 0 && X < W && Y < H) ? px[(size_t)Y * W + X] : 0u;
        }
        img.dirty = true;
        return img;
    }
#endif
    PImage img(w,h);
    ::std::vector<unsigned char> buf(w*h*4);
    glReadPixels(x,(PApplet::g_papplet?PApplet::g_papplet->winHeight:0)-(y+h),w,h,GL_RGBA,GL_UNSIGNED_BYTE,buf.data());
    for(int iy=0;iy<h;iy++) for(int ix=0;ix<w;ix++){
        int si=((h-1-iy)*w+ix)*4;
        int di=iy*w+ix;
        img.pixels[di]=(buf[si+3]<<24)|(buf[si]<<16)|(buf[si+1]<<8)|buf[si+2];
    }
    img.dirty=true;
    return img;
}

::std::vector<::std::string> PApplet::loadStrings(const ::std::string& path) {
    ::std::vector<::std::string> lines;
    ::std::string sketchDir = processingEnvironmentVariable("PROCESSING_SKETCH_PATH");
    if (!sketchDir.empty()) sketchDir += "/";
    ::std::ifstream f(path);
    if (!f) f.open(sketchDir + path);
    if (!f) f.open(sketchDir + "data/" + path);
    ::std::string line;
    while (::std::getline(f, line)) lines.push_back(line);
    return lines;
}
bool PApplet::saveStrings(const ::std::string& path, const ::std::vector<::std::string>& lines) {
    ::std::ofstream f(path); if (!f) return false;
    for (auto& l : lines) f << l << "\n"; return true;
}
::std::vector<unsigned char> PApplet::loadBytes(const ::std::string& path) {
    ::std::ifstream f(path, ::std::ios::binary);
    return ::std::vector<unsigned char>((::std::istreambuf_iterator<char>(f)), ::std::istreambuf_iterator<char>());
}
bool PApplet::saveBytes(const ::std::string& path, const ::std::vector<unsigned char>& data) {
    ::std::ofstream f(path, ::std::ios::binary); if (!f) return false;
    f.write(reinterpret_cast<const char*>(data.data()), data.size()); return true;
}
::std::vector<::std::string> PApplet::split(const ::std::string& s, char delim) {
    ::std::vector<::std::string> r; ::std::string t;
    for (char c : s) { if (c==delim){r.push_back(t);t.clear();}else t+=c; }
    r.push_back(t); return r;
}
::std::vector<::std::string> PApplet::splitTokens(const ::std::string& s, const ::std::string& delims) {
    ::std::vector<::std::string> r; ::std::string t;
    for (char c : s) { if (delims.find(c)!=::std::string::npos){if(!t.empty()){r.push_back(t);t.clear();}}else t+=c; }
    if (!t.empty()) r.push_back(t); return r;
}
::std::string PApplet::join(const ::std::vector<::std::string>& v, const ::std::string& sep) {
    ::std::string r; for (size_t i=0;i<v.size();i++){if(i)r+=sep;r+=v[i];} return r;
}
::std::string PApplet::trim(const ::std::string& s) {
    size_t a=s.find_first_not_of(" \t\n\r"), b=s.find_last_not_of(" \t\n\r");
    return a==::std::string::npos?"":s.substr(a,b-a+1);
}
::std::string PApplet::nf(int v)                                      { return Processing::nf(v); }
::std::string PApplet::nf(int v, int digits)                          { return Processing::nf(v, digits); }
::std::string PApplet::nf(float v, int digits)                        { return Processing::nf(v, digits); }
::std::string PApplet::nf(float v, int left, int right)                { return Processing::nf(v, left, right); }
::std::vector<::std::string> PApplet::nf(const ::std::vector<int>& nums)   { return Processing::nf(nums); }
::std::vector<::std::string> PApplet::nf(const ::std::vector<int>& nums, int digits) { return Processing::nf(nums, digits); }
::std::vector<::std::string> PApplet::nf(const ::std::vector<float>& nums, int left, int right) { return Processing::nf(nums, left, right); }

::std::string PApplet::nfc(int v)                                      { return Processing::nfc(v); }
::std::string PApplet::nfc(float v, int right)                         { return Processing::nfc(v, right); }
::std::vector<::std::string> PApplet::nfc(const ::std::vector<int>& nums)  { return Processing::nfc(nums); }
::std::vector<::std::string> PApplet::nfc(const ::std::vector<float>& nums, int right) { return Processing::nfc(nums, right); }

::std::string PApplet::nfp(int v)                                      { return Processing::nfp(v); }
::std::string PApplet::nfp(int v, int digits)                          { return Processing::nfp(v, digits); }
::std::string PApplet::nfp(float v, int left, int right)               { return Processing::nfp(v, left, right); }
::std::vector<::std::string> PApplet::nfp(const ::std::vector<int>& nums)  { return Processing::nfp(nums); }
::std::vector<::std::string> PApplet::nfp(const ::std::vector<int>& nums, int digits) { return Processing::nfp(nums, digits); }
::std::vector<::std::string> PApplet::nfp(const ::std::vector<float>& nums, int left, int right) { return Processing::nfp(nums, left, right); }

::std::string PApplet::nfs(int v)                                      { return Processing::nfs(v); }
::std::string PApplet::nfs(int v, int digits)                          { return Processing::nfs(v, digits); }
::std::string PApplet::nfs(float v, int left, int right)               { return Processing::nfs(v, left, right); }
::std::vector<::std::string> PApplet::nfs(const ::std::vector<int>& nums)  { return Processing::nfs(nums); }
::std::vector<::std::string> PApplet::nfs(const ::std::vector<int>& nums, int digits) { return Processing::nfs(nums, digits); }
::std::vector<::std::string> PApplet::nfs(const ::std::vector<float>& nums, int left, int right) { return Processing::nfs(nums, left, right); }
::std::string PApplet::hex(int v) {
    ::std::ostringstream ss; ss<<::std::uppercase<<::std::hex<<::std::setw(8)<<::std::setfill('0')<<v; return ss.str();
}
::std::string PApplet::binary(int v) {
    ::std::string r; for(int i=31;i>=0;i--) r+=((v>>i)&1)?'1':'0'; return r;
}

} // namespace Processing

#ifdef _MSC_VER
#pragma warning(pop)
#endif


// cppmode-parity2: PImage::blend and PImage::save (both renderers, plain CPU code)
namespace Processing {
void PImage::blend(const PImage& src, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh, int mode) {
    if (dw <= 0 || dh <= 0 || sw <= 0 || sh <= 0) return;
    for (int iy = 0; iy < dh; iy++)
        for (int ix = 0; ix < dw; ix++) {
            int x = dx + ix, y = dy + iy;
            if (x < 0 || y < 0 || x >= width || y >= height) continue;
            int srcX = sx + (int)(ix * (float)sw / dw), srcY = sy + (int)(iy * (float)sh / dh);
            set(x, y, blendColor(get(x, y), src.get(srcX, srcY), mode));
        }
    dirty = true;
}
bool PImage::save(const ::std::string& filename) const {
    if (width <= 0 || height <= 0 || (int)pixels.size() < width * height) return false;
    ::std::string fn = filename;
    if (!fn.empty() && fn[0] != '/') {
        ::std::string base = processingEnvironmentVariable("PROCESSING_SKETCH_PATH");
        if (!base.empty()) fn = base + "/" + fn;
    }
    ::std::vector<unsigned char> rgba((size_t)width * height * 4);
    for (size_t i = 0; i < (size_t)width * height; i++) {
        unsigned int p = pixels[i];
        rgba[i*4+0] = (p >> 16) & 255; rgba[i*4+1] = (p >> 8) & 255; rgba[i*4+2] = p & 255; rgba[i*4+3] = (p >> 24) & 255;
    }
    ::std::vector<unsigned char> rgb((size_t)width * height * 3);
    for (size_t i = 0; i < (size_t)width * height; i++) { rgb[i*3] = rgba[i*4]; rgb[i*3+1] = rgba[i*4+1]; rgb[i*3+2] = rgba[i*4+2]; }
    ::std::string ext = fn.size() >= 4 ? fn.substr(fn.size() - 4) : "";
    for (auto& ch : ext) ch = (char)::std::tolower((unsigned char)ch);
    int ok = 0;
    if (ext == ".jpg" || ext == "jpeg")      ok = stbi_write_jpg(fn.c_str(), width, height, 3, rgb.data(), 95);
    else if (ext == ".bmp")                  ok = stbi_write_bmp(fn.c_str(), width, height, 3, rgb.data());
    else if (ext == ".tga" || ext == ".tif" || ext == "tiff") ok = cppmodeWriteTgaOrTiff(fn.c_str(), ext, width, height, rgb.data());
    else { if (ext != ".png") fn += ".png"; ok = stbi_write_png(fn.c_str(), width, height, 4, rgba.data(), width * 4); }
    if (!ok) fprintf(stderr, "PImage.save: could not write %s\n", fn.c_str());
    return ok != 0;
}
} // namespace Processing
