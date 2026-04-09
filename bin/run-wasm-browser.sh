#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"
WEBCPP="$REPO_ROOT/bin/webcpp.sh"

INPUT_FILE="${1:-$REPO_ROOT/compiler/test.cpp}"
OUT_DIR="${2:-$REPO_ROOT/out/browser}"
PORT="${3:-8080}"

[[ -f "$WEBCPP" ]] || { echo "Error: missing $WEBCPP" >&2; exit 1; }
[[ -f "$INPUT_FILE" ]] || { echo "Error: input file not found: $INPUT_FILE" >&2; exit 2; }
command -v python3 >/dev/null 2>&1 || { echo "Error: python3 not found" >&2; exit 3; }

INPUT_FILE="$(cd "$(dirname "$INPUT_FILE")" && pwd -P)/$(basename "$INPUT_FILE")"

mkdir -p "$OUT_DIR"
OUT_DIR="$(cd "$OUT_DIR" && pwd -P)"
STEM="$(basename "$INPUT_FILE")"
STEM="${STEM%.*}"
WASM_OUT="$OUT_DIR/$STEM.wasm"

bash "$WEBCPP" --file "$INPUT_FILE" --wasm-out "$WASM_OUT"

WASM_REL="${WASM_OUT#$REPO_ROOT/}"
RUNNER_URL="http://127.0.0.1:${PORT}/tools/browser/run-wasm.html?wasm=/${WASM_REL}"

echo "[browser] wasm: $WASM_OUT"
echo "[browser] opening runner at: $RUNNER_URL"

echo "[browser] press Ctrl+C to stop server"
cd "$REPO_ROOT"
python3 -m http.server "$PORT"
