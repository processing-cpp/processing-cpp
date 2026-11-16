#!/usr/bin/env bash
# native/build.sh: build libcppmode-ts for every platform CppMode ships on.
#
#   bash native/build.sh            # all targets
#   bash native/build.sh linux      # one of: linux | windows | macos-x64 | macos-arm64
#
# Linux uses the system gcc. Windows and macOS are cross-compiled with zig
# (pacman -S zig, or pip install ziglang). JNI headers are vendored in
# native/jni/, so no JDK is needed for any target.
set -euo pipefail

REPO="$(cd "$(dirname "$0")/.." && pwd)"
NATIVE="$REPO/native"
VENDOR="$NATIVE/vendor"
TS="$VENDOR/tree-sitter/lib"
# grammar: the first vendor/*/src with a parser.c that isn't tree-sitter itself
GRAMMAR=""
for d in "$VENDOR"/*/src "$VENDOR"/*; do
  case "$d" in */tree-sitter/lib/src|*/tree-sitter) continue ;; esac
  [ -f "$d/parser.c" ] && { GRAMMAR="$d"; break; }
done
[ -n "$GRAMMAR" ] || { echo "no grammar found under $VENDOR/*/src"; exit 1; }
echo "grammar: $GRAMMAR"

SOURCES=(
  "$TS/src/lib.c"
  "$GRAMMAR/parser.c"
  "$GRAMMAR/scanner.c"
  "$NATIVE/CppModeTs.c"
)

# -fvisibility=hidden keeps tree-sitter's ts_* symbols private, so other
# modes that load their own tree-sitter into the same JVM can't clash with
# ours. The JNI entry points stay visible through JNIEXPORT.
COMMON=(-O2 -shared -fPIC -fvisibility=hidden -DTREE_SITTER_HIDE_SYMBOLS -DTREE_SITTER_HIDDEN_SYMBOLS
        -I"$TS/include" -I"$TS/src" -I"$GRAMMAR"
        -I"$NATIVE/jni")

find_zig() {
  if command -v zig >/dev/null 2>&1; then echo "zig"; return; fi
  if python3 -c "import ziglang" >/dev/null 2>&1; then echo "python3 -m ziglang"; return; fi
  echo ""
}

build_linux() {
  local out="$REPO/libs/linux-x64"; mkdir -p "$out"
  gcc "${COMMON[@]}" -I"$NATIVE/jni/unix" "${SOURCES[@]}" \
      -Wl,-z,defs -o "$out/libcppmode-ts.so" -lm
  echo "  linux-x64:   $(du -h "$out/libcppmode-ts.so" | cut -f1)"
}

build_zig() {   # $1 zig target, $2 libs subdir, $3 file name, $4 jni_md dir
  local zig; zig="$(find_zig)"
  if [ -z "$zig" ]; then
    echo "  $2: skipped (install zig: pacman -S zig)"; return
  fi
  local out="$REPO/libs/$2"; mkdir -p "$out"
  $zig cc -target "$1" "${COMMON[@]}" -I"$NATIVE/jni/$4" "${SOURCES[@]}" \
      -o "$out/$3"
  rm -f "$out"/*.pdb "$out"/*.lib   # zig drops these next to Windows dlls
  echo "  $2: $(du -h "$out/$3" | cut -f1)"
}

build_windows()     { build_zig x86_64-windows-gnu windows-x64 cppmode-ts.dll      win32; }
build_macos_x64()   { build_zig x86_64-macos       macos-x64   libcppmode-ts.dylib unix; }
build_macos_arm64() { build_zig aarch64-macos      macos-arm64 libcppmode-ts.dylib unix; }

target="${1:-all}"
echo "Building libcppmode-ts ($target)"
case "$target" in
  linux)       build_linux ;;
  windows)     build_windows ;;
  macos-x64)   build_macos_x64 ;;
  macos-arm64) build_macos_arm64 ;;
  all)         build_linux; build_windows; build_macos_x64; build_macos_arm64 ;;
  *) echo "unknown target: $target"; exit 1 ;;
esac

# Only JNI symbols should be exported, never tree-sitter's ts_*.
so="$REPO/libs/linux-x64/libcppmode-ts.so"
if [ -f "$so" ]; then
  leaked=$(nm -D --defined-only "$so" | grep -c " ts_" || true)
  echo "  exported ts_* symbols on linux: $leaked (should be 0)"
fi
