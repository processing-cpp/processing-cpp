// Runtime checks for CppMode's Java-compatible helpers. Each CHECK compares against what
// Java Processing gives for the same call. No window is opened. Built by test_runtime.py.
#include "Processing.h"
#include <cstdio>
namespace Processing {

static int g_fail = 0, g_total = 0;
template<class A, class B>
static void check(const char* what, const A& got, const B& want, int line) {
    g_total++;
    std::ostringstream g, w; g << got; w << want;
    if (g.str() != w.str()) { g_fail++; std::printf("FAIL  line %d: %s\n      got  %s\n      want %s\n", line, what, g.str().c_str(), w.str().c_str()); }
}
#define CHECK(expr, want) check(#expr, (expr), (want), __LINE__)

// The checks run inside a PApplet subclass, the same place sketch code lives.
struct Checks : public PApplet {
  void setup() override {}
  void draw() override {}
  int runChecks() {
    // parseInt / parseFloat / parseBoolean (Processing's rules)
    CHECK(parseInt("42"), 42);           CHECK(parseInt("3.7"), 3);       CHECK(parseInt("-12"), -12);
    CHECK(parseInt("12abc"), 0);         CHECK(parseInt(" 5"), 0);        CHECK(parseInt(""), 0);
    CHECK(parseInt("99999999999"), 0);   CHECK(parseInt("x", -1), -1);
    CHECK(parseFloat("2.5"), 2.5f);      CHECK(parseFloat(" 1.5f "), 1.5f); CHECK(parseFloat("1e3"), 1000.0f);
    CHECK(std::isnan(parseFloat("abc")), true); CHECK(parseFloat("x", 0.f), 0.f);
    CHECK(parseBoolean("TRUE"), true);   CHECK(parseBoolean("1"), false); CHECK(parseBoolean("yes"), false);
    { String s = "17"; String f = "2.25"; CHECK(int(s), 17); CHECK(float(f), 2.25f); }

    // strings
    CHECK(String("hello").hashCode(), 99162322);
    CHECK(String("").hashCode(), 0);
    { auto p = split("a::b::", "::"); CHECK(p.size(), 3u); CHECK(join(p, "|"), "a|b|"); }
    { auto p = split("a,b,,c", ','); CHECK(p.size(), 4u); }
    { auto p = splitTokens("  a b\tc  "); CHECK(p.size(), 3u); }
    CHECK(trim("  x \t"), "x");
    CHECK(nf(3, 3), "003");              CHECK(nf(3.14159f, 1, 2), "3.14");  CHECK(nfc(1234567), "1,234,567");
    CHECK(hex(255, 2), "FF");            CHECK(binary(5, 4), "0101");        CHECK(unhex("FF"), 255);

    // IntDict / FloatDict / StringDict
    {
        IntDict d; d.set("b", 2); d.set("a", 5); d.increment("c"); d.increment("c"); d.add("b", 10);
        CHECK(join(d.keyArray(), ","), "b,a,c");
        CHECK(d.get("b"), 12); CHECK(d.sum(), 19); CHECK(d.maxKey(), "b"); CHECK(d.minKey(), "c");
        d.sortValues();        CHECK(join(d.keyArray(), ","), "c,a,b");
        d.sortKeysReverse();   CHECK(join(d.keyArray(), ","), "c,b,a");
        CHECK(d.remove("a"), 5); CHECK(d.size(), 2); CHECK(d.hasKey("a"), false); CHECK(d.get("zz", -1), -1);
        FloatDict f; f.set("x", 1.5f); f.mult("x", 2); CHECK(f.get("x"), 3.0f);
        StringDict s; s.set("k2", "b"); s.set("k1", "a"); s.sortKeys(); CHECK(s.key(0), "k1"); CHECK(s.value(0), "a");
    }

    // IntList / FloatList / StringList
    {
        IntList il; il.append(1); il.append(2); il.add(0, 10); il.insert(0, 7); CHECK(il.join(","), "7,11,2");
        CHECK(il.sum(), 20); CHECK(il.max(), 11); CHECK(il.min(), 2);
        FloatList fl; fl.append(1.f); fl.add(0, 2.f); fl.mult(0, 2.f); CHECK(fl.get(0), 6.0f); CHECK(fl.size(), 1);
        StringList sl; sl.append("ab"); sl.append("cd"); sl.upper(); CHECK(sl.join(","), "AB,CD");
    }

    // Java-style static classes (CppMode turns Math.abs into Math::abs)
    CHECK(Integer::MAX_VALUE, 2147483647); CHECK(Integer::MIN_VALUE, -2147483647 - 1);
    CHECK(Integer::toHexString(255), "ff"); CHECK(Integer::toBinaryString(5), "101");
    CHECK(Math::abs(-3), 3); CHECK(Math::max(2, 7), 7);
    CHECK(Float::isNaN(Float::NaN), true);
    CHECK(System::currentTimeMillis() > 1600000000000LL, true);

    // math helpers
    CHECK(constrain(5, 0, 3), 3);  CHECK(map(5, 0, 10, 0, 100), 50.0f); CHECK(norm(5, 0, 10), 0.5f);
    CHECK(lerp(0, 10, .5f), 5.0f); CHECK(dist(0, 0, 3, 4), 5.0f);      CHECK(mag(3, 4), 5.0f);
    CHECK(round(2.5f), 3);         CHECK(round(-2.5f), -2);            CHECK(sq(3), 9.0f);

    // color
    {
        color c = color(255, 128, 0);
        CHECK(red(c), 255.0f); CHECK(green(c), 128.0f); CHECK(blue(c), 0.0f); CHECK(alpha(c), 255.0f);
        CHECK(hex(c), "FFFF8000");
    }

    // PImage pixel ops (CPU side)
    {
        PImage* a = createImage(4, 4, ARGB); PImage* b = createImage(4, 4, ARGB);
        for (auto& p : a->pixels) p = 0xFF0000FFu;
        for (auto& p : b->pixels) p = 0xFFFF0000u;
        a->blend(b, 0, 0, 4, 4, 0, 0, 2, 2, ADD);
        CHECK(hex(a->get(0, 0)), "FFFF00FF"); CHECK(hex(a->get(3, 3)), "FF0000FF");
        PImage* c = a->get(0, 0, 2, 2); CHECK(c->width, 2); CHECK(hex(c->get(1, 1)), "FFFF00FF");
        c->resize(4, 0); CHECK(c->height, 4);
    }

    // fill/stroke(color, alpha) and PShape color setters like Java (cppmode-coloralpha)
    {
        color red = color(255, 0, 0);
        stroke(red, 100); CHECK(strokeR, 1.0f); CHECK(strokeG, 0.0f); CHECK((int)(strokeA * 255 + 0.5f), 100);
        colorMode(RGB, 1.0f); fill(red, 0.5f); CHECK((int)(fillA * 255 + 0.5f), 128); colorMode(RGB, 255);
        PShape ps;
        ps.setFill(color(0, 255, 0)); CHECK(ps.fillG, 1.0f); ps.setFill(false); CHECK(ps.hasFill, false);
        ps.setStroke(0xFF0000FFu); CHECK(ps.strokeB, 1.0f); CHECK(ps.hasStroke, true);
    }
    std::printf("%d/%d runtime checks pass\n", g_total - g_fail, g_total);
    return g_fail ? 1 : 0;
  }
};
} // namespace Processing

int main() { Processing::Checks c; return c.runChecks(); }
