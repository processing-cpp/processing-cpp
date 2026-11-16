package processing.mode.cpp;

import processing.app.Problem;
import processing.app.Sketch;

import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.function.Consumer;
import java.util.regex.*;

/**
 * Live lint checker for CppMode — two-phase feedback.
 *
 * <h2>Phase 1: tree-sitter parse errors (instant, ~0 ms)</h2>
 * <p>As soon as the user stops typing (after {@value #PARSE_DEBOUNCE_MS} ms),
 * the sketch is re-parsed by {@link TsEngine}.  Any ERROR or MISSING nodes in
 * the concrete syntax tree are reported immediately as squiggle underlines in
 * the editor.  These indicate structural syntax problems (mismatched braces,
 * missing semicolons, etc.) that are caught without invoking the compiler.
 *
 * <h2>Phase 2: g++ type errors (~200–800 ms with PCH)</h2>
 * <p>After a longer debounce ({@value #GPP_DEBOUNCE_MS} ms), the full
 * pipeline is run ({@link CppBuild#generateSketchOutput}), the resulting C++
 * is written to a temp file, and {@code g++ -fsyntax-only} is invoked.  This
 * catches type errors, undeclared identifiers, etc.  A precompiled header
 * (PCH) for {@code Processing.h} is built once and reused to keep this phase
 * fast.
 *
 * <p>Both phases report problems via the same {@link Consumer<List<Problem>>}
 * callback, replacing the previous set of problems atomically.
 */
public class CppLinter {

  private static final int PARSE_DEBOUNCE_MS = 150;
  private static final int GPP_DEBOUNCE_MS   = 800;

  // g++ diagnostic pattern:  <file>:<line>[:<col>]: error|warning: <msg>
  private static final Pattern GCC_DIAG = Pattern.compile(
      "sketch\\.pde:(\\d+)(?::\\d+)?:\\s+(error|warning):\\s+(.+)");

  // tree-sitter error node scanning — we look for ERROR nodes in the CST
  private static final String TS_ERROR_TYPE   = "ERROR";
  private static final String TS_MISSING_TYPE = "MISSING";

  // ── State ─────────────────────────────────────────────────────────────────

  private final ScheduledExecutorService scheduler =
      Executors.newSingleThreadScheduledExecutor(r -> {
        Thread t = new Thread(r, "CppLinter");
        t.setDaemon(true);
        return t;
      });

  private ScheduledFuture<?> pendingParse;
  private ScheduledFuture<?> pendingGpp;

  private final Consumer<List<Problem>> consumer;
  private final CppMode mode;

  private volatile File pchFile; // cached PCH path
  private volatile boolean parseHadErrors = false; // set by Phase 1, read by Phase 2

  // ── Constructor ───────────────────────────────────────────────────────────

  public CppLinter(CppMode mode, Consumer<List<Problem>> consumer) {
    this.mode     = mode;
    this.consumer = consumer;
  }

  // ── Schedule ──────────────────────────────────────────────────────────────

  /**
   * Schedule a lint pass.  Called from the editor's {@code DocumentListener}
   * on every keystroke.  Cancels and replaces any pending check.
   *
   * @param sketch     current sketch (for tab metadata)
   * @param currentTab index of the active tab
   * @param liveText   unsaved text of the active tab
   */
  public synchronized void scheduleCheck(Sketch sketch, int currentTab, String liveText) {
    // Cancel pending passes
    if (pendingParse != null) pendingParse.cancel(false);
    if (pendingGpp   != null) pendingGpp.cancel(false);

    // Build the concatenated source once — shared between both phases
    String[] sourceHolder = {null};

    // Phase 1: instant tree-sitter parse errors
    pendingParse = scheduler.schedule(() -> {
      String src = buildSource(sketch, currentTab, liveText);
      sourceHolder[0] = src;
      runParseCheck(src, sketch.getCodeCount(), buildTabStartLines(sketch, currentTab, liveText));
    }, PARSE_DEBOUNCE_MS, TimeUnit.MILLISECONDS);

    // Phase 2: g++ type errors (uses the same source when it's ready)
    pendingGpp = scheduler.schedule(() -> {
      String src = sourceHolder[0] != null
          ? sourceHolder[0]
          : buildSource(sketch, currentTab, liveText);
      runGppCheck(src, sketch.getCodeCount(), buildTabStartLines(sketch, currentTab, liveText));
    }, GPP_DEBOUNCE_MS, TimeUnit.MILLISECONDS);
  }

  public void shutdown() { scheduler.shutdownNow(); }

  // ── Phase 1: tree-sitter parse check ─────────────────────────────────────

  private void runParseCheck(String source, int tabCount, int[] tabStartLines) {
    List<Problem> problems = new ArrayList<>();
    try {
      String sanitised = CppBuild.sanitise(source);
      TsEngine engine = TsEngine.getOrNull();
      if (engine == null) return;
      try (TsEngine.ParseResult result = engine.parse(sanitised)) {
        if (result.hasErrors()) {
          collectErrorNodes(result.root(), result, problems, tabStartLines);
        }
      }
    } catch (Exception e) {
      System.err.println("[CppLinter] Phase 1 error: " + e);
    }

    parseHadErrors = !problems.isEmpty();
    deliverPhaseOne(List.copyOf(problems));
  }

  /**
   * Collect all error and missing-token nodes in the tree. A missing token
   * keeps its normal type (for example ";") and is only flagged as missing,
   * so check the flags, not just the type name.
   */
  private void collectErrorNodes(TsNode node,
                                  TsEngine.ParseResult result,
                                  List<Problem> out, int[] tabStartLines) {
    boolean missing = node.isMissing();
    if (missing || node.isError() || TS_ERROR_TYPE.equals(node.getType())) {
      int line = node.getStartRow() + 1;   // 1-based line in all tabs joined together
      int col  = node.getStartCol();
      // Find the tab this line belongs to
      int tabIdx = 0, lineInTab = line - 1;
      for (int i = tabStartLines.length - 1; i >= 0; i--) {
        if (line >= tabStartLines[i]) { tabIdx = i; lineInTab = line - tabStartLines[i]; break; }
      }
      String msg = missing
          ? "Syntax error: missing \"" + node.getType() + "\""
          : "Syntax error";
      out.add(new CppProblem(true, tabIdx, lineInTab, col, col + 1, msg));
      return; // don't recurse into error subtrees
    }
    for (int i = 0; i < node.getChildCount(); i++) {
      TsNode child = node.getChild(i);
      if (child != null) collectErrorNodes(child, result, out, tabStartLines);
    }
  }

  // ── Phase 2: g++ syntax check ─────────────────────────────────────────────

  private void runGppCheck(String source, int tabCount, int[] tabStartLines) {
    // Syntax errors are already marked by Phase 1, on the right line. The
    // compiler would only add follow-on errors, so leave those marks alone.
    if (parseHadErrors) return;
    List<Problem> problems = new ArrayList<>();
    Path tmp = null;
    boolean gppRan = false;  // true only if g++ actually produced output we could parse
    try {
      File runtimeDir  = mode.getRuntimeDir();
      File processingH = new File(runtimeDir, "Processing.h");
      if (!processingH.exists()) {
        System.err.println("[CppLinter] Phase 2 skipped: Processing.h not found at " + processingH);
        return; // keep Phase 1 results
      }

      boolean isWin = System.getProperty("os.name","").toLowerCase().contains("win");
      String  gpp;
      try {
        gpp = CppBuild.findGpp(isWin);
      } catch (Exception e) {
        System.err.println("[CppLinter] Phase 2 skipped: g++ not found — " + e.getMessage());
        return; // keep Phase 1 results
      }

      File gch = ensurePch(runtimeDir, processingH, gpp);

      // Generate C++ via the pipeline — if this throws, parse errors are
      // already shown from Phase 1, so just keep them.
      String lintSource;
      try {
        lintSource = CppBuild.generateSketchOutput(source);
      } catch (Exception e) {
        System.err.println("[CppLinter] Phase 2 skipped: code-gen failed — " + e.getMessage());
        return; // keep Phase 1 results
      }

      tmp = Files.createTempFile("cppmode_lint_", ".cpp");
      Files.writeString(tmp, lintSource);

      List<String> cmd = new ArrayList<>();
      cmd.add(gpp);
      cmd.add("-fsyntax-only");
      cmd.add("-std=" + CppBuild.bestCppStd(gpp));
      cmd.add("-I"); cmd.add(runtimeDir.getAbsolutePath());
      File bundledInc = new File(runtimeDir.getParentFile(), "libs/include");
      if (bundledInc.exists() && isWin) { cmd.add("-I"); cmd.add(bundledInc.getAbsolutePath()); }
      if (gch != null) { cmd.add("-I"); cmd.add(gch.getParentFile().getAbsolutePath()); }
      cmd.add(tmp.toAbsolutePath().toString());

      ProcessBuilder pb = CppBuild.toolchainEnv(new ProcessBuilder(cmd));
      pb.redirectErrorStream(true);
      Process proc = pb.start();
      byte[] raw = proc.getInputStream().readNBytes(64 * 1024);
      proc.waitFor(20, TimeUnit.SECONDS);
      gppRan = true;

      String output = new String(raw);
      for (String line : output.split("\n")) {
        Matcher m = GCC_DIAG.matcher(line);
        if (!m.find()) continue;
        int     rawLine = Integer.parseInt(m.group(1));
        boolean isErr   = "error".equals(m.group(2));
        String  msg     = CppErrors.describe(m.group(3), CppBuild.rewriteGccError(m.group(3).trim()));

        // Map rawLine back to tab + local line
        int tabIdx    = 0;
        int lineInTab = rawLine - 1;
        for (int i = tabCount - 1; i >= 0; i--) {
          if (rawLine >= tabStartLines[i]) {
            tabIdx    = i;
            lineInTab = rawLine - tabStartLines[i];
            break;
          }
        }
        if (lineInTab < 0) continue;
        problems.add(new CppProblem(isErr, tabIdx, lineInTab, 0, -1, msg));
      }
    } catch (Exception e) {
      System.err.println("[CppLinter] Phase 2 error: " + e);
    } finally {
      if (tmp != null) try { Files.deleteIfExists(tmp); } catch (Exception ignored2) {}
    }

    // Only replace Phase 1 results if g++ actually ran.
    // If g++ was unavailable or the pipeline failed, keep Phase 1 squiggles.
    if (gppRan) {
      List<Problem> result = List.copyOf(problems);
      javax.swing.SwingUtilities.invokeLater(() -> consumer.accept(result));
    }
  }

  // ── Delivery ──────────────────────────────────────────────────────────────

  /** Phase-1 delivery — push parse errors immediately. */
  private void deliverPhaseOne(List<Problem> problems) {
    javax.swing.SwingUtilities.invokeLater(() -> consumer.accept(problems));
  }

  // ── PCH management ───────────────────────────────────────────────────────

  private File ensurePch(File runtimeDir, File processingH, String gpp) {
    if (pchFile != null && pchFile.exists()) return pchFile;

    String os  = System.getProperty("os.name", "").toLowerCase();
    String sub = os.contains("win")  ? "cache/windows-x64"
               : os.contains("mac")  ? "cache/macos-" +
                 (System.getProperty("os.arch","").contains("aarch64") ? "arm64" : "x64")
               : "cache/linux-x64";
    File cacheDir = new File(runtimeDir.getParentFile(), sub);
    cacheDir.mkdirs();
    File gch = new File(cacheDir, "Processing.h.gch");
    if (gch.exists()) { pchFile = gch; return gch; }

    try {
      ProcessBuilder pb = CppBuild.toolchainEnv(new ProcessBuilder(
          gpp, "-x", "c++-header", "-std=c++23",
          "-I", runtimeDir.getAbsolutePath(),
          processingH.getAbsolutePath(),
          "-o", gch.getAbsolutePath()));
      pb.redirectErrorStream(true);
      Process proc = pb.start();
      proc.getInputStream().readAllBytes();
      proc.waitFor(30, TimeUnit.SECONDS);
      if (proc.exitValue() == 0) { pchFile = gch; return gch; }
    } catch (Exception ignored) {}
    return null;
  }

  // ── Source helpers ────────────────────────────────────────────────────────

  private static String buildSource(Sketch sketch, int currentTab, String liveText) {
    int tabCount = sketch.getCodeCount();
    StringBuilder sb = new StringBuilder();
    for (int i = 0; i < tabCount; i++) {
      String prog = (i == currentTab) ? liveText : sketch.getCode(i).getProgram();
      if (prog == null) prog = "";
      sb.append(prog).append("\n");
    }
    return sb.toString();
  }

  private static int[] buildTabStartLines(Sketch sketch, int currentTab, String liveText) {
    int tabCount = sketch.getCodeCount();
    int[] starts = new int[tabCount];
    int cur = 1;
    for (int i = 0; i < tabCount; i++) {
      starts[i] = cur;
      String prog = (i == currentTab) ? liveText : sketch.getCode(i).getProgram();
      if (prog == null) prog = "";
      cur += countLines(prog) + 1;
    }
    return starts;
  }

  private static int countLines(String s) {
    int n = 0;
    for (int i = 0; i < s.length(); i++) if (s.charAt(i) == '\n') n++;
    return n;
  }
}
