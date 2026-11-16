package processing.mode.cpp;

import processing.app.syntax.SyntaxDocument;
import processing.app.syntax.Token;
import processing.app.syntax.TokenMarker;

import javax.swing.text.Segment;
import java.util.Arrays;
import java.util.Set;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicReference;

/**
 * Tree-sitter-backed syntax token marker for the Processing jEdit text area.
 *
 * <p>Walks the concrete syntax tree produced by our vendored tree-sitter-cpp
 * grammar and maps node types to jEdit token types.  No query API is used —
 * all classification is done by examining {@link TsNode#getType()}.
 *
 * <h2>Token mapping</h2>
 * <ul>
 *   <li>comments → COMMENT1 / COMMENT2</li>
 *   <li>string / char literals → LITERAL1</li>
 *   <li>number literals → LITERAL2</li>
 *   <li>primitive types, keywords → KEYWORD1</li>
 *   <li>Processing lifecycle names (draw, setup, …) → KEYWORD2</li>
 *   <li>Processing built-in variables → KEYWORD2</li>
 *   <li>Function names → KEYWORD3</li>
 *   <li>preprocessor directives → KEYWORD1</li>
 * </ul>
 */
public class TsTokenMarker extends TokenMarker {

  // ── Token-type per char (one byte per UTF-16 position) ───────────────────

  private static final byte[] EMPTY = new byte[0];
  private final AtomicReference<byte[]> highlights = new AtomicReference<>(EMPTY);
  private volatile String highlightedSource = "";

  // ── Document reference (injected via setDoc) ──────────────────────────────

  private volatile SyntaxDocument doc;

  /** Called by the editor to wire up the document so we can read source text. */
  public void setDoc(SyntaxDocument doc) {
    this.doc = doc;
  }

  // ── Async rebuilder ───────────────────────────────────────────────────────

  private final ScheduledExecutorService scheduler =
      Executors.newSingleThreadScheduledExecutor(r -> {
        Thread t = new Thread(r, "CppTsHighlighter");
        t.setDaemon(true);
        return t;
      });

  private ScheduledFuture<?> pending;

  // ── Processing-specific keyword sets ─────────────────────────────────────

  private static final Set<String> LIFECYCLE = SketchAnalyzer.LIFECYCLE_NAMES;

  private static final Set<String> BUILTIN_VARS = Set.of(
      "width", "height", "mouseX", "mouseY", "pmouseX", "pmouseY",
      "mousePressed", "keyPressed", "key", "keyCode", "frameCount",
      "frameRate", "focused"
  );

  private static final Set<String> PROCESSING_TYPES = Set.of(
      "PVector", "PImage", "PGraphics", "PShape", "PFont", "PShader",
      "PMatrix", "PMatrix2D", "PMatrix3D", "Table", "TableRow",
      "XML", "JSONObject", "JSONArray", "BufferedReader", "PrintWriter",
      "color", "boolean", "byte", "char", "double", "float", "int", "long",
      "String"
  );

  // ── Constructor / lifecycle ───────────────────────────────────────────────

  public TsTokenMarker() {}

  /** Required by TokenMarker — we don't use keyword colorings. */
  @Override
  public void addColoring(String keyword, String coloring) {
    // no-op: token types are determined by tree-sitter node types, not keyword lists
  }

  /** Called by the editor whenever the document changes. */
  public synchronized void invalidate() {
    if (pending != null) pending.cancel(false);
    pending = scheduler.schedule(this::rebuildHighlights, 150, TimeUnit.MILLISECONDS);
  }

  /** Shut down the background scheduler. */
  public void dispose() { scheduler.shutdownNow(); }

  // ── TokenMarker implementation ────────────────────────────────────────────

  @Override
  protected byte markTokensImpl(byte token, Segment line, int lineIndex) {
    byte[] hl  = highlights.get();
    String src = highlightedSource;

    if (hl.length == 0) {
      addToken(line.count, Token.NULL);
      return Token.NULL;
    }

    int lineStart = offsetOfLine(src, lineIndex);
    if (lineStart < 0 || lineStart >= src.length()) {
      addToken(line.count, Token.NULL);
      return Token.NULL;
    }

    int  pos      = 0;
    byte lastType = (lineStart < hl.length) ? hl[lineStart] : Token.NULL;
    int  runLen   = 0;

    while (pos < line.count) {
      int  charPos = lineStart + pos;
      byte type    = (charPos < hl.length) ? hl[charPos] : Token.NULL;
      if (type != lastType) {
        if (runLen > 0) addToken(runLen, lastType);
        lastType = type;
        runLen   = 0;
      }
      runLen++;
      pos++;
    }
    if (runLen > 0) addToken(runLen, lastType);
    return Token.NULL;
  }

  // ── Highlight rebuilding ──────────────────────────────────────────────────

  private void rebuildHighlights() {
    try {
      if (!Ts.isAvailable()) return;
      TsEngine engine = TsEngine.getOrNull();
      if (engine == null) return;

      String src = currentDocSource();
      if (src == null || src.isEmpty()) return;

      byte[] hl = new byte[src.length()];
      Arrays.fill(hl, Token.NULL);

      try (TsEngine.ParseResult result = engine.parse(src)) {
        TsNode root = result.root();
        if (root != null) walkNode(root, src, hl);
      }

      highlightedSource = src;
      highlights.set(hl);
      javax.swing.SwingUtilities.invokeLater(this::fireTokensChanged);

    } catch (Exception ignored) {}
  }

  /**
   * Recursively walk the parse tree and fill {@code hl} with token types.
   */
  private void walkNode(TsNode node, String src, byte[] hl) {
    String type = node.getType();
    int    s    = node.getStartByte();
    int    e    = node.getEndByte();

    byte tok = nodeTypeToToken(type, node, src, s, e);
    if (tok != Token.NULL) {
      fillSpan(hl, s, e, tok);
      return; // leaf — don't recurse into it
    }

    // Recurse into children
    int n = node.getChildCount();
    for (int i = 0; i < n; i++) {
      TsNode child = node.getChild(i);
      if (child != null) walkNode(child, src, hl);
    }
  }

  /**
   * Map a node type (and optionally its text) to a token type.
   * Returns {@link Token#NULL} to recurse into children.
   */
  private byte nodeTypeToToken(String type, TsNode node, String src, int s, int e) {
    return switch (type) {
      // ── Comments ────────────────────────────────────────────────────────
      case "comment" -> Token.COMMENT1;

      // ── String / char literals ───────────────────────────────────────────
      case "string_literal", "string_content",
           "char_literal", "raw_string_literal" -> Token.LITERAL1;

      // ── Number literals ──────────────────────────────────────────────────
      case "number_literal" -> Token.LITERAL2;

      // ── Preprocessor ────────────────────────────────────────────────────
      case "preproc_include", "preproc_def", "preproc_function_def",
           "preproc_call", "preproc_ifdef", "preproc_if",
           "preproc_else", "preproc_elif", "preproc_endif",
           "preproc_directive", "#include", "#define", "#ifdef",
           "#ifndef", "#if", "#else", "#elif", "#endif", "#pragma" -> Token.KEYWORD1;

      // ── Type keywords ────────────────────────────────────────────────────
      case "primitive_type", "type_qualifier",
           "type_specifier"        -> Token.KEYWORD1;

      // ── Named identifiers — classify by text ─────────────────────────────
      case "identifier", "type_identifier",
           "namespace_identifier",
           "field_identifier"      -> classifyIdentifier(nodeText(src, s, e));

      // ── Function declarator name ─────────────────────────────────────────
      case "function_declarator"   -> Token.NULL; // recurse

      // ── Keyword literals ─────────────────────────────────────────────────
      // These show up as anonymous nodes with their literal text as type
      default -> {
        if (isKeywordType(type)) yield Token.KEYWORD1;
        yield Token.NULL; // recurse
      }
    };
  }

  /** C++ reserved words that appear as their own node type in tree-sitter. */
  private static boolean isKeywordType(String type) {
    return switch (type) {
      case "auto", "bool", "break", "case", "catch", "class", "const",
           "constexpr", "continue", "decltype", "default", "delete",
           "do", "else", "enum", "explicit", "extern", "final",
           "for", "friend", "goto", "if", "inline", "mutable",
           "namespace", "new", "noexcept", "nullptr", "operator",
           "override", "private", "protected", "public", "register",
           "reinterpret_cast", "return", "sizeof", "static", "static_cast",
           "struct", "switch", "template", "this", "throw", "true", "false",
           "try", "typedef", "typeid", "typename", "union", "using",
           "virtual", "volatile", "while", "void",
           // C types that appear as keywords
           "int", "float", "double", "char", "long", "short",
           "unsigned", "signed", "size_t", "nullptr_t" -> true;
      default -> false;
    };
  }

  private byte classifyIdentifier(String text) {
    if (text == null || text.isEmpty()) return Token.NULL;
    if (LIFECYCLE.contains(text))       return Token.KEYWORD2;
    if (BUILTIN_VARS.contains(text))    return Token.KEYWORD2;
    if (PROCESSING_TYPES.contains(text)) return Token.KEYWORD1;
    return Token.NULL;
  }

  // ── Helpers ───────────────────────────────────────────────────────────────

  private static String nodeText(String src, int s, int e) {
    if (s < 0 || e > src.length() || s >= e) return "";
    return src.substring(s, e);
  }

  private static void fillSpan(byte[] hl, int s, int e, byte tok) {
    int end = Math.min(e, hl.length);
    for (int i = Math.max(0, s); i < end; i++) hl[i] = tok;
  }

  private String currentDocSource() {
    SyntaxDocument d = doc;
    if (d == null) return null;
    String[] holder = {null};
    try {
      javax.swing.SwingUtilities.invokeAndWait(() -> {
        try { holder[0] = d.getText(0, d.getLength()); }
        catch (Exception ignored) {}
      });
    } catch (Exception ignored) {}
    return holder[0];
  }

  private static int offsetOfLine(String src, int lineIndex) {
    if (lineIndex == 0) return 0;
    int line = 0;
    for (int i = 0; i < src.length(); i++) {
      if (src.charAt(i) == '\n' && ++line == lineIndex) return i + 1;
    }
    return -1;
  }

  private void fireTokensChanged() {
    SyntaxDocument d = doc;
    if (d != null) {
      try { d.tokenizeLines(); }
      catch (Exception ignored) {}
    }
  }
}
