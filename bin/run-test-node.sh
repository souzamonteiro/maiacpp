#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"
WEBCPP="$REPO_ROOT/bin/webcpp.sh"
NODE_RUNNER="$REPO_ROOT/tools/node/run-wasm-node.js"

INPUT_FILE="${1:-$REPO_ROOT/compiler/test.cpp}"
OUT_DIR="${2:-$REPO_ROOT/out/node}"

[[ -f "$WEBCPP" ]] || { echo "Error: missing $WEBCPP" >&2; exit 1; }
[[ -f "$NODE_RUNNER" ]] || { echo "Error: missing $NODE_RUNNER" >&2; exit 1; }
[[ -f "$INPUT_FILE" ]] || { echo "Error: input file not found: $INPUT_FILE" >&2; exit 2; }

mkdir -p "$OUT_DIR"
STEM="$(basename "$INPUT_FILE")"
STEM="${STEM%.*}"
WASM_OUT="$OUT_DIR/$STEM.wasm"

bash "$WEBCPP" --file "$INPUT_FILE" --wasm-out "$WASM_OUT"
node "$NODE_RUNNER" "$WASM_OUT"

echo "[node] wasm: $WASM_OUT"
