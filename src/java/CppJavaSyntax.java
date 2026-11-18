package processing.mode.cpp;

import java.util.List;
import java.util.Set;

/**
 * Processing/Java surface syntax that C++ can't parse, rewritten token by token
 * before tree-sitter runs:
 *
 *   #FF8800            ->  0xFFFF8800              (opaque color, like Java)
 *   Math.abs(x)        ->  Math::abs(x)            (static members of Java's classes)
 *   System.out.println ->  System::out.println
 *   thread("work")     ->  thread([&]{ work(); })  (C++ can't find a method by name)
 *   int("42")          ->  parseInt("42")          (in C++ int(...) is a cast, which can't
 *   float("2.5")       ->  parseFloat("2.5")        take a string literal)
 *   boolean("true")    ->  parseBoolean("true")
 *
 * Built on {@link CppLexer}, so strings, chars, comments and directives are
 * never touched, and every replacement stays on its own line.
 */
final class CppJavaSyntax {
  private CppJavaSyntax() {}

  /** Java classes whose static members the sketch reaches with a dot. */
  static final Set<String> STATIC_CLASSES = Set.of(
      "Math", "System", "Integer", "Float", "Double", "Long", "Byte", "Character",
      "Collections", "Arrays", "String");

  /** Processing's conversion functions that C++ would read as casts. */
  static final java.util.Map<String, String> PARSERS = java.util.Map.of(
      "int", "parseInt", "float", "parseFloat", "boolean", "parseBoolean");

  static String rewrite(String src) {
    if (src.indexOf('#') < 0 && src.indexOf('.') < 0 && src.indexOf('"') < 0) return src;
    List<CppLexer.Token> toks = CppLexer.tokenize(src);
    StringBuilder out = new StringBuilder(src.length() + 32);
    int k = 0;
    while (k < toks.size()) {
      CppLexer.Token t = toks.get(k);
      switch (t.kind()) {
        case COLOR -> { out.append("0xFF").append(t.text(), 1, 7); k++; }
        case IDENT -> {
          int used = rewriteIdent(toks, k, out);
          if (used == 0) { out.append(t.text()); k++; } else k += used;
        }
        default -> { out.append(t.text()); k++; }
      }
    }
    return out.toString();
  }

  /** Handles an identifier at k. Returns how many tokens it consumed, or 0 to leave it as is. */
  private static int rewriteIdent(List<CppLexer.Token> toks, int k, StringBuilder out) {
    CppLexer.Token t = toks.get(k);
    if (isMemberAccess(toks, k)) return 0;   // obj.Math, ns::thread, p->String: not ours

    // Math . abs  ->  Math :: abs
    if (STATIC_CLASSES.contains(t.text())) {
      int dot = nextSignificant(toks, k);
      if (dot >= 0 && toks.get(dot).is(".")) {
        int member = nextSignificant(toks, dot);
        if (member >= 0 && toks.get(member).kind() == CppLexer.Kind.IDENT) {
          out.append(t.text());
          for (int j = k + 1; j < dot; j++) out.append(toks.get(j).text());
          out.append("::");
          return dot - k + 1;
        }
      }
      return 0;
    }

    // int ( "42" )  ->  parseInt ( "42" ): only a single string literal, so casts stay casts
    String parser = PARSERS.get(t.text());
    if (parser != null) {
      int open = nextSignificant(toks, k);
      if (open < 0 || !toks.get(open).is("(")) return 0;
      int str = nextSignificant(toks, open);
      if (str < 0 || toks.get(str).kind() != CppLexer.Kind.STRING || !toks.get(str).text().startsWith("\"")) return 0;
      int close = nextSignificant(toks, str);
      if (close < 0 || !toks.get(close).is(")")) return 0;
      out.append(parser);
      return 1;   // only the type name changes; ( "..." ) is copied as is
    }

    // thread ( "name" )  ->  thread([&]{ name(); })
    if (t.is("thread")) {
      int open = nextSignificant(toks, k);
      if (open < 0 || !toks.get(open).is("(")) return 0;
      int str = nextSignificant(toks, open);
      if (str < 0 || toks.get(str).kind() != CppLexer.Kind.STRING) return 0;
      String name = plainStringContent(toks.get(str).text());
      if (name == null || !isIdentifier(name)) return 0;
      int close = nextSignificant(toks, str);
      if (close < 0 || !toks.get(close).is(")")) return 0;
      if (hasNewline(toks, k, close)) return 0;   // keep line numbers exact
      out.append("thread([&]{ ").append(name).append("(); })");
      return close - k + 1;
    }
    return 0;
  }

  private static boolean isMemberAccess(List<CppLexer.Token> toks, int k) {
    int p = prevSignificant(toks, k);
    if (p < 0) return false;
    String s = toks.get(p).text();
    return s.equals(".") || s.equals("->") || s.equals("::");
  }

  private static int nextSignificant(List<CppLexer.Token> toks, int k) {
    for (int j = k + 1; j < toks.size(); j++) if (toks.get(j).significant()) return j;
    return -1;
  }

  private static int prevSignificant(List<CppLexer.Token> toks, int k) {
    for (int j = k - 1; j >= 0; j--) if (toks.get(j).significant()) return j;
    return -1;
  }

  private static boolean hasNewline(List<CppLexer.Token> toks, int from, int to) {
    for (int j = from; j <= to; j++) {
      CppLexer.Token t = toks.get(j);
      if (t.kind() == CppLexer.Kind.NEWLINE || t.text().indexOf('\n') >= 0) return true;
    }
    return false;
  }

  /** "abc" -> abc. Prefixed, raw or escaped strings return null. */
  private static String plainStringContent(String lit) {
    if (lit.length() < 2 || lit.charAt(0) != '"' || lit.charAt(lit.length() - 1) != '"') return null;
    String body = lit.substring(1, lit.length() - 1);
    return body.indexOf('\\') >= 0 ? null : body;
  }

  private static boolean isIdentifier(String s) {
    if (s.isEmpty() || !(Character.isLetter(s.charAt(0)) || s.charAt(0) == '_')) return false;
    for (int j = 1; j < s.length(); j++) if (!(Character.isLetterOrDigit(s.charAt(j)) || s.charAt(j) == '_')) return false;
    return true;
  }
}
