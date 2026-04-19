#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"

INPUT_FILE="${1:-$REPO_ROOT/compiler/examples/test.cpp}"
OUT_DIR="${2:-$REPO_ROOT/out/console}"

[[ -f "$INPUT_FILE" ]] || { echo "Error: input file not found: $INPUT_FILE" >&2; exit 2; }

CXX="${CXX:-clang++}"
if ! command -v "$CXX" >/dev/null 2>&1; then
  if command -v g++ >/dev/null 2>&1; then
    CXX="g++"
  else
    echo "Error: no C++ compiler found (clang++/g++)" >&2
    exit 3
  fi
fi

mkdir -p "$OUT_DIR"
STEM="$(basename "$INPUT_FILE")"
STEM="${STEM%.*}"
BIN_OUT="$OUT_DIR/$STEM.native"

"$CXX" -std=c++98 -O0 -g "$INPUT_FILE" -o "$BIN_OUT"
"$BIN_OUT"

echo "[console] binary: $BIN_OUT"
