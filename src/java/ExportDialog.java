package processing.mode.cpp;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.GridLayout;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.BiFunction;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

/**
 * The Export Application dialog, built like Processing's own (Java mode's):
 * a line of explanation, a checkbox per platform, and Export / Cancel. It
 * uses plain Swing components, so it takes the IDE's look and theme. Adds a
 * renderer menu, preset to the toolbar's choice.
 */
final class ExportDialog {
  private ExportDialog() {}

  /** A platform the export can be made for. */
  enum Target {
    WINDOWS_X64("windows-amd64", "Windows", "windows-x86_64", "windows-x86-64"),
    MACOS_X64("macos-x86_64", "macOS (Intel 64-bit)", "macos-x86_64", "macos-x86-64"),
    MACOS_ARM64("macos-aarch64", "macOS (Apple Silicon)", "macos-aarch64", "macos-arm64"),
    LINUX_X64("linux-amd64", "Linux (64-bit)", "linux-x86_64", "linux-x86-64"),
    LINUX_ARM64("linux-aarch64", "Linux (ARM 64-bit)", "linux-aarch64", "linux-arm64"),
    /** 32-bit Raspberry Pi; only C++ Mode builds for it */
    LINUX_ARM32("linux-arm", "Linux (ARM 32-bit)", "linux-arm", "linux-arm");

    /** the export folder, Processing's names */
    final String folder;
    final String label;
    /** Lua Mode's runner folder name */
    final String lua;
    /** Rust Mode's cache folder name */
    final String rust;

    Target(String folder, String label, String lua, String rust) {
      this.folder = folder; this.label = label; this.lua = lua; this.rust = rust;
    }

    boolean windows() { return this == WINDOWS_X64; }
    boolean mac() { return this == MACOS_X64 || this == MACOS_ARM64; }

    /** The computer the IDE runs on. */
    static Target here() {
      String os = System.getProperty("os.name", "").toLowerCase();
      String arch = System.getProperty("os.arch", "").toLowerCase();
      boolean arm = arch.contains("aarch64") || arch.contains("arm64");
      if (os.contains("win")) return WINDOWS_X64;
      if (os.contains("mac")) return arm ? MACOS_ARM64 : MACOS_X64;
      return arm ? LINUX_ARM64 : LINUX_X64;
    }
  }

  /** For a greyed-out platform whose missing piece can be installed from the dialog. */
  interface Installer {
    boolean can(Target t, String renderer);
    /** installs off the Swing thread; returns when done */
    void install(Target t);
  }

  /** What was picked. */
  static final class Choice {
    final List<Target> targets;
    final String renderer;
    /** the PNG for the app icon, or null */
    final java.io.File icon;
    /** Linux exports also as one AppImage file */
    final boolean appImage;
    /** where the platform folders go (the sketch folder unless changed) */
    final java.io.File outDir;
    Choice(List<Target> targets, String renderer, java.io.File icon, boolean appImage, java.io.File outDir) {
      this.targets = targets; this.renderer = renderer; this.icon = icon; this.appImage = appImage; this.outDir = outDir;
    }
  }

  /**
   * @param renderers  the renderer names, as in the toolbar menu
   * @param current    the toolbar's renderer
   * @param why        for a target and renderer, why it can't be exported
   *                   (shown next to its greyed-out checkbox), or null if it can
   * @return the choice, or null when cancelled or nothing was picked
   */
  static Choice show(Component parent, java.io.File sketchFolder, String[] renderers, String current,
                     BiFunction<Target, String, String> why) {
    return show(parent, sketchFolder, renderers, current, why, null);
  }

  /** The platforms every mode exports for (not the 32-bit Raspberry Pi). */
  static final Target[] COMMON = { Target.WINDOWS_X64, Target.MACOS_X64, Target.MACOS_ARM64, Target.LINUX_X64, Target.LINUX_ARM64 };

  static Choice show(Component parent, java.io.File sketchFolder, String[] renderers, String current,
                     BiFunction<Target, String, String> why, Installer installer) {
    return show(parent, sketchFolder, renderers, current, why, installer, COMMON);
  }

  static Choice show(Component parent, java.io.File sketchFolder, String[] renderers, String current,
                     BiFunction<Target, String, String> why, Installer installer, Target[] shown) {
    JPanel panel = new JPanel();
    panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));

    JLabel intro = new JLabel("<html><body style='width: 300px'>Export to Application creates "
      + "double-clickable, standalone applications for the selected platforms.</body></html>");
    intro.setAlignmentX(Component.LEFT_ALIGNMENT);
    panel.add(intro);
    panel.add(Box.createVerticalStrut(12));

    JPanel platforms = new JPanel(new GridLayout(0, 1, 0, 2));
    platforms.setBorder(BorderFactory.createTitledBorder("Platforms"));
    platforms.setAlignmentX(Component.LEFT_ALIGNMENT);
    Map<Target, JCheckBox> boxes = new LinkedHashMap<>();
    Map<Target, JLabel> notes = new LinkedHashMap<>();
    Map<Target, javax.swing.JButton> installs = new LinkedHashMap<>();
    Runnable[] refreshRef = { null };
    JComboBox<String> rendererBox = new JComboBox<>(renderers);
    for (Target t : shown) {
      JPanel row = new JPanel(new BorderLayout(8, 0));
      JCheckBox box = new JCheckBox(t.label);
      JLabel note = new JLabel();
      note.setEnabled(false);
      note.setFont(note.getFont().deriveFont(note.getFont().getSize2D() - 1f));
      row.add(box, BorderLayout.WEST);
      row.add(note, BorderLayout.CENTER);
      if (installer != null) {
        javax.swing.JButton install = new javax.swing.JButton("Install");
        install.setVisible(false);
        install.addActionListener(e -> {
          install.setEnabled(false);
          install.setText("Installing...");
          new Thread(() -> {
            installer.install(t);
            javax.swing.SwingUtilities.invokeLater(() -> {
              install.setText("Install");
              install.setEnabled(true);
              refreshRef[0].run();
              if (boxes.get(t).isEnabled()) boxes.get(t).setSelected(true);
            });
          }, "export-install").start();
        });
        row.add(install, BorderLayout.EAST);
        installs.put(t, install);
      }
      platforms.add(row);
      boxes.put(t, box);
      notes.put(t, note);
    }
    panel.add(platforms);
    panel.add(Box.createVerticalStrut(10));

    JPanel rendererRow = new JPanel(new BorderLayout(8, 0));
    rendererRow.setAlignmentX(Component.LEFT_ALIGNMENT);
    rendererRow.add(new JLabel("Renderer"), BorderLayout.WEST);
    JComboBox<String> renderer = rendererBox;
    renderer.setSelectedItem(current);
    rendererRow.add(renderer, BorderLayout.CENTER);
    panel.add(rendererRow);
    panel.add(Box.createVerticalStrut(10));

    // the icon: a PNG, made into .icns, .ico and the Linux icon on export
    JPanel iconPanel = new JPanel(new BorderLayout(8, 0));
    iconPanel.setBorder(BorderFactory.createTitledBorder("Icon"));
    iconPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
    JLabel preview = new JLabel("none", javax.swing.SwingConstants.CENTER);
    preview.setPreferredSize(new java.awt.Dimension(48, 48));
    javax.swing.JTextField iconPath = new javax.swing.JTextField(18);
    iconPath.setEditable(false);
    Runnable showIcon = () -> {
      String p = iconPath.getText();
      javax.swing.ImageIcon ic = null;
      if (!p.isEmpty()) {
        try {
          java.awt.image.BufferedImage img = javax.imageio.ImageIO.read(new java.io.File(p));
          if (img != null) ic = new javax.swing.ImageIcon(AppIcons.scale(img, 44));
        } catch (Exception ignored) {}
      }
      preview.setIcon(ic);
      preview.setEnabled(ic != null);  // greyed "none" without one, the real colors with one
      preview.setText(ic == null ? (p.isEmpty() ? "none" : "?") : "");
    };
    for (String n : new String[] { "icon.png", "data/icon.png" }) {
      java.io.File f = new java.io.File(sketchFolder, n);
      if (f.isFile()) { iconPath.setText(f.getAbsolutePath()); break; }
    }
    showIcon.run();
    javax.swing.JButton browse = new javax.swing.JButton("Choose...");
    browse.addActionListener(e -> {
      javax.swing.JFileChooser fc = new javax.swing.JFileChooser(iconPath.getText().isEmpty()
        ? sketchFolder : new java.io.File(iconPath.getText()).getParentFile());
      fc.setDialogTitle("App icon (PNG, 512 x 512 or bigger looks best)");
      fc.setFileFilter(new javax.swing.filechooser.FileNameExtensionFilter("PNG images", "png"));
      if (fc.showOpenDialog(panel) == javax.swing.JFileChooser.APPROVE_OPTION) {
        iconPath.setText(fc.getSelectedFile().getAbsolutePath());
        showIcon.run();
      }
    });
    javax.swing.JButton clear = new javax.swing.JButton("None");
    clear.addActionListener(e -> { iconPath.setText(""); showIcon.run(); });
    JPanel iconButtons = new JPanel(new GridLayout(1, 2, 4, 0));
    iconButtons.add(browse);
    iconButtons.add(clear);
    JPanel iconRight = new JPanel(new BorderLayout(0, 4));
    iconRight.add(iconPath, BorderLayout.NORTH);
    iconRight.add(iconButtons, BorderLayout.SOUTH);
    iconPanel.add(preview, BorderLayout.WEST);
    iconPanel.add(iconRight, BorderLayout.CENTER);
    panel.add(iconPanel);
    panel.add(Box.createVerticalStrut(8));

    // where it goes: the sketch folder, as in Processing, unless picked
    JPanel outPanel = new JPanel(new BorderLayout(8, 0));
    outPanel.setBorder(BorderFactory.createTitledBorder("Export to"));
    outPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
    javax.swing.JTextField outPath = new javax.swing.JTextField(sketchFolder.getAbsolutePath(), 18);
    outPath.setEditable(false);
    javax.swing.JButton outChoose = new javax.swing.JButton("Choose...");
    outChoose.addActionListener(e -> {
      javax.swing.JFileChooser fc = new javax.swing.JFileChooser(new java.io.File(outPath.getText()));
      fc.setDialogTitle("Export to");
      fc.setFileSelectionMode(javax.swing.JFileChooser.DIRECTORIES_ONLY);
      if (fc.showOpenDialog(panel) == javax.swing.JFileChooser.APPROVE_OPTION)
        outPath.setText(fc.getSelectedFile().getAbsolutePath());
    });
    outPanel.add(outPath, BorderLayout.CENTER);
    outPanel.add(outChoose, BorderLayout.EAST);
    panel.add(outPanel);
    panel.add(Box.createVerticalStrut(8));

    JCheckBox appImage = new JCheckBox("Linux: also make an AppImage (one file that runs on most distributions)");
    appImage.setAlignmentX(Component.LEFT_ALIGNMENT);
    if (AppImage.possible()) appImage.setSelected(true);
    else {
      appImage.setEnabled(false);
      appImage.setToolTipText("AppImages are made on Linux");
    }
    panel.add(appImage);

    Target here = Target.here();
    Runnable refresh = () -> {
      String r = (String) renderer.getSelectedItem();
      for (Target t : shown) {
        String reason = why.apply(t, r);
        JCheckBox box = boxes.get(t);
        box.setEnabled(reason == null);
        if (reason != null) box.setSelected(false);
        notes.get(t).setText(reason == null ? "" : reason);
        javax.swing.JButton ib = installs.get(t);
        if (ib != null) ib.setVisible(reason != null && installer.can(t, r));
      }
      platforms.revalidate();
    };
    refreshRef[0] = refresh;
    refresh.run();
    if (boxes.containsKey(here) && boxes.get(here).isEnabled()) boxes.get(here).setSelected(true);
    renderer.addActionListener(e -> refresh.run());

    Object[] options = { "Export", "Cancel" };
    int answer = JOptionPane.showOptionDialog(parent, panel, "Export Application",
      JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE, null, options, options[0]);
    if (answer != 0) return null;
    List<Target> picked = new ArrayList<>();
    for (Map.Entry<Target, JCheckBox> e : boxes.entrySet())
      if (e.getValue().isSelected()) picked.add(e.getKey());
    if (picked.isEmpty()) return null;
    String ip = iconPath.getText();
    return new Choice(picked, (String) renderer.getSelectedItem(),
      ip.isEmpty() ? null : new java.io.File(ip), appImage.isSelected() && appImage.isEnabled(),
      new java.io.File(outPath.getText()));
  }
}
