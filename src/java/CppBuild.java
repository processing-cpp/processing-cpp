package processing.mode.cpp;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;

import processing.app.RunnerListener;
import processing.app.Sketch;

/**
 * CppBuild — compiles a Processing sketch to a native binary.
 *
 * <h2>New pipeline (tree-sitter)</h2>
 * <pre>
 *   1. Concatenate all tabs into one UTF-8 string
 *   2. Sanitise (strip BOM, normalise line endings, strip raw-string digit-seps)
 *   3. TsEngine.parse()        → concrete syntax tree
 *   4. SketchAnalyzer.analyse() → SketchAnalysis (what goes where)
 *   5. SketchEmitter.emit()    → final C++ source string
 *   6. Write to buildDir/Sketch_run.cpp
 *   7. g++ compile + link
 * </pre>
 *
 * <p>The old regex-based {@code prepareCode()} / {@code Parser} / AST passes
 * are replaced by steps 3-5.  The g++ invocation (step 7) and all platform
 * detection / install-wizard logic is retained.
 */
public class CppBuild {
  // cppmode-issue6: on Windows, put the compiler's own bin folder first on PATH. Its helpers
  // (cc1plus, as, ld) sit in folders without DLLs, so without this Windows loads the
  // DLLs of whatever other mingw comes first on PATH, and the build dies with no output.
  public static ProcessBuilder toolchainEnv(ProcessBuilder pb) {
    try {
      if (!System.getProperty("os.name").toLowerCase().contains("win")) return pb;
      java.util.List<String> c = pb.command();
      if (c.isEmpty()) return pb;
      File exe = new File(c.get(0));
      String n = exe.getName().toLowerCase();
      if (!exe.isAbsolute() || exe.getParentFile() == null) return pb;
      if (!(n.contains("g++") || n.contains("gcc") || n.equals("c++.exe") || n.equals("cpp.exe"))) return pb;
      java.util.Map<String, String> env = pb.environment();   // case-insensitive on Windows
      String bin = exe.getParentFile().getAbsolutePath();
      String old = env.getOrDefault("PATH", "");
      if (!old.toLowerCase().startsWith(bin.toLowerCase() + ";"))
        env.put("PATH", bin + (old.isEmpty() ? "" : ";" + old));
    } catch (Exception ignored) {}
    return pb;
  }


  // ── Fields ────────────────────────────────────────────────────────────────

  private final Sketch sketch;
  private final CppMode mode;
  final File buildDir;
  private final File runtimeDir;

  /** Number of generated header lines before user code (for error mapping). */
  public int headerLineCount = 0;

  /** 1-based line in the combined source where each tab starts. */
  private int[] tabStartLines = {1};

  // ── Constructor ───────────────────────────────────────────────────────────

  public CppBuild(Sketch sketch, CppMode mode) {
    this.sketch     = sketch;
    this.mode       = mode;
    this.buildDir   = sketch.makeTempFolder();
    this.runtimeDir = mode != null ? mode.getRuntimeDir() : null;
  }

  // ── Public pipeline entry points ──────────────────────────────────────────

  /**
   * Full pipeline: parse → analyse → emit → write → g++.
   * Returns the compiled binary, or {@code null} on error (after notifying
   * {@code listener}).
   */
  public File compile(RunnerListener listener) throws Exception {
    checkGppAvailable(listener);

    File sketchSrc = writeSketch(listener);
    if (sketchSrc == null) return null;

    boolean isWindows = System.getProperty("os.name").toLowerCase().contains("win");
    boolean isMac     = System.getProperty("os.name").toLowerCase().contains("mac");
    String  ext       = isWindows ? ".exe" : "";
    File    binary    = new File(buildDir, sketch.getName() + ext);
    String  gpp       = findGpp(isWindows);

    String stdOverride = (listener instanceof CppEditor ce) ? ce.getSelectedCppStd() : null;
    List<String> cmd = buildCommand(gpp, sketchSrc, binary, isWindows, isMac, stdOverride);
    listener.statusNotice("$ " + String.join(" ", cmd));

    ProcessBuilder pb = toolchainEnv(new ProcessBuilder(cmd));
    pb.redirectErrorStream(true);
    pb.directory(buildDir);
    pb.environment().put("PROCESSING_SKETCH_PATH", sketch.getFolder().getAbsolutePath());
    Process proc = pb.start();

    StringBuilder output = new StringBuilder();
    int firstErrLine = -1;

    try (BufferedReader br = new BufferedReader(
             new InputStreamReader(proc.getInputStream()))) {
      String line;
      while ((line = br.readLine()) != null) {
        output.append(line).append("\n");
        if (firstErrLine < 0) {
          Matcher m = ERR_LINE.matcher(line);
          if (m.find()) {
            try { firstErrLine = Integer.parseInt(m.group(1)); }
            catch (NumberFormatException ignored) {}
          }
        }
        if (shownHit == null && line.contains("error:")) {
          CppErrors.Hit h0 = CppErrors.match(line);
          if (h0 != null) { shownHit = h0; printDiagnostic(h0.text()); printDiagnostic("More: " + h0.url()); }
        }
        printDiagnostic(line);
      }
    }

    int exit = proc.waitFor();
    if (exit != 0) {
      if (firstErrLine > 0 && listener instanceof CppEditor ce) {
        // firstErrLine is a line in all tabs joined together: find its tab
        int tab = 0;
        for (int i = tabStartLines.length - 1; i >= 0; i--)
          if (firstErrLine >= tabStartLines[i]) { tab = i; break; }
        final int ft = tab;
        final int el = Math.max(1, firstErrLine - tabStartLines[tab] + 1);
        javax.swing.SwingUtilities.invokeLater(() -> {
          if (sketch.getCurrentCodeIndex() != ft) sketch.setCurrentCode(ft);
          ce.highlightErrorLine(el);
        });
      }
      CppErrors.Hit hit = shownHit != null ? shownHit : CppErrors.find(output.toString());
      boolean alreadyShown = shownHit != null;
      shownHit = null;
      if (hit != null) {
        if (!alreadyShown) { printDiagnostic(hit.text()); printDiagnostic("More: " + hit.url()); }
        listener.statusError(hit.text());
      } else {
        listener.statusError("Build failed, see console for errors.");
      }
      throw new Exception(output.toString());
    }

    copyRuntimeAssets(binary, isWindows, isMac);
    listener.statusNotice("Built: " + binary.getName());
    return binary;
  }

  /**
   * Run only the tree-sitter pipeline and return the generated C++ source.
   * Used by {@link CppLinter} to produce the source it passes to
   * {@code g++ -fsyntax-only}.
   */
  public static String generateSketchOutput(String sketchSource) throws Exception {
    String sanitised = sanitise(sketchSource);
    TsEngine engine = TsEngine.getOrNull();
    if (engine == null) throw new IllegalStateException("tree-sitter native library not available");
    try (TsEngine.ParseResult result = engine.parse(sanitised)) {
      SketchAnalysis analysis = SketchAnalyzer.analyse(result);
      return SketchEmitter.emit(analysis, result);
    }
  }

  // ── Sketch writing ────────────────────────────────────────────────────────

  File writeSketch(RunnerListener listener) throws Exception {
    // 1. Collect all tabs
    int tabCount = sketch.getCodeCount();
    StringBuilder raw = new StringBuilder();
    tabStartLines = new int[tabCount];
    int line = 1;
    for (int i = 0; i < tabCount; i++) {
      tabStartLines[i] = line;
      String prog = sketch.getCode(i).getProgram();
      if (prog == null) prog = "";
      raw.append(prog).append("\n");
      for (int k = 0; k < prog.length(); k++) if (prog.charAt(k) == '\n') line++;
      line++;
    }

    // 2. Sanitise
    String sanitised = sanitise(raw.toString());

    // 3-5. Parse → analyse → emit
    TsEngine engine = TsEngine.getOrNull();
    if (engine == null) throw new Exception("tree-sitter native library not available — rebuild the .so in native/");
    String cpp;
    try (TsEngine.ParseResult result = engine.parse(sanitised)) {
      if (result.hasErrors()) {
        // Tree-sitter error-recovery means we still get a partial tree;
        // proceed but mark the line for the editor.
        // (g++ will produce a clearer error message anyway.)
        listener.statusNotice("Parse warnings — proceeding with partial tree.");
      }
      SketchAnalysis analysis = SketchAnalyzer.analyse(result);
      headerLineCount = 0; // #line directives map errors to sketch lines
      cpp = SketchEmitter.emit(analysis, result);
    }

    // 6. Write
    buildDir.mkdirs();
    File out = new File(buildDir, "Sketch_run.cpp");
    Files.writeString(out.toPath(), cpp);
    return out;
  }

  // ── Sanitisation (kept as minimal string-level cleanup) ───────────────────

  /**
   * Minimal pre-parse sanitisation.  Unlike the old regex pipeline, this
   * does NOT transform the sketch semantics — it only normalises encoding
   * artefacts that tree-sitter would otherwise reject or misparse.
   */
  public static String sanitise(String src) {
    // Strip BOM
    if (src.startsWith("\uFEFF")) src = src.substring(1);

    // Normalise Windows line endings
    src = src.replace("\r\n", "\n").replace("\r", "\n");

    // C++14 digit separator hack: remove ' between hex/binary digits
    // (tree-sitter-cpp supports them, but some g++ versions don't)
    src = src.replaceAll("(?<=[0-9a-fA-FxXbB])'(?=[0-9a-fA-F])", "");

    // cppmode-hexcolor: #RRGGBB colors and thread("name"), before tree-sitter sees the code
    src = CppJavaSyntax.rewrite(src);  // cppmode-lexer: #RRGGBB, Math.abs, thread("name") via CppLexer

    return src;
  }

  // ── Error pattern ─────────────────────────────────────────────────────────

  private static final Pattern ERR_LINE = Pattern.compile(
      "sketch\\.pde:(\\d+):\\d+:\\s+error:");

  // ── g++ error rewriting ───────────────────────────────────────────────────

  /**
   * Rewrites common g++ error messages to use Processing-friendly language.
   * Strips "Processing::" namespace prefix from user-visible messages.
   */
  public static String rewriteGccError(String msg) {
    msg = msg.replace("Processing::", "");
    msg = msg.replace("struct PSketch", "sketch");
    return msg;
  }

  // ── Compiler command construction ─────────────────────────────────────────

  private List<String> buildCommand(String gpp, File src, File binary,
                                     boolean isWindows, boolean isMac,
                                     String stdOverride) throws Exception {
    String std = stdOverride != null ? stdOverride : bestCppStd(gpp);
    List<String> cmd = new ArrayList<>();
    cmd.add(gpp);
    cmd.add("-std=" + std);
    cmd.add("-O2");

    // Include paths. libprocessing's header folder goes first: macOS and
    // Windows filesystems don't tell processing.h from Processing.h, so with
    // the runtime folder first <processing.h> would find Processing.h.
    if (useLibProcessing())
      for (File inc : libProcessingIncludes()) { cmd.add("-I"); cmd.add(inc.getAbsolutePath()); }
    cmd.add("-I"); cmd.add(runtimeDir.getAbsolutePath());
    File bundledInc = new File(runtimeDir.getParentFile(), "libs/include");
    if (bundledInc.exists()) { cmd.add("-I"); cmd.add(bundledInc.getAbsolutePath()); }

    // libprocessing (wgpu) backend
    boolean lp = useLibProcessing();
    if (lp) {
      cmd.add("-DCPPMODE_BACKEND_LIBPROCESSING"); cmd.add("-DPROCESSING_HAS_STB_IMAGE"); cmd.add("-DPROCESSING_HAS_STB_TRUETYPE");
    }

    // Platform link flags
    if (isWindows) {
      addWindowsLinkFlags(cmd);
    } else if (isMac) {
      addMacLinkFlags(cmd);
    } else {
      addLinuxLinkFlags(cmd);
    }

    // Cached Processing.o
    String os = System.getProperty("os.name", "").toLowerCase();
    String cacheSubDir = os.contains("win")  ? "cache/windows-x64"
                       : os.contains("mac")  ? "cache/macos-" +
                         (System.getProperty("os.arch","").contains("aarch64")
                             ? "arm64" : "x64")
                       : "cache/linux-x64";
    if (lp) cacheSubDir += "-libprocessing";
    File cacheDir    = new File(runtimeDir.getParentFile(), cacheSubDir);
    File processingO = new File(cacheDir, "Processing.o");
    File defaultsO   = new File(cacheDir, "Processing_defaults.o");
    if (lp && !processingO.exists()) buildLibProcessingCache(gpp, std, cacheDir);

    cmd.add(src.getAbsolutePath());
    if (processingO.exists()) cmd.add(processingO.getAbsolutePath());
    if (defaultsO.exists())   cmd.add(defaultsO.getAbsolutePath());

    cmd.add("-o"); cmd.add(binary.getAbsolutePath());

    // FreeType: only when the cached Processing.o was built with it
    File ftMarker = new File(cacheDir, "freetype.flags");
    if (ftMarker.exists()) {
      for (String f : Files.readString(ftMarker.toPath()).trim().split("\\s+"))
        if (!f.isEmpty()) cmd.add(f);
    }

    if (lp) {
      // cppmode-ship: per-platform linking. Linux and macOS find the library through rpath
      // (its folder, plus next to the binary); Windows has no rpath, so the DLL is copied next to the .exe.
      File libDir = libProcessingLibDir();
      String lpOs = System.getProperty("os.name").toLowerCase();
      cmd.add("-L" + libDir.getAbsolutePath());
      if (lpOs.contains("win")) {
        File dll = new File(libDir, lpLibName());
        java.nio.file.Files.copy(dll.toPath(), new File(binary.getAbsoluteFile().getParentFile(), dll.getName()).toPath(),
                                 java.nio.file.StandardCopyOption.REPLACE_EXISTING);
      } else {
        cmd.add("-Wl,-rpath," + libDir.getAbsolutePath());
        cmd.add(lpOs.contains("mac") ? "-Wl,-rpath,@executable_path" : "-Wl,-rpath,$ORIGIN");
      }
      cmd.add("-lprocessing");
    }

    // Debug flag (presence of DEBUG file in sketch folder)
    if (sketch != null && new File(sketch.getFolder(), "DEBUG").exists()) {
      cmd.add("-DPROCESSING_DEBUG");
    }

    return cmd;
  }

  // ── libprocessing backend ────────────────────────────────────────────────

  /** On when CPPMODE_RENDERER=libprocessing, the editor dropdown says so, or the sketch has a LIBPROCESSING file. */
  boolean useLibProcessing() {
    boolean want;
    if ("libprocessing".equalsIgnoreCase(System.getenv("CPPMODE_RENDERER"))) want = true;
    else if (selectedRenderer != null) want = "libprocessing".equalsIgnoreCase(selectedRenderer);
    else want = sketch != null && new File(sketch.getFolder(), "LIBPROCESSING").exists();
    // cppmode-ship: without the library, build with OpenGL instead of failing at link time
    if (want && libProcessingLibDir() == null) {
      if (!lpMissingReported) {
        lpMissingReported = true;
        System.err.println("[CppMode] The libprocessing renderer is selected, but its library (" + lpLibName()
            + ") wasn't found in libs/" + lpPlatform() + "/libprocessing or ~/lp-fork. Using OpenGL.");
      }
      return false;
    }
    return want;
  }

  /** Set by the editor's renderer dropdown; null means not chosen. */
  public static volatile String selectedRenderer = null;

  // cppmode-ship: where the libprocessing renderer comes from. Order: LIBPROCESSING_HOME,
  // the copy bundled in the mode (libs/<platform>/libprocessing), then ~/lp-fork for development.
  static String lpPlatform() {
    String sys = System.getProperty("os.name").toLowerCase();
    String arch = System.getProperty("os.arch").toLowerCase();
    boolean arm = arch.contains("aarch64") || arch.contains("arm64");
    if (sys.contains("win")) return "windows-x64";
    if (sys.contains("mac")) return arm ? "macos-arm64" : "macos-x64";
    return arm ? "linux-aarch64" : "linux-x64";
  }

  static String lpLibName() {
    String sys = System.getProperty("os.name").toLowerCase();
    if (sys.contains("win")) return "processing.dll";
    if (sys.contains("mac")) return "libprocessing.dylib";
    return "libprocessing.so";
  }

  /** The installed mode's folder, found from where CppMode.jar lives (mode/CppMode.jar). */
  static File modeRoot() {
    try {
      File jar = new File(CppBuild.class.getProtectionDomain().getCodeSource().getLocation().toURI());
      return jar.getParentFile().getParentFile();
    } catch (Exception e) {
      return null;
    }
  }

  static File bundledLibProcessing() {
    File root = modeRoot();
    if (root == null) return null;
    File d = new File(root, "libs/" + lpPlatform() + "/libprocessing");
    return new File(d, lpLibName()).isFile() ? d : null;
  }

  static File libProcessingHome() {
    String env = System.getenv("LIBPROCESSING_HOME");
    if (env != null && !env.isBlank()) return new File(env);
    File bundled = bundledLibProcessing();
    if (bundled != null) return bundled;
    return new File(System.getProperty("user.home"), "lp-fork");
  }

  /** The folder holding the library itself, or null when it can't be found. */
  static File libProcessingLibDir() {
    File h = libProcessingHome();
    for (File d : new File[]{h, new File(h, "target/release")})
      if (new File(d, lpLibName()).isFile()) return d;
    return null;
  }

  private static volatile boolean lpMissingReported = false;

  static List<File> libProcessingIncludes() {
    File h = libProcessingHome();
    List<File> out = new ArrayList<>();
    for (String sub : new String[]{"crates/processing_ffi/include", "crates/processing_ffi",
                                   "include", "target/include", "target/release/include"}) {
      File d = new File(h, sub);
      if (d.isDirectory()) out.add(d);
    }
    return out;
  }

  /** One-time build of Processing.o / Processing_defaults.o with the backend define. */
  private void buildLibProcessingCache(String gpp, String std, File cacheDir) throws Exception {
    cacheDir.mkdirs();
    for (String name : new String[]{"Processing", "Processing_defaults"}) {
      File src = new File(runtimeDir, name + ".cpp");
      if (!src.exists()) continue;
      List<String> c = new ArrayList<>(List.of(gpp, "-std=" + std, "-O2", "-c",
          "-DCPPMODE_BACKEND_LIBPROCESSING", "-DPROCESSING_HAS_STB_IMAGE", "-DPROCESSING_HAS_STB_TRUETYPE"));
      // header folder before the runtime folder (see buildCommand)
      for (File inc : libProcessingIncludes()) { c.add("-I"); c.add(inc.getAbsolutePath()); }
      c.add("-I"); c.add(runtimeDir.getAbsolutePath());
      // FreeType letters, when build-cache.py found FreeType for the OpenGL cache
      File ftSrc = new File(runtimeDir.getParentFile(), "cache/linux-x64/freetype.flags");
      if (name.equals("Processing") && ftSrc.exists() && new File("/usr/include/freetype2").isDirectory()) {
        c.add("-I/usr/include/freetype2"); c.add("-DPROCESSING_USE_FREETYPE");
        java.nio.file.Files.copy(ftSrc.toPath(), new File(cacheDir, "freetype.flags").toPath(),
            java.nio.file.StandardCopyOption.REPLACE_EXISTING);
      }
      File bundledInc = new File(runtimeDir.getParentFile(), "libs/include");
      if (bundledInc.exists()) { c.add("-I"); c.add(bundledInc.getAbsolutePath()); }
      c.add(src.getAbsolutePath());
      c.add("-o"); c.add(new File(cacheDir, name + ".o").getAbsolutePath());
      System.err.println("[CppMode] building libprocessing cache: " + name + ".o");
      Process pr = toolchainEnv(new ProcessBuilder(c)).redirectErrorStream(true).start();
      String log = new String(pr.getInputStream().readAllBytes());
      if (pr.waitFor() != 0) {
        System.err.println(log);
        throw new Exception("libprocessing cache build failed for " + name + ".cpp");
      }
    }
  }

  private static void addWindowsLinkFlags(List<String> cmd) {
    cmd.addAll(List.of("-lglfw3", "-lglew32", "-lopengl32", "-lgdi32",
        "-lwinmm", "-mwindows", "-DGLEW_STATIC"));
  }

  private static void addMacLinkFlags(List<String> cmd) {
    cmd.addAll(List.of(
        "-framework", "OpenGL",
        "-framework", "Cocoa",
        "-framework", "IOKit",
        "-framework", "CoreFoundation",
        "-lglfw", "-lGLEW"));
  }

  private static void addLinuxLinkFlags(List<String> cmd) {
    cmd.addAll(List.of("-lGL", "-lGLEW", "-lglfw", "-ldl", "-lm", "-lpthread"));
  }

  // ── g++ version probe ─────────────────────────────────────────────────────

  /** Returns the best C++ standard supported by the given g++. */
  public static String bestCppStd(String gpp) {
    for (String std : new String[]{"c++23", "c++2b", "c++20", "c++17"}) {
      try {
        ProcessBuilder pb = toolchainEnv(new ProcessBuilder(gpp, "-std=" + std, "-x", "c++",
            "-fsyntax-only", "-"));
        pb.redirectErrorStream(true);
        Process p = pb.start();
        p.getOutputStream().close();
        p.getInputStream().readAllBytes();
        if (p.waitFor() == 0) return std;
      } catch (Exception ignored) {}
    }
    return "c++17";
  }

  private static int gppMajorVersion(String gpp) {
    try {
      Process p = Runtime.getRuntime().exec(new String[]{gpp, "--version"});
      String out = new String(p.getInputStream().readAllBytes());
      Matcher m = Pattern.compile("(\\d+)\\.\\d+\\.\\d+").matcher(out);
      return m.find() ? Integer.parseInt(m.group(1)) : -1;
    } catch (Exception e) { return -1; }
  }

  // ── g++ finder ────────────────────────────────────────────────────────────

  public static String findGpp(boolean isWindows) {
    if (isWindows) {
      String appdata = System.getenv("APPDATA");
      String home    = System.getProperty("user.home", "");
      for (String p : new String[]{
          (appdata != null ? appdata : "") + "\\CppMode\\gcc\\mingw64\\bin\\g++.exe",
          "C:\\msys64\\mingw64\\bin\\g++.exe",
          "C:\\msys2\\mingw64\\bin\\g++.exe",
          home + "\\msys64\\mingw64\\bin\\g++.exe",
          "C:\\msys64\\ucrt64\\bin\\g++.exe"}) {
        if (new File(p).exists()) return p;
      }
    }
    return "g++";
  }

  // ── g++ availability check ────────────────────────────────────────────────

  // error code already printed during this build (shown above the compiler's text)
  private static CppErrors.Hit shownHit;

  private void checkGppAvailable(RunnerListener listener) throws Exception {
    String os = System.getProperty("os.name", "").toLowerCase();
    boolean isWin = os.contains("win");
    boolean isMac = os.contains("mac");
    String gpp = findGpp(isWin);

    // Quick test: can we actually invoke it?
    try {
      Process p = Runtime.getRuntime().exec(new String[]{gpp, "--version"});
      if (p.waitFor() == 0) return; // all good
    } catch (Exception ignored) {}

    // Not found — delegate to InstallWizard
    if (InstallWizard.run(listener)) return;

    String msg = isWin
        ? "g++ not found — install MSYS2 or use the auto-installer."
        : isMac
          ? "g++ not found — install via: brew install gcc"
          : "g++ not found — install via your package manager (e.g. apt install g++)";
    listener.statusError(msg);
    throw new Exception("g++ not found");
  }

  // ── Runtime asset copying ─────────────────────────────────────────────────

  private void copyRuntimeAssets(File binary, boolean isWindows, boolean isMac)
      throws Exception {
    File sketchFolder = sketch.getFolder();

    // default.ttf
    File font = new File(runtimeDir, "default.ttf");
    if (font.exists()) {
      File dest = new File(sketchFolder, "default.ttf");
      if (!dest.exists()) Files.copy(font.toPath(), dest.toPath());
    }

    // macOS: copy bundled dylibs so the binary can find them at runtime
    if (isMac) {
      File macLibsDir = new File(runtimeDir.getParentFile(), "libs/macos");
      if (macLibsDir.exists()) {
        File[] dylibs = macLibsDir.listFiles((d, n) -> n.endsWith(".dylib"));
        if (dylibs != null) {
          for (File dylib : dylibs) {
            for (File destDir : new File[]{new File(binary.getParent()), sketchFolder}) {
              try {
                Files.copy(dylib.toPath(), new File(destDir, dylib.getName()).toPath(),
                    StandardCopyOption.REPLACE_EXISTING);
              } catch (Exception ignored) {}
            }
          }
        }
      }
    }

    // Windows: copy bundled DLLs
    if (isWindows) {
      File bundledDir = new File(runtimeDir.getParentFile(), "libs/windows-x64");
      if (bundledDir.exists()) {
        File binaryDir = new File(binary.getParent());
        binaryDir.mkdirs();
        File[] dlls = bundledDir.listFiles((d, n) -> n.endsWith(".dll"));
        if (dlls != null) {
          for (File dll : dlls) {
            File dest = new File(binaryDir, dll.getName());
            if (!dest.exists())
              try { Files.copy(dll.toPath(), dest.toPath()); } catch (Exception ignored) {}
          }
        }
      }
    }
  }

  // ── Diagnostic printing ───────────────────────────────────────────────────

  private static void printDiagnostic(String line) {
    String rewritten = rewriteGccError(line);
    if (line.contains("error:") || line.contains("warning:") || line.contains("note:")) {
      System.err.println(rewritten);
    } else if (!line.isBlank()) {
      System.out.println(rewritten);
    }
  }
}
