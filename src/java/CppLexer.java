package processing.mode.cpp;

import java.util.ArrayList;
import java.util.List;

/**
 * Small C++ lexer for the rewrites that have to happen before tree-sitter sees
 * the sketch (Processing's #RRGGBB colors aren't valid C++, so the parser
 * can't be the one to find them). It splits the source into tokens with exact
 * offsets: concatenating every token's text gives back the input unchanged, so
 * a rewrite only replaces the tokens it means to and line numbers never move.
 */
final class CppLexer {
  enum Kind { WS, NEWLINE, COMMENT, STRING, CHAR, PREPROC, NUMBER, IDENT, COLOR, PUNCT }

  record Token(Kind kind, int start, int end, String text) {
    boolean is(String s) { return text.equals(s); }
    boolean significant() { return kind != Kind.WS && kind != Kind.NEWLINE && kind != Kind.COMMENT; }
  }

  private final String src;
  private final int n;
  private int i = 0;
  private boolean lineStart = true;
  private final List<Token> out = new ArrayList<>();

  private CppLexer(String src) { this.src = src; this.n = src.length(); }

  static List<Token> tokenize(String src) {
    CppLexer lx = new CppLexer(src);
    lx.run();
    return lx.out;
  }

  private void add(Kind k, int start) {
    out.add(new Token(k, start, i, src.substring(start, i)));
  }

  private void run() {
    while (i < n) {
      int start = i;
      char c = src.charAt(i);
      if (c == '\n') { i++; add(Kind.NEWLINE, start); lineStart = true; continue; }
      if (c == ' ' || c == '\t' || c == '\r' || c == '\f') {
        while (i < n && (src.charAt(i) == ' ' || src.charAt(i) == '\t' || src.charAt(i) == '\r' || src.charAt(i) == '\f')) i++;
        add(Kind.WS, start); continue;
      }
      boolean atLineStart = lineStart;
      lineStart = false;
      if (c == '#' && isColorAt(i)) { i += 7; add(Kind.COLOR, start); continue; }
      if (c == '#' && atLineStart) { lexPreproc(); add(Kind.PREPROC, start); continue; }
      if (c == '/' && peek(1) == '/') { while (i < n && src.charAt(i) != '\n') i++; add(Kind.COMMENT, start); continue; }
      if (c == '/' && peek(1) == '*') {
        int e = src.indexOf("*/", i + 2); i = e < 0 ? n : e + 2; add(Kind.COMMENT, start); continue;
      }
      if (isIdentStart(c)) {
        while (i < n && isIdentPart(src.charAt(i))) i++;
        // string/char prefixes: u8"", L'', R"()", u8R"()" ...
        if (i < n && (src.charAt(i) == '"' || src.charAt(i) == '\'') && isLiteralPrefix(src.substring(start, i))) {
          boolean raw = src.charAt(i - 1) == 'R';
          if (src.charAt(i) == '"') { if (raw) lexRaw(); else lexQuoted('"'); add(Kind.STRING, start); }
          else { lexQuoted('\''); add(Kind.CHAR, start); }
          continue;
        }
        add(Kind.IDENT, start); continue;
      }
      if (c == '"') { lexQuoted('"'); add(Kind.STRING, start); continue; }
      if (c == '\'') { lexQuoted('\''); add(Kind.CHAR, start); continue; }
      if (Character.isDigit(c) || (c == '.' && Character.isDigit(peek(1)))) { lexNumber(); add(Kind.NUMBER, start); continue; }
      if ((c == ':' && peek(1) == ':') || (c == '-' && peek(1) == '>')) { i += 2; add(Kind.PUNCT, start); continue; }
      if (c == '.' && peek(1) == '.' && peek(2) == '.') { i += 3; add(Kind.PUNCT, start); continue; }
      i++; add(Kind.PUNCT, start);
    }
  }

  private char peek(int k) { return i + k < n ? src.charAt(i + k) : '\0'; }

  private static boolean isIdentStart(char c) { return Character.isLetter(c) || c == '_' || c == '$'; }
  private static boolean isIdentPart(char c) { return Character.isLetterOrDigit(c) || c == '_' || c == '$'; }

  private static boolean isLiteralPrefix(String p) {
    switch (p) {
      case "L": case "u": case "U": case "u8": case "R": case "LR": case "uR": case "UR": case "u8R": return true;
      default: return false;
    }
  }

  /** #RRGGBB: exactly six hex digits, not glued to an identifier on either side. */
  private boolean isColorAt(int at) {
    if (at + 7 > n) return false;
    if (at > 0 && isIdentPart(src.charAt(at - 1))) return false;
    for (int k = at + 1; k < at + 7; k++) if (Character.digit(src.charAt(k), 16) < 0) return false;
    return at + 7 == n || !isIdentPart(src.charAt(at + 7));
  }

  /** A directive runs to the end of the line, following backslash continuations. */
  private void lexPreproc() {
    while (i < n) {
      char c = src.charAt(i);
      if (c == '\n' && (i == 0 || src.charAt(i - 1) != '\\')) return;
      // a comment can end a directive line; keep /* ... */ spanning lines inside it
      if (c == '/' && peek(1) == '*') { int e = src.indexOf("*/", i + 2); i = e < 0 ? n : e + 2; continue; }
      i++;
    }
  }

  /** "..." or '...' with escapes. An unterminated literal stops at the end of the line. */
  private void lexQuoted(char q) {
    i++;
    while (i < n) {
      char c = src.charAt(i);
      if (c == '\\' && i + 1 < n) { i += 2; continue; }
      if (c == '\n') return;
      i++;
      if (c == q) return;
    }
  }

  /** R"delim( ... )delim" */
  private void lexRaw() {
    int open = src.indexOf('(', i);
    if (open < 0) { i = n; return; }
    String end = ")" + src.substring(i + 1, open) + "\"";
    int e = src.indexOf(end, open + 1);
    i = e < 0 ? n : e + end.length();
  }

  /** pp-number: digits, letters, dots, and a sign right after an exponent (1e-5, 0x1p+3). */
  private void lexNumber() {
    i++;
    while (i < n) {
      char c = src.charAt(i);
      char p = src.charAt(i - 1);
      if (isIdentPart(c) || c == '.') { i++; continue; }
      if ((c == '+' || c == '-') && (p == 'e' || p == 'E' || p == 'p' || p == 'P')) { i++; continue; }
      return;
    }
  }
}
