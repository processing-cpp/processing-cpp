#!/usr/bin/env python3
"""CppMode: probe code generation with small sketches that use common patterns.
Each one goes through CppCLI and g++ -fsyntax-only in both renderers. Nothing runs.
Run from the CppMode folder:  python3 scripts/codegen_probe.py [name-filter]"""
import os, subprocess, sys, tempfile, concurrent.futures as cf
from pathlib import Path

MODE = Path.cwd()
if not (MODE / "mode/CppMode.jar").exists(): sys.exit("run this from the CppMode folder")
LP_INC = Path.home() / "lp-fork/crates/processing_ffi/include"
CLI = ["java", "-cp", str(MODE / "mode/CppMode.jar"), "processing.mode.cpp.CppCLI", "--mode-folder", str(MODE),
       "--native-lib", str(MODE / "libs/linux-x64/libcppmode-ts.so")]
GPP = ["g++", "-std=c++2c", "-fsyntax-only", "-w", "-x", "c++", "-I", str(MODE / "src")]

S = {}
S["inheritance"] = """
class Shape { public: float x, y; Shape(float x_, float y_) { x = x_; y = y_; } virtual void show() { point(x, y); } virtual ~Shape() {} };
class Box : public Shape { public: Box(float x_, float y_) : Shape(x_, y_) {} void show() override { rect(x, y, 10, 10); } };
Array<Shape*> shapes(2);
void setup() { size(200, 200); shapes[0] = new Shape(10, 10); shapes[1] = new Box(50, 50); }
void draw() { for (int i = 0; i < 2; i++) shapes[i]->show(); }
"""
S["class_uses_later_class"] = """
class Owner { public: Pet* pet = nullptr; void feed() { if (pet) pet->eat(); } };
class Pet { public: int food = 0; void eat() { food++; } };
Owner o;
void setup() { o.pet = new Pet(); o.feed(); println(o.pet->food); }
"""
S["static_member"] = """
class Counter { public: static int count; Counter() { count++; } };
int Counter::count = 0;
void setup() { Counter a; Counter b; println(Counter::count); }
"""
S["overloads_from_class"] = """
float scaleIt(float v) { return v * 2; }
float scaleIt(float v, float k) { return v * k; }
class User { public: float go() { return scaleIt(1.0) + scaleIt(1.0, 3.0); } };
User u;
void setup() { println(u.go()); }
"""
S["default_args_from_class"] = """
void mark(float x, float y, float r = 5) { circle(x, y, r); }
class Dot { public: void show() { mark(10, 10); mark(20, 20, 8); } };
Dot d;
void setup() { size(100, 100); }
void draw() { d.show(); }
"""
S["recursion"] = """
int fib(int n) { return n < 2 ? n : fib(n - 1) + fib(n - 2); }
void setup() { println(fib(10)); }
"""
S["const_array_bound"] = """
const int N = 4;
float vals[N] = {1, 2, 3, 4};
class Avg { public: float get() { float s = 0; for (int i = 0; i < N; i++) s += vals[i]; return s / N; } };
Avg a;
void setup() { println(a.get()); }
"""
S["enum_switch"] = """
enum Mode { IDLE, RUN, STOP };
Mode mode = RUN;
void setup() { switch (mode) { case IDLE: println("idle"); break; case RUN: println("run"); break; default: break; } }
"""
S["key_compare"] = """
void setup() { size(100, 100); }
void draw() {}
void keyPressed() { if (key == 'a') println("a"); if (keyCode == UP) println("up"); if (key == CODED) println("coded"); }
"""
S["mouse_events"] = """
float px = 0;
void setup() { size(100, 100); }
void draw() { if (mousePressed) px = mouseX; }
void mousePressed() { println("press " + mouseX); }
void mouseDragged() { px = mouseX; }
void mouseReleased() { println("release"); }
void mouseWheel(MouseEvent e) { println(e.getCount()); }
"""
S["string_methods"] = """
String s = "Hello World";
void setup() { println(s.length()); println(s.substring(0, 5)); println(s.charAt(1)); println(s.indexOf("World")); println(s.toUpperCase()); println(s.equals("x") ? "same" : "diff"); }
"""
S["template_fn"] = """
template <typename T> T biggest(T a, T b) { return a > b ? a : b; }
void setup() { println(biggest(3, 7)); println(biggest(2.5, 1.5)); }
"""
S["std_vector"] = """
#include <vector>
std::vector<int> nums;
void setup() { for (int i = 0; i < 5; i++) nums.push_back(i * i); for (int n : nums) println(n); }
"""
S["lambda"] = """
#include <functional>
std::function<float(float)> f = [](float x) { return x * x; };
void setup() { println(f(3)); }
"""
S["value_global_runtime_ctor"] = """
class Grid { public: int w, h; Grid(int w_, int h_) { w = w_; h = h_; } };
Grid grid(width / 10, height / 10);
class Cell { public: int cols() { return grid.w; } };
Cell c;
void setup() { size(200, 100); println(c.cols()); }
"""
S["pointer_global_new_in_setup"] = """
class Ball { public: float x = 0; void move() { x += speed; } };
float speed = 2;
Ball* ball;
void setup() { ball = new Ball(); }
void draw() { ball->move(); }
"""
S["function_returns_class"] = """
Ball makeBall() { return Ball(); }
class Ball { public: float r = 10; };
void setup() { Ball b = makeBall(); println(b.r); }
"""
S["class_calls_later_sketch_fn"] = """
class Walker { public: void step() { advance(1); } };
float pos = 0;
void advance(float d) { pos += d; }
Walker w;
void setup() { w.step(); println(pos); }
"""
S["mutual_pointers"] = """
class A; class B;
class A { public: B* other = nullptr; };
class B { public: A* other = nullptr; };
void setup() { A* a = new A(); B* b = new B(); a->other = b; b->other = a; println("ok"); }
"""
S["user_settings"] = """
void settings() { size(300, 200); smooth(4); }
void setup() { background(0); }
void draw() { circle(width / 2, height / 2, 50); }
"""
S["static_mode"] = """
size(200, 200);
background(255);
for (int i = 0; i < 10; i++) { line(i * 20, 0, i * 20, height); }
int count = 3;
println("count " + count);
"""
S["draw_only_runtime_global"] = """
float cx = width / 2;
class Mark { public: void show() { circle(cx, 50, 10); } };
Mark m;
void draw() { m.show(); }
"""
S["framerate_var_and_fn"] = """
void setup() { frameRate(30); }
void draw() { if (frameCount % 30 == 0) println(frameRate); }
"""
S["char_print"] = """
char c = 'A';
void setup() { println(c); print(c); println(); println("c is " + c); }
"""
S["int_float_math"] = """
void setup() { int a = 7 / 2; float b = 7 / 2.0; float c = (float)a / 2; int d = (int)3.9; println(a + " " + b + " " + c + " " + d); }
"""
S["operator_overload"] = """
class Vec { public: float x, y; Vec(float x_, float y_) : x(x_), y(y_) {} Vec operator+(const Vec& o) const { return Vec(x + o.x, y + o.y); } };
void setup() { Vec v = Vec(1, 2) + Vec(3, 4); println(v.x); }
"""
S["arraylist"] = """
class Particle { public: float x = 0; void update() { x += 1; } };
ArrayList<Particle> ps;
void setup() { for (int i = 0; i < 3; i++) ps.add(new Particle()); }
void draw() { for (int i = 0; i < ps.size(); i++) ps.get(i)->update(); }
"""
S["pvector_math"] = """
PVector pos(10, 20);
PVector vel = PVector(1, 1);
class Mover { public: void go() { pos.add(vel); vel.limit(5); } };
Mover mv;
void setup() { mv.go(); println(pos.x); PVector d = PVector::sub(pos, vel); println(d.mag()); }
"""
S["colors"] = """
color bg = color(20, 30, 40);
class Painter { public: color c = color(255, 0, 0); void paint() { fill(c); stroke(bg); } };
Painter p;
void setup() { background(bg); p.paint(); println(red(bg)); }
"""
S["multi_decl_runtime"] = """
float a = width, b = 3;
class Use { public: float sum() { return a + b; } };
Use u;
void setup() { size(100, 100); println(u.sum()); }
"""
S["global_comment_and_strings"] = """
// a comment mentioning setup() { and } braces
String note = "class Fake { }; void setup() {";
void setup() { println(note); /* draw() { */ }
"""

FLT = sys.argv[1] if len(sys.argv) > 1 else ""
def run(name):
    gen = subprocess.run(CLI, input=S[name], capture_output=True, text=True)
    if gen.returncode != 0 or not gen.stdout.strip():
        return name, "translate", (gen.stderr.strip().splitlines() or ["?"])[-1][:150], None
    with tempfile.NamedTemporaryFile("w", suffix=".cpp", delete=False) as f:
        f.write(gen.stdout); cpp = f.name
    res = []
    for extra in ([], ["-DCPPMODE_BACKEND_LIBPROCESSING", "-I", str(LP_INC)]):
        r = subprocess.run(GPP + extra + [cpp], capture_output=True, text=True)
        err = next((l for l in r.stderr.splitlines() if " error: " in l), r.stderr.strip()[:150] or "?")
        res.append(None if r.returncode == 0 else err[:170])
    os.unlink(cpp)
    return name, "compiled", res[0], res[1]

names = [n for n in S if FLT in n]
print(f"probing {len(names)} sketches...")
with cf.ThreadPoolExecutor(max_workers=2) as ex:
    rows = list(ex.map(run, names))
bad = 0
for name, stage, gl, lp in rows:
    if stage == "translate": print(f"FAIL  {name:28s} translate: {gl}"); bad += 1
    elif gl or lp:
        bad += 1
        print(f"FAIL  {name:28s} {'opengl: ' + gl if gl else ''}{'  |  lp: ' + lp if lp and lp != gl else ''}")
    else: print(f"ok    {name}")
print(f"\n{len(rows) - bad} of {len(rows)} compile in both renderers")
