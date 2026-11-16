package processing.mode.cpp;

/**
 * Java-style string joining. In Java "x = " + x builds a string; in C++ a
 * text literal is a char pointer, so the same code does pointer arithmetic or
 * fails to compile. This wraps every plain text literal that sits next to a
 * binary + as ::std::string("..."), and Processing.h supplies + for strings
 * with numbers. Comments, char literals, raw/prefixed strings and
 * preprocessor lines (#line "sketch.pde") are left alone. Line numbers don't
 * change because nothing adds or removes line breaks.
 */
final class CppStrings {
  private CppStrings() {}

  static String wrapConcat(String src) {
    int n = src.length();
    StringBuilder out = new StringBuilder(n + 64);
    int i = 0, copied = 0;
    boolean lineStart = true;
    while (i < n) {
      char c = src.charAt(i);
      if (c == '\n') { lineStart = true; i++; continue; }
      if (lineStart && (c == ' ' || c == '\t')) { i++; continue; }
      if (lineStart && c == '#') {                       // preprocessor line (with \ continuations)
        while (i < n && !(src.charAt(i) == '\n' && src.charAt(i - 1) != '\\')) i++;
        continue;
      }
      lineStart = false;
      if (c == '/' && i + 1 < n && src.charAt(i + 1) == '/') { while (i < n && src.charAt(i) != '\n') i++; continue; }
      if (c == '/' && i + 1 < n && src.charAt(i + 1) == '*') {
        int e = src.indexOf("*/", i + 2); i = e < 0 ? n : e + 2; continue;
      }
      if (c == '\'') { i = skipQuoted(src, i, '\''); continue; }
      if (c == '"') {
        boolean prefixed = i > 0 && isIdent(src.charAt(i - 1));
        if (prefixed && src.charAt(i - 1) == 'R') { i = skipRaw(src, i); continue; }
        int gStart = i;
        int gEnd = skipQuoted(src, i, '"');
        // adjacent literals ("a" "b") form one group
        while (true) {
          int j = gEnd;
          while (j < n && (src.charAt(j) == ' ' || src.charAt(j) == '\t')) j++;
          if (j < n && src.charAt(j) == '"') gEnd = skipQuoted(src, j, '"'); else break;
        }
        if (!prefixed && (plusAfter(src, gEnd) || plusBefore(src, gStart))) {
          out.append(src, copied, gStart).append("::std::string(").append(src, gStart, gEnd).append(')');
          copied = gEnd;
        }
        i = gEnd;
        continue;
      }
      i++;
    }
    out.append(src, copied, n);
    return out.toString();
  }

  private static boolean isIdent(char c) { return Character.isLetterOrDigit(c) || c == '_'; }

  private static int skipQuoted(String s, int i, char q) {
    int n = s.length(); i++;
    while (i < n) {
      char c = s.charAt(i);
      if (c == '\\') { i += 2; continue; }
      if (c == q || c == '\n') return i + 1;
      i++;
    }
    return n;
  }

  private static int skipRaw(String s, int i) {          // R"delim( ... )delim"
    int open = s.indexOf('(', i);
    if (open < 0) return s.length();
    String end = ")" + s.substring(i + 1, open) + "\"";
    int e = s.indexOf(end, open);
    return e < 0 ? s.length() : e + end.length();
  }

  // a binary + right after (not ++ or +=)
  private static boolean plusAfter(String s, int j) {
    int n = s.length();
    while (j < n && Character.isWhitespace(s.charAt(j))) j++;
    return j < n && s.charAt(j) == '+' && (j + 1 >= n || (s.charAt(j + 1) != '+' && s.charAt(j + 1) != '='));
  }

  // a binary + right before (not ++)
  private static boolean plusBefore(String s, int j) {
    j--;
    while (j >= 0 && Character.isWhitespace(s.charAt(j))) j--;
    return j >= 0 && s.charAt(j) == '+' && (j == 0 || s.charAt(j - 1) != '+');
  }
  // ── mousePressed / keyPressed as values ───────────────────────────────────
  // In Java a sketch can define void mousePressed() and still read the
  // mousePressed variable. In C++ the sketch's function hides the variable, so
  // every use of the name that isn't a call or the function's own declaration
  // is pointed at the runtime's field instead. Filled in by the patch script.
  static String[][] EVENT_VARS = {{"frameRate","(PApplet::g_papplet->_frameRate)"},{"keyPressed","(PApplet::g_papplet->_keyPressed)"},{"mousePressed","(PApplet::g_papplet->_mousePressed)"}};   // {name, replacement}

  static String fixEventVars(String src) {
    String[][] vars = activeVars(src);
    if (vars.length == 0 && !src.contains("pixels")) return src;  // cppmode-pixels-length
    StringBuilder out = new StringBuilder(src.length() + 64);
    int n = src.length(), i = 0, seg = 0;
    boolean lineStart = true;
    while (i < n) {
      char c = src.charAt(i);
      int skipTo = -1;
      if (c == '\n') { lineStart = true; i++; continue; }
      if (lineStart && (c == ' ' || c == '\t')) { i++; continue; }
      if (lineStart && c == '#') {
        int j = i; while (j < n && !(src.charAt(j) == '\n' && src.charAt(j - 1) != '\\')) j++; skipTo = j;
      }
      lineStart = false;
      if (skipTo < 0 && c == '/' && i + 1 < n && src.charAt(i + 1) == '/') { int j = src.indexOf('\n', i); skipTo = j < 0 ? n : j; }
      if (skipTo < 0 && c == '/' && i + 1 < n && src.charAt(i + 1) == '*') { int j = src.indexOf("*/", i + 2); skipTo = j < 0 ? n : j + 2; }
      if (skipTo < 0 && c == '\'') skipTo = skipQuoted(src, i, '\'');
      if (skipTo < 0 && c == '"') skipTo = (i > 0 && src.charAt(i - 1) == 'R') ? skipRaw(src, i) : skipQuoted(src, i, '"');
      if (skipTo >= 0) {
        out.append(replaceVars(src.substring(seg, i), vars)).append(src, i, skipTo);
        i = seg = skipTo; continue;
      }
      i++;
    }
    out.append(replaceVars(src.substring(seg), vars));
    return out.toString();
  }

  // a sketch's own variable with the same name wins over the runtime field
  private static boolean sketchDeclares(String src, String name) {
    return java.util.regex.Pattern.compile(
      "(?<![\\w.>:])(?:[A-Za-z_][\\w:]*(?:<[^;(){}]*>)?)(?:\\s*[*&]+\\s*|\\s+)" + name
      + "\\s*(?:\\[[^\\]]*\\]\\s*)*(?:=|;|,|\\{)").matcher(src).find();
  }

  private static String[][] activeVars(String src) {
    java.util.List<String[]> out = new java.util.ArrayList<>();
    for (String[] v : EVENT_VARS)
      if (src.contains(v[0]) && !sketchDeclares(src, v[0])) out.add(v);
    return out.toArray(new String[0][]);
  }

  private static String replaceVars(String code, String[][] vars) {
    // Java's pixels.length (also pg.pixels.length): pixels is a std::vector here
    code = code.replaceAll("\\bpixels\\s*\\.\\s*length\\b(?!\\s*\\()", "pixels.size()");
    for (String[] v : vars)
      code = code.replaceAll("(?<![\\w.>:])" + v[0] + "\\b(?!\\s*\\()", java.util.regex.Matcher.quoteReplacement(v[1]));
    return code;
  }

  // ── Prototypes inside PSketch ─────────────────────────────────────────────
  // Sketches written for C++ sometimes declare a function before defining it
  // (void reset(); ... void reset() { }). Inside a class that is an error, and
  // never needed: members can call each other in any order. Blank out such a
  // prototype when the class also defines that function. The line stays, so
  // line numbers don't move.
  private static final java.util.regex.Pattern PROTO = java.util.regex.Pattern.compile(
      "(?m)^  (?=[A-Za-z_])(?!return\\b|delete\\b|else\\b)[\\w:<>,]+[\\w:<>,\\*&\\s]*?[\\s\\*&]+(\\w+)\\s*\\([^;{}()]*\\)\\s*(?:const\\s*)?;[ \\t]*$");

  static String dropMemberPrototypes(String src) {
    int start = src.indexOf("struct PSketch : public PApplet {");
    if (start < 0) return src;
    int end = src.indexOf("\n};", start);
    if (end < 0) end = src.length();
    String body = src.substring(start, end);
    java.util.regex.Matcher m = PROTO.matcher(body);
    StringBuilder sb = new StringBuilder();
    int last = 0;
    while (m.find()) {
      String name = m.group(1);
      java.util.regex.Pattern def = java.util.regex.Pattern.compile(
          "(?m)^  [^;\\n]*\\b" + java.util.regex.Pattern.quote(name) + "\\s*\\([^;{}]*\\)[^;{}\\n]*\\{");
      if (def.matcher(body).find()) { sb.append(body, last, m.start()); last = m.end(); }
    }
    if (last == 0) return src;
    sb.append(body.substring(last));
    return src.substring(0, start) + sb + src.substring(end);
  }

  // Java class members are visible to the sketch by default; C++ class members
  // are private. Sketch classes (written at column 0) become structs, which
  // differ from classes only in that default. Explicit private: stays private.
  static String publicClasses(String src) {
    return src.replaceAll("(?m)^class(\\s+[A-Za-z_]\\w*\\s*(?:[:{;]|$))", "struct$1");
  }
}
