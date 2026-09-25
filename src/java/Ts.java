package processing.mode.cpp;

import java.io.File;

public final class Ts {
  private Ts() {}

  private static volatile boolean available = false;
  private static volatile boolean tried     = false;

  public static synchronized boolean init(File modeFolder) {
    if (tried) return available;
    tried = true;
    try {
      String os = System.getProperty("os.name", "").toLowerCase();
      String subdir, libname;
      if (os.contains("linux"))      { subdir = "linux-x64";   libname = "libcppmode-ts.so"; }
      else if (os.contains("mac"))   { subdir = "macos";        libname = "libcppmode-ts.dylib"; }
      else if (os.contains("win"))   { subdir = "windows-x64";  libname = "cppmode-ts.dll"; }
      else { System.err.println("[CppMode] tree-sitter: unsupported OS"); return false; }
      File lib = new File(modeFolder, "libs/" + subdir + "/" + libname);
      if (!lib.exists()) { System.err.println("[CppMode] ts lib not found: " + lib); return false; }
      System.load(lib.getAbsolutePath());
      available = true;
    } catch (Throwable t) {
      System.err.println("[CppMode] tree-sitter load failed: " + t);
    }
    return available;
  }

  public static boolean isAvailable() { return available; }

  public static native long    parse(String source);
  public static native void    freeTree(long tree);
  public static native long[]  rootNode(long tree);
  public static native String  nodeType(long[] node);
  public static native int     nodeStartByte(long[] node);
  public static native int     nodeEndByte(long[] node);
  public static native int     nodeStartRow(long[] node);
  public static native int     nodeStartCol(long[] node);
  public static native int     nodeEndRow(long[] node);
  public static native int     nodeEndCol(long[] node);
  public static native int     nodeChildCount(long[] node);
  public static native int     nodeNamedChildCount(long[] node);
  public static native long[]  nodeChild(long[] node, int i);
  public static native long[]  nodeNamedChild(long[] node, int i);
  public static native long[]  nodeChildByFieldName(long[] node, String field);
  public static native boolean nodeIsNamed(long[] node);
  public static native boolean nodeHasError(long[] node);
  public static native boolean nodeIsError(long[] node);
  public static native boolean nodeIsMissing(long[] node);

  public static String textOf(long[] node, String source) {
    int s = nodeStartByte(node), e = nodeEndByte(node);
    if (s < 0 || e > source.length() || s > e) return "";
    return source.substring(s, e);
  }

  public static boolean hasErrors(String source) {
    long tree = parse(source);
    if (tree == 0) return true;
    try { long[] root = rootNode(tree); return root == null || nodeHasError(root); }
    finally { freeTree(tree); }
  }
}
