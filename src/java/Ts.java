package processing.mode.cpp;

import java.io.File;

/**
 * Thin JNI binding to our vendored tree-sitter + tree-sitter-cpp native lib.
 *
 * Nodes are passed as long[4] (a packed TSNode struct — 32 bytes on 64-bit) to
 * avoid per-node Java heap allocation and GC pressure.  The caller owns trees
 * and must call freeTree() when done.
 *
 * Usage:
 *   if (!Ts.init(modeFolder)) { ... fallback ... }
 *   long tree = Ts.parse(source);
 *   long[] root = Ts.rootNode(tree);
 *   // walk...
 *   Ts.freeTree(tree);
 */
public final class Ts {

  private Ts() {}

  // ── init ──────────────────────────────────────────────────────────────────

  private static volatile boolean available = false;
  private static volatile boolean tried     = false;

  /**
   * Load the native library from the mode's libs/linux-x64/ directory.
   * Safe to call multiple times.  Returns true if the library is available.
   */
  public static synchronized boolean init(File modeFolder) {
    if (tried) return available;
    tried = true;
    try {
      String os = System.getProperty("os.name", "").toLowerCase();
      String subdir;
      String libname;
      if (os.contains("linux")) {
        subdir  = "linux-x64";
        libname = "libcppmode-ts.so";
      } else if (os.contains("mac")) {
        subdir  = System.getProperty("os.arch", "").matches("aarch64|arm64") ? "macos-arm64" : "macos-x64";
        libname = "libcppmode-ts.dylib";
      } else if (os.contains("win")) {
        subdir  = "windows-x64";
        libname = "cppmode-ts.dll";
      } else {
        System.err.println("[CppMode] tree-sitter: unsupported OS: " + os);
        return false;
      }

      // Build candidate paths.
      java.util.List<File> candidates = new java.util.ArrayList<>();
      // 1. Passed modeFolder
      candidates.add(new File(modeFolder, "libs/" + subdir + "/" + libname));
      // 2. Jar-relative: CppMode.jar lives at <modeRoot>/mode/CppMode.jar
      //    so go up two levels from the jar to find <modeRoot>
      try {
        java.net.URL url = Ts.class.getProtectionDomain().getCodeSource().getLocation();
        File jarFile = new File(url.toURI());
        File modeRoot = jarFile.getParentFile().getParentFile(); // .../CppMode/
        candidates.add(new File(modeRoot, "libs/" + subdir + "/" + libname));
      } catch (Throwable ignored) {}
      // 3. Hardcoded sketchbook fallback
      candidates.add(new File(System.getProperty("user.home"),
          "sketchbook/modes/CppMode/libs/" + subdir + "/" + libname));

      System.err.println("[CppMode] tree-sitter init: modeFolder=" + modeFolder.getAbsolutePath());

      for (File lib : candidates) {
        System.err.println("[CppMode] tree-sitter: trying " + lib.getAbsolutePath()
            + " exists=" + lib.exists());
        if (!lib.exists()) continue;
        try {
          System.load(lib.getAbsolutePath());
          available = true;
          System.err.println("[CppMode] tree-sitter: loaded " + lib.getAbsolutePath());
          return true;
        } catch (UnsatisfiedLinkError ule) {
          // "already loaded in another classloader" means it IS loaded — treat as OK.
          if (ule.getMessage() != null && ule.getMessage().contains("already loaded")) {
            available = true;
            System.err.println("[CppMode] tree-sitter: already loaded (shared classloader)");
            return true;
          }
          System.err.println("[CppMode] tree-sitter: load failed for " + lib + ": " + ule);
        }
      }
      System.err.println("[CppMode] tree-sitter: no candidate loaded.");
    } catch (Throwable t) {
      System.err.println("[CppMode] tree-sitter native load failed: " + t);
    }
    return available;
  }

  public static boolean isAvailable() { return available; }

  /** Force a fresh init attempt even if a previous one failed. */
  public static synchronized boolean reinit(File modeFolder) {
    tried = false;
    available = false;
    return init(modeFolder);
  }

  // ── tree lifetime ─────────────────────────────────────────────────────────

  /** Parse UTF-8 source; returns opaque tree handle, or 0 on failure. */
  public static native long parse(String source);

  /** Free a tree returned by parse(). */
  public static native void freeTree(long tree);

  // ── node access (node = long[4]) ──────────────────────────────────────────

  /** Root node of the tree, or null if tree == 0. */
  public static native long[] rootNode(long tree);

  /** Grammar node type string, e.g. "function_definition". */
  public static native String nodeType(long[] node);

  /** UTF-8 byte offset of the node's start in the source. */
  public static native int nodeStartByte(long[] node);

  /** UTF-8 byte offset of the node's end in the source. */
  public static native int nodeEndByte(long[] node);

  /** 0-based row of the node's start position. */
  public static native int nodeStartRow(long[] node);

  /** 0-based column (byte) of the node's start position. */
  public static native int nodeStartCol(long[] node);

  /** 0-based row of the node's end position. */
  public static native int nodeEndRow(long[] node);

  /** 0-based column (byte) of the node's end position. */
  public static native int nodeEndCol(long[] node);

  /** Total child count (named + anonymous). */
  public static native int nodeChildCount(long[] node);

  /** Named child count. */
  public static native int nodeNamedChildCount(long[] node);

  /** i-th child (named or anonymous), or null if out of range. */
  public static native long[] nodeChild(long[] node, int i);

  /** i-th named child, or null if out of range. */
  public static native long[] nodeNamedChild(long[] node, int i);

  /** Child by field name (e.g. "name", "body"), or null if absent. */
  public static native long[] nodeChildByFieldName(long[] node, String field);

  /** True if the node is a named (non-anonymous) node. */
  public static native boolean nodeIsNamed(long[] node);

  /** True if the subtree rooted here contains any ERROR or MISSING nodes. */
  public static native boolean nodeHasError(long[] node);

  /** True if this node itself is an ERROR node. */
  public static native boolean nodeIsError(long[] node);

  /** True if this node is a MISSING node (inserted by error recovery). */
  public static native boolean nodeIsMissing(long[] node);

  // ── convenience ───────────────────────────────────────────────────────────

  /**
   * Extract the source text for a node.
   * Byte == char for ASCII; Processing sketches are typically ASCII-safe.
   */
  public static String textOf(long[] node, String source) {
    int s = nodeStartByte(node);
    int e = nodeEndByte(node);
    if (s < 0 || e > source.length() || s > e) return "";
    return source.substring(s, e);
  }

  /** Parse and immediately check for syntax errors. */
  public static boolean hasErrors(String source) {
    long tree = parse(source);
    if (tree == 0) return true;
    try {
      long[] root = rootNode(tree);
      return root == null || nodeHasError(root);
    } finally {
      freeTree(tree);
    }
  }
}
