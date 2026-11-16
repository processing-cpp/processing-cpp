package processing.mode.cpp;

import processing.app.Sketch;

import java.util.*;
import java.util.concurrent.*;

/**
 * Go-to-definition for CppMode, powered by tree-sitter node walking.
 *
 * <p>On every document change (debounced, {@value #DEBOUNCE_MS} ms), the
 * full sketch source is re-parsed and definition nodes are collected by
 * walking the concrete syntax tree.  Function definitions, variable
 * declarations, class/struct definitions, and typedef names are indexed.
 *
 * <p>When the user Ctrl+clicks (or presses F3) on a word, the editor calls
 * {@link #findDefinition(String)}.  If a matching definition exists the
 * editor jumps to it; otherwise it silently does nothing.
 *
 * <h2>Thread-safety</h2>
 * The index is rebuilt on a private daemon thread; reads happen on the EDT.
 * The index swap is atomic (volatile reference to an immutable map).
 */
public class TsGoToDefinition {

  private static final int DEBOUNCE_MS = 200;

  // ── Definition record ─────────────────────────────────────────────────────

  /**
   * A single symbol definition found by walking the parse tree.
   *
   * @param name      the symbol name (identifier text from the source)
   * @param tabIndex  0-based index into the sketch's tab list
   * @param byteOffset byte offset of the name node's start within the
   *                   concatenated sketch source
   * @param row       0-based line number within the tab
   * @param col       0-based column within that line
   */
  public record Definition(String name, int tabIndex, int byteOffset, int row, int col) {}

  // ── State ─────────────────────────────────────────────────────────────────

  /** Current index: symbol name → first (outermost) definition. */
  private volatile Map<String, Definition> index = Collections.emptyMap();

  /** The source string the index was built from (for staleness checks). */
  private volatile String indexedSource = "";

  // ── Async updater ─────────────────────────────────────────────────────────

  private final ScheduledExecutorService scheduler =
      Executors.newSingleThreadScheduledExecutor(r -> {
        Thread t = new Thread(r, "CppGoToDef");
        t.setDaemon(true);
        return t;
      });

  private ScheduledFuture<?> pending;

  // ── Constructor ───────────────────────────────────────────────────────────

  public TsGoToDefinition() {}

  // ── Public API ────────────────────────────────────────────────────────────

  /**
   * Schedule a re-index.  Call this from the editor's {@code DocumentListener}
   * with the full concatenated sketch source and the byte offset where each
   * tab starts.
   *
   * @param source        the concatenated sketch source (all tabs joined)
   * @param tabStartBytes byte offset at which each tab starts inside
   *                      {@code source}
   */
  public synchronized void scheduleIndex(String source, int[] tabStartBytes) {
    if (pending != null) pending.cancel(false);
    pending = scheduler.schedule(
        () -> rebuild(source, tabStartBytes),
        DEBOUNCE_MS, TimeUnit.MILLISECONDS);
  }

  /**
   * Look up a definition by exact symbol name.
   *
   * @param name the identifier to find
   * @return the best-matching {@link Definition}, or {@code null} if unknown
   */
  public Definition findDefinition(String name) {
    return index.get(name);
  }

  /** Returns a snapshot of all currently indexed definitions. */
  public Collection<Definition> allDefinitions() {
    return index.values();
  }

  /** Shut down the background indexing thread. */
  public void dispose() { scheduler.shutdownNow(); }

  // ── Rebuilding ────────────────────────────────────────────────────────────

  private void rebuild(String source, int[] tabStartBytes) {
    if (!Ts.isAvailable()) return;
    TsEngine engine = TsEngine.getOrNull();
    if (engine == null) return;

    try {
      String sanitised = CppBuild.sanitise(source);
      Map<String, Definition> newIndex = new LinkedHashMap<>();

      try (TsEngine.ParseResult result = engine.parse(sanitised)) {
        TsNode root = result.root();
        if (root != null) {
          collectDefinitions(root, source, sanitised, tabStartBytes, newIndex);
        }
      }

      indexedSource = source;
      index = Collections.unmodifiableMap(newIndex);
    } catch (Exception ignored) {}
  }

  /**
   * Walk the parse tree and collect named definitions into {@code out}.
   * We look for:
   * <ul>
   *   <li>function_definition → name field (declarator → identifier)</li>
   *   <li>declaration with init_declarator or simple declarator → identifier</li>
   *   <li>class_specifier, struct_specifier → name field</li>
   *   <li>type_definition (typedef) → type_declarator</li>
   * </ul>
   */
  private void collectDefinitions(TsNode node, String source, String sanitised,
                                   int[] tabStartBytes,
                                   Map<String, Definition> out) {
    String type = node.getType();

    switch (type) {
      case "function_definition" -> {
        TsNode nameNode = extractFunctionName(node);
        if (nameNode != null) recordDefinition(nameNode, source, sanitised, tabStartBytes, out);
      }
      case "declaration" -> {
        // Scan declarator children for identifiers
        collectDeclaratorNames(node, source, sanitised, tabStartBytes, out);
      }
      case "class_specifier", "struct_specifier", "enum_specifier" -> {
        TsNode nameNode = node.getChildByFieldName("name");
        if (nameNode != null) recordDefinition(nameNode, source, sanitised, tabStartBytes, out);
      }
      case "type_definition" -> {
        // typedef T Name; — the last declarator holds the alias
        int n = node.getChildCount();
        for (int i = n - 1; i >= 0; i--) {
          TsNode child = node.getChild(i);
          if (child == null) continue;
          String ct = child.getType();
          if ("type_identifier".equals(ct) || "identifier".equals(ct)) {
            recordDefinition(child, source, sanitised, tabStartBytes, out);
            break;
          }
        }
      }
    }

    // Recurse into children (skip leaf-like nodes)
    if (!isLeaf(type)) {
      int n = node.getChildCount();
      for (int i = 0; i < n; i++) {
        TsNode child = node.getChild(i);
        if (child != null) collectDefinitions(child, source, sanitised, tabStartBytes, out);
      }
    }
  }

  /** Extract the identifier name node from a function_definition. */
  private static TsNode extractFunctionName(TsNode funcDef) {
    TsNode decl = funcDef.getChildByFieldName("declarator");
    if (decl == null) return null;
    // function_declarator → declarator field is the name
    return findFirstIdentifier(decl);
  }

  /** Recursively find the first identifier/type_identifier node. */
  private static TsNode findFirstIdentifier(TsNode node) {
    if (node == null) return null;
    String t = node.getType();
    if ("identifier".equals(t) || "type_identifier".equals(t)) return node;
    // Check field "declarator" first for nested declarators
    TsNode inner = node.getChildByFieldName("declarator");
    if (inner != null) {
      TsNode found = findFirstIdentifier(inner);
      if (found != null) return found;
    }
    int n = node.getChildCount();
    for (int i = 0; i < n; i++) {
      TsNode child = node.getChild(i);
      TsNode found = findFirstIdentifier(child);
      if (found != null) return found;
    }
    return null;
  }

  /** Collect names from a declaration node's declarators. */
  private void collectDeclaratorNames(TsNode decl, String source, String sanitised,
                                       int[] tabStartBytes, Map<String, Definition> out) {
    int n = decl.getChildCount();
    for (int i = 0; i < n; i++) {
      TsNode child = decl.getChild(i);
      if (child == null) continue;
      TsNode ident = findFirstIdentifier(child);
      if (ident != null) recordDefinition(ident, source, sanitised, tabStartBytes, out);
    }
  }

  private void recordDefinition(TsNode nameNode, String source, String sanitised,
                                  int[] tabStartBytes, Map<String, Definition> out) {
    int byteOffset = nameNode.getStartByte();
    if (byteOffset < 0 || byteOffset >= sanitised.length()) return;
    int end = nameNode.getEndByte();
    if (end > sanitised.length()) end = sanitised.length();
    String symbolName = sanitised.substring(byteOffset, end);
    if (symbolName.isBlank()) return;

    int tabIdx  = tabIndexForByte(byteOffset, tabStartBytes);
    int row     = nameNode.getStartRow();
    int col     = nameNode.getStartCol();

    out.putIfAbsent(symbolName, new Definition(symbolName, tabIdx, byteOffset, row, col));
  }

  private static boolean isLeaf(String type) {
    return switch (type) {
      case "identifier", "type_identifier", "field_identifier",
           "namespace_identifier", "number_literal", "string_literal",
           "char_literal", "comment", "primitive_type" -> true;
      default -> false;
    };
  }

  // ── Helpers ───────────────────────────────────────────────────────────────

  private static int tabIndexForByte(int byteOffset, int[] tabStartBytes) {
    for (int i = tabStartBytes.length - 1; i >= 0; i--) {
      if (byteOffset >= tabStartBytes[i]) return i;
    }
    return 0;
  }

  // ── Static helper: build tabStartBytes for a Sketch ──────────────────────

  /**
   * Builds the byte-offset array used by {@link #scheduleIndex}.
   *
   * @param sketch     the current sketch
   * @param currentTab the tab whose live text is supplied in {@code liveText}
   * @param liveText   unsaved text for the current tab (may differ from saved)
   * @return pair: [0] = concatenated source, [1] = int[] tabStartBytes
   */
  public static Object[] buildSourceAndOffsets(Sketch sketch,
                                                int currentTab,
                                                String liveText) {
    int tabCount = sketch.getCodeCount();
    int[] starts  = new int[tabCount];
    StringBuilder sb = new StringBuilder();
    for (int i = 0; i < tabCount; i++) {
      starts[i] = sb.length();
      String prog = (i == currentTab) ? liveText : sketch.getCode(i).getProgram();
      if (prog == null) prog = "";
      sb.append(prog).append("\n");
    }
    return new Object[]{ sb.toString(), starts };
  }
}
