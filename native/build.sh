#!/usr/bin/env bash
set -e
REPO="$(cd "$(dirname "$0")/.." && pwd)"
NATIVE="$REPO/native"
VENDOR="$NATIVE/vendor"
OUT="$REPO/libs/linux-x64"
SO="$OUT/libcppmode-ts.so"
JAVA_HOME="${JAVA_HOME:-$(dirname "$(dirname "$(readlink -f "$(which javac)")")")}"
echo "JAVA_HOME: $JAVA_HOME"
mkdir -p "$OUT"
gcc -O2 -shared -fPIC \
    -I"$VENDOR/tree-sitter/lib/include" \
    -I"$JAVA_HOME/include" \
    -I"$JAVA_HOME/include/linux" \
    "$VENDOR/tree-sitter/lib/src/lib.c" \
    "$VENDOR/tree-sitter-cpp/parser.c" \
    "$VENDOR/tree-sitter-cpp/scanner.c" \
    "$NATIVE/CppModeTs.c" \
    -o "$SO" -lm
echo "Built: $SO ($(du -sh "$SO" | cut -f1))"
