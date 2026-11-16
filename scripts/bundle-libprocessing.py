#!/usr/bin/env python3
"""Copy the libprocessing renderer (library + header) into the mode, for shipping.

Source: LIBPROCESSING_HOME, else ~/lp-fork. Build it there first:
    cargo build --release -p processing_ffi
Destination: libs/<platform>/libprocessing/{<library>, include/processing.h}
"""
import os, platform, shutil, sys
from pathlib import Path

mode = Path(__file__).resolve().parent.parent
src = Path(os.environ.get("LIBPROCESSING_HOME") or Path.home() / "lp-fork")
system, machine = platform.system(), platform.machine().lower()
arm = machine in ("arm64", "aarch64")
if system == "Windows":   plat, lib = "windows-x64", "processing.dll"
elif system == "Darwin":  plat, lib = ("macos-arm64" if arm else "macos-x64"), "libprocessing.dylib"
else:                     plat, lib = ("linux-aarch64" if arm else "linux-x64"), "libprocessing.so"

lib_src = src / "target" / "release" / lib
hdr_src = src / "crates" / "processing_ffi" / "include" / "processing.h"
for f in (lib_src, hdr_src):
    if not f.is_file():
        sys.exit(f"missing {f}\nbuild it first: cd {src} && cargo build --release -p processing_ffi")

dest = mode / "libs" / plat / "libprocessing"
(dest / "include").mkdir(parents=True, exist_ok=True)
shutil.copy2(lib_src, dest / lib)
# strip debug symbols: the release build carries ~85 MB of them
import subprocess
if system == "Linux":   subprocess.run(["strip", "--strip-unneeded", str(dest / lib)], check=False)
elif system == "Darwin": subprocess.run(["strip", "-x", str(dest / lib)], check=False)
shutil.copy2(hdr_src, dest / "include" / "processing.h")
extra = []
if system == "Windows":  # the import library mingw links against
    for name in ("processing.dll.lib", "libprocessing.dll.a"):
        if (src / "target" / "release" / name).is_file():
            shutil.copy2(src / "target" / "release" / name, dest / name); extra.append(name)
size = (dest / lib).stat().st_size / 1e6
print(f"bundled {lib} ({size:.1f} MB) {' + '.join(extra)} and processing.h into {dest.relative_to(mode)}")
