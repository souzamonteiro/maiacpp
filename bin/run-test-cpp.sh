#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"

usage() {
  cat <<'EOF'
Usage: run-test-cpp.sh [--file FILE] [--target TARGET]

Targets:
  console   Compile and run native executable
  node      Compile to WASM and run in Node
  browser   Compile to WASM and serve browser runner
  all       Run console + node, then prepare browser command

Options:
  --file FILE     Input C++ file (default: ./compiler/test.cpp)
  --target T      Target: console|node|browser|all (default: all)
  --              Forward remaining args to node/browser wrappers
  -h, --help      Show this help
EOF
}

INPUT_FILE="$REPO_ROOT/compiler/test.cpp"
TARGET="all"
FORWARD_ARGS=()

while [[ $# -gt 0 ]]; do
  case "$1" in
    --file)
      [[ $# -ge 2 ]] || { echo "Error: missing value for --file" >&2; exit 1; }
      INPUT_FILE="$2"
      shift 2
      ;;
    --target)
      [[ $# -ge 2 ]] || { echo "Error: missing value for --target" >&2; exit 1; }
      TARGET="$2"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    --)
      shift
      FORWARD_ARGS+=("$@")
      break
      ;;
    *)
      echo "Error: unknown option $1" >&2
      usage
      exit 1
      ;;
  esac
done

[[ -f "$INPUT_FILE" ]] || { echo "Error: input file not found: $INPUT_FILE" >&2; exit 2; }

case "$TARGET" in
  console)
    bash "$SCRIPT_DIR/run-test-console.sh" "$INPUT_FILE"
    ;;
  node)
    bash "$SCRIPT_DIR/run-test-node.sh" --file "$INPUT_FILE" "${FORWARD_ARGS[@]}"
    ;;
  browser)
    bash "$SCRIPT_DIR/run-wasm-browser.sh" --file "$INPUT_FILE" "${FORWARD_ARGS[@]}"
    ;;
  all)
    bash "$SCRIPT_DIR/run-test-console.sh" "$INPUT_FILE"
    bash "$SCRIPT_DIR/run-test-node.sh" --file "$INPUT_FILE" "${FORWARD_ARGS[@]}"
    echo "[all] To run in browser now:"
    echo "  bash $SCRIPT_DIR/run-wasm-browser.sh --file $INPUT_FILE"
    ;;
  *)
    echo "Error: invalid target '$TARGET'" >&2
    usage
    exit 1
    ;;
esac
