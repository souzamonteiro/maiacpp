#!/usr/bin/env bash
# Build/test script for compiler/examples/test.cpp
# Works from any cwd: bash compiler/examples/build_test.sh

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd -P)"
INPUT_FILE="$SCRIPT_DIR/test.cpp"
OUT_BASE="$REPO_ROOT/out/examples-test"
DIST_DIR="$REPO_ROOT/out/examples-test-dist"

echo "==> MaiaCpp: parse + transpile test.cpp"
mkdir -p "$OUT_BASE"
bash "$REPO_ROOT/bin/webcpp.sh" \
	--file "$INPUT_FILE" \
	--ast-json-out "$OUT_BASE/test.ast.json" \
	--c-out "$OUT_BASE/test.generated.c"

echo "==> MaiaCpp: node wasm run (legacy mode)"
bash "$REPO_ROOT/bin/run-test-node.sh" \
	--file "$INPUT_FILE" \
	--out-dir "$OUT_BASE/node" \
	--mode legacy

echo "==> MaiaCpp: dist build + node runner"
bash "$REPO_ROOT/bin/run-test-node.sh" \
	--file "$INPUT_FILE" \
	--mode dist \
	--out-dir "$DIST_DIR" \
	--name test

echo "==> Dist browser runner generated at: $DIST_DIR/browser-runner.html"
echo "==> All steps OK"
