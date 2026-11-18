package processing.mode.cpp;
public class CppJavaSyntaxTest {
  static int fails = 0;
  static void check(String in, String want) {
    String got = CppJavaSyntax.rewrite(in);
    StringBuilder all = new StringBuilder(); for (var t : CppLexer.tokenize(in)) all.append(t.text());
    boolean round = all.toString().equals(in);
    boolean ok = got.equals(want) && round && got.split("\n", -1).length == in.split("\n", -1).length;
    if (!ok) { fails++; System.out.println("FAIL\n  in:   " + in.replace("\n","\\n") + "\n  want: " + want.replace("\n","\\n") + "\n  got:  " + got.replace("\n","\\n") + (round ? "" : "\n  (lexer round-trip broken)")); }
  }
  public static void main(String[] a) {
    check("color c = #FF0000;", "color c = 0xFFFF0000;");
    check("color[] p = { #00ff00,\n  #123ABC };", "color[] p = { 0xFF00ff00,\n  0xFF123ABC };");
    check("#define X 1\n#include \"a.h\"\nint y;", "#define X 1\n#include \"a.h\"\nint y;");
    check("#define M(a) \\\n  (a + #FF0000)\nint z;", "#define M(a) \\\n  (a + #FF0000)\nint z;");
    check("String s = \"#FF0000 Math.abs thread(\\\"x\\\")\"; // #00FF00 Math.abs", "String s = \"#FF0000 Math.abs thread(\\\"x\\\")\"; // #00FF00 Math.abs");
    check("/* Math.abs(1) #FFFFFF */ fill(#abcdef);", "/* Math.abs(1) #FFFFFF */ fill(0xFFabcdef);");
    check("int x = a#ABCDEF; y = #ABCDEFG; z = #ABCDE;", "int x = a#ABCDEF; y = #ABCDEFG; z = #ABCDE;");
    check("char k = '#'; char q = '\"';", "char k = '#'; char q = '\"';");
    check("auto r = R\"x(Math.abs #FF0000)x\";", "auto r = R\"x(Math.abs #FF0000)x\";");
    check("float v = Math.abs(-1) + Math . max(1, 2);", "float v = Math::abs(-1) + Math :: max(1, 2);");
    check("System.out.println(Integer.MAX_VALUE);", "System::out.println(Integer::MAX_VALUE);");
    check("obj.Math.abs(1); p->String.x; ns::Integer.y;", "obj.Math.abs(1); p->String.x; ns::Integer.y;");
    check("String s = String.format(\"%d\", 1); String t;", "String s = String::format(\"%d\", 1); String t;");
    check("float f = 1.5e-3; float g = .5; int h = 0x1F;", "float f = 1.5e-3; float g = .5; int h = 0x1F;");
    check("thread(\"work\");", "thread([&]{ work(); });");
    check("thread( \"work2\" ) ;", "thread([&]{ work2(); }) ;");
    check("thread(name); thread(\"not valid\"); t.thread(\"x\"); thread(\"a\\n\");", "thread(name); thread(\"not valid\"); t.thread(\"x\"); thread(\"a\\n\");");
    check("thread(\n\"work\");", "thread(\n\"work\");");
    check("int Mathx = 1; MyMath.abs(2);", "int Mathx = 1; MyMath.abs(2);");
    check("u8\"Math.abs\" L'#' Math.PI", "u8\"Math.abs\" L'#' Math::PI");
    check("", "");
    check("\"unterminated\nMath.abs(1);", "\"unterminated\nMath::abs(1);");
    check("int n = int(\"42\"); float f = float( \"2.5\" ); boolean b = boolean(\"true\");", "int n = parseInt(\"42\"); float f = parseFloat( \"2.5\" ); boolean b = parseBoolean(\"true\");");
    check("int a = int(3.7f); float b = float(x); int c = int(s); std::function<int(int)> fn;", "int a = int(3.7f); float b = float(x); int c = int(s); std::function<int(int)> fn;");
    check("int d = int(\"4\" + s); p.int(\"x\"); int(*fp)(\"q\");", "int d = int(\"4\" + s); p.int(\"x\"); int(*fp)(\"q\");");
    check("float g = float(u8\"1.5\");", "float g = float(u8\"1.5\");");
    System.out.println(fails == 0 ? "all ok" : fails + " failing");
  }
}
