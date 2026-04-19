#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"
WEBCPP="$REPO_ROOT/bin/webcpp.sh"
NODE_RUNNER="$REPO_ROOT/tools/node/run-wasm-node.js"

usage() {
	cat <<'EOF'
Usage: run-test-node.sh [options]

Options:
	--file FILE          Input C++ file (default: ./compiler/examples/test.cpp)
	--out-dir DIR        Output/Dist directory (default: ./out/node)
	--mode MODE          legacy|dist (default: legacy)
	--name NAME          Dist app name (used with --mode dist)
	--webc-out-base BASE MaiaC webc output base override
	--                  Forward remaining args to webcpp.sh
	-h, --help           Show this help

Legacy mode:
	Generates wasm and runs tools/node/run-wasm-node.js.

Dist mode:
	Runs MaiaC dist pipeline directly via webcpp (--dist-run).
EOF
}

INPUT_FILE="$REPO_ROOT/compiler/examples/test.cpp"
OUT_DIR="$REPO_ROOT/out/node"
MODE="legacy"
DIST_NAME=""
WEBC_OUT_BASE=""
FORWARD_ARGS=()

while [[ $# -gt 0 ]]; do
	case "$1" in
		--file)
			[[ $# -ge 2 ]] || { echo "Error: missing value for --file" >&2; exit 1; }
			INPUT_FILE="$2"
			shift 2
			;;
		--out-dir)
			[[ $# -ge 2 ]] || { echo "Error: missing value for --out-dir" >&2; exit 1; }
			OUT_DIR="$2"
			shift 2
			;;
		--mode)
			[[ $# -ge 2 ]] || { echo "Error: missing value for --mode" >&2; exit 1; }
			MODE="$2"
			shift 2
			;;
		--name)
			[[ $# -ge 2 ]] || { echo "Error: missing value for --name" >&2; exit 1; }
			DIST_NAME="$2"
			shift 2
			;;
		--webc-out-base)
			[[ $# -ge 2 ]] || { echo "Error: missing value for --webc-out-base" >&2; exit 1; }
			WEBC_OUT_BASE="$2"
			shift 2
			;;
		--)
			shift
			FORWARD_ARGS+=("$@")
			break
			;;
		-h|--help)
			usage
			exit 0
			;;
		*)
			echo "Error: unknown option: $1" >&2
			usage
			exit 1
			;;
	esac
done

[[ -f "$WEBCPP" ]] || { echo "Error: missing $WEBCPP" >&2; exit 1; }
[[ -f "$INPUT_FILE" ]] || { echo "Error: input file not found: $INPUT_FILE" >&2; exit 2; }

mkdir -p "$OUT_DIR"
STEM="$(basename "$INPUT_FILE")"
STEM="${STEM%.*}"
WASM_OUT="$OUT_DIR/$STEM.wasm"

case "$MODE" in
	legacy)
		[[ -f "$NODE_RUNNER" ]] || { echo "Error: missing $NODE_RUNNER" >&2; exit 1; }
		bash "$WEBCPP" --file "$INPUT_FILE" --wasm-out "$WASM_OUT" ${FORWARD_ARGS+"${FORWARD_ARGS[@]}"}
		node "$NODE_RUNNER" "$WASM_OUT"
		echo "[node] wasm: $WASM_OUT"
		;;
	dist)
		DIST_ARGS=(--file "$INPUT_FILE" --dist-run --out-dir "$OUT_DIR")
		if [[ -n "$DIST_NAME" ]]; then
			DIST_ARGS+=(--name "$DIST_NAME")
		fi
		if [[ -n "$WEBC_OUT_BASE" ]]; then
			DIST_ARGS+=(--webc-out-base "$WEBC_OUT_BASE")
		fi
		if [[ ${#FORWARD_ARGS[@]} -gt 0 ]]; then
			DIST_ARGS+=("${FORWARD_ARGS[@]}")
		fi
		bash "$WEBCPP" "${DIST_ARGS[@]}"
		echo "[node] dist: $OUT_DIR"
		;;
	*)
		echo "Error: invalid --mode '$MODE' (expected legacy|dist)" >&2
		exit 1
		;;
esac
