package processing.mode.cpp;

import processing.app.Problem;
import java.util.ArrayList;
import java.util.List;

/**
 * Fast tree-sitter pass over the sketch source.
 * Walks the parse tree and emits a CppProblem for every ERROR or MISSING
 * node.  Runs in ~1ms; used as a first-pass before g++ kicks in.
 */
public class TsParser {

  private TsParser() {}

  /**
   * Parse source and return syntax-error problems.
   * tabIndex is the sketch tab this source belongs to (for CppProblem).
   * tabStartLines[i] maps concatenated-source line → tab index, same as
   * CppLinter's own mapping -- but here we receive per-tab source so we
   * always pass 0-indexed lines within that tab.
   */
  public static List<Problem> check(String source, int tabIndex) {
    List<Problem> out = new ArrayList<>();
    if (!Ts.isAvailable()) return out;
    long tree = Ts.parse(source);
    if (tree == 0) return out;
    try {
      long[] root = Ts.rootNode(tree);
      if (root == null) return out;
      if (!Ts.nodeHasError(root)) return out;  // fast exit — no errors
      walk(root, source, tabIndex, out);
    } finally {
      Ts.freeTree(tree);
    }
    return out;
  }

  private static void walk(long[] node, String source, int tabIndex, List<Problem> out) {
    if (Ts.nodeIsError(node)) {
      int row = Ts.nodeStartRow(node);
      int col = Ts.nodeStartCol(node);
      String text = Ts.textOf(node, source);
      String snippet = text.length() > 40 ? text.substring(0, 40) + "…" : text;
      String msg = snippet.isEmpty()
          ? "Syntax error"
          : "Syntax error near: " + snippet.replace('\n', ' ');
      out.add(new CppProblem(true, tabIndex, row, col, -1, msg));
      return; // don't recurse into error nodes — one problem per cluster
    }
    // MISSING nodes omitted -- too many false positives on valid Processing sketches
    int n = Ts.nodeChildCount(node);
    for (int i = 0; i < n; i++) {
      long[] child = Ts.nodeChild(node, i);
      if (child != null && Ts.nodeHasError(child)) {
        walk(child, source, tabIndex, out);
      }
    }
  }
}
