#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"

INPUT_FILE="$REPO_ROOT/compiler/examples/example_class_plus_one.cpp"
OUT_DIR="${1:-$REPO_ROOT/out/browser}"
PORT="${2:-8080}"

bash "$SCRIPT_DIR/run-wasm-browser.sh" --file "$INPUT_FILE" --out-dir "$OUT_DIR" --port "$PORT" "${@:3}"
