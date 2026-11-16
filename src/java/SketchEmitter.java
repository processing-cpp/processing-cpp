package processing.mode.cpp;

import java.util.List;
import java.util.regex.Pattern;

/**
 * Transforms a {@link SketchAnalysis} + raw source text into the final C++
 * source that will be compiled by {@code g++}.
 *
 * <h2>Output structure</h2>
 * <pre>
 *   // ── File scope ──────────────────────────────────────────────────────
 *   #include "Processing.h"
 *   #include &lt;user_headers&gt;
 *   #define  USER_MACROS
 *   using namespace UserNamespaces;
 *
 *   // ── Hoisted classes (sorted so base classes precede derived) ────────
 *   struct/class Foo { ... };
 *
 *   // ── Enum declarations ────────────────────────────────────────────────
 *   enum class Color { ... };
 *
 *   // ── Sketch namespace ────────────────────────────────────────────────
 *   namespace Processing {
 *
 *     // Forward declarations (prevent ordering errors)
 *     void helper(int x);
 *
 *     struct PSketch : public PApplet {
 *       // Global variables become data members
 *       float x = 0;
 *
 *       // Lifecycle functions with 'override'
 *       void setup() override { size(800, 600); }
 *       void draw()  override { ellipse(x, 50, 80, 80); }
 *
 *       // User-defined global functions become methods
 *       void helper(int x) { ... }
 *     };
 *
 *   } // namespace Processing
 * </pre>
 *
 * <h2>Source reconstruction</h2>
 * All items are emitted by slicing the <em>original</em> source via byte
 * offsets stored in {@link SketchAnalysis.Item}.  Only two mutations are
 * applied as text:
 * <ol>
 *   <li>Lifecycle functions gain an {@code override} keyword in their
 *       declarator.</li>
 *   <li>Pointer-typed data members without an initializer get {@code = nullptr}.</li>
 * </ol>
 * Everything else (formatting, comments, blank lines) is preserved verbatim.
 */
public final class SketchEmitter {

  // Injected at the very top so Processing.h is always included.
  // The CALLBACK guard fixes glu.h on Linux (it's a Windows __stdcall macro).
  private static final String PROCESSING_INCLUDE =
      "#ifndef CALLBACK\n#define CALLBACK\n#endif\n"
      + "#include \"Processing.h\"\n";

  private static final String CLASS_API = "#include \"Processing_api.h\"\n";

  // ── Regex helpers for in-place text mutations ─────────────────────────────

  // Matches a function signature up to the opening brace/semicolon so we can
  // splice "override" after the parameter list (and optional cv-qualifiers).
  // Group 1: everything up to and including ')' + optional cv-qualifier
  private static final Pattern SIGNATURE_END = Pattern.compile(
      "^(.*?\\)(?:\\s*const)?(?:\\s*noexcept)?)\\s*(\\{|;)",
      Pattern.DOTALL);

  // Pointer declarator without an initializer — insert "= nullptr"
  private static final Pattern PTR_DECL_NO_INIT = Pattern.compile(
      "(\\*+\\s*)(\\w+)(\\s*;)");

  // ── Entry point ───────────────────────────────────────────────────────────

  public static String emit(SketchAnalysis analysis, TsEngine.ParseResult result) {
    return CppStrings.publicClasses(CppStrings.dropMemberPrototypes(CppStrings.fixEventVars(CppStrings.wrapConcat(new SketchEmitter(analysis, result).build()))));
  }

  // ── Instance ──────────────────────────────────────────────────────────────

  private final SketchAnalysis  analysis;
  private final TsEngine.ParseResult result;
  private final StringBuilder   out = new StringBuilder(4096);

  private SketchEmitter(SketchAnalysis analysis, TsEngine.ParseResult result) {
    this.analysis = analysis;
    this.result   = result;
  }

  // ── Build ─────────────────────────────────────────────────────────────────

  private String build() {
    // 1. Processing.h always first
    emit(PROCESSING_INCLUDE);

    // 2. Preprocessor lines (#include, #define, #pragma, ...) at file scope
    for (SketchAnalysis.Item it : analysis.preprocessorLines()) {
      emitRaw(it);
      nl();
    }

    // 3. using-namespace declarations at file scope
    for (SketchAnalysis.Item it : analysis.usingDeclarations()) {
      emitRaw(it);
      nl();
    }

    // Everything else lives in namespace Processing so user classes can see
    // PVector, PImage, ellipse(), etc. without qualification.
    nl();
    emit("namespace Processing {\n");

    // 4. Namespace-scope globals first: consts are often used by classes
    //    (array bounds, loop limits) so they must come before them.
    if (!analysis.isStaticMode()) {
      // classes named by moved globals (Ball* balls[]) must be declared first
      if (!analysis.hoistedClasses().isEmpty()) {
        resetLine();
        for (SketchAnalysis.Item it : analysis.hoistedClasses()) {
          String kw = switch (it.nodeType()) {
            case "class_specifier"  -> "class";
            case "struct_specifier" -> "struct";
            default -> null;
          };
          if (kw != null && !it.name().isEmpty()) emit(kw + " " + it.name() + ";\n");
        }
      }
      for (SketchAnalysis.Item it : globalVars()) {
        if ((belongsAtNamespaceScope(rawText(it)) || usedByClass(it)) && !valueOfSketchClass(it)) emitMovedGlobal(it);
      }
    }

    // 5. Sketch-state proxies so user classes can use width, mouseX, random()
    //    etc. like Processing's inner classes. Inside PSketch the real
    //    PApplet members shadow these.
    if (!analysis.hoistedClasses().isEmpty()) {
      resetLine();
      // a sketch global named like a class helper (width, key...) replaces that helper
      if (!analysis.isStaticMode())
        for (SketchAnalysis.Item g : globalVars())
          if (belongsAtNamespaceScope(rawText(g)) || usedByClass(g))
            for (String n : declNames(rawText(g))) emit("#define CPPMODE_NO_PROXY_" + n + "\n");
      emit(CLASS_API);
    }

    // 6. Enums, then classes (topologically sorted)
    for (SketchAnalysis.Item it : analysis.enumDeclarations()) {
      emitRaw(it);
      ensureSemicolon();
      nl();
    }
    // Sketch functions called from classes: forward-declare the classes so
    // parameter types resolve, then declare the free forwarders.
    if (!analysis.forwarders().isEmpty()) {
      resetLine();
      for (SketchAnalysis.Item it : analysis.hoistedClasses()) {
        String kw = switch (it.nodeType()) {
          case "class_specifier"  -> "class";
          case "struct_specifier" -> "struct";
          case "union_specifier"  -> "union";
          default -> null;   // templates: can't forward-declare simply
        };
        if (kw != null && !it.name().isEmpty()) emit(kw + " " + it.name() + ";\n");
      }
      for (SketchAnalysis.Forwarder f : analysis.forwarders())
        emit(f.returnType() + " " + f.name() + f.declParams() + ";\n");
    }
    for (SketchAnalysis.Item it : sortedClasses()) {
      emitRaw(it);
      ensureSemicolon();
      nl();
      // globals holding this class by value (or defining its static members) come right after it
      if (!analysis.isStaticMode())
        for (SketchAnalysis.Item g : globalVars())
          if ((belongsAtNamespaceScope(rawText(g)) || usedByClass(g)) && valueOfSketchClass(g)
              && it.name().equals(classOfValue(g))) emitMovedGlobal(g);
    }

    if (analysis.isStaticMode()) {
      emitStaticMode();
    } else {
      emitActiveMode();
    }

    for (SketchAnalysis.Forwarder f : analysis.forwarders()) {
      boolean isVoid = f.returnType().equals("void");
      emit(f.returnType() + " " + f.name() + f.defParams() + " { "
          + (isVoid ? "" : "return ")
          + "static_cast<PSketch*>(PApplet::g_papplet)->" + f.name() + f.args() + "; }\n");
    }
    emit("\n} // namespace Processing\n");
    emitMain();
    return out.toString();
  }

  // ── Active mode (has setup() or draw()) ───────────────────────────────────

  private void emitActiveMode() {
    // Globals that can't be non-static data members go to namespace scope:
    // const/constexpr (needed for array bounds), static, and unsized arrays.
    List<SketchAnalysis.Item> members = new java.util.ArrayList<>();
    for (SketchAnalysis.Item it : globalVars()) {
      if (!belongsAtNamespaceScope(rawText(it)) && !usedByClass(it)) members.add(it);
    }

    emit("\nstruct PSketch : public PApplet {\n");
    // moved globals: a reference member keeps Processing's own members from shadowing them
    for (SketchAnalysis.Item g : globalVars())
      if (!belongsAtNamespaceScope(rawText(g)) && usedByClass(g))
        for (String n : declNames(rawText(g)))
          emit("  decltype(::Processing::" + n + ")& " + n + " = ::Processing::" + n + ";\n");

    for (SketchAnalysis.Item it : members) {
      lineMark(it);
      emit("  ");
      emit(defaultPointerMembers(fixParenInitText(fixParenInit(rawText(it)))));
      nl();
    }

    for (SketchAnalysis.Item it : analysis.lifecycleFunctions()) {
      emitFunctionWithIndent(it, true);
    }
    for (SketchAnalysis.Item it : analysis.globalFunctions()) {
      emitFunctionWithIndent(it, false);
    }

    if (!deferredInits.isEmpty() && !deferredInitsUsed) {
      resetLine();
      emit("  void setup() override { " + String.join(" ", deferredInits) + " }\n");
    }
    resetLine();
    emit("}; // struct PSketch\n");
  }

  /**
   * `Label a(50);` is a valid global but not a valid class member (C++ reads
   * it as a function declaration). Rewrite each `name(args)` declarator to
   * `name = Type(args)`, which means the same thing and works as a member.
   */
  /**
   * Text fallback for fixParenInit when tree-sitter reads `T x(a, b);` as a
   * function declaration (the most vexing parse). If every argument is a value,
   * not a `Type name` parameter, rewrite to `T x = T(a, b);`. Empty parens stay.
   */
  private static String fixParenInitText(String decl) {
    java.util.regex.Matcher m = java.util.regex.Pattern.compile(
        "(?s)^(\\s*)((?:const\\s+)?[A-Za-z_][\\w:]*(?:<[^;()]*>)?)\\s+([A-Za-z_]\\w*)\\s*\\(([^()]*(?:\\([^()]*\\)[^()]*)*)\\)\\s*;\\s*$").matcher(decl);
    if (!m.matches() || m.group(4).isBlank()) return decl;
    for (String a : m.group(4).split(",")) {
      String t = a.trim();
      if (t.matches("(?s)(?:const\\s+)?[A-Za-z_][\\w:<>]*(?:\\s*[\\*&]+\\s*|\\s+)[A-Za-z_]\\w*(?:\\s*=.*)?")
          && !t.matches("[A-Za-z_]\\w*")) return decl;   // looks like a parameter
    }
    return m.group(1) + m.group(2) + " " + m.group(3) + " = " + m.group(2) + "(" + m.group(4) + ");";
  }

  private static String fixParenInit(String decl) {
    if (decl.indexOf('(') < 0) return decl;
    TsEngine engine = TsEngine.getOrNull();
    if (engine == null) return decl;
    try (TsEngine.ParseResult r = engine.parse(decl)) {
      TsNode root = r.root();
      if (root == null || root.getNamedChildCount() == 0) return decl;
      TsNode d = root.getNamedChild(0);
      if (!"declaration".equals(d.getType())) return decl;
      TsNode type = d.getChildByFieldName("type");
      if (type == null) return decl;
      String typeText = r.textOf(type);
      StringBuilder out = new StringBuilder();
      int last = 0;
      for (int i = 0; i < d.getNamedChildCount(); i++) {
        TsNode c = d.getNamedChild(i);
        if (!"init_declarator".equals(c.getType())) continue;
        TsNode name = c.getChildByFieldName("declarator");
        TsNode val  = c.getChildByFieldName("value");
        if (name == null || val == null) continue;
        if (!"identifier".equals(name.getType()) || !"argument_list".equals(val.getType())) continue;
        out.append(decl, last, c.getStartByte())
           .append(r.textOf(name)).append(" = ").append(typeText).append(r.textOf(val));
        last = c.getEndByte();
      }
      if (last == 0) return decl;
      return out.append(decl.substring(last)).toString();
    } catch (Throwable t) {
      return decl;
    }
  }

  private static boolean belongsAtNamespaceScope(String decl) {
    String t = decl.stripLeading();
    if (t.startsWith("const ") || t.startsWith("constexpr ") || t.startsWith("static ")
        || t.startsWith("typedef ") || t.startsWith("using ")) return true;
    // int a[] = {...};  (size deduced from initializer: illegal as a member)
    int eq = t.indexOf('=');
    String lhs = eq >= 0 ? t.substring(0, eq) : t;
    if (lhs.matches("(?s).*\\b[A-Za-z_]\\w*::[A-Za-z_]\\w*(?:\\s*\\[.*)?\\s*$")) return true;   // Class::member definition
    return lhs.matches("(?s).*\\[\\s*\\]\\s*$");
  }

  /** Names declared by a global variable declaration (int a = 1, b[3];  ->  a, b). */
  static java.util.List<String> declNames(String decl) {
    java.util.List<String> names = new java.util.ArrayList<>();
    StringBuilder seg = new StringBuilder();
    int depth = 0;
    for (int i = 0; i <= decl.length(); i++) {
      char c = i < decl.length() ? decl.charAt(i) : ';';
      if (c == '(' || c == '[' || c == '{' || c == '<') depth++;
      if (c == ')' || c == ']' || c == '}' || c == '>') depth = Math.max(0, depth - 1);
      if (depth == 0 && (c == ',' || c == ';')) {
        String s = seg.toString();
        int cut = s.length();
        for (int j = 0, d = 0; j < s.length(); j++) {
          char k = s.charAt(j);
          if (d == 0 && (k == '=' || k == '(' || k == '[' || k == '{')) { cut = j; break; }
          if (k == '<') d++; else if (k == '>') d--;
        }
        java.util.regex.Matcher fp = java.util.regex.Pattern.compile("\\(\\s*[\\*&]+\\s*([A-Za-z_]\\w*)").matcher(s);
        java.util.regex.Matcher m = java.util.regex.Pattern.compile("([A-Za-z_]\\w*)\\s*$").matcher(s.substring(0, cut));
        if (fp.find() && fp.start() == cut) names.add(fp.group(1));   // float (*kernels[8])[3]
        else if (m.find()) names.add(m.group(1));
        seg.setLength(0);
        if (c == ';') break;
        continue;
      }
      seg.append(c);
    }
    return names;
  }

  private static String stripCommentsAndStrings(String code) {
    return code.replaceAll("(?s)//[^\\n]*|/\\*.*?\\*/|\"(?:\\\\.|[^\"\\\\])*\"|'(?:\\\\.|[^'\\\\])*'", " ");
  }

  /** A global holding a sketch class by value (Flock flock;) needs the class defined first. */
  private boolean valueOfSketchClass(SketchAnalysis.Item it) {
    String t = rawText(it).strip();
    for (SketchAnalysis.Item c : analysis.hoistedClasses()) {
      if (c.name().isEmpty()) continue;
      java.util.regex.Matcher m = java.util.regex.Pattern.compile(
          "^(?:static\\s+|const\\s+)*" + java.util.regex.Pattern.quote(c.name()) + "\\b\\s*([\\*&]?)").matcher(t);
      if (m.find() && m.group(1).isEmpty()) return true;
      if (java.util.regex.Pattern.compile("\\b" + java.util.regex.Pattern.quote(c.name()) + "::").matcher(t).find()) return true;
    }
    return false;
  }

  // ── Moved globals whose starting value needs the running sketch ───────────
  // A global moved out for a class is built before setup(), when width, mouseX,
  // loadImage() etc. aren't ready. Such a global is declared empty instead and
  // assigned at the start of setup(), the way Java runs field initializers.
  private final java.util.List<String> deferredInits = new java.util.ArrayList<>();
  private boolean deferredInitsUsed = false;

  private static final java.util.regex.Pattern RUNTIME_NAMES = java.util.regex.Pattern.compile(
      "\\b(?:new|width|height|mouseX|mouseY|pmouseX|pmouseY|frameCount|frameRate|key|keyCode|millis|second|"
      + "minute|hour|day|month|year|random|randomGaussian|noise|loadImage|loadFont|createFont|loadStrings|"
      + "loadBytes|loadTable|loadJSONObject|loadJSONArray|loadShape|loadShader|loadXML|createImage|"
      + "createGraphics|createShape|color|lerpColor|displayWidth|displayHeight|pixelWidth|pixelHeight|"
      + "sketchPath|dataPath|textWidth|textAscent|textDescent|get)\\b");

  private static final java.util.regex.Pattern SIMPLE_DECL = java.util.regex.Pattern.compile(
      "(?s)^(\\s*)(?:static\\s+)?([A-Za-z_][\\w:]*(?:<[^;=()]*>)?(?:\\s*\\*+\\s*|\\s+))([A-Za-z_]\\w*)\\s*(?:=\\s*(.+?)|\\((.+)\\))\\s*;\\s*$");

  /** True when a starting value needs the running sketch. */
  private static boolean isRuntimeValue(String value, java.util.Collection<String> sketchFunctions) {
    if (RUNTIME_NAMES.matcher(value).find()) return true;
    for (String f : sketchFunctions)
      if (java.util.regex.Pattern.compile("\\b" + java.util.regex.Pattern.quote(f) + "\\s*\\(").matcher(value).find()) return true;
    return false;
  }

  /** Split on top-level commas (outside (), [], {}, <>). */
  private static java.util.List<String> topLevelCommas(String s) {
    java.util.List<String> out = new java.util.ArrayList<>();
    int depth = 0, start = 0;
    for (int i = 0; i < s.length(); i++) {
      char c = s.charAt(i);
      if (c == '(' || c == '[' || c == '{' || c == '<') depth++;
      else if (c == ')' || c == ']' || c == '}' || c == '>') depth = Math.max(0, depth - 1);
      else if (c == ',' && depth == 0) { out.add(s.substring(start, i)); start = i + 1; }
    }
    out.add(s.substring(start));
    return out;
  }

  private static final java.util.regex.Pattern FIRST_DECLARATOR = java.util.regex.Pattern.compile(
      "(?s)^(\\s*(?:static\\s+)?[A-Za-z_][\\w:]*(?:<[^;=()]*>)?)(\\s*\\*+\\s*|\\s+)([A-Za-z_]\\w*)\\s*(?:=\\s*(.+))?$");
  private static final java.util.regex.Pattern NEXT_DECLARATOR = java.util.regex.Pattern.compile(
      "(?s)^\\s*(\\**)\\s*([A-Za-z_]\\w*)\\s*(?:=\\s*(.+))?$");

  private static final java.util.regex.Pattern SIZED_ARRAY = java.util.regex.Pattern.compile(
      "(?s)^(\\s*)(?:static\\s+)?([A-Za-z_][\\w:]*(?:<[^;=()]*>)?\\s*\\**)\\s*([A-Za-z_]\\w*)\\s*(?=\\[)((?:\\[[^\\]]+\\])+)\\s*=\\s*(\\{.*\\})\\s*;\\s*$");

  /**
   * Split runtime starting values out of a global declaration.
   * Single declarator: {"T x{};", "x = rhs;", type, name, rhs}.
   * Several declarators: {"T a{}, b = 3;", "a = width;"}. Null when nothing needs moving.
   */
  String[] splitRuntimeInit(String decl, java.util.Collection<String> sketchFunctions) {
    java.util.List<String> names = declNames(decl);
    java.util.regex.Matcher arr = SIZED_ARRAY.matcher(decl);
    if (names.size() == 1 && arr.matches()) {
      if (!isRuntimeValue(arr.group(5), sketchFunctions)) return null;
      String n = arr.group(3);
      return new String[] { arr.group(1) + arr.group(2) + " " + n + arr.group(4) + "{};",
          "{ ::std::remove_reference_t<decltype(" + n + ")> cppmode_tmp_ = " + arr.group(5).trim() + "; ::std::memcpy(" + n + ", cppmode_tmp_, sizeof(" + n + ")); }" };
    }
    if (names.size() == 1) {
      java.util.regex.Matcher m = SIMPLE_DECL.matcher(decl);
      if (!m.matches()) return null;
      String type = m.group(2).trim(), name = m.group(3);
      String init = m.group(4) != null ? m.group(4).trim() : null;
      String args = m.group(5);
      if (!isRuntimeValue(init != null ? init : args, sketchFunctions)) return null;
      String rhs = init != null ? init : type + "(" + args + ")";
      return new String[] { m.group(1) + type + " " + name + "{};", name + " = " + rhs + ";", type, name, rhs };
    }
    String body = decl.strip();
    if (!body.endsWith(";")) return null;
    java.util.List<String> segs = topLevelCommas(body.substring(0, body.length() - 1));
    java.util.regex.Matcher f = FIRST_DECLARATOR.matcher(segs.get(0));
    if (!f.matches()) return null;
    String type = f.group(1).trim();
    StringBuilder declOut = new StringBuilder(type).append(' ');
    StringBuilder assigns = new StringBuilder();
    boolean any = false;
    for (int i = 0; i < segs.size(); i++) {
      String stars, name, init;
      if (i == 0) { stars = f.group(2).trim(); name = f.group(3); init = f.group(4); }
      else {
        java.util.regex.Matcher n = NEXT_DECLARATOR.matcher(segs.get(i));
        if (!n.matches()) return null;
        stars = n.group(1); name = n.group(2); init = n.group(3);
      }
      if (i > 0) declOut.append(", ");
      if (init != null && isRuntimeValue(init, sketchFunctions)) {
        declOut.append(stars).append(name).append("{}");
        assigns.append(name).append(" = ").append(init.trim()).append("; ");
        any = true;
      } else {
        declOut.append(stars).append(name);
        if (init != null) declOut.append(" = ").append(init.trim());
      }
    }
    if (!any) return null;
    return new String[] { declOut.append(';').toString(), assigns.toString().trim() };
  }

  /** Top-level statements that are really declarations, e.g. float (*kernels[8])[3] = {...};
   *  which tree-sitter reads as an expression (a function-style cast). */
  private static final java.util.regex.Pattern DECL_LIKE_STATEMENT = java.util.regex.Pattern.compile(
      "(?s)^\\s*(?:const\\s+|static\\s+)*[A-Za-z_][\\w:]*(?:<[^;]*>)?\\s*\\(\\s*[\\*&]+\\s*[A-Za-z_]\\w*.*;\\s*$");

  private boolean isDeclLikeStatement(SketchAnalysis.Item it) {
    return DECL_LIKE_STATEMENT.matcher(rawText(it)).matches();
  }

  /** Sketch globals, plus declaration-like statements tree-sitter didn't recognise. */
  private java.util.List<SketchAnalysis.Item> globalVars() {
    java.util.List<SketchAnalysis.Item> g = new java.util.ArrayList<>(analysis.globalVariables ());
    if (!analysis.isStaticMode())
      for (SketchAnalysis.Item s : analysis.topLevelStatements ()) if (isDeclLikeStatement(s)) g.add(s);
    return g;
  }

  private java.util.List<SketchAnalysis.Item> nonDeclStatements() {
    if (analysis.isStaticMode()) return analysis.topLevelStatements ();
    java.util.List<SketchAnalysis.Item> r = new java.util.ArrayList<>();
    for (SketchAnalysis.Item s : analysis.topLevelStatements ()) if (!isDeclLikeStatement(s)) r.add(s);
    return r;
  }

  /** Write a namespace-scope global; one moved for a class with a runtime starting value is split. */
  private void emitMovedGlobal(SketchAnalysis.Item it) {
    String text = rawText(it);
    if (!belongsAtNamespaceScope(text) && usedByClass(it)) {
      java.util.List<String> fns = new java.util.ArrayList<>();
      for (SketchAnalysis.Item f : analysis.globalFunctions()) fns.add(f.name());
      String[] parts = splitRuntimeInit(text, fns);
      if (parts != null && parts.length == 5 && valueOfSketchClass(it)) {
        // class with maybe no default constructor: raw storage now, built in setup()
        String type = parts[2], name = parts[3];
        lineMark(it);
        emit("alignas(" + type + ") unsigned char cppmode_store_" + name + "[sizeof(" + type + ")]; "
            + type + "& " + name + " = *reinterpret_cast<" + type + "*>(cppmode_store_" + name + ");"); nl();
        deferredInits.add("new (&" + name + ") " + type + "(" + parts[4] + ");");
        return;
      }
      if (parts != null) {
        lineMark(it); emit(parts[0]); nl();
        deferredInits.add(parts[1]);
        return;
      }
    }
    emitRaw(it); nl();
  }


  /** Name of the sketch class a global holds by value (Grid grid;) or defines a member of (int Counter::n). */
  private String classOfValue(SketchAnalysis.Item g) {
    String t = rawText(g).strip();
    for (SketchAnalysis.Item c : analysis.hoistedClasses()) {
      if (c.name().isEmpty()) continue;
      String q = java.util.regex.Pattern.quote(c.name());
      if (java.util.regex.Pattern.compile("^(?:static\\s+|const\\s+)*" + q + "\\b").matcher(t).find()) return c.name();
      if (java.util.regex.Pattern.compile("\\b" + q + "::").matcher(t).find()) return c.name();
    }
    return "";
  }

  /** a mentions b, directly or through a global that holds a b by value. */
  private boolean classDependsOn(SketchAnalysis.Item a, SketchAnalysis.Item b) {
    if (a == b || b.name().isEmpty()) return false;
    String t = stripCommentsAndStrings(rawText(a));
    if (java.util.regex.Pattern.compile("\\b" + java.util.regex.Pattern.quote(b.name()) + "\\b").matcher(t).find()) return true;
    for (SketchAnalysis.Item g : globalVars()) {
      if (!valueOfSketchClass(g) || !b.name().equals(classOfValue(g))) continue;
      for (String n : declNames(rawText(g)))
        if (java.util.regex.Pattern.compile("(?<![\\w.>:])" + java.util.regex.Pattern.quote(n) + "\\b").matcher(t).find()) return true;
    }
    return false;
  }

  /** Classes in an order where each comes after the classes it uses; cycles keep the original order. */
  private java.util.List<SketchAnalysis.Item> sortedClasses() {
    java.util.List<SketchAnalysis.Item> src = new java.util.ArrayList<>(analysis.hoistedClasses());
    java.util.List<SketchAnalysis.Item> out = new java.util.ArrayList<>();
    java.util.Set<SketchAnalysis.Item> done = new java.util.HashSet<>();
    while (out.size() < src.size()) {
      boolean progress = false;
      for (SketchAnalysis.Item c : src) {
        if (done.contains(c)) continue;
        boolean ready = true;
        for (SketchAnalysis.Item b : src) if (!done.contains(b) && b != c && classDependsOn(c, b)) { ready = false; break; }
        if (ready) { out.add(c); done.add(c); progress = true; }
      }
      if (!progress) for (SketchAnalysis.Item c : src) if (!done.contains(c)) { out.add(c); done.add(c); break; }
    }
    return out;
  }

  /** True when a sketch class mentions this global by name (classes can't see PSketch members). */
  private java.util.Set<SketchAnalysis.Item> movedForClasses;

  /** Moved for a class, or mentioned by the starting value of another moved global. */
  private boolean usedByClass(SketchAnalysis.Item it) {
    if (movedForClasses == null) {
      movedForClasses = new java.util.HashSet<>();
      for (SketchAnalysis.Item g : globalVars())
        if (directlyUsedByClass(g)) movedForClasses.add(g);
      boolean grew = true;
      while (grew) {
        grew = false;
        java.util.List<SketchAnalysis.Item> add = new java.util.ArrayList<>();
        for (SketchAnalysis.Item g : globalVars()) {
          if (movedForClasses.contains(g)) continue;
          outer:
          for (String n : declNames(rawText(g))) {
            java.util.regex.Pattern p = java.util.regex.Pattern.compile(
                "(?<![\\w.>])" + java.util.regex.Pattern.quote(n) + "\\b");
            for (SketchAnalysis.Item m : movedForClasses)
              if (p.matcher(stripCommentsAndStrings(rawText(m))).find()) { add.add(g); break outer; }
          }
        }
        if (!add.isEmpty()) { movedForClasses.addAll(add); grew = true; }
      }
    }
    return movedForClasses.contains(it);
  }

  private boolean directlyUsedByClass(SketchAnalysis.Item it) {
    if (analysis.isStaticMode() || analysis.hoistedClasses().isEmpty()) return false;
    for (String n : declNames(rawText(it))) {
      java.util.regex.Pattern p = java.util.regex.Pattern.compile("(?<![\\w.>])" + java.util.regex.Pattern.quote(n) + "\\b");
      for (SketchAnalysis.Item c : analysis.hoistedClasses())
        if (p.matcher(stripCommentsAndStrings(rawText(c))).find()) return true;
    }
    return false;
  }

  // ── Static mode (no setup()/draw()) ───────────────────────────────────────

  /**
   * Static sketches are a flat list of statements. Declarations and bare
   * statements run in order inside setup(); size()/fullScreen() move to
   * settings() so the window is created at the right size; the sketch
   * draws once.
   */
  private void emitStaticMode() {
    List<SketchAnalysis.Item> body = new java.util.ArrayList<>();
    body.addAll(globalVars());
    body.addAll(nonDeclStatements());
    body.sort(java.util.Comparator.comparingInt(SketchAnalysis.Item::startByte));

    List<SketchAnalysis.Item> sizeCalls = new java.util.ArrayList<>();
    List<SketchAnalysis.Item> rest = new java.util.ArrayList<>();
    for (SketchAnalysis.Item it : body) {
      String t = rawText(it).stripLeading();
      if (t.startsWith("size(") || t.startsWith("size (")
          || t.startsWith("fullScreen(")) sizeCalls.add(it);
      else rest.add(it);
    }

    emit("\nstruct PSketch : public PApplet {\n");
    // moved globals: a reference member keeps Processing's own members from shadowing them
    for (SketchAnalysis.Item g : globalVars())
      if (!belongsAtNamespaceScope(rawText(g)) && usedByClass(g))
        for (String n : declNames(rawText(g)))
          emit("  decltype(::Processing::" + n + ")& " + n + " = ::Processing::" + n + ";\n");
    if (!sizeCalls.isEmpty()) {
      emit("  void settings() override {\n");
      for (SketchAnalysis.Item it : sizeCalls) { lineMark(it); emit("    " + rawText(it)); nl(); }
      emit("  }\n");
    }
    emit("  void setup() override {\n");
    for (SketchAnalysis.Item it : rest) {
      lineMark(it);
      emit(indentBlock(rawText(it), "    "));
      nl();
    }
    emit("    noLoop();\n");
    emit("  }\n");
    for (SketchAnalysis.Item it : analysis.globalFunctions()) {
      emitFunctionWithIndent(it, false);
    }
    if (!deferredInits.isEmpty() && !deferredInitsUsed) {
      resetLine();
      emit("  void setup() override { " + String.join(" ", deferredInits) + " }\n");
    }
    resetLine();
    emit("}; // struct PSketch\n");
  }

  // ── Entry point ───────────────────────────────────────────────────────────

  private void emitMain() {
    emit("\n#ifdef _WIN32\n");
    emit("#include <windows.h>\n");
    emit("int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {\n");
    emit("  Processing::PSketch sketch;\n");
    emit("  sketch.run();\n");
    emit("  return 0;\n");
    emit("}\n");
    emit("#else\n");
    emit("int main() {\n");
    emit("  Processing::PSketch sketch;\n");
    emit("  sketch.run();\n");
    emit("  return 0;\n");
    emit("}\n");
    emit("#endif\n");
  }

  // ── Function emit ─────────────────────────────────────────────────────────

  // CPPMODE_SIZE_ANYWHERE: size()/fullScreen() as a statement anywhere in setup(),
  // also when setup() is on one line. Not obj.size(), obj->size() or std::size().
  private static final Pattern SIZE_CALL = Pattern.compile(
      "(?<![\\w.>:])(?:size|fullScreen)\\s*\\((?:[^;()]|\\([^()]*\\))*\\)\\s*;");
  private String hoistedSize = null;

  private void emitFunctionWithIndent(SketchAnalysis.Item it, boolean addOverride) {
    String text = rawText(it);
    // Java Processing runs size() from settings(), before the window exists.
    // Move it there unless the sketch has its own settings(). The statement is
    // cut but its line break stays, so line numbers don't shift.
    boolean isSetup = addOverride && "setup".equals(it.name());
    if (isSetup && analysis.lifecycleFunctions().stream().noneMatch(f -> "settings".equals(f.name()))) {
      java.util.regex.Matcher m = SIZE_CALL.matcher(text);
      if (m.find()) {
        hoistedSize = m.group().trim();
        text = text.substring(0, m.start()) + text.substring(m.end());
      }
    }
    if (isSetup && !deferredInits.isEmpty()) {
      int b = text.indexOf('{');
      if (b >= 0) { text = text.substring(0, b + 1) + " " + String.join(" ", deferredInits) + text.substring(b + 1); deferredInitsUsed = true; }
    }
    lineMark(it);
    text = indentBlock(text, "  ");
    if (addOverride) text = spliceOverride(text);
    emit(text);
    nl();
    if (isSetup && hoistedSize != null) {
      emit("  void settings() override { " + hoistedSize + " }\n");
    }
  }

  /**
   * Splice {@code override} into the function signature after the parameter
   * list (and any cv-qualifiers) and before the opening brace.
   * Example: {@code void draw() {} } → {@code void draw() override {} }
   */
  private static String spliceOverride(String text) {
    var m = SIGNATURE_END.matcher(text);
    if (m.find() && !text.contains("override")) {
      return m.group(1) + " override " + m.group(2) + text.substring(m.end());
    }
    return text;
  }

  // ── Forward-declaration emit ──────────────────────────────────────────────

  private void emitForwardDecl(SketchAnalysis.ForwardDecl fd) {
    emit("  ");
    emit(fd.returnType());
    emit(" ");
    emit(fd.name());
    emit(fd.params());
    if (fd.isConst()) emit(" const");
    emit(";\n");
  }

  // ── Text helpers ──────────────────────────────────────────────────────────

  private String rawText(SketchAnalysis.Item it) {
    return result.source().substring(it.startByte(), it.endByte());
  }

  private void emitRaw(SketchAnalysis.Item it) {
    lineMark(it);
    emit(rawText(it));
  }

  /**
   * Emit a #line directive so g++ reports errors against the original sketch
   * line instead of the generated file. Must start at column 0.
   */
  private void lineMark(SketchAnalysis.Item it) {
    nl();
    String src = result.source();
    int line = 1;
    int end = Math.min(it.startByte(), src.length());
    for (int i = 0; i < end; i++) if (src.charAt(i) == '\n') line++;
    emit("#line " + line + " \"sketch.pde\"\n");
  }

  /** Point diagnostics back at the generated file after copied sketch code. */
  private void resetLine() {
    nl();
    int line = 2; // the #line directive's own line + 1
    for (int i = 0; i < out.length(); i++) if (out.charAt(i) == '\n') line++;
    emit("#line " + line + " \"Sketch_run.cpp\"\n");
  }

  private void emit(String s) {
    out.append(s);
  }

  private void nl() {
    if (out.isEmpty() || out.charAt(out.length() - 1) != '\n') out.append('\n');
  }

  /** Ensure the last non-whitespace character is ';' (for class/enum bodies). */
  private void ensureSemicolon() {
    String s = out.toString().stripTrailing();
    if (!s.endsWith(";")) out.append(";");
  }

  /**
   * Indent every line of {@code text} with {@code prefix}, skipping blank
   * lines and preserving existing indentation.
   */
  private static String indentBlock(String text, String prefix) {
    String[] lines = text.split("\n", -1);
    StringBuilder sb = new StringBuilder(text.length() + lines.length * prefix.length());
    for (int i = 0; i < lines.length; i++) {
      String line = lines[i];
      if (!line.isBlank()) sb.append(prefix);
      sb.append(line);
      if (i < lines.length - 1) sb.append('\n');
    }
    return sb.toString();
  }

  /**
   * For declarations inside the struct body, default uninitialized pointer
   * members to {@code = nullptr}.  Matches patterns like {@code Foo* bar;}.
   */
  private static String defaultPointerMembers(String text) {
    if (text.contains("=") || text.contains("{")) return text;   // has a value already (or a body)
    return PTR_DECL_NO_INIT.matcher(text)
        .replaceAll(m -> m.group(1) + m.group(2) + " = nullptr" + m.group(3));
  }

  // ── Public utility: line-count of generated header ────────────────────────

  /**
   * Count the number of lines that will appear before the first sketch source
   * line in the emitted output.  Used by the error-line mapper in
   * {@link CppBuild} to translate g++ line numbers back to sketch lines.
   */
  public static int headerLineCount(SketchAnalysis analysis) {
    int lines = 1; // Processing.h include
    lines += analysis.preprocessorLines().size();
    lines += analysis.usingDeclarations().size();
    if (!analysis.hoistedClasses().isEmpty())
      lines += analysis.hoistedClasses().size() + 1; // +1 for blank line
    if (!analysis.enumDeclarations().isEmpty())
      lines += analysis.enumDeclarations().size() + 1;
    lines += 3; // namespace Processing {\n\n + forwardDecls blank
    lines += analysis.forwardDecls().size();
    lines += 2; // struct PSketch... \n\n
    return lines;
  }
}
