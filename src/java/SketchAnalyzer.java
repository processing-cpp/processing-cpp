package processing.mode.cpp;



import java.util.*;

/**
 * Walks a tree-sitter-processing-cpp CST and produces a {@link SketchAnalysis}.
 *
 * <h2>Pipeline role</h2>
 * <pre>
 *   TsEngine.parse(source) → ParseResult
 *                          → SketchAnalyzer.analyse(result) → SketchAnalysis
 *                          → SketchEmitter.emit(analysis, result) → String (C++ source)
 * </pre>
 *
 * <h2>Classification rules</h2>
 * <ul>
 *   <li>Nodes of type {@code preproc_include}, {@code preproc_def},
 *       {@code preproc_ifdef}, {@code preproc_if}, {@code preproc_call},
 *       {@code preproc_function_def} → {@code preprocessorLines}</li>
 *   <li>{@code using_declaration} with no {@code =} → {@code usingDeclarations}
 *       when they name a namespace; type aliases go into the namespace body.</li>
 *   <li>{@code enum_specifier} at top level → {@code enumDeclarations}</li>
 *   <li>{@code class_specifier} / {@code struct_specifier} /
 *       {@code union_specifier} at top level → {@code hoistedClasses}</li>
 *   <li>{@code declaration} at top level (not a typedef) → {@code globalVariables}</li>
 *   <li>{@code function_definition} whose name is in {@link #LIFECYCLE_NAMES}
 *       → {@code lifecycleFunctions}; all others → {@code globalFunctions}</li>
 *   <li>{@code template_declaration} wrapping a function or class → forwarded
 *       into the appropriate bucket.</li>
 * </ul>
 */
public final class SketchAnalyzer {

  // ── Lifecycle set ─────────────────────────────────────────────────────────

  public static final Set<String> LIFECYCLE_NAMES = Set.of(
      "setup", "draw", "settings",
      "mousePressed", "mouseReleased", "mouseClicked",
      "mouseMoved", "mouseDragged", "mouseWheel",
      "keyPressed", "keyReleased", "keyTyped",
      "windowMoved", "windowResized"
  );

  // Processing types that must be hoisted as class names (not variable names)
  private static final Set<String> PROCESSING_TYPES = Set.of(
      "PVector", "PImage", "PGraphics", "PShape", "PFont", "PShader",
      "Table", "TableRow", "XML", "JSONObject", "JSONArray",
      "BufferedReader", "PrintWriter"
  );

  // ── Entry point ───────────────────────────────────────────────────────────

  public static SketchAnalysis analyse(TsEngine.ParseResult result) {
    return new SketchAnalyzer(result).run();
  }

  // ── Instance ──────────────────────────────────────────────────────────────

  private final TsEngine.ParseResult result;

  private final List<SketchAnalysis.Item> preprocessorLines  = new ArrayList<>();
  private final List<SketchAnalysis.Item> usingDeclarations  = new ArrayList<>();
  private final List<SketchAnalysis.Item> enumDeclarations   = new ArrayList<>();
  private final List<SketchAnalysis.Item> hoistedClasses     = new ArrayList<>();
  private final List<SketchAnalysis.Item> globalVariables    = new ArrayList<>();
  private final List<SketchAnalysis.Item> lifecycleFunctions = new ArrayList<>();
  private final List<SketchAnalysis.Item> globalFunctions    = new ArrayList<>();
  private final List<SketchAnalysis.Item> topLevelStatements = new ArrayList<>();
  // Non-template, non-lifecycle function_definition nodes, for forwarders
  private final List<TsNode> plainFunctionNodes = new ArrayList<>();

  // Names of all top-level functions defined in the sketch (for fwd-decl generation)
  private final Map<String, SketchAnalysis.Item> functionsByName = new LinkedHashMap<>();
  // Names called anywhere at top level before they are defined
  private final Set<String> calledNames = new LinkedHashSet<>();

  private SketchAnalyzer(TsEngine.ParseResult result) {
    this.result = result;
  }

  private SketchAnalysis run() {
    TsNode root = result.root();
    // First pass: classify every top-level node
    for (int i = 0; i < root.getChildCount(); i++) {
      classifyTopLevel(root.getChild(i));
    }

    // Second pass: generate forward declarations for functions referenced
    // before they appear in document order.
    List<SketchAnalysis.ForwardDecl> forwardDecls = buildForwardDecls();

    boolean hasSetup = lifecycleFunctions.stream()
        .anyMatch(it -> it.name().equals("setup"));
    boolean hasDraw  = lifecycleFunctions.stream()
        .anyMatch(it -> it.name().equals("draw"));
    boolean isStaticMode = !hasSetup && !hasDraw;

    return new SketchAnalysis(
        List.copyOf(preprocessorLines),
        List.copyOf(usingDeclarations),
        List.copyOf(enumDeclarations),
        sortHoistedClasses(),
        List.copyOf(globalVariables),
        List.copyOf(lifecycleFunctions),
        List.copyOf(globalFunctions),
        forwardDecls,
        List.copyOf(topLevelStatements),
        buildForwarders(),
        isStaticMode
    );
  }

  // ── Top-level node classification ─────────────────────────────────────────

  private void classifyTopLevel(TsNode n) {
    if (!n.isNamed()) return; // skip anonymous (punctuation) nodes

    String type = n.getType();
    switch (type) {
      case "preproc_include",
           "preproc_def",
           "preproc_function_def",
           "preproc_call",
           "preproc_ifdef",
           "preproc_if",
           "preproc_arg" ->
          preprocessorLines.add(item(n, "", false));

      case "using_declaration" -> {
        String text = result.textOf(n);
        // "using namespace Foo;" goes to file scope; type aliases stay in namespace
        if (text.contains("namespace"))
          usingDeclarations.add(item(n, "", false));
        else
          globalVariables.add(item(n, "", false)); // type aliases / using-decls
      }

      case "alias_declaration" ->
          globalVariables.add(item(n, "", false));

      case "enum_specifier" ->
          enumDeclarations.add(item(n, typeName(n), false));

      case "class_specifier", "struct_specifier", "union_specifier" ->
          hoistedClasses.add(item(n, typeName(n), false));

      case "template_declaration" ->
          classifyTemplate(n);

      case "function_definition" ->
          classifyFunction(n);

      case "declaration" ->
          classifyDeclaration(n);

      case "expression_statement" -> {
          // static-mode bare statement: becomes part of setup()
          topLevelStatements.add(item(n, "", false));
          collectCallees(n);
      }

      case "comment" -> { /* skip */ }

      default ->
          // Anything else (namespace_definition, linkage_specification, …)
          // goes through verbatim as a global variable / statement
          globalVariables.add(item(n, "", false));
    }
  }

  // ── Template declarations ─────────────────────────────────────────────────

  private void classifyTemplate(TsNode n) {
    // template<...> class Foo { ... }; → hoisted class
    // template<...> void foo() { ... } → function
    TsNode body = firstNamedChildOfTypes(n, "class_specifier", "struct_specifier",
        "union_specifier", "function_definition", "declaration");
    if (body == null) {
      globalVariables.add(item(n, "", false));
      return;
    }
    switch (body.getType()) {
      case "class_specifier", "struct_specifier", "union_specifier" ->
          hoistedClasses.add(item(n, typeName(body), false));
      case "function_definition" ->
          classifyFunctionNode(n, body);  // use outer node span, inner for name
      default ->
          globalVariables.add(item(n, "", false));
    }
  }

  // ── Function classification ───────────────────────────────────────────────

  private void classifyFunction(TsNode n) {
    classifyFunctionNode(n, n);
  }

  private void classifyFunctionNode(TsNode outerNode, TsNode funcNode) {
    String name = functionName(funcNode);
    boolean isPointerReturn = isPointerReturnType(funcNode);
    boolean isLifecycle = LIFECYCLE_NAMES.contains(name);

    SketchAnalysis.Item item = item(outerNode, name, isLifecycle, isPointerReturn);
    if (!isLifecycle && outerNode == funcNode) plainFunctionNodes.add(funcNode);

    if (isLifecycle) {
      lifecycleFunctions.add(item);
    } else {
      globalFunctions.add(item);
    }
    functionsByName.put(name, item);

    // Collect callee names from the function body for forward-decl analysis
    TsNode body = funcNode.getChildByFieldName("body");
    if (body != null) collectCallees(body);
  }

  // ── Variable / declaration classification ────────────────────────────────

  private void classifyDeclaration(TsNode n) {
    // Skip pure typedef lines that are Processing type aliases
    TsNode typeNode = n.getChildByFieldName("type");
    if (typeNode != null && "typedef".equals(result.textOf(typeNode).trim())) {
      globalVariables.add(item(n, "", false));
      return;
    }
    // function pointer typedef or other non-variable declaration
    globalVariables.add(item(n, declaratorName(n), false));
  }

  // ── Forwarders for sketch functions called from user classes ─────────────

  /**
   * Processing lets inner classes call the sketch's own functions. Our
   * classes live outside PSketch, so for every sketch function whose name
   * appears as a call inside a class body we build a free forwarding
   * function. Functions we can't forward safely (templates, auto/static
   * returns, pointer/reference returns, unnamed params) are skipped and
   * g++ reports the call as usual.
   */
  private List<SketchAnalysis.Forwarder> buildForwarders() {
    if (hoistedClasses.isEmpty() || plainFunctionNodes.isEmpty()) return List.of();
    StringBuilder classText = new StringBuilder();
    for (SketchAnalysis.Item c : hoistedClasses)
      classText.append(result.source(), c.startByte(), c.endByte()).append('\n');

    List<SketchAnalysis.Forwarder> out = new ArrayList<>();
    for (TsNode fn : plainFunctionNodes) {
      SketchAnalysis.Forwarder f = forwarderFor(fn);
      if (f == null) continue;
      if (!java.util.regex.Pattern.compile("(?<![\\w.>:])" + java.util.regex.Pattern.quote(f.name()) + "\\s*\\(")
          .matcher(classText).find()) continue;
      out.add(f);
    }
    return List.copyOf(out);
  }

  private SketchAnalysis.Forwarder forwarderFor(TsNode fn) {
    TsNode decl = fn.getChildByFieldName("declarator");
    if (decl == null || !"function_declarator".equals(decl.getType())) return null;
    TsNode nameNode = decl.getChildByFieldName("declarator");
    TsNode params = decl.getChildByFieldName("parameters");
    if (nameNode == null || params == null || !"identifier".equals(nameNode.getType())) return null;

    String src = result.source();
    String returnType = src.substring(fn.getStartByte(), decl.getStartByte()).trim();
    if (returnType.isEmpty() || returnType.matches("(?s).*\\b(static|auto|virtual|inline|constexpr|consteval)\\b.*"))
      return null;
    // anything between ')' and the body (const, noexcept, -> trailing type) : skip
    TsNode body = fn.getChildByFieldName("body");
    if (body != null && !src.substring(params.getEndByte(), body.getStartByte()).isBlank()) return null;

    StringBuilder defP = new StringBuilder("(");
    StringBuilder args = new StringBuilder("(");
    int n = 0;
    for (int i = 0; i < params.getNamedChildCount(); i++) {
      TsNode p = params.getNamedChild(i);
      String type = p.getType();
      if ("comment".equals(type)) continue;
      if (!type.equals("parameter_declaration") && !type.equals("optional_parameter_declaration"))
        return null; // variadic etc.
      TsNode pd = p.getChildByFieldName("declarator");
      if (pd == null) return null;               // unnamed parameter
      TsNode id = leafNode(pd);
      if (id == null || !"identifier".equals(id.getType())) return null;
      int end = p.getEndByte();
      TsNode dflt = p.getChildByFieldName("default_value");
      if (dflt != null) end = pd.getEndByte();
      if (n++ > 0) { defP.append(", "); args.append(", "); }
      defP.append(src, p.getStartByte(), end);
      args.append(result.textOf(id));
    }
    // void f(void)
    String declParams = src.substring(params.getStartByte(), params.getEndByte());
    return new SketchAnalysis.Forwarder(returnType, result.textOf(nameNode),
        declParams, defP.append(')').toString(), args.append(')').toString());
  }

  // ── Forward-declaration analysis ─────────────────────────────────────────

  /**
   * Very lightweight forward-reference detection: any function name that
   * appears in {@link #calledNames} but whose definition comes <em>after</em>
   * its first call site in document order gets a forward declaration.
   *
   * <p>This is conservative — it emits forward decls for all functions that
   * are ever called, not just the ordering-violating subset.  That is safe
   * because duplicate forward declarations are harmless in C++.
   */
  private List<SketchAnalysis.ForwardDecl> buildForwardDecls() {
    if (true) return List.of();
    List<SketchAnalysis.ForwardDecl> result = new ArrayList<>();
    for (Map.Entry<String, SketchAnalysis.Item> e : functionsByName.entrySet()) {
      if (LIFECYCLE_NAMES.contains(e.getKey())) continue; // already in PSketch
      result.add(buildForwardDecl(e.getKey(), e.getValue()));
    }
    return List.copyOf(result);
  }

  private SketchAnalysis.ForwardDecl buildForwardDecl(String name, SketchAnalysis.Item item) {
    // Re-parse the function node to extract return-type and param-list text.
    // This is done by slicing the original source rather than AST traversal,
    // since we only need the text.
    String src = result.source();
    String fullText = src.substring(item.startByte(), item.endByte());

    // Find the declarator node in the CST for precise spans
    // (Fall back to text heuristics if needed — accurate enough for fwd decls)
    int parenOpen  = fullText.indexOf('(');
    int parenClose = findMatchingParen(fullText, parenOpen);
    if (parenOpen < 0 || parenClose < 0) {
      return new SketchAnalysis.ForwardDecl("auto", name, "()", false);
    }
    String beforeParen = fullText.substring(0, parenOpen).trim();
    // returnType is everything before the function name in the declarator
    int nameEnd = beforeParen.lastIndexOf(name);
    String returnType = nameEnd > 0 ? beforeParen.substring(0, nameEnd).trim() : "void";
    String params     = fullText.substring(parenOpen, parenClose + 1);
    boolean isConst   = fullText.substring(parenClose + 1).stripLeading().startsWith("const");
    return new SketchAnalysis.ForwardDecl(returnType, name, params, isConst);
  }

  // ── Hoisted-class ordering ────────────────────────────────────────────────

  /**
   * Topologically sorts hoisted classes so that a base class appears before
   * its derived class.  Falls back to document order if cycles are detected.
   */
  private List<SketchAnalysis.Item> sortHoistedClasses() {
    if (hoistedClasses.size() <= 1) return List.copyOf(hoistedClasses);

    // Build a map name → item for quick lookup
    Map<String, SketchAnalysis.Item> byName = new LinkedHashMap<>();
    for (SketchAnalysis.Item c : hoistedClasses) byName.put(c.name(), c);

    // For each class, find base class names that are also in byName
    // by scanning the source text of each class node
    Map<String, List<String>> deps = new LinkedHashMap<>();
    for (SketchAnalysis.Item c : hoistedClasses) {
      String text = result.source().substring(c.startByte(), c.endByte());
      List<String> bases = new ArrayList<>();
      // Look for ": public Foo, protected Bar" patterns
      int colon = text.indexOf(':');
      if (colon > 0) {
        String afterColon = text.substring(colon + 1);
        int brace = afterColon.indexOf('{');
        if (brace > 0) {
          String basePart = afterColon.substring(0, brace);
          for (String tok : basePart.split("[,\\s]+")) {
            tok = tok.replaceAll("\\b(public|protected|private|virtual)\\b", "").trim();
            if (byName.containsKey(tok)) bases.add(tok);
          }
        }
      }
      deps.put(c.name(), bases);
    }

    // Kahn's topological sort
    Map<String, Integer> inDegree = new LinkedHashMap<>();
    for (String k : byName.keySet()) inDegree.put(k, 0);
    for (List<String> bases : deps.values())
      for (String b : bases) inDegree.merge(b, 0, Integer::sum); // ensure present
    for (Map.Entry<String, List<String>> e : deps.entrySet())
      for (String b : e.getValue()) inDegree.merge(e.getKey(), 1, Integer::sum);

    Queue<String> ready = new ArrayDeque<>();
    for (Map.Entry<String, Integer> e : inDegree.entrySet())
      if (e.getValue() == 0) ready.add(e.getKey());

    List<SketchAnalysis.Item> sorted = new ArrayList<>();
    while (!ready.isEmpty()) {
      String cur = ready.poll();
      if (byName.containsKey(cur)) sorted.add(byName.get(cur));
      for (Map.Entry<String, List<String>> e : deps.entrySet()) {
        if (e.getValue().contains(cur)) {
          int deg = inDegree.merge(e.getKey(), -1, Integer::sum);
          if (deg == 0) ready.add(e.getKey());
        }
      }
    }
    // If topo sort is incomplete (cycle), append remaining in document order
    Set<String> emitted = new LinkedHashSet<>();
    for (SketchAnalysis.Item it : sorted) emitted.add(it.name());
    for (SketchAnalysis.Item it : hoistedClasses)
      if (!emitted.contains(it.name())) sorted.add(it);

    return List.copyOf(sorted);
  }

  // ── Callee collection (for forward-decl analysis) ─────────────────────────

  /** Recursively collect all identifiers used in call_expression positions. */
  private void collectCallees(TsNode n) {
    if (n == null) return;
    if ("call_expression".equals(n.getType())) {
      TsNode fn = n.getChildByFieldName("function");
      if (fn != null && "identifier".equals(fn.getType())) {
        calledNames.add(result.textOf(fn));
      }
    }
    for (int i = 0; i < n.getChildCount(); i++) collectCallees(n.getChild(i));
  }

  // ── Helpers ───────────────────────────────────────────────────────────────

  /** Build an Item from a node. */
  private SketchAnalysis.Item item(TsNode n, String name, boolean isLifecycle) {
    return item(n, name, isLifecycle, false);
  }

  private SketchAnalysis.Item item(TsNode n, String name,
                                    boolean isLifecycle, boolean isPointerReturn) {
    return new SketchAnalysis.Item(
        n.getType(), n.getStartByte(), n.getEndByte(),
        name, isLifecycle, isPointerReturn);
  }

  /** Extract the name of a type definition (class/struct/union/enum). */
  private String typeName(TsNode n) {
    TsNode nameNode = n.getChildByFieldName("name");
    return nameNode != null ? result.textOf(nameNode) : "";
  }

  /** Extract the name of the first declarator in a declaration node. */
  private String declaratorName(TsNode n) {
    for (int i = 0; i < n.getChildCount(); i++) {
      TsNode c = n.getChild(i);
      if (c.getType().contains("declarator")) {
        return leafIdentifier(c);
      }
    }
    return "";
  }

  /** Extract the function name from a function_definition node. */
  private String functionName(TsNode n) {
    TsNode decl = n.getChildByFieldName("declarator");
    if (decl == null) return "";
    // function_declarator → declarator (identifier or nested_declarator)
    TsNode inner = decl.getChildByFieldName("declarator");
    if (inner != null) return result.textOf(leafNode(inner));
    return leafIdentifier(decl);
  }

  /** Whether the return type of a function_definition contains a '*'. */
  private boolean isPointerReturnType(TsNode n) {
    TsNode typeNode = n.getChildByFieldName("type");
    if (typeNode == null) return false;
    String text = result.textOf(typeNode);
    TsNode decl = n.getChildByFieldName("declarator");
    if (decl != null && "pointer_declarator".equals(decl.getType())) return true;
    return text.contains("*");
  }

  /** Walk down to the leftmost named leaf (identifier / field_identifier). */
  private TsNode leafNode(TsNode n) {
    if (n.getChildCount() == 0) return n;
    TsNode named = n.getChildByFieldName("declarator");
    if (named != null) return leafNode(named);
    for (int i = 0; i < n.getChildCount(); i++) {
      TsNode c = n.getChild(i);
      if (c.isNamed()) return leafNode(c);
    }
    return n;
  }

  private String leafIdentifier(TsNode n) {
    TsNode leaf = leafNode(n);
    return result.textOf(leaf);
  }

  /** Return the first named child whose type is one of the given types. */
  private static TsNode firstNamedChildOfTypes(TsNode n, String... types) {
    Set<String> wanted = Set.of(types);
    for (int i = 0; i < n.getChildCount(); i++) {
      TsNode c = n.getChild(i);
      if (c.isNamed() && wanted.contains(c.getType())) return c;
    }
    return null;
  }

  /** Find the matching close-paren index in {@code text} for an open-paren at {@code open}. */
  private static int findMatchingParen(String text, int open) {
    if (open < 0 || open >= text.length()) return -1;
    int depth = 0;
    for (int i = open; i < text.length(); i++) {
      char c = text.charAt(i);
      if (c == '(') depth++;
      else if (c == ')') { depth--; if (depth == 0) return i; }
    }
    return -1;
  }
}
