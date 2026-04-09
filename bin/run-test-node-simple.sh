#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"

INPUT_FILE="$REPO_ROOT/compiler/example_class_plus_one.cpp"
OUT_DIR="${1:-$REPO_ROOT/out/node}"

bash "$SCRIPT_DIR/run-test-node.sh" "$INPUT_FILE" "$OUT_DIR"
