package processing.mode.cpp;

import java.io.File;

/**
 * Quick diagnostic: loads libcppmode-ts and parses a trivial snippet.
 * Exit 0 = OK, Exit 1 = failure.
 *
 * Usage:
 *   java -cp CppMode.jar processing.mode.cpp.TsProbe [mode-folder]
 */
public class TsProbe {
  public static void main(String[] args) {
    try {
      File folder = args.length > 0 ? new File(args[0]) : new File(".");
      if (!Ts.init(folder)) {
        System.err.println("[TsProbe] Failed to load native library from: " + folder);
        System.exit(1);
      }
      long tree = Ts.parse("void setup() {}");
      if (tree == 0) {
        System.err.println("[TsProbe] parse() returned null tree");
        System.exit(1);
      }
      long[] root = Ts.rootNode(tree);
      if (root == null) {
        Ts.freeTree(tree);
        System.err.println("[TsProbe] rootNode() returned null");
        System.exit(1);
      }
      String rootType = Ts.nodeType(root);  // must be called before freeTree
      Ts.freeTree(tree);
      System.out.println("[TsProbe] OK — root node type: " + rootType);
      System.exit(0);
    } catch (Throwable t) {
      System.err.println("[TsProbe] Exception: " + t);
      System.exit(1);
    }
  }
}
