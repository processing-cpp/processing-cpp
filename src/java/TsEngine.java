package processing.mode.cpp;

import java.io.File;

/**
 * Singleton that owns the tree-sitter parser for CppMode.
 *
 * <p>Uses our vendored tree-sitter + tree-sitter-cpp grammar compiled to
 * {@code libs/linux-x64/libcppmode-ts.so} (and equivalent on other platforms).
 * The native library is loaded once via {@link Ts#init(File)} at mode startup.
 *
 * <p>Nodes are represented as {@link TsNode} objects wrapping {@code long[4]}
 * packed TSNode structs — no per-node Java heap allocation.
 *
 * <p>Thread-safety: {@link #parse} creates a fresh TSParser per call (cheap)
 * because our JNI always calls ts_parser_new/delete around each parse.
 * ParseResult is NOT thread-safe; do not share across threads.
 */
public final class TsEngine {

  // ── Singleton ────────────────────────────────────────────────────────────

  private static volatile TsEngine INSTANCE;

  public static TsEngine get() {
    TsEngine e = INSTANCE;
    if (e == null) throw new IllegalStateException("TsEngine.init() was not called");
    return e;
  }

  /** Returns {@code null} if the native library is unavailable. */
  public static TsEngine getOrNull() {
    if (INSTANCE != null) return INSTANCE;
    if (!Ts.isAvailable()) {
      lazyInit();
    } else {
      // available=true but INSTANCE=null: lib loaded in another context, just create it.
      synchronized (TsEngine.class) {
        if (INSTANCE == null) INSTANCE = new TsEngine();
      }
    }
    return INSTANCE; // null if still unavailable
  }

  private static synchronized void lazyInit() {
    if (INSTANCE != null || Ts.isAvailable()) return;

    // Step 1: unconditionally write a debug breadcrumb so we can diagnose
    // even when Processing swallows stderr.
    java.io.File debugFile = new java.io.File(
        System.getProperty("user.home"), "cppmode-ts-debug.txt");
    StringBuilder dbg = new StringBuilder();
    dbg.append("lazyInit called\n");
    dbg.append("user.home=").append(System.getProperty("user.home")).append('\n');

    // Step 2: try to derive mode root from jar / class location
    java.io.File modeRoot = null;
    try {
      java.security.CodeSource cs =
          TsEngine.class.getProtectionDomain().getCodeSource();
      dbg.append("codeSource=").append(cs).append('\n');
      if (cs != null) {
        java.net.URL url = cs.getLocation();
        dbg.append("location=").append(url).append('\n');
        java.io.File loc = new java.io.File(url.toURI());
        dbg.append("loc=").append(loc).append(" isDir=").append(loc.isDirectory()).append('\n');
        if (!loc.isDirectory()) {
          // It's CppMode.jar → go up two levels to get mode root
          modeRoot = loc.getParentFile().getParentFile();
        } else {
          // Loaded from classes dir; go up until we find a libs/ sibling
          java.io.File f = loc;
          for (int i = 0; i < 6 && f != null; i++, f = f.getParentFile()) {
            if (new java.io.File(f, "libs").isDirectory()) { modeRoot = f; break; }
          }
        }
        dbg.append("modeRoot=").append(modeRoot).append('\n');
      }
    } catch (Throwable t) {
      dbg.append("codeSource error: ").append(t).append('\n');
    }

    // Step 3: also check the hardcoded sketchbook path
    java.io.File sketchbookMode = new java.io.File(
        System.getProperty("user.home"), "sketchbook/modes/CppMode");
    java.io.File sketchbookSo = new java.io.File(
        sketchbookMode, "libs/linux-x64/libcppmode-ts.so");
    dbg.append("sketchbook .so exists=").append(sketchbookSo.exists()).append('\n');
    if (modeRoot != null) {
      java.io.File derivedSo = new java.io.File(
          modeRoot, "libs/linux-x64/libcppmode-ts.so");
      dbg.append("derived .so exists=").append(derivedSo.exists()).append('\n');
    }

    // Step 4: write the debug file before we attempt anything that could crash
    try (java.io.FileWriter fw = new java.io.FileWriter(debugFile)) {
      fw.write(dbg.toString());
    } catch (Throwable ignored) {}

    // Step 5: try init with candidates
    if (modeRoot != null && Ts.reinit(modeRoot)) {
      INSTANCE = new TsEngine(); return;
    }
    if (Ts.reinit(sketchbookMode)) {
      INSTANCE = new TsEngine();
    }
  }

  /**
   * Initialise the engine (load native lib).  Safe to call more than once;
   * subsequent calls are no-ops.
   */
  public static synchronized void init(File modeFolder) {
    if (INSTANCE != null) return;
    if (Ts.init(modeFolder)) {
      INSTANCE = new TsEngine();
    }
  }

  /** CLI / test variant for API compatibility. */
  public static synchronized void initForCli(File nativeLib, File modeFolder) {
    init(modeFolder);
  }

  // ── Construction ─────────────────────────────────────────────────────────

  private TsEngine() {}

  // ── Public API ────────────────────────────────────────────────────────────

  /**
   * Parse {@code source} and return the result.  The caller is responsible for
   * calling {@link ParseResult#close()} when done to free the native tree.
   */
  public ParseResult parse(String source) {
    long tree = Ts.parse(source);
    return new ParseResult(source, tree);
  }

  /** Convenience: parse and immediately check for syntax errors. */
  public boolean hasErrors(String source) {
    try (ParseResult r = parse(source)) {
      return r.hasErrors();
    }
  }

  // ── ParseResult ───────────────────────────────────────────────────────────

  /**
   * Holds a parsed tree together with its source text.
   * Must be used in a try-with-resources block so the native tree is freed.
   */
  public static final class ParseResult implements AutoCloseable {

    private final String source;
    private final long   tree;   // TSTree*

    private ParseResult(String source, long tree) {
      this.source = source;
      this.tree   = tree;
    }

    /** Raw source text. */
    public String source() { return source; }

    /** Root node of the parse tree; null if parse failed. */
    public TsNode root() {
      return TsNode.wrap(Ts.rootNode(tree));
    }

    /** True if the tree contains any ERROR or MISSING nodes. */
    public boolean hasErrors() {
      TsNode r = root();
      return r == null || r.hasError();
    }

    /**
     * Source text for a node by byte offsets.
     * Byte == char for ASCII; typical Processing sketches are ASCII-safe.
     */
    public String textOf(TsNode n) {
      int s = n.getStartByte(), e = n.getEndByte();
      if (s < 0 || e > source.length() || s > e) return "";
      return source.substring(s, e);
    }

    /** Source text for a named field child, or {@code ""} if absent. */
    public String fieldText(TsNode n, String field) {
      TsNode child = n.getChildByFieldName(field);
      return (child != null) ? textOf(child) : "";
    }

    @Override
    public void close() {
      if (tree != 0) Ts.freeTree(tree);
    }
  }
}
