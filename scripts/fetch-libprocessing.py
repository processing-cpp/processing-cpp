#!/usr/bin/env python3
"""Download the libprocessing renderer from the fork's GitHub release into libs/<platform>/libprocessing.

    python3 scripts/fetch-libprocessing.py              # latest release
    python3 scripts/fetch-libprocessing.py v0.0.8-fork.2

The library files are too big for git (see .gitignore); this is how a release of CppMode gets them.
"""
import io, json, platform, shutil, subprocess, sys, tarfile, tempfile, urllib.request, zipfile
from pathlib import Path

REPO = "llamasjc/libprocessing"
MODE = Path(__file__).resolve().parent.parent
# release asset suffix -> (CppMode platform folder, library file name)
TARGETS = {
    "linux-x86_64.tar.gz": ("linux-x64", "libprocessing.so"),
    "macos-aarch64.tar.gz": ("macos-arm64", "libprocessing.dylib"),
    "windows-x64.zip": ("windows-x64", "processing.dll"),
}
EXTRA = ("processing.dll.lib", "libprocessing.dll.a")  # Windows import libraries, when present

def get(url):
    req = urllib.request.Request(url, headers={"User-Agent": "cppmode-fetch"})
    with urllib.request.urlopen(req) as r:
        return r.read()

tag = sys.argv[1] if len(sys.argv) > 1 else json.loads(get(f"https://api.github.com/repos/{REPO}/releases/latest"))["tag_name"]
print(f"libprocessing {tag} from {REPO}")
for suffix, (plat, lib) in TARGETS.items():
    name = f"libprocessing-{tag}-{suffix}"
    try:
        data = get(f"https://github.com/{REPO}/releases/download/{tag}/{name}")
    except Exception as e:
        print(f"  {plat}: not in this release ({e})"); continue
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        if suffix.endswith(".zip"):
            zipfile.ZipFile(io.BytesIO(data)).extractall(tmp)
        else:
            tarfile.open(fileobj=io.BytesIO(data)).extractall(tmp)
        found = {p.name: p for p in tmp.rglob("*") if p.is_file()}
        if lib not in found or "processing.h" not in found:
            print(f"  {plat}: archive has no {lib} or processing.h, skipped"); continue
        dest = MODE / "libs" / plat / "libprocessing"
        (dest / "include").mkdir(parents=True, exist_ok=True)
        shutil.copy2(found[lib], dest / lib)
        shutil.copy2(found["processing.h"], dest / "include" / "processing.h")
        for extra in EXTRA:
            if extra in found: shutil.copy2(found[extra], dest / extra)
        if plat == "linux-x64" and platform.system() == "Linux" and shutil.which("strip"):
            subprocess.run(["strip", "--strip-unneeded", str(dest / lib)], check=False)
        print(f"  {plat}: {lib} ({(dest / lib).stat().st_size / 1e6:.1f} MB) -> {dest.relative_to(MODE)}")
