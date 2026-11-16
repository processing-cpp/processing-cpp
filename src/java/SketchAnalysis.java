package processing.mode.cpp;

import java.util.List;

/**
 * Immutable result of analysing a Processing sketch's concrete syntax tree.
 *
 * <p>Every field is a {@code List} of {@link Item} records extracted directly
 * from the tree-sitter CST.  The lists preserve document order so the emitter
 * can reconstruct source text faithfully; ordering <em>across</em> categories
 * is the emitter's responsibility (e.g. classes go before the namespace, etc.).
 *
 * <p>All {@link Item} records carry the raw UTF-8 byte span of their node so
 * the emitter can slice original source text with {@link
 * TsEngine.ParseResult#textOf} rather than pretty-printing from scratch.
 * Byte offsets follow tree-sitter's convention (0-indexed, exclusive end).
 */
public record SketchAnalysis(

    /** {@code #include}, {@code #define}, {@code #pragma}, etc. — always file-scope. */
    List<Item> preprocessorLines,

    /** Top-level {@code using namespace Foo;} or {@code using Foo::Bar;} — file-scope. */
    List<Item> usingDeclarations,

    /** Top-level {@code enum} / {@code enum class} declarations — file-scope. */
    List<Item> enumDeclarations,

    /**
     * Top-level class / struct / union definitions that must be hoisted
     * <em>above</em> the {@code namespace Processing} block so that the
     * PSketch struct can inherit from them.  Ordering within this list
     * respects forward-reference constraints (classes that reference other
     * classes appear after them).
     */
    List<Item> hoistedClasses,

    /**
     * Declarations of global variables — they become data members of
     * the generated {@code PSketch} struct.
     */
    List<Item> globalVariables,

    /**
     * Top-level function definitions whose names appear in the lifecycle set
     * ({@code setup}, {@code draw}, {@code mousePressed}, …).  The emitter
     * adds {@code override} to each and wraps them inside PSketch.
     */
    List<Item> lifecycleFunctions,

    /**
     * All other top-level function definitions.  They become ordinary methods
     * (or static free-functions, depending on emit strategy) inside PSketch.
     */
    List<Item> globalFunctions,

    /**
     * Forward-declare candidates: functions referenced before they are
     * defined.  The analyser populates this after a lightweight reference
     * scan so the emitter can prepend forward declarations inside the
     * namespace.
     */
    List<ForwardDecl> forwardDecls,

    /**
     * Bare top-level statements ({@code size(400,400);}), in document order.
     * Only meaningful in static mode, where they become the body of setup().
     */
    List<Item> topLevelStatements,

    /**
     * Sketch functions that user classes call. The emitter declares a free
     * function for each before the classes and defines it after PSketch,
     * forwarding to the running sketch.
     */
    List<Forwarder> forwarders,

    /** Whether the sketch has no {@code setup()} or {@code draw()} at all. */
    boolean isStaticMode

) {

  // ── Item ─────────────────────────────────────────────────────────────────

  /**
   * A single item extracted from the CST.
   *
   * @param nodeType    the tree-sitter node type string (e.g.
   *                    {@code "function_definition"})
   * @param startByte   UTF-8 start byte in the original source
   * @param endByte     UTF-8 end byte (exclusive) in the original source
   * @param name        identifier / function name if applicable, else {@code ""}
   * @param isLifecycle {@code true} if this function is a lifecycle method
   * @param isPointerReturn {@code true} if the return type contains {@code *}
   *                        (used by the emitter to emit {@code = nullptr}
   *                        defaults where appropriate)
   */
  public record Item(
      String  nodeType,
      int     startByte,
      int     endByte,
      String  name,
      boolean isLifecycle,
      boolean isPointerReturn
  ) {}

  // ── Forwarder ────────────────────────────────────────────────────────────

  /**
   * @param returnType text before the function name, e.g. {@code "void"}
   * @param name       function name
   * @param declParams parameter list as written, with defaults, e.g. {@code "(int x = 5)"}
   * @param defParams  same without default values, for the out-of-line definition
   * @param args       argument list forwarding each parameter, e.g. {@code "(x)"}
   */
  public record Forwarder(String returnType, String name,
                          String declParams, String defParams, String args) {}

  // ── ForwardDecl ───────────────────────────────────────────────────────────

  /**
   * A minimal forward declaration derived from a {@link Item} that represents
   * a function definition.  The emitter uses the raw text segments to
   * reconstruct the declaration without re-parsing.
   *
   * @param returnType   text of the return type (may include modifiers)
   * @param name         function name
   * @param params       text of the parameter list (with parentheses), e.g.
   *                     {@code "(int x, float y)"}
   * @param isConst      whether the function is const-qualified
   */
  public record ForwardDecl(
      String  returnType,
      String  name,
      String  params,
      boolean isConst
  ) {}
}
