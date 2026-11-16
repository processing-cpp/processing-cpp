import pathlib, sys, shutil
MARK = "cppmode-fontname"
PC, BE = pathlib.Path("src/Processing.cpp"), pathlib.Path("src/LibProcessingBackend.h")
pc, be = PC.read_text(), BE.read_text()
if MARK in pc: print("already patched"); sys.exit(0)
INC = r"""    // cppmode-fontname: ask fontconfig for an installed font by name, the way Java finds
    // system fonts ("DejaVu Sans", "DejaVu Sans Bold", "Arial"). fc-match always answers
    // with *some* font, so only take it when its family or full name is what was asked.
    #ifndef _WIN32
    {
        auto norm = [](::std::string s) { ::std::string o; for (char c : s) if (c != ' ' && c != '-' && c != '_') o += (char)tolower((unsigned char)c); return o; };
        auto fcmatch = [&](const ::std::string& pattern) -> ::std::string {
            ::std::string q; for (char c : pattern) { if (c == '\'') q += "'\\''"; else q += c; }
            FILE* p = processingOpenPipe(("fc-match -f '%{family}|%{fullname}|%{file}' '" + q + "' 2>/dev/null").c_str(), "r");
            if (!p) return "";
            char buf[1024]; ::std::string out; while (fgets(buf, sizeof buf, p)) out += buf;
            processingClosePipe(p);
            size_t a = out.find('|'), b = out.find('|', a == ::std::string::npos ? 0 : a + 1);
            if (a == ::std::string::npos || b == ::std::string::npos) return "";
            ::std::string fams = out.substr(0, a) + "," + out.substr(a + 1, b - a - 1) + ",", file = out.substr(b + 1);
            while (!file.empty() && (file.back() == '\n' || file.back() == '\r')) file.pop_back();
            ::std::string want = norm(nameNoExt);
            for (size_t s = 0, e; (e = fams.find(',', s)) != ::std::string::npos; s = e + 1)
                if (norm(fams.substr(s, e - s)) == want) return file;
            return "";
        };
        ::std::string hit = fcmatch(nameNoExt);
        if (hit.empty()) {  // "DejaVu Sans Bold" -> family "DejaVu Sans", style "Bold"
            size_t sp = nameNoExt.find_last_of(" -");
            if (sp != ::std::string::npos) hit = fcmatch(nameNoExt.substr(0, sp) + ":style=" + nameNoExt.substr(sp + 1));
        }
        if (!hit.empty()) paths.insert(paths.begin(), hit);
    }
    #endif
"""
if "processingClosePipe" not in pc: INC = INC.replace("processingClosePipe(p)", "pclose(p)")
a1 = "    // Also search system font dirs recursively (Linux: fc-list output)\n"
a2 = "        uint64_t id = (it != _fonts.end()) ? it->second : (_fonts[path] = processing_load_font(path.c_str()));\n"
n2 = a2 + ("        if (!id && path.find('/') == std::string::npos && path.find('\\\\') == std::string::npos)\n"
           "            id = _fonts[path] = processing_create_font(path.c_str());  // " + MARK + ": by family name\n")
bad = [x for x, t in ((a1, pc), (a2, be)) if t.count(x) != 1]
if bad: print("NOT PATCHED, nothing changed. Not found:", *[b.strip()[:60] for b in bad]); sys.exit(1)
pathlib.Path(".bak/font").mkdir(parents=True, exist_ok=True)
shutil.copy2(PC, ".bak/font/Processing.cpp"); shutil.copy2(BE, ".bak/font/LibProcessingBackend.h")
PC.write_text(pc.replace(a1, INC + a1, 1)); BE.write_text(be.replace(a2, n2, 1))
print("patched", PC, "and", BE)
