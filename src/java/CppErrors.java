package processing.mode.cpp;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Numbered compiler errors. Each entry turns a g++ message into a code and a
 * plain explanation; the code has a page at processing-cpp.github.io/error/.
 * Patterns are deliberately narrow: no code is better than a wrong code.
 * g++ quotes names with typographic quotes (or ' in the C locale), so the
 * patterns use "." where a quote appears.
 */
final class CppErrors {
  static final String BASE = "https://processing-cpp.github.io/error/";

  record Hit(String code, String message) {
    String url()  { return BASE + code + ".html"; }
    String text() { return code + ": " + message; }
  }

  private record Rule(String code, Pattern pattern, String message) {}

  private static Rule rule(String code, String regex, String message) {
    return new Rule(code, Pattern.compile(regex), message);
  }

  private static final String NS = "(?:Processing::)?";

  // $1 in a message is replaced by the pattern's first group
  private static final Rule[] RULES = {
    // E0001..E0003: types that own a graphics resource can't be copied
    rule("PTR", "use of deleted function .*\\b(PGraphics|PImage|PShader)\\b", ""),
    // E0008: event/lifecycle function with the wrong parameters, so "override" has nothing to override
    rule("EVT", "(?:\\w+::)*(setup|draw|settings|mouse(?:Pressed|Released|Clicked|Moved|Dragged|Wheel|Entered|Exited)|key(?:Pressed|Released|Typed))\\(([^)]*)\\). marked .override., but does not override", ""),
    rule("PTR", "conversion from ." + NS + "(PGraphics|PImage|PShader)\\*. to non-scalar type", ""),
    rule("PTR", "cannot convert ." + NS + "(PGraphics|PImage|PShader). to ." + NS + "\\1\\*.", ""),
    rule("E0004", "structured binding declaration cannot have type", 
         "Java-style array declarations aren't supported. Write int a[] = {...} or use a list type"),
    rule("E0004", "expected unqualified-id before .\\[. token",
         "Java-style array declarations aren't supported. Write int a[] = {...} or use a list type"),
    rule("E0005", "request for member .* in .*::get\\(.*which is of pointer type",
         "ArrayList.get() returns a pointer, not a value. Use -> instead of ."),
    rule("E0006", "expected primary-expression before .\\.. token",
         "Java static access (Type.member) is not valid in C++. Use Type::member"),
    rule("E0007", "changes meaning of ." + NS + "(\\w+).",
         "Variable name shadows the Processing type $1. Pick a different name"),
  };

  /** The code for one g++ message, or null when it isn't one we know. */
  static Hit match(String gccMessage) {
    if (gccMessage == null) return null;
    for (Rule r : RULES) {
      Matcher m = r.pattern().matcher(gccMessage);
      if (!m.find()) continue;
      if (r.code().equals("EVT")) {
        String fn = m.group(1), got = m.group(2).trim();
        String want = fn.equals("mouseWheel") ? "mouseWheel(MouseEvent e)"
            : fn.startsWith("mouse") ? fn + "() or " + fn + "(MouseEvent e)"
            : fn.startsWith("key") ? fn + "() or " + fn + "(KeyEvent e)"
            : fn + "()";
        return new Hit("E0008", fn + "(" + got + ") has the wrong parameters, so Processing never calls it. Write " + want);
      }
      if (r.code().equals("PTR")) {
        String t = m.group(1);
        String code = t.equals("PGraphics") ? "E0001" : t.equals("PImage") ? "E0002" : "E0003";
        return new Hit(code, t + " needs to be a pointer. Use " + t + "* instead of " + t);
      }
      String msg = r.message();
      if (msg.contains("$1") && m.groupCount() >= 1) msg = msg.replace("$1", m.group(1));
      return new Hit(r.code(), msg);
    }
    return null;
  }

  /** First known error in a whole g++ output, or null. */
  static Hit find(String gccOutput) {
    if (gccOutput == null) return null;
    for (String line : gccOutput.split("\n")) {
      if (!line.contains("error:")) continue;
      Hit h = match(line);
      if (h != null) return h;
    }
    return null;
  }

  /** Text for a linter squiggle: the coded text when known, else the fallback. */
  static String describe(String rawGccMessage, String fallback) {
    Hit h = match(rawGccMessage);
    return h == null ? fallback : h.text() + "  (" + h.url() + ")";
  }

  private CppErrors() {}
}
