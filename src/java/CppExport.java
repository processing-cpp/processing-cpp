package processing.mode.cpp;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.regex.*;
import processing.app.RunnerListener;
import processing.app.Sketch;

/** Export Application, moved out of CppBuild (cppmode-export). Ported from the last
 *  committed CppBuild that had it; see git history before ebac1f7. */
public class CppExport {
  private final Sketch sketch;
  private final CppMode mode;
  private final File runtimeDir;
  private final CppBuild build;

  public CppExport(Sketch sketch, CppMode mode, File runtimeDir) {
    this.sketch = sketch; this.mode = mode; this.runtimeDir = runtimeDir;
    this.build = new CppBuild(sketch, mode);
  }

  private boolean cmdExists(String cmd) {
    try {
      String os = System.getProperty("os.name","").toLowerCase();
      ProcessBuilder pb;
      if (os.contains("win")) {
        // On Windows use 'where' instead of 'which'
        pb = new ProcessBuilder("where", cmd);
      } else {
        pb = new ProcessBuilder("which", cmd);
      }
      pb.redirectErrorStream(true);
      return pb.start().waitFor() == 0;
    } catch (Exception e) { return false; }
  }

  private boolean compilerExists(ExportTarget t) {
    String os = System.getProperty("os.name","").toLowerCase();
    boolean isWin = os.contains("win");

    // macOS cross-compile needs osxcross - not bundled
    if (t == ExportTarget.MACOS_X64 || t == ExportTarget.MACOS_ARM64)
      return cmdExists(t.compiler);

    // Linux targets: need Linux/Mac to cross-compile
    if (!t.isWindows) {
      if (t == ExportTarget.MACOS_X64 || t == ExportTarget.MACOS_ARM64)
        return cmdExists(t.compiler);
      if (isWin) return false; // not available on Windows - need Linux machine
      if (muslToolchainInstalled(t)) return true;
      return cmdExists(t.compiler);
    }

    // Windows target
    if (isWin) {
      String[] msysPaths = {
        "C:\\msys64\\mingw64\\bin\\g++.exe",
        "C:\\msys2\\mingw64\\bin\\g++.exe",
        System.getProperty("user.home") + "\\msys64\\mingw64\\bin\\g++.exe"
      };
      for (String p : msysPaths)
        if (new java.io.File(p).exists()) return true;
      return cmdExists("g++");
    }
    return cmdExists(t.compiler);
  }

  public enum ExportTarget {
    LINUX_X64   ("linux-x64",    "g++",                        "Linux 64-bit (x86_64)",         false),
    LINUX_ARM64 ("linux-arm64",  "aarch64-linux-gnu-g++",      "Linux ARM64 (RPi 4/5, 64-bit)", false),
    LINUX_ARM32 ("linux-arm32",  "arm-linux-gnueabihf-g++",    "Linux ARM32 (RPi 2/3, 32-bit)", false),
    WINDOWS_X64 ("windows-x64", "x86_64-w64-mingw32-g++",     "Windows 64-bit",                true),
    MACOS_X64   ("macos-x64",   "x86_64-apple-darwin23-c++",  "macOS Intel 64-bit",            false),
    MACOS_ARM64 ("macos-arm64", "aarch64-apple-darwin23-c++", "macOS Apple Silicon",           false);

    public final String dirName, compiler, label;
    public final boolean isWindows;
    ExportTarget(String d, String c, String l, boolean w) {
      dirName=d; compiler=c; label=l; isWindows=w;
    }
  }

  public void showExportDialog(Sketch sketch, RunnerListener listener) {
    java.util.Map<ExportTarget,Boolean> avail = new java.util.LinkedHashMap<>();
    for (ExportTarget t : ExportTarget.values()) avail.put(t, compilerExists(t));

    String os = System.getProperty("os.name","").toLowerCase();
    boolean isWin = os.contains("win");
    java.awt.Color bg    = new java.awt.Color(28,30,38);
    java.awt.Color bgRow = new java.awt.Color(35,37,48);
    java.awt.Color green = new java.awt.Color(100,220,120);
    java.awt.Color red   = new java.awt.Color(240,80,80);
    java.awt.Color blue  = new java.awt.Color(210,228,255);

    javax.swing.JDialog dlg = new javax.swing.JDialog((java.awt.Frame)null,"Export Application",true);
    dlg.setLayout(new java.awt.BorderLayout(8,8));
    dlg.getContentPane().setBackground(bg);

    // ââ Title ââââââââââââââââââââââââââââââââââââââââââââââââââââââââââââââââ
    javax.swing.JLabel title = new javax.swing.JLabel("  Export Application");
    title.setForeground(blue);
    title.setFont(title.getFont().deriveFont(java.awt.Font.BOLD,15f));
    title.setBorder(javax.swing.BorderFactory.createEmptyBorder(14,8,4,8));
    dlg.add(title, java.awt.BorderLayout.NORTH);

    javax.swing.JPanel center = new javax.swing.JPanel(new java.awt.BorderLayout(4,6));
    center.setBackground(bg);
    center.setBorder(javax.swing.BorderFactory.createEmptyBorder(0,14,4,14));

    // ââ Icon picker ââââââââââââââââââââââââââââââââââââââââââââââââââââââââââ
    javax.swing.JPanel iconPanel = new javax.swing.JPanel(new java.awt.BorderLayout(6,4));
    iconPanel.setBackground(bg);
    iconPanel.setBorder(javax.swing.BorderFactory.createEmptyBorder(4,0,8,0));

    // Top row: label + hint
    javax.swing.JPanel iconTop = new javax.swing.JPanel(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT,4,0));
    iconTop.setBackground(bg);
    javax.swing.JLabel iconLbl = new javax.swing.JLabel("App Icon (PNG)  ");
    iconLbl.setForeground(blue);
    iconLbl.setFont(iconLbl.getFont().deriveFont(java.awt.Font.BOLD, 12f));
    javax.swing.JLabel iconHint = new javax.swing.JLabel("optional Â· 256Ã256 recommended Â· leave blank to skip");
    iconHint.setForeground(new java.awt.Color(110,115,150));
    iconHint.setFont(iconHint.getFont().deriveFont(10f));
    iconTop.add(iconLbl); iconTop.add(iconHint);
    iconPanel.add(iconTop, java.awt.BorderLayout.NORTH);

    // Bottom row: preview + path field + buttons
    javax.swing.JPanel iconBottom = new javax.swing.JPanel(new java.awt.BorderLayout(6,0));
    iconBottom.setBackground(bg);

    // Icon preview label (shows thumbnail)
    javax.swing.JLabel iconPreview = new javax.swing.JLabel();
    iconPreview.setPreferredSize(new java.awt.Dimension(48,48));
    iconPreview.setOpaque(true);
    iconPreview.setBackground(new java.awt.Color(40,42,55));
    iconPreview.setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(60,65,90)));
    iconPreview.setHorizontalAlignment(javax.swing.JLabel.CENTER);
    iconBottom.add(iconPreview, java.awt.BorderLayout.WEST);

    // Path field - read only, updated by file chooser
    javax.swing.JTextField iconField = new javax.swing.JTextField();
    iconField.setBackground(new java.awt.Color(40,42,55));
    iconField.setForeground(blue); iconField.setCaretColor(blue);
    iconField.setBorder(javax.swing.BorderFactory.createCompoundBorder(
      javax.swing.BorderFactory.createLineBorder(new java.awt.Color(60,65,90)),
      javax.swing.BorderFactory.createEmptyBorder(2,6,2,6)));
    iconField.setFont(iconField.getFont().deriveFont(11f));

    // Helper to update preview
    Runnable[] updatePreview = {null};
    updatePreview[0] = () -> {
      String p = iconField.getText().trim();
      if (!p.isEmpty() && new File(p).exists()) {
        try {
          java.awt.image.BufferedImage img = javax.imageio.ImageIO.read(new File(p));
          if (img != null) {
            java.awt.Image scaled = img.getScaledInstance(44, 44, java.awt.Image.SCALE_SMOOTH);
            iconPreview.setIcon(new javax.swing.ImageIcon(scaled));
            iconPreview.setText("");
          }
        } catch (Exception ignored) { iconPreview.setIcon(null); iconPreview.setText("?"); }
      } else {
        iconPreview.setIcon(null);
        iconPreview.setText(p.isEmpty() ? "none" : "!");
        iconPreview.setForeground(new java.awt.Color(120,125,155));
      }
    };

    // Pre-fill if sketch has icon.png
    for (String p : new String[]{"icon.png","data/icon.png"}) {
      File ic = new File(sketch.getFolder(), p);
      if (ic.exists()) { iconField.setText(ic.getAbsolutePath()); break; }
    }
    updatePreview[0].run();

    // Buttons panel
    javax.swing.JPanel iconBtns = new javax.swing.JPanel(new java.awt.GridLayout(2,1,2,2));
    iconBtns.setBackground(bg);

    javax.swing.JButton iconBrowse = new javax.swing.JButton("Browse...");
    iconBrowse.setFont(iconBrowse.getFont().deriveFont(11f));
    iconBrowse.addActionListener(e -> {
      javax.swing.JFileChooser fc = new javax.swing.JFileChooser(
        iconField.getText().isEmpty() ? sketch.getFolder().getAbsolutePath()
          : new File(iconField.getText()).getParent());
      fc.setDialogTitle("Select App Icon (PNG)");
      fc.setFileFilter(new javax.swing.filechooser.FileNameExtensionFilter("PNG Images (*.png)","png"));
      fc.setFileSelectionMode(javax.swing.JFileChooser.FILES_ONLY);
      fc.setPreferredSize(new java.awt.Dimension(700, 500));
      if (fc.showOpenDialog(dlg) == javax.swing.JFileChooser.APPROVE_OPTION) {
        iconField.setText(fc.getSelectedFile().getAbsolutePath());
        updatePreview[0].run();
      }
    });

    javax.swing.JButton iconClear = new javax.swing.JButton("Clear");
    iconClear.setFont(iconClear.getFont().deriveFont(11f));
    iconClear.setForeground(new java.awt.Color(180,80,80));
    iconClear.addActionListener(e -> {
      iconField.setText("");
      updatePreview[0].run();
    });

    iconBtns.add(iconBrowse); iconBtns.add(iconClear);
    iconBottom.add(iconField, java.awt.BorderLayout.CENTER);
    iconBottom.add(iconBtns, java.awt.BorderLayout.EAST);
    iconPanel.add(iconBottom, java.awt.BorderLayout.CENTER);
    center.add(iconPanel, java.awt.BorderLayout.NORTH);

    // ââ Target checkboxes ââââââââââââââââââââââââââââââââââââââââââââââââââââ
    javax.swing.JPanel targets = new javax.swing.JPanel();
    targets.setLayout(new javax.swing.BoxLayout(targets, javax.swing.BoxLayout.Y_AXIS));
    targets.setBackground(bg);
    java.util.Map<ExportTarget,javax.swing.JCheckBox> boxes = new java.util.LinkedHashMap<>();

    for (ExportTarget t : ExportTarget.values()) {
      // Show all targets everywhere - unavailable ones shown grayed out
      boolean ok = avail.getOrDefault(t, false);
      boolean isMac = t==ExportTarget.MACOS_X64 || t==ExportTarget.MACOS_ARM64;

      javax.swing.JPanel row = new javax.swing.JPanel(new java.awt.FlowLayout(java.awt.FlowLayout.LEFT,5,2));
      row.setBackground(bgRow);
      row.setMaximumSize(new java.awt.Dimension(Integer.MAX_VALUE, 34));

      javax.swing.JCheckBox cb = new javax.swing.JCheckBox(t.label);
      cb.setBackground(bgRow); cb.setEnabled(ok);
      cb.setForeground(ok ? blue : red);
      if (!ok) cb.setFont(cb.getFont().deriveFont(java.awt.Font.ITALIC));
      cb.setSelected(ok && (
        (isWin && t==ExportTarget.WINDOWS_X64) ||
        (!isWin && (t==ExportTarget.LINUX_X64 || t==ExportTarget.WINDOWS_X64))));
      if (isMac && ok) cb.addActionListener(e -> {
        if (cb.isSelected()) javax.swing.JOptionPane.showMessageDialog(dlg,
          "<html><b>macOS export</b> requires <b>osxcross</b> on your PATH.<br>"+
          "This may take several minutes to compile.<br>"+
          "If osxcross is not installed, export will fail.</html>",
          "macOS Warning", javax.swing.JOptionPane.WARNING_MESSAGE);
      });
      row.add(cb);
      boxes.put(t, cb);

      javax.swing.JLabel statusLbl = new javax.swing.JLabel(ok ? "â" : "â not found");
      statusLbl.setForeground(ok ? green : red);
      statusLbl.setFont(statusLbl.getFont().deriveFont(10f));
      row.add(statusLbl);

      if (!ok) {
        javax.swing.JButton installBtn = new javax.swing.JButton("Install");
        installBtn.setBackground(new java.awt.Color(50,80,150));
        installBtn.setForeground(java.awt.Color.WHITE);
        installBtn.setFont(installBtn.getFont().deriveFont(10f));
        installBtn.setMargin(new java.awt.Insets(1,6,1,6));
        final ExportTarget ft=t; final javax.swing.JCheckBox fcb=cb;
        final javax.swing.JLabel fsl=statusLbl;
        installBtn.addActionListener(e -> {
          installBtn.setText("Installing...");
          installBtn.setEnabled(false);
          new Thread(() -> {
            boolean ok2 = installCompilerForTarget(ft, listener);
            javax.swing.SwingUtilities.invokeLater(() -> {
              if (ok2 && compilerExists(ft)) {
                fcb.setEnabled(true); fcb.setForeground(blue);
                fcb.setFont(fcb.getFont().deriveFont(java.awt.Font.PLAIN));
                fsl.setText("â ready"); fsl.setForeground(green);
                installBtn.setText("â Done");
                installBtn.setBackground(new java.awt.Color(40,110,55));
              } else {
                installBtn.setText("Retry"); installBtn.setEnabled(true);
                fsl.setText("â failed");
              }
            });
          }).start();
        });
        row.add(installBtn);
        String hint = getInstallHint(t);
        if (!hint.isEmpty()) {
          javax.swing.JLabel hl = new javax.swing.JLabel("  " + hint);
          hl.setForeground(new java.awt.Color(110,115,145));
          hl.setFont(hl.getFont().deriveFont(9f));
          row.add(hl);
        }
      }
      targets.add(row);
      targets.add(javax.swing.Box.createVerticalStrut(2));
    }

    javax.swing.JScrollPane scroll = new javax.swing.JScrollPane(targets);
    scroll.setBorder(null); scroll.getViewport().setBackground(bg);
    center.add(scroll, java.awt.BorderLayout.CENTER);
    dlg.add(center, java.awt.BorderLayout.CENTER);

    // ââ Buttons ââââââââââââââââââââââââââââââââââââââââââââââââââââââââââââââ
    javax.swing.JPanel btnRow = new javax.swing.JPanel(new java.awt.FlowLayout(java.awt.FlowLayout.RIGHT,10,10));
    btnRow.setBackground(bg);
    javax.swing.JButton cancel = new javax.swing.JButton("Cancel");
    javax.swing.JButton export = new javax.swing.JButton("Export");
    export.setBackground(new java.awt.Color(55,95,175)); export.setForeground(java.awt.Color.WHITE);
    boolean[] go = {false};
    cancel.addActionListener(e -> dlg.dispose());
    export.addActionListener(e -> { go[0]=true; dlg.dispose(); });
    btnRow.add(cancel); btnRow.add(export);
    dlg.add(btnRow, java.awt.BorderLayout.SOUTH);

    dlg.pack();
    dlg.setMinimumSize(new java.awt.Dimension(560, 380));
    dlg.setLocationRelativeTo(null);
    dlg.setVisible(true);
    if (!go[0]) return;

    java.util.List<ExportTarget> selected = new java.util.ArrayList<>();
    for (java.util.Map.Entry<ExportTarget,javax.swing.JCheckBox> e : boxes.entrySet())
      if (e.getValue().isSelected()) selected.add(e.getKey());
    if (selected.isEmpty()) return;

    final String iconPath = iconField.getText().trim();
    new Thread(() -> runExport(sketch, listener, selected, iconPath)).start();
  }

  private String getInstallHint(ExportTarget t) {
    String os = System.getProperty("os.name","").toLowerCase();
    boolean isWin = os.contains("win");
    if (isWin) {
      switch (t) {
        case WINDOWS_X64: return "opens MSYS2 installer";
        case LINUX_X64: case LINUX_ARM64: case LINUX_ARM32:
          return "requires Linux - export from a Linux machine";
        case MACOS_X64: case MACOS_ARM64: return "opens osxcross guide";
        default: return "";
      }
    }
    switch (t) {
      case WINDOWS_X64: return isWin ? "opens MSYS2 installer" : "pacman -S mingw-w64-gcc";
      case LINUX_X64:   return "downloads ~50MB toolchain";
      case LINUX_ARM64: return "downloads ~100MB toolchain";
      case LINUX_ARM32: return "downloads ~100MB toolchain";
      case MACOS_X64: case MACOS_ARM64: return "opens osxcross guide";
      default: return "";
    }
  }

  private boolean installCompilerForTarget(ExportTarget t, RunnerListener listener) {
    String os = System.getProperty("os.name","").toLowerCase();
    boolean isWin = os.contains("win");
    try {



      String cmd = null;
      switch (t) {
        case WINDOWS_X64:
          if (isWin) {
            // On Windows, need MSYS2
            java.awt.Desktop.getDesktop().browse(new java.net.URI("https://www.msys2.org"));
            javax.swing.JOptionPane.showMessageDialog(null,
              "<html><b>Opening MSYS2 download page.</b><br><br>" +
              "After installing MSYS2, open the MSYS2 terminal and run:<br><br>" +
              "<code>pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-glfw mingw-w64-x86_64-glew</code><br><br>" +
              "Then restart Processing and try again.</html>",
              "Install MSYS2", javax.swing.JOptionPane.INFORMATION_MESSAGE);
            return false;
          }
          cmd = cmdExists("yay")
            ? "yay -S --noconfirm --needed mingw-w64-gcc mingw-w64-glfw mingw-w64-glew"
            : "pkexec pacman -S --noconfirm --needed mingw-w64-gcc mingw-w64-glfw mingw-w64-glew";
          break;
        case LINUX_X64:
        case LINUX_ARM64:
        case LINUX_ARM32:
          if (isWin) {
            javax.swing.JOptionPane.showMessageDialog(null,
              "<html><b>Linux export not available on Windows.</b><br><br>" +
              "C++ must be compiled natively for each platform.<br><br>" +
              "To build Linux exports:<br>" +
              "&nbsp;&nbsp;â¢ Open this sketch on a Linux machine<br>" +
              "&nbsp;&nbsp;â¢ Use File â Export Application there<br><br>" +
              "The exported Linux AppImage will run on any Linux distro.</html>",
              "Linux Export", javax.swing.JOptionPane.INFORMATION_MESSAGE);
            return false;
          }
          // On Linux/Mac: download musl toolchain
          return downloadMuslToolchain(t, listener);
        case MACOS_X64: case MACOS_ARM64:
          java.awt.Desktop.getDesktop().browse(
            new java.net.URI("https://github.com/tpoechtrager/osxcross#installation"));
          javax.swing.JOptionPane.showMessageDialog(null,
            "<html><b>Opening osxcross setup guide.</b><br><br>" +
            "Follow the instructions to install osxcross,<br>" +
            "then add it to your PATH and restart Processing.</html>",
            "macOS Cross-Compiler", javax.swing.JOptionPane.INFORMATION_MESSAGE);
          return false;
        default: return false;
      }
      if (cmd != null) {
        listener.statusNotice("Installing " + t.label + " compiler...");
        ProcessBuilder pb = isWin
          ? new ProcessBuilder("cmd", "/c", cmd)
          : new ProcessBuilder("bash", "-c", cmd);
        pb.redirectErrorStream(true);
        Process p = pb.start();
        StringBuilder out = new StringBuilder();
        try (java.io.BufferedReader br = new java.io.BufferedReader(
            new java.io.InputStreamReader(p.getInputStream()))) {
          String line;
          while ((line = br.readLine()) != null) {
            System.out.println("[install] " + line);
            out.append(line).append('\n');
          }
        }
        int ret = p.waitFor();
        if (ret != 0) {
          String log = out.toString();
          String snippet = log.length() > 800 ? log.substring(log.length()-800) : log;
          javax.swing.SwingUtilities.invokeLater(() ->
            javax.swing.JOptionPane.showMessageDialog(null,
              "<html><b>Install failed (exit " + ret + ")</b><br><br>" +
              "<pre>" + snippet.replace("<","&lt;").replace(">","&gt;") + "</pre></html>",
              "Install Failed", javax.swing.JOptionPane.ERROR_MESSAGE));
          return false;
        }
        return true;
      }
    } catch (Exception e) {
      System.err.println("Install failed: " + e.getMessage());
      javax.swing.JOptionPane.showMessageDialog(null,
        "<html>Install failed: " + e.getMessage() + "</html>",
        "Install Error", javax.swing.JOptionPane.ERROR_MESSAGE);
    }
    return false;
  }

  private void runExport(Sketch sketch, RunnerListener listener,
                         java.util.List<ExportTarget> targets, String iconPath) {
    listener.statusNotice("Exporting...");
    String skName = sketch.getName();
    File outBase = new File(sketch.getFolder().getParentFile(), skName + "-export");
    outBase.mkdirs();
    String modeSrc = runtimeDir.getParentFile().getAbsolutePath() + "/src";

    // Use writeSketch() to preprocess the sketch into Sketch_run.cpp
    java.util.List<File> sources = new java.util.ArrayList<>();
    File exportBuildDir = sketch.makeTempFolder();
    try {
      // writeSketch produces Sketch_run.cpp with all preprocessing done
      CppBuild helper = new CppBuild(sketch, mode);
      helper.writeSketch(listener);
      File sketchCpp = new File(helper.buildDir, "Sketch_run.cpp");
      if (sketchCpp.exists()) {
        sources.add(sketchCpp);
      } else {
        throw new Exception("Sketch_run.cpp not found after writeSketch");
      }
    } catch (Exception ex) {
      // Fallback: copy .pde as .cpp
      for (processing.app.SketchCode sc : sketch.getCode()) {
        File f = sc.getFile();
        if (f != null && f.exists()) {
          String name = f.getName().replaceAll("\\.pde$", ".cpp");
          File cpp = new File(exportBuildDir, name);
          try {
            java.nio.file.Files.copy(f.toPath(), cpp.toPath(),
              java.nio.file.StandardCopyOption.REPLACE_EXISTING);
            sources.add(cpp);
          } catch (Exception ignored) {}
        }
      }
    }
    if (sources.isEmpty()) { listener.statusError("No source files"); return; }

    // Auto-install missing dependencies for each target before compiling
    installExportDeps(targets, listener);

    int done = 0;
    java.util.List<String> results = new java.util.ArrayList<>();
    for (ExportTarget t : targets) {
      listener.statusNotice("Exporting " + t.label + "...");
      try {
        exportTarget(sketch, t, outBase, modeSrc, skName, sources, listener, iconPath);
        results.add("\u2713 " + t.label); done++;
      } catch (Exception ex) {
        results.add("\u2717 " + t.label + ": " + ex.getMessage());
        System.err.println("Export failed " + t.label + ": " + ex.getMessage());
      }
    }

    final int d = done, tot = targets.size();
    final String sum = String.join("<br>", results);
    final String path = outBase.getAbsolutePath();
    listener.statusNotice(d + "/" + tot + " targets exported");
    javax.swing.SwingUtilities.invokeLater(() -> {
      javax.swing.JOptionPane.showMessageDialog(null,
        "<html><b>Export: " + d + "/" + tot + " targets</b><br><br>"
        + sum + "<br><br>Output: " + path + "</html>",
        "Export Result", d>0
          ? javax.swing.JOptionPane.INFORMATION_MESSAGE
          : javax.swing.JOptionPane.ERROR_MESSAGE);
      if (d > 0) {
        try { new ProcessBuilder("xdg-open", path).start(); }
        catch (Exception ignored) {}
      }
    });
  }

  private void exportTarget(Sketch sketch, ExportTarget t,
                            File outBase, String modeSrc, String skName,
                            java.util.List<File> sources,
                            RunnerListener listener, String iconPath) throws Exception {
    // cppmode-lpexport: libprocessing only where its library exists for the target (Linux x64 for now)
    File lpLibDir = CppBuild.libProcessingLibDir();
    boolean lpExport = build.useLibProcessing() && t.name().equals("LINUX_X64")
        && CppBuild.lpPlatform().equals("linux-x64") && lpLibDir != null;
    if (build.useLibProcessing() && !lpExport)
      System.err.println("[export] " + t.name() + ": no libprocessing library for this target, exporting it with OpenGL");
    File dir = new File(outBase, t.dirName); dir.mkdirs();
    String binName = skName + (t.isWindows ? ".exe" : "");
    File binOut = new File(dir, binName);

    java.util.List<String> cmd = new java.util.ArrayList<>();
    String os = System.getProperty("os.name","").toLowerCase();
    boolean onWin = os.contains("win");
    if (onWin && t == ExportTarget.WINDOWS_X64) {
      // Native Windows build - find MSYS2 g++
      String compiler = "g++";
      String[] msysPaths = {
        "C:\\msys64\\mingw64\\bin\\g++.exe",
        "C:\\msys2\\mingw64\\bin\\g++.exe",
        System.getProperty("user.home") + "\\msys64\\mingw64\\bin\\g++.exe"
      };
      for (String p : msysPaths)
        if (new File(p).exists()) { compiler = p; break; }
      cmd.add(compiler);
    } else if (!t.isWindows && t != ExportTarget.MACOS_X64
               && t != ExportTarget.MACOS_ARM64) {
      // Linux targets: use bundled musl toolchain if available
      File muslComp = lpExport ? null : getMuslCompiler(t);  // libprocessing needs glibc + dynamic linking
      if (muslComp != null) {
        cmd.add(muslComp.getAbsolutePath());
      } else {
        cmd.add(t.compiler); // fall back to system compiler
      }
    } else {
      cmd.add(t.compiler);
    }
    cmd.add("-std=" + CppBuild.bestCppStd(t.compiler)); cmd.add("-O2");
    if (lpExport) {
      cmd.add("-DCPPMODE_BACKEND_LIBPROCESSING");
      for (File inc : CppBuild.libProcessingIncludes()) cmd.add("-I" + inc.getAbsolutePath());
    }
    for (File s : sources) cmd.add(s.getAbsolutePath());
    // Use pre-built cached .o if available for this target
    String exportCacheDir = t.isWindows ? "cache/windows-x64"
      : (t == ExportTarget.LINUX_ARM64 ? "cache/linux-arm64"
      : (t == ExportTarget.LINUX_ARM32 ? "cache/linux-arm32"
      : "cache/linux-x64"));
    if (lpExport) {
      exportCacheDir += "-libprocessing";
      if (!new File(runtimeDir.getParentFile(), exportCacheDir + "/Processing.o").exists()) {
        listener.statusError("Run the sketch once with the libprocessing renderer before exporting it, so its build cache exists.");
        return;
      }
    }
    File exportCache = new File(runtimeDir.getParentFile(), exportCacheDir);
    File cachedProcessingO = new File(exportCache, "Processing.o");
    File cachedDefaultsO   = new File(exportCache, "Processing_defaults.o");
    if (cachedProcessingO.exists()) {
      cmd.add(cachedProcessingO.getAbsolutePath());
    } else {
      cmd.add(modeSrc + "/Processing.cpp");
    }
    if (cachedDefaultsO.exists()) {
      cmd.add(cachedDefaultsO.getAbsolutePath());
    } else {
      cmd.add(modeSrc + "/Processing_defaults.cpp");
    }
    cmd.add("-I" + modeSrc);
    cmd.add("-DPROCESSING_HAS_STB_IMAGE");
    // Debug mode (and the website base URL used in error messages) are
    // both controlled by config/cppmode.properties, a single
    // consolidated file at the root of the CppMode folder (a sibling of
    // src/). Shipped builds always have debug=0. To see PDEBUG(...)
    // output, edit that file to set debug=1 and re-run the sketch --
    // no environment variables to remember, no rebuilding CppBuild
    // itself, works the same regardless of which terminal (or none at
    // all) launched the IDE. This is the SAME file scripts/rebuild-
    // engine.sh reads for the standalone engine-rebuild path, so both
    // ways of building stay in sync automatically.
    java.io.File configFile = new java.io.File(runtimeDir.getParentFile(), "config/cppmode.properties");
    if (configFile.exists()) {
      try {
        java.util.Properties props = new java.util.Properties();
        try (java.io.InputStream in = new java.io.FileInputStream(configFile)) {
          props.load(in);
        }
        if ("1".equals(props.getProperty("debug", "0").trim())) {
          cmd.add("-DPROCESSING_DEBUG");
        }
        String websiteUrl = props.getProperty("website.base.url");
        if (websiteUrl != null && !websiteUrl.trim().isEmpty()) {
          cmd.add("-DPROCESSING_WEBSITE_URL=\"" + websiteUrl.trim() + "\"");
        }
      } catch (java.io.IOException e) {
        // If the config file can't be read for some reason, just
        // proceed with a normal, non-debug build rather than failing
        // the compile entirely over a missing diagnostic toggle.
      }
    }
    cmd.add("-DPROCESSING_EXPORT");


    // main() is now emitted inside Sketch_run.cpp -- don't link main.cpp separately

    // For Windows: pre-compile icon resource and add to link
    if (t.isWindows && iconPath != null && !iconPath.isEmpty()
        && new File(iconPath).exists()) {
      File resFile = compileIconResource(iconPath, dir, t);
      if (resFile != null) cmd.add(resFile.getAbsolutePath());
    }

    if (t.isWindows)      addWinFlags(cmd);
    else if (t==ExportTarget.MACOS_X64 || t==ExportTarget.MACOS_ARM64) addMacFlags(cmd);
    else                  addLinuxFlags(cmd);

    cmd.add("-o"); cmd.add(binOut.getAbsolutePath());

    System.out.println("$ " + String.join(" ", cmd));
    if (lpExport) {
      cmd.add("-L" + lpLibDir.getAbsolutePath());
      cmd.add("-Wl,-rpath,$ORIGIN");
      cmd.add("-lprocessing");
      dir.mkdirs();
      copyFile(new File(lpLibDir, CppBuild.lpLibName()), new File(dir, CppBuild.lpLibName()));
    }
    ProcessBuilder pb = new ProcessBuilder(cmd);
    pb.redirectErrorStream(true);
    pb.directory(sketch.getFolder());
    Process p = pb.start();
    StringBuilder log = new StringBuilder();
    try (java.io.BufferedReader br = new java.io.BufferedReader(
        new java.io.InputStreamReader(p.getInputStream()))) {
      String line; while ((line=br.readLine())!=null) { System.out.println(line); log.append(line).append("\n"); }
    }
    if (p.waitFor() != 0) throw new Exception(log.substring(0, Math.min(400, log.length())));

    // Post-build
    if (t.isWindows) { mkBat(dir, binName); } // no DLLs needed - statically linked
    else if (t==ExportTarget.MACOS_X64 || t==ExportTarget.MACOS_ARM64) mkAppBundle(dir, skName, binOut, iconPath);
    else {
      // Try to produce an AppImage (single-file, shows icon in Dolphin)
      boolean madeAppImage = mkAppImage(dir, binName, skName, iconPath, listener);
      if (!madeAppImage) mkSh(dir, binName); // fallback to run.sh
    }

    // Copy data/
    File data = new File(sketch.getFolder(), "data");
    if (data.exists()) copyDir(data, new File(dir, "data"));

    // Copy font
    File fontSrc = new File(runtimeDir.getParentFile(), "fonts/ProcessingSansPro-Regular.ttf");
    if (fontSrc.exists()) copyFile(fontSrc, new File(dir, "ProcessingSansPro-Regular.ttf"));
  }

  private void addLinuxFlags(java.util.List<String> cmd) {
    // Use static GLFW/GLEW if available, otherwise dynamic
    boolean staticGlfw = new java.io.File("/usr/lib/libglfw3.a").exists()
                      || new java.io.File("/usr/local/lib/libglfw3.a").exists();
    boolean staticGlew = new java.io.File("/usr/lib/libGLEW.a").exists()
                      || new java.io.File("/usr/local/lib/libGLEW.a").exists();
    if (staticGlfw && staticGlew) {
      cmd.add("-Wl,-Bstatic"); cmd.add("-lglfw3"); cmd.add("-lGLEW");
      cmd.add("-Wl,-Bdynamic");
    } else {
      cmd.add("-lglfw"); cmd.add("-lGLEW");
    }
    cmd.add("-lGL"); cmd.add("-lm"); cmd.add("-ldl"); cmd.add("-lpthread");
    cmd.add("-lX11"); cmd.add("-lXrandr"); cmd.add("-lXi");
    cmd.add("-lXxf86vm"); cmd.add("-lXinerama"); cmd.add("-lXcursor");
    cmd.add("-static-libgcc"); cmd.add("-static-libstdc++");
  }

  private void addWinFlags(java.util.List<String> cmd) {
    String os = System.getProperty("os.name","").toLowerCase();
    String[] roots;
    if (os.contains("win")) {
      // On Windows, use MSYS2 mingw64 paths
      roots = new String[]{
        "C:\\msys64\\mingw64",
        "C:\\msys2\\mingw64",
        System.getProperty("user.home") + "\\msys64\\mingw64"
      };
    } else {
      // On Linux, use cross-compile sysroot
      roots = new String[]{
        "/usr/x86_64-w64-mingw32",
        "/usr/local/x86_64-w64-mingw32",
        "/opt/mingw-w64/x86_64-w64-mingw32"
      };
    }
    String root = roots[0];
    for (String r : roots) if (new File(r+(os.contains("win")?"\\lib":"/lib")).exists()) { root=r; break; }
    String sep = os.contains("win") ? "\\" : "/";

    // Include paths - add all that exist
    for (String inc : new String[]{root+sep+"include", root+sep+"include"+sep+"GL"})
      if (new File(inc).exists()) cmd.add("-I"+inc);

    // Lib path
    cmd.add("-L"+root+sep+"lib");

    // Also try bin dir for DLL import libs
    if (new File(root+sep+"bin").exists()) cmd.add("-L"+root+sep+"bin");

    // Link libraries - DLLs shipped alongside exe
    cmd.add("-lglfw3");
    cmd.add("-lglew32");
    cmd.add("-lopengl32");
    cmd.add("-lgdi32");
    cmd.add("-luser32");
    cmd.add("-lshell32");
    cmd.add("-lwinmm");
    cmd.add("-lole32");
    cmd.add("-luuid");
    cmd.add("-mwindows");       // WinMain = no console terminal
    cmd.add("-static-libgcc");
    cmd.add("-static-libstdc++");
  }

  private void addMacFlags(java.util.List<String> cmd) {
    String[] roots = {
      System.getProperty("user.home")+"/osxcross/target/SDK/MacOSX14.0.sdk",
      "/opt/osxcross/target/SDK/MacOSX14.0.sdk"
    };
    for (String r : roots) if (new File(r).exists()) { cmd.add("--sysroot="+r); break; }
    cmd.add("-framework"); cmd.add("OpenGL");
    cmd.add("-framework"); cmd.add("Cocoa");
    cmd.add("-framework"); cmd.add("IOKit");
    cmd.add("-framework"); cmd.add("CoreVideo");
    cmd.add("-lglfw"); cmd.add("-lGLEW");
    cmd.add("-mmacosx-version-min=11.0");
  }

  private void copyWinDlls(File dir) {
    String[] dlls = {"glfw3.dll","glew32.dll","libgcc_s_seh-1.dll",
                     "libstdc++-6.dll","libwinpthread-1.dll"};
    // Bundled DLLs ship with CppMode - use these first (always available)
    File bundledLibs = new File(runtimeDir.getParentFile(), "libs/windows-x64");
    // Fallback: system mingw installation
    String[] searchRoots = {
      bundledLibs.getAbsolutePath(),
      "/usr/x86_64-w64-mingw32/bin",
      "/usr/x86_64-w64-mingw32/lib",
      "/usr/lib/gcc/x86_64-w64-mingw32"
    };
    for (String dll : dlls) {
      File found = findFile(dll, searchRoots);
      if (found != null) {
        try { copyFile(found, new File(dir, dll)); System.out.println("[dll] copied " + dll); }
        catch (Exception e) { System.err.println("[dll] failed " + dll + ": " + e.getMessage()); }
      } else {
        System.err.println("[dll] WARNING: not found: " + dll);
      }
    }
  }

  private File findFile(String name, String[] roots) {
    for (String root : roots) {
      File f = new File(root, name);
      if (f.exists()) return f;
      // Search one level deep
      File dir = new File(root);
      if (dir.exists() && dir.listFiles() != null) {
        for (File sub : dir.listFiles()) {
          if (sub.isDirectory()) {
            File f2 = new File(sub, name);
            if (f2.exists()) return f2;
            // One more level
            if (sub.listFiles() != null) {
              for (File sub2 : sub.listFiles()) {
                if (sub2.isDirectory()) {
                  File f3 = new File(sub2, name);
                  if (f3.exists()) return f3;
                }
              }
            }
          }
        }
      }
    }
    return null;
  }

  private void mkBat(File dir, String bin) throws Exception {
    String skName = bin.replace(".exe","");
    try (PrintWriter pw = new PrintWriter(new File(dir,"run.bat"))) {
      pw.println("@echo off");
      pw.println("cd /d \"%~dp0\"");
      pw.println("set PROCESSING_SKETCH_NAME=" + skName);
      pw.println("set PROCESSING_SKETCH_PATH=%CD%");
      pw.println("start \"\" \"" + bin + "\"");
    }
    try (PrintWriter pw = new PrintWriter(new File(dir,"launch.vbs"))) {
      pw.println("Set sh = CreateObject(\"WScript.Shell\")");
      pw.println("sh.CurrentDirectory = CreateObject(\"Scripting.FileSystemObject\").GetParentFolderName(WScript.ScriptFullName)");
      pw.println("sh.Run Chr(34) & sh.CurrentDirectory & \"\\\\" + bin + "\" & Chr(34), 0, False");
    }
  }

  private void mkSh(File dir, String bin) throws Exception {
    String skName = bin;
    File sh = new File(dir,"run.sh");
    try (PrintWriter pw = new PrintWriter(sh)) {
      pw.println("#!/bin/bash");
      pw.println("cd \"$(dirname \"$0\")\"");
      pw.println("export PROCESSING_SKETCH_NAME=\"" + skName + "\"");
      pw.println("export PROCESSING_SKETCH_PATH=\"$(pwd)\"");
      pw.println("./" + bin + " \"$@\"");
    }
    sh.setExecutable(true);
    new File(dir,bin).setExecutable(true);
  }

  private void mkAppBundle(File dir, String name, File bin, String iconPath) throws Exception {
    File app = new File(dir, name + ".app");
    File macosDir = new File(app, "Contents/MacOS"); macosDir.mkdirs();
    File res = new File(app, "Contents/Resources"); res.mkdirs();
    File bundleBin = new File(macosDir, name);
    if (bin.exists()) { bin.renameTo(bundleBin); bundleBin.setExecutable(true); }
    try (PrintWriter pw = new PrintWriter(new File(app, "Contents/Info.plist"))) {
      pw.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
      pw.println("<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">");
      pw.println("<plist version=\"1.0\"><dict>");
      pw.println("  <key>CFBundleName</key><string>" + name + "</string>");
      pw.println("  <key>CFBundleExecutable</key><string>" + name + "</string>");
      pw.println("  <key>CFBundleIdentifier</key><string>processing.cpp." + name + "</string>");
      pw.println("  <key>CFBundleVersion</key><string>1.0</string>");
      pw.println("  <key>CFBundlePackageType</key><string>APPL</string>");
      pw.println("  <key>NSHighResolutionCapable</key><true/>");
      if (iconPath != null && !iconPath.isEmpty() && new File(iconPath).exists()) {
        pw.println("  <key>CFBundleIconFile</key><string>" + name + "</string>");
        try { copyFile(new File(iconPath), new File(res, name + ".png")); } catch (Exception ignored) {}
      }
      pw.println("</dict></plist>");
    }
    // run.sh inside bundle
    File sh = new File(macosDir, "run.sh");
    try (PrintWriter pw = new PrintWriter(sh)) {
      pw.println("#!/bin/bash");
      pw.println("cd \"$(dirname \"$0\")\"");
      pw.println("export PROCESSING_SKETCH_NAME=\"" + name + "\"");
      pw.println("./" + name + " \"$@\"");
    }
    sh.setExecutable(true);
  }

  private void embedIcon(File bin, ExportTarget t, String iconPng, File outDir) {
    try {
      if (t.isWindows) embedIconWindows(bin, iconPng, outDir);
      else if (t == ExportTarget.LINUX_X64 || t == ExportTarget.LINUX_ARM64
            || t == ExportTarget.LINUX_ARM32) {
        // Copy icon.png next to binary for Linux
        copyFile(new File(iconPng), new File(outDir, "icon.png"));
        // Create .desktop file
        File desktop = new File(outDir, bin.getName() + ".desktop");
        try (PrintWriter pw = new PrintWriter(desktop)) {
          pw.println("[Desktop Entry]");
          pw.println("Type=Application");
          pw.println("Name=" + bin.getName());
          pw.println("Exec=" + bin.getAbsolutePath());
          pw.println("Icon=" + new File(outDir, "icon.png").getAbsolutePath());
          pw.println("Terminal=false");
        }
      }
    } catch (Exception e) { System.err.println("[icon] " + e.getMessage()); }
  }

  private void embedIconWindows(File exe, String iconPng, File outDir) throws Exception {
    // Convert PNG -> ICO using ImageMagick
    File icoFile = new File(outDir, "_icon.ico");
    String convert = System.getProperty("os.name","").toLowerCase().contains("win") ? "magick" : "convert";
    if (cmdExists(convert)) {
      new ProcessBuilder(convert, iconPng,
        "-define", "icon:auto-resize=256,128,64,48,32,16",
        icoFile.getAbsolutePath())
        .redirectErrorStream(true).start().waitFor();
    }
    if (!icoFile.exists()) {
      System.err.println("[icon] ICO conversion failed - copying PNG alongside exe");
      copyFile(new File(iconPng), new File(outDir, "icon.png"));
      return;
    }
    // Create .rc and compile with windres
    File rcFile = new File(outDir, "_icon.rc");
    try (PrintWriter pw = new PrintWriter(rcFile)) {
      pw.println("1 ICON \"" + icoFile.getAbsolutePath().replace("\\", "/") + "\"");
    }
    String windres = cmdExists("x86_64-w64-mingw32-windres")
      ? "x86_64-w64-mingw32-windres" : "windres";
    File resFile = new File(outDir, "_icon.res");
    int rc = new ProcessBuilder(windres, rcFile.getAbsolutePath(),
      "-O", "coff", "-o", resFile.getAbsolutePath())
      .redirectErrorStream(true).start().waitFor();
    // Clean up temp files; copy ico alongside exe regardless
    copyFile(icoFile, new File(outDir, exe.getName().replace(".exe","") + ".ico"));
    icoFile.delete(); rcFile.delete(); resFile.delete();
    System.out.println("[icon] Windows icon created alongside exe");
  }

  private void installExportDeps(java.util.List<ExportTarget> targets, RunnerListener listener) {
    boolean needsWin = targets.stream().anyMatch(t -> t.isWindows);
    boolean needsLinux = targets.stream().anyMatch(t ->
      t==ExportTarget.LINUX_X64||t==ExportTarget.LINUX_ARM64||t==ExportTarget.LINUX_ARM32);
    if (needsWin) {
      boolean glfwOk = new File("/usr/x86_64-w64-mingw32/lib/libglfw3.a").exists()
                    || new File("/usr/x86_64-w64-mingw32/lib/libglfw3dll.a").exists();
      boolean glewOk = new File("/usr/x86_64-w64-mingw32/include/GL/glew.h").exists();
      if (!glfwOk || !glewOk) {
        listener.statusNotice("Installing Windows cross-compile libs...");
        try {
          String cmd = cmdExists("yay")
            ? "yay -S --noconfirm --needed mingw-w64-glfw mingw-w64-glew"
            : "pkexec pacman -S --noconfirm --needed mingw-w64-glfw mingw-w64-glew";
          new ProcessBuilder("bash","-c",cmd).inheritIO().start().waitFor();
        } catch (Exception ignored) {}
      }
    }
    if (needsLinux) {
      boolean glfwOk = new File("/usr/lib/libglfw3.a").exists()
                    || new File("/usr/lib/libglfw.so").exists();
      if (!glfwOk) {
        listener.statusNotice("Installing Linux native libs...");
        try {
          String cmd = cmdExists("pacman")
            ? "pkexec pacman -S --noconfirm --needed glfw-x11 glew"
            : "pkexec apt-get install -y libglfw3-dev libglew-dev";
          new ProcessBuilder("bash","-c",cmd).inheritIO().start().waitFor();
        } catch (Exception ignored) {}
      }
    }
  }

  private void copyFile(File src, File dst) throws Exception {
    dst.getParentFile().mkdirs();
    Files.copy(src.toPath(), dst.toPath(), java.nio.file.StandardCopyOption.REPLACE_EXISTING);
  }

  private void copyDir(File src, File dst) throws Exception {
    dst.mkdirs();
    File[] files = src.listFiles();
    if (files == null) return;
    for (File f : files) {
      if (f.isDirectory()) copyDir(f, new File(dst, f.getName()));
      else copyFile(f, new File(dst, f.getName()));
    }
  }

  private boolean mkAppImage(File dir, String binName, String skName,
                             String iconPath, RunnerListener listener) {
    try {
      File tools = new File(runtimeDir.getParentFile(), "tools");
      File appImageTool = new File(tools, "appimagetool-x86_64.AppImage");
      if (!appImageTool.exists()) {
        System.err.println("[appimage] appimagetool not found");
        return false;
      }

      // Build AppDir
      File appDir = new File(dir, skName + ".AppDir");
      File usrBin = new File(appDir, "usr/bin");
      usrBin.mkdirs();

      // Copy binary
      File binary = new File(dir, binName);
      File bundledBin = new File(usrBin, skName);
      copyFile(binary, bundledBin);
      // cppmode-lpexport: libprocessing sits next to the binary inside the AppImage too ($ORIGIN rpath)
      File lpLib = new File(dir, CppBuild.lpLibName());
      if (lpLib.exists()) copyFile(lpLib, new File(usrBin, lpLib.getName()));
      bundledBin.setExecutable(true);

      // Copy data/ and font
      File dataDir = new File(dir, "data");
      if (dataDir.exists()) copyDir(dataDir, new File(usrBin, "data"));
      File font = new File(dir, "ProcessingSansPro-Regular.ttf");
      if (font.exists()) copyFile(font, new File(usrBin, "ProcessingSansPro-Regular.ttf"));

      // Icon - use skName directly, no dashes (Dolphin is picky)
      String iconName = skName;
      File iconDst = new File(appDir, iconName + ".png");
      if (iconPath != null && !iconPath.isEmpty() && new File(iconPath).exists()) {
        copyFile(new File(iconPath), iconDst);
      } else if (cmdExists("convert")) {
        new ProcessBuilder("convert", "-size", "256x256", "xc:#3366cc",
          iconDst.getAbsolutePath()).redirectErrorStream(true).start().waitFor();
      }
      // .DirIcon is required for Dolphin/Nautilus to show the icon
      if (iconDst.exists()) copyFile(iconDst, new File(appDir, ".DirIcon"));

      // .desktop file
      File desktop = new File(appDir, skName + ".desktop");
      try (PrintWriter pw = new PrintWriter(desktop)) {
        pw.println("[Desktop Entry]");
        pw.println("Version=1.0");
        pw.println("Type=Application");
        pw.println("Name=" + skName);
        pw.println("Comment=Made with Processing C++ Mode");
        pw.println("Exec=" + skName);
        pw.println("Icon=" + iconName);
        pw.println("Terminal=false");
        pw.println("Categories=Application;");
      }

      // AppRun - write via Files.write to avoid quote escaping issues
      File appRun = new File(appDir, "AppRun");
      String appRunContent = "#!/bin/bash\n"
        + "SELF=$(readlink -f \"$0\")\n"
        + "HERE=$(dirname \"$SELF\")\n"
        + "export PROCESSING_SKETCH_NAME=" + skName + "\n"
        + "export PROCESSING_SKETCH_PATH=\"$HERE/usr/bin\"\n"
        + "cd \"$HERE/usr/bin\"\n"
        + "exec \"$HERE/usr/bin/" + skName + "\" \"$@\"\n";
      Files.write(appRun.toPath(), appRunContent.getBytes());
      appRun.setExecutable(true);

      // Run appimagetool
      listener.statusNotice("Packaging AppImage...");
      File appImageOut = new File(dir, skName + "-x86_64.AppImage");
      ProcessBuilder pb = new ProcessBuilder(
        appImageTool.getAbsolutePath(), "--no-appstream",
        appDir.getAbsolutePath(), appImageOut.getAbsolutePath());
      pb.environment().put("ARCH", "x86_64");
      pb.redirectErrorStream(true);
      StringBuilder log = new StringBuilder();
      Process p = pb.start();
      try (java.io.BufferedReader br = new java.io.BufferedReader(
          new java.io.InputStreamReader(p.getInputStream()))) {
        String line;
        while ((line = br.readLine()) != null) {
          System.out.println("[appimage] " + line);
          log.append(line).append('\n');
        }
      }
      int exit = p.waitFor();
      if (exit != 0 || !appImageOut.exists()) {
        System.err.println("[appimage] failed:\n" + log);
        return false;
      }
      appImageOut.setExecutable(true);
      binary.delete();
      deleteDir(appDir);

      // Create a .desktop wrapper for Dolphin/file managers
      // This avoids display rotation issues when launching AppImage from file manager
      File desktopLauncher = new File(dir, skName + ".desktop");
      String iconPath2 = (iconPath != null && !iconPath.isEmpty() && new File(iconPath).exists())
        ? iconPath : "";
      try (PrintWriter pw = new PrintWriter(desktopLauncher)) {
        pw.println("[Desktop Entry]");
        pw.println("Version=1.0");
        pw.println("Type=Application");
        pw.println("Name=" + skName);
        pw.println("Comment=Made with Processing C++ Mode");
        pw.println("Exec=bash -c \"cd $(dirname %k) && ./" + appImageOut.getName() + "\"");
        pw.println("Icon=" + (iconPath2.isEmpty() ? "application-x-executable" : iconPath2));
        pw.println("Terminal=false");
        pw.println("Categories=Application;");
        pw.println("StartupNotify=true");
      }
      desktopLauncher.setExecutable(true);
      System.out.println("[appimage] created: " + appImageOut.getName());
      return true;
    } catch (Exception e) {
      System.err.println("[appimage] " + e.getMessage());
      return false;
    }
  }

  private void deleteDir(File d) {
    if (!d.exists()) return;
    File[] fs = d.listFiles();
    if (fs != null) for (File f : fs) { if (f.isDirectory()) deleteDir(f); else f.delete(); }
    d.delete();
  }




  // Compile PNG icon into a Windows .res file for linking
  private File compileIconResource(String iconPng, File outDir, ExportTarget t) {
    try {
      String os = System.getProperty("os.name","").toLowerCase();
      boolean onWin = os.contains("win");

      // PNG -> ICO using ImageMagick (magick on Windows, convert on Linux)
      File icoFile = new File(outDir, "_icon_tmp.ico");
      String imageMagick = onWin ? "magick" : "convert";

      // On Windows, also try full MSYS2 path
      if (onWin && !cmdExists("magick")) {
        String[] magickPaths = {
          "C:\\msys64\\usr\\bin\\magick.exe",
          "C:\\msys64\\mingw64\\bin\\magick.exe",
          "C:\\Program Files\\ImageMagick-7.0.0-Q16\\magick.exe"
        };
        for (String p : magickPaths)
          if (new File(p).exists()) { imageMagick = p; break; }
      }

      if (!cmdExists(imageMagick) && !new File(imageMagick).exists()) {
        System.err.println("[icon] ImageMagick not found - skipping icon embed");
        System.err.println("[icon] Install ImageMagick: https://imagemagick.org/script/download.php#windows");
        return null;
      }

      int rc = new ProcessBuilder(imageMagick, iconPng,
        "-define", "icon:auto-resize=256,128,64,48,32,16",
        icoFile.getAbsolutePath())
        .redirectErrorStream(true).start().waitFor();

      if (rc != 0 || !icoFile.exists()) {
        System.err.println("[icon] PNG->ICO conversion failed");
        return null;
      }

      // .rc file - use forward slashes for windres
      File rcFile = new File(outDir, "_icon_tmp.rc");
      String icoPath = icoFile.getAbsolutePath().replace("\\", "/");
      String rcContent = "1 ICON \"" + icoPath + "\"\n";
      Files.write(rcFile.toPath(), rcContent.getBytes());

      // Find windres - on Windows use MSYS2 windres, on Linux use mingw windres
      String windres;
      if (onWin) {
        windres = "C:\\msys64\\mingw64\\bin\\windres.exe";
        if (!new File(windres).exists())
          windres = "C:\\msys2\\mingw64\\bin\\windres.exe";
        if (!new File(windres).exists())
          windres = "windres"; // fallback to PATH
      } else {
        windres = cmdExists("x86_64-w64-mingw32-windres")
          ? "x86_64-w64-mingw32-windres" : "windres";
      }

      File resFile = new File(outDir, "_icon_tmp.res");
      StringBuilder windresLog = new StringBuilder();
      ProcessBuilder wpb = new ProcessBuilder(windres,
        rcFile.getAbsolutePath(), "-O", "coff", "-o", resFile.getAbsolutePath());
      wpb.redirectErrorStream(true);
      Process wp = wpb.start();
      try (java.io.BufferedReader br = new java.io.BufferedReader(
          new java.io.InputStreamReader(wp.getInputStream()))) {
        String line; while ((line=br.readLine())!=null) windresLog.append(line).append('\n');
      }
      int rc2 = wp.waitFor();
      icoFile.delete(); rcFile.delete();

      if (rc2 != 0 || !resFile.exists()) {
        System.err.println("[icon] windres failed: " + windresLog);
        return null;
      }
      System.out.println("[icon] Windows icon resource compiled successfully");
      return resFile;
    } catch (Exception e) {
      System.err.println("[icon] compileIconResource: " + e.getMessage());
      return null;
    }
  }


  // ââ Musl cross-compiler toolchains (pre-built, no system install needed) ââ

  private static final String MUSL_BASE = "https://musl.cc/";

  private static final java.util.Map<ExportTarget, String[]> TOOLCHAIN_INFO =
    new java.util.LinkedHashMap<ExportTarget, String[]>() {{
      put(ExportTarget.LINUX_X64,   new String[]{
        "x86_64-linux-musl-cross.tgz",
        "x86_64-linux-musl-cross/bin/x86_64-linux-musl-g++"});
      put(ExportTarget.LINUX_ARM64, new String[]{
        "aarch64-linux-musl-cross.tgz",
        "aarch64-linux-musl-cross/bin/aarch64-linux-musl-g++"});
      put(ExportTarget.LINUX_ARM32, new String[]{
        "armv7l-linux-musleabihf-cross.tgz",
        "armv7l-linux-musleabihf-cross/bin/armv7l-linux-musleabihf-g++"});
    }};

  // Get the bundled musl toolchain compiler path for a target
  private File getMuslCompiler(ExportTarget t) {
    String[] info = TOOLCHAIN_INFO.get(t);
    if (info == null) return null;
    File toolsDir = new File(runtimeDir.getParentFile(), "tools/cross");
    File compiler = new File(toolsDir, info[1]);
    return compiler.exists() ? compiler : null;
  }

  // Check if musl toolchain is installed for target
  private boolean muslToolchainInstalled(ExportTarget t) {
    return getMuslCompiler(t) != null;
  }

  // Download and extract musl toolchain with progress dialog
  private boolean downloadMuslToolchain(ExportTarget t, RunnerListener listener) {
    String[] info = TOOLCHAIN_INFO.get(t);
    if (info == null) return false;

    String tgzName = info[0];
    String url = MUSL_BASE + tgzName;
    File toolsDir = new File(runtimeDir.getParentFile(), "tools/cross");
    toolsDir.mkdirs();
    File tgzFile = new File(toolsDir, tgzName);

    // Progress dialog
    javax.swing.JDialog prog = new javax.swing.JDialog((java.awt.Frame)null,
      "Downloading " + t.label + " toolchain", false);
    javax.swing.JProgressBar bar = new javax.swing.JProgressBar(0, 100);
    bar.setStringPainted(true);
    bar.setString("Connecting...");
    javax.swing.JLabel lbl = new javax.swing.JLabel(
      "  Downloading from musl.cc (~100MB)  ");
    lbl.setBorder(javax.swing.BorderFactory.createEmptyBorder(8,12,4,12));
    prog.setLayout(new java.awt.BorderLayout());
    prog.add(lbl, java.awt.BorderLayout.NORTH);
    prog.add(bar, java.awt.BorderLayout.CENTER);
    prog.pack();
    prog.setMinimumSize(new java.awt.Dimension(400, prog.getHeight()));
    prog.setLocationRelativeTo(null);
    prog.setVisible(true);

    try {
      // Download with progress
      java.net.URL urlObj = new java.net.URL(url);
      java.net.HttpURLConnection conn =
        (java.net.HttpURLConnection) urlObj.openConnection();
      conn.setConnectTimeout(15000);
      conn.setReadTimeout(60000);
      long total = conn.getContentLengthLong();
      listener.statusNotice("Downloading " + tgzName + "...");

      try (java.io.InputStream in = conn.getInputStream();
           java.io.FileOutputStream out = new java.io.FileOutputStream(tgzFile)) {
        byte[] buf = new byte[65536];
        long downloaded = 0;
        int n;
        while ((n = in.read(buf)) > 0) {
          out.write(buf, 0, n);
          downloaded += n;
          if (total > 0) {
            final int pct = (int)(downloaded * 100 / total);
            final String mb = String.format("%.1f / %.1f MB",
              downloaded/1048576.0, total/1048576.0);
            javax.swing.SwingUtilities.invokeLater(() -> {
              bar.setValue(pct);
              bar.setString(mb);
            });
          }
        }
      }
      conn.disconnect();

      // Extract
      bar.setString("Extracting...");
      bar.setIndeterminate(true);
      listener.statusNotice("Extracting " + tgzName + "...");

      // Use tar command (available on Linux, Mac, and Windows 10+)
      String os = System.getProperty("os.name","").toLowerCase();
      ProcessBuilder pb;
      if (os.contains("win")) {
        pb = new ProcessBuilder("tar", "-xzf",
          tgzFile.getAbsolutePath(), "-C", toolsDir.getAbsolutePath());
      } else {
        pb = new ProcessBuilder("tar", "-xzf",
          tgzFile.getAbsolutePath(), "-C", toolsDir.getAbsolutePath());
      }
      pb.redirectErrorStream(true);
      int exitCode = pb.start().waitFor();

      tgzFile.delete(); // clean up archive after extraction
      prog.dispose();

      if (exitCode != 0 || !muslToolchainInstalled(t)) {
        listener.statusError("Toolchain extraction failed");
        return false;
      }

      // Make compiler executable (Linux/Mac)
      File compiler = getMuslCompiler(t);
      if (compiler != null) {
        compiler.setExecutable(true);
        // Make all binaries in bin/ executable
        File binDir = compiler.getParentFile();
        if (binDir.listFiles() != null)
          for (File f : binDir.listFiles()) f.setExecutable(true);
      }

      listener.statusNotice(t.label + " toolchain ready");
      return true;

    } catch (Exception e) {
      prog.dispose();
      System.err.println("[toolchain] download failed: " + e.getMessage());
      javax.swing.SwingUtilities.invokeLater(() ->
        javax.swing.JOptionPane.showMessageDialog(null,
          "<html><b>Download failed:</b><br>" + e.getMessage() + "<br><br>" +
          "Check your internet connection and try again.</html>",
          "Download Failed", javax.swing.JOptionPane.ERROR_MESSAGE));
      if (tgzFile.exists()) tgzFile.delete();
      return false;
    }
  }


  public static String rewriteGccError(String line) {
    line = line.replace("std::__cxx11::basic_string<char>", "String");
    line = line.replace("std::basic_string<char>", "String");
    line = line.replace("‘std::__cxx11::basic_string<char>’", "String");
    line = line.replace("‘std::basic_string<char>’", "String");
    line = line.replace("{aka ‘std::__cxx11::basic_string<char>’}", "");
    if (line.contains("std::strong_ordering") || line.contains("std::weak_ordering") || line.contains("std::partial_ordering"))
      line = line.replaceAll("std::(strong|weak|partial)_ordering", "comparison_result") + " — note: <=> returns a comparison result, not an int; use <, >, or == instead";
    line = line.replace("Processing::Sketch::", "");
    line = line.replace("Processing::PApplet::", "");
    line = line.replace("Processing::", "");
    if (line.contains("return-statement with a value") && line.contains("returning"))
      return line.replaceAll("return-statement with a value.*", "cannot return a value from a void function");
    if (line.contains("too few arguments to function"))
      return line.replaceAll("too few arguments to function .(.+?).", "too few arguments to $1");
    return line;
  }


}
