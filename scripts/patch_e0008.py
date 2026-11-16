import pathlib, sys
P = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else "src/java/CppErrors.java"); s = P.read_text()
if "E0008" in s: print("E0008 already patched"); sys.exit(0)
rule_anchor = '    rule("PTR", "use of deleted function .*\\\\b(PGraphics|PImage|PShader)\\\\b", ""),\n'
rule_new = ('    // E0008: event/lifecycle function with the wrong parameters, so "override" has nothing to override\n'
            '    rule("EVT", "(?:\\\\w+::)*(setup|draw|settings|mouse(?:Pressed|Released|Clicked|Moved|Dragged|Wheel|Entered|Exited)'
            '|key(?:Pressed|Released|Typed))\\\\(([^)]*)\\\\). marked .override., but does not override", ""),\n')
match_anchor = '      if (r.code().equals("PTR")) {\n'
match_new = '''      if (r.code().equals("EVT")) {
        String fn = m.group(1), got = m.group(2).trim();
        String want = fn.equals("mouseWheel") ? "mouseWheel(MouseEvent e)"
            : fn.startsWith("mouse") ? fn + "() or " + fn + "(MouseEvent e)"
            : fn.startsWith("key") ? fn + "() or " + fn + "(KeyEvent e)"
            : fn + "()";
        return new Hit("E0008", fn + "(" + got + ") has the wrong parameters, so Processing never calls it. Write " + want);
      }
'''
bad = [n for n, a in (("rule list", rule_anchor), ("match()", match_anchor)) if s.count(a) != 1]
if bad: print("E0008 NOT PATCHED, nothing changed:", ", ".join(bad)); sys.exit(1)
P.write_text(s.replace(rule_anchor, rule_anchor + rule_new, 1).replace(match_anchor, match_new + match_anchor, 1))
print("patched " + str(P))
