import pathlib, sys, shutil
P = pathlib.Path("src/java/InstallWizard.java"); s = P.read_text()
if "cppmode-issue7" in s: print("already patched"); sys.exit(0)
old = '''        ProcessBuilder setxPb = new ProcessBuilder(
          "setx", "PATH", "C:\\\\msys64\\\\mingw64\\\\bin;%PATH%");
        setxPb.redirectErrorStream(true);
        Process setx = setxPb.start();
        streamToLog(setx);
        setx.waitFor();'''
new = '''        // cppmode-issue7: add the msys folder to the USER PATH only. setx with %PATH%
        // wrote the literal text (no shell expands it) and wiped the user's PATH, and
        // even expanded it would copy the system PATH in and cut it at 1024 chars.
        // Read and write the raw registry value so entries like %USERPROFILE%\\... stay
        // expandable (REG_EXPAND_SZ); clearing a dummy variable afterwards makes Windows
        // broadcast the change so new terminals see it, like setx does.
        String ps =
            "$add = 'C:\\\\msys64\\\\mingw64\\\\bin'; " +
            "$k = [Microsoft.Win32.Registry]::CurrentUser.CreateSubKey('Environment'); " +
            "$p = [string]$k.GetValue('Path', '', [Microsoft.Win32.RegistryValueOptions]::DoNotExpandEnvironmentNames); " +
            "if (($p -split ';') -contains $add) { 'PATH already has ' + $add } " +
            "else { $new = if ($p) { $p.TrimEnd(';') + ';' + $add } else { $add }; " +
            "$k.SetValue('Path', $new, [Microsoft.Win32.RegistryValueKind]::ExpandString); " +
            "[Environment]::SetEnvironmentVariable('CPPMODE_PATH_REFRESH', $null, 'User'); " +
            "'Added ' + $add + ' to the user PATH' }; $k.Close()";
        ProcessBuilder pathPb = new ProcessBuilder(
          "powershell.exe", "-NoProfile", "-NonInteractive", "-ExecutionPolicy", "Bypass", "-Command", ps);
        pathPb.redirectErrorStream(true);
        Process pathProc = pathPb.start();
        streamToLog(pathProc);
        if (pathProc.waitFor() != 0)
          throw new Exception("powershell exited with code " + pathProc.exitValue());'''
if s.count(old) != 1: print("NOT PATCHED: setx block not found as expected"); sys.exit(1)
pathlib.Path(".bak/issue7").mkdir(parents=True, exist_ok=True); shutil.copy2(P, ".bak/issue7/InstallWizard.java")
P.write_text(s.replace(old, new, 1)); print("patched", P, "(backup in .bak/issue7)")
