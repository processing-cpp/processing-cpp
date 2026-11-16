package processing.mode.cpp;

/**
 * Lightweight wrapper around a packed tree-sitter TSNode (long[4]).
 *
 * Provides an object-oriented API over the static {@link Ts} JNI methods so
 * that callers don't have to pass arrays around directly.  No heap allocation
 * per field access — every method delegates straight to a native call.
 *
 * A {@code null} return from any child accessor means the child does not exist
 * (mirrors the ai.seart.treesitter.Node API we replaced).
 */
public final class TsNode {

  final long[] data; // packed TSNode: long[4] = 32 bytes

  TsNode(long[] data) {
    this.data = data;
  }

  /** Wrap a raw long[] from a Ts.* call; returns null if data is null. */
  static TsNode wrap(long[] data) {
    return (data == null) ? null : new TsNode(data);
  }

  // ── Type / identity ───────────────────────────────────────────────────────

  /** Grammar node type, e.g. {@code "function_definition"}. */
  public String getType()            { return Ts.nodeType(data); }

  /** True if this is a named (non-anonymous) node. */
  public boolean isNamed()           { return Ts.nodeIsNamed(data); }

  // ── Byte offsets ──────────────────────────────────────────────────────────

  public int getStartByte()          { return Ts.nodeStartByte(data); }
  public int getEndByte()            { return Ts.nodeEndByte(data); }

  // ── Row / col (0-based) ───────────────────────────────────────────────────

  public int getStartRow()           { return Ts.nodeStartRow(data); }
  public int getStartCol()           { return Ts.nodeStartCol(data); }
  public int getEndRow()             { return Ts.nodeEndRow(data); }
  public int getEndCol()             { return Ts.nodeEndCol(data); }

  // ── Children ──────────────────────────────────────────────────────────────

  /** Total child count (named + anonymous). */
  public int getChildCount()         { return Ts.nodeChildCount(data); }

  /** Named child count. */
  public int getNamedChildCount()    { return Ts.nodeNamedChildCount(data); }

  /** i-th child (named or anonymous); null if out of range. */
  public TsNode getChild(int i)      { return wrap(Ts.nodeChild(data, i)); }

  /** i-th named child; null if out of range. */
  public TsNode getNamedChild(int i) { return wrap(Ts.nodeNamedChild(data, i)); }

  /** Child by field name (e.g. {@code "name"}, {@code "body"}); null if absent. */
  public TsNode getChildByFieldName(String field) {
    return wrap(Ts.nodeChildByFieldName(data, field));
  }

  // ── Error flags ───────────────────────────────────────────────────────────

  /** True if this subtree contains any ERROR or MISSING node. */
  public boolean hasError()          { return Ts.nodeHasError(data); }

  /** True if this node itself is an ERROR node. */
  public boolean isError()           { return Ts.nodeIsError(data); }

  /** True if this node is a MISSING node (error-recovery insertion). */
  public boolean isMissing()         { return Ts.nodeIsMissing(data); }

  // ── Convenience ───────────────────────────────────────────────────────────

  @Override
  public String toString() {
    return "TsNode[" + getType() + " @" + getStartByte() + ".." + getEndByte() + "]";
  }
}
