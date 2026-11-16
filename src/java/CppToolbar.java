/* -*- mode: java; c-basic-offset: 2; indent-tabs-mode: nil -*- */

package processing.mode.cpp;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.prefs.Preferences;

import javax.swing.Box;
import javax.swing.JComboBox;

import processing.app.ui.Editor;
import processing.app.ui.EditorButton;
import processing.app.ui.EditorToolbar;


public class CppToolbar extends EditorToolbar {

  private static final String   PREF_NODE     = "processing/cppmode";
  private static final String   PREF_RENDERER = "cpp.renderer";
  private static final String[] RENDERERS     = {"OpenGL", "libprocessing"};

  private final CppEditor cppEditor;
  private JComboBox<String> rendererCombo;


  public CppToolbar(Editor editor) {
    super(editor);
    this.cppEditor = (CppEditor) editor;
  }


  @Override
  public void handleRun(int modifiers) {
    boolean present = (modifiers & ActionEvent.SHIFT_MASK) != 0;
    cppEditor.handleRun(present, null, null);
  }

  @Override
  public void handleStop() {
    cppEditor.handleStop();
  }


  @Override
  public List<EditorButton> createButtons() {
    List<EditorButton> buttons = new ArrayList<>();

    runButton = new EditorButton(this,
                                 "/lib/toolbar/run",
                                 "Run",
                                 "Run (Shift for fullscreen)") {
      @Override
      public void actionPerformed(ActionEvent e) {
        handleRun(e.getModifiers());
      }
    };
    buttons.add(runButton);

    stopButton = new EditorButton(this,
                                  "/lib/toolbar/stop",
                                  "Stop") {
      @Override
      public void actionPerformed(ActionEvent e) {
        handleStop();
      }
    };
    buttons.add(stopButton);

    return buttons;
  }
  @Override
  public void addModeButtons(Box box, javax.swing.JLabel label) {
    Preferences prefs = Preferences.userRoot().node(PREF_NODE);

    // Renderer selector -- to the left of the mode selector
    String savedRenderer = prefs.get(PREF_RENDERER, "OpenGL");
    rendererCombo = new JComboBox<>(RENDERERS);
    rendererCombo.setSelectedItem(savedRenderer);
    CppBuild.selectedRenderer = savedRenderer;
    rendererCombo.setMaximumSize(new Dimension(120, 26));
    rendererCombo.setToolTipText("Renderer");
    rendererCombo.setFocusable(false);
    rendererCombo.addActionListener(e -> {
      String sel = (String) rendererCombo.getSelectedItem();
      CppBuild.selectedRenderer = sel;
      if (sel != null) prefs.put(PREF_RENDERER, sel);
    });
    box.add(Box.createHorizontalStrut(8));
    box.add(rendererCombo);
  }

  public String getSelectedRenderer() {
    if (rendererCombo != null) return (String) rendererCombo.getSelectedItem();
    return Preferences.userRoot().node(PREF_NODE).get(PREF_RENDERER, "OpenGL");
  }
}
