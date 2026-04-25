#!/usr/bin/env bash
# build_all.sh — Transpile every C++98 test in the suite through the full
#                MaiaCpp → MaiaC pipeline, keeping intermediate artefacts.
#
# For each NN_category/*.cpp this script produces:
#   NN_category/<name>.c      — MaiaCpp C-translation (inspect line-by-line)
#   NN_category/<name>.wat    — MaiaC WAT output      (inspect line-by-line)
#   NN_category/dist/         — runnable WASM dist (node-runner.sh, *.wasm …)
#
# Usage (from anywhere):
#   bash /path/to/suite/build_all.sh [--verbose] [FILTER]
#
#   FILTER  optional substring — only build dirs whose name contains FILTER
#           e.g.  bash build_all.sh 04  → only 04_classes
#
# Exit code: 0 if every build succeeded, 1 if any failed.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
MAIACPP_ROOT="$(cd "$SCRIPT_DIR/../../.." && pwd -P)"
WEBCPP="$MAIACPP_ROOT/bin/webcpp.sh"

VERBOSE=0
FILTER=""
for arg in "$@"; do
    case "$arg" in
        --verbose|-v) VERBOSE=1 ;;
        *)            FILTER="$arg" ;;
    esac
done

if [[ ! -x "$WEBCPP" && ! -f "$WEBCPP" ]]; then
    echo "ERROR: webcpp.sh not found at: $WEBCPP" >&2
    exit 1
fi

PASS=0
FAIL=0
SKIP=0

build_one() {
    local src="$1"
    local dir
    dir="$(dirname "$src")"
    local stem
    stem="$(basename "${src%.cpp}")"
    local c_out="${src%.cpp}.c"
    local wat_out="${src%.cpp}.wat"
    local dist_dir="$dir/dist"

    echo ""
    echo "==> $(basename "$dir")/$stem.cpp"

    local cmd=("$WEBCPP" "$src"
        --c-out   "$c_out"
        --wat-out "$wat_out"
        --dist    --out-dir "$dist_dir"
        --name    "$stem")

    if [[ "$VERBOSE" -eq 1 ]]; then
        "${cmd[@]}"
    else
        if ! "${cmd[@]}" 2>&1 | grep -E '^\[(webcpp|webc)\]|ERROR|error:' ; then
            true   # suppress noisy MaiaC progress lines
        fi
    fi

    local rc="${PIPESTATUS[0]:-${?}}"
    if [[ "$rc" -ne 0 ]]; then
        echo "    FAILED (exit $rc)"
        FAIL=$((FAIL + 1))
        return
    fi

    local size_wasm=0
    local wasm_file="$dist_dir/$stem.wasm"
    if [[ -f "$wasm_file" ]]; then
        size_wasm="$(wc -c < "$wasm_file")"
    fi

    echo "    OK — C: $(basename "$c_out")  WAT: $(basename "$wat_out")  WASM: ${size_wasm}B"
    PASS=$((PASS + 1))
}

for test_dir in "$SCRIPT_DIR"/*/; do
    [[ -d "$test_dir" ]] || continue
    dir_name="$(basename "$test_dir")"
    # Apply optional filter
    [[ -z "$FILTER" || "$dir_name" == *"$FILTER"* ]] || continue

    for src in "$test_dir"*.cpp; do
        [[ -f "$src" ]] || continue
        build_one "$src"
    done
done

echo ""
echo "========================================"
echo "Build complete: $PASS built, $FAIL failed, $SKIP skipped"
echo "========================================"

if [[ "$FAIL" -gt 0 ]]; then
    exit 1
fi
