#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"
WEBCPP="$REPO_ROOT/bin/webcpp.sh"

usage() {
	cat <<'EOF'
Usage: run-wasm-browser.sh [options]

Options:
	--file FILE          Input C++ file (default: ./compiler/test.cpp)
	--out-dir DIR        Output/Dist directory (default: ./out/browser)
	--port PORT          HTTP port for python server (default: 8080)
	--mode MODE          legacy|dist (default: legacy)
	--name NAME          Dist app name (used with --mode dist)
	--webc-out-base BASE MaiaC webc output base override
	--                  Forward remaining args to webcpp.sh
	-h, --help           Show this help

Legacy mode:
	Generates a wasm via --wasm-out and opens tools/browser/run-wasm.html.

Dist mode:
	Generates MaiaC dist package and opens browser-runner.html.
EOF
}

INPUT_FILE="$REPO_ROOT/compiler/test.cpp"
OUT_DIR="$REPO_ROOT/out/browser"
PORT="8080"
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
		--port)
			[[ $# -ge 2 ]] || { echo "Error: missing value for --port" >&2; exit 1; }
			PORT="$2"
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
command -v python3 >/dev/null 2>&1 || { echo "Error: python3 not found" >&2; exit 3; }

INPUT_FILE="$(cd "$(dirname "$INPUT_FILE")" && pwd -P)/$(basename "$INPUT_FILE")"

mkdir -p "$OUT_DIR"
OUT_DIR="$(cd "$OUT_DIR" && pwd -P)"
STEM="$(basename "$INPUT_FILE")"
STEM="${STEM%.*}"
WASM_OUT="$OUT_DIR/$STEM.wasm"
SERVE_DIR="$REPO_ROOT"

case "$MODE" in
	legacy)
		bash "$WEBCPP" --file "$INPUT_FILE" --wasm-out "$WASM_OUT" "${FORWARD_ARGS[@]}"
		WASM_WEB_PATH=""
		if [[ "$WASM_OUT" == "$REPO_ROOT"/* ]]; then
			WASM_REL="${WASM_OUT#$REPO_ROOT/}"
			WASM_WEB_PATH="/${WASM_REL}"
		else
			FALLBACK_DIR="$REPO_ROOT/out/browser"
			mkdir -p "$FALLBACK_DIR"
			FALLBACK_WASM="$FALLBACK_DIR/$STEM.wasm"
			cp -f "$WASM_OUT" "$FALLBACK_WASM"
			WASM_WEB_PATH="/out/browser/$STEM.wasm"
			echo "[browser] note: output dir is outside repo; copied wasm to $FALLBACK_WASM for HTTP serving"
		fi
		RUNNER_URL="http://127.0.0.1:${PORT}/tools/browser/run-wasm.html?wasm=${WASM_WEB_PATH}"
		echo "[browser] wasm: $WASM_OUT"
		;;
	dist)
		DIST_ARGS=(--file "$INPUT_FILE" --dist --out-dir "$OUT_DIR")
		if [[ -n "$DIST_NAME" ]]; then
			DIST_ARGS+=(--name "$DIST_NAME")
		fi
		if [[ -n "$WEBC_OUT_BASE" ]]; then
			DIST_ARGS+=(--webc-out-base "$WEBC_OUT_BASE")
		fi
		DIST_ARGS+=("${FORWARD_ARGS[@]}")
		bash "$WEBCPP" "${DIST_ARGS[@]}"
		SERVE_DIR="$OUT_DIR"
		RUNNER_URL="http://127.0.0.1:${PORT}/browser-runner.html"
		echo "[browser] dist: $OUT_DIR"
		;;
	*)
		echo "Error: invalid --mode '$MODE' (expected legacy|dist)" >&2
		exit 1
		;;
esac

echo "[browser] opening runner at: $RUNNER_URL"

echo "[browser] press Ctrl+C to stop server"
cd "$SERVE_DIR"
python3 -m http.server "$PORT"
