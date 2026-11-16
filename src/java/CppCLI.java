package processing.mode.cpp;

import java.io.*;
import java.nio.charset.StandardCharsets;

/**
 * Standalone CLI translator for the web compile server.
 *
 * <p>Reads a Processing (.pde) sketch from stdin and writes the generated
 * C++ source to stdout.  Errors are written to stderr and the process exits
 * with code 1.
 *
 * <p>Uses the same tree-sitter pipeline as the IDE:
 * <pre>
 *   stdin  →  sanitise  →  TsEngine.parse  →  SketchAnalyzer  →  SketchEmitter  →  stdout
 * </pre>
 *
 * Usage:
 * <pre>
 *   java -cp CppMode.jar processing.mode.cpp.CppCLI \
 *        --native-lib /path/to/tree_sitter_processing_cpp.so \
 *        < sketch.pde
 * </pre>
 * The {@code --native-lib} flag is optional; without it the library is
 * resolved from the classpath or the default install location.
 */
public class CppCLI {

  public static void main(String[] args) throws Exception {
    // Optional --native-lib <path> argument
    File nativeLib = null;
    for (int i = 0; i < args.length - 1; i++) {
      if ("--native-lib".equals(args[i])) {
        nativeLib = new File(args[i + 1]);
      }
    }

    // Initialise TsEngine. When running as CLI the "mode folder" concept
    // does not apply, so we use the directory of the native lib if provided,
    // or fall back to a dummy folder so TsEngine can try classpath extraction.
    // Mode folder (the one containing libs/): --mode-folder <dir>, or a bare
    // argument, or worked out from --native-lib (<mode>/libs/<platform>/<file>),
    // or the current directory.
    File modeFolder = null;
    for (int i = 0; i < args.length; i++) {
      if ("--native-lib".equals(args[i])) { i++; continue; }
      if ("--mode-folder".equals(args[i])) { if (i + 1 < args.length) modeFolder = new File(args[++i]); continue; }
      if (!args[i].startsWith("--") && modeFolder == null) modeFolder = new File(args[i]);
    }
    if (modeFolder == null && nativeLib != null) {
      File platformDir = nativeLib.getAbsoluteFile().getParentFile();
      File libsDir = platformDir != null ? platformDir.getParentFile() : null;
      modeFolder = (libsDir != null && libsDir.getParentFile() != null) ? libsDir.getParentFile() : platformDir;
    }
    if (modeFolder == null) modeFolder = new File(".");
    TsEngine.initForCli(nativeLib, modeFolder);
    if (TsEngine.getOrNull() == null) {
      System.err.println("Error: tree-sitter native library not found. Looked under "
          + modeFolder.getAbsolutePath() + "/libs/. Pass the CppMode folder as an argument.");
      System.exit(2);
    }

    String input = new String(System.in.readAllBytes(), StandardCharsets.UTF_8);
    try {
      System.out.print(CppBuild.generateSketchOutput(input));
    } catch (Exception e) {
      System.err.println("Error: " + e.getMessage());
      System.exit(1);
    }
  }
}
