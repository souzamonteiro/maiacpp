#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
DEFAULT_WASM="$SCRIPT_DIR/command_line_args.wasm"

if [[ $# -eq 0 ]]; then
  exec node "$SCRIPT_DIR/node-runner.js" "$DEFAULT_WASM"
fi

exec node "$SCRIPT_DIR/node-runner.js" "$@"
