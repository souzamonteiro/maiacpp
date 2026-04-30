#!/usr/bin/env bash
# build_all.sh — Compile every C++ example in the course through both
#                g++ (native) and the MaiaCpp → MaiaC pipeline.
#
# For each NN_category/*.cpp this script produces:
#   NN_category/<name>          — native g++ binary
#   NN_category/<name>.c        — MaiaCpp C-translation
#   NN_category/<name>.wat      — MaiaC WAT output
#   NN_category/dist/           — runnable WASM dist (node-runner.sh, *.wasm …)
#
# Usage (from anywhere):
#   bash /path/to/programming_in_cpp_course_en/build_all.sh [--verbose] [FILTER]
#
#   FILTER  optional substring — only build dirs whose name contains FILTER
#           e.g.  bash build_all.sh 03  → only 03_functions
#
# Exit code: 0 if every build succeeded, 1 if any failed.

set -uo pipefail

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

GPP_PASS=0
GPP_FAIL=0
MAIACPP_PASS=0
MAIACPP_FAIL=0
MAIACPP_SKIP=0

# ── compile with g++ ─────────────────────────────────────────────────────────
build_gpp() {
    local src="$1"
    local stem
    stem="$(basename "${src%.cpp}")"
    local dir
    dir="$(dirname "$src")"
    local out="$dir/$stem"

    if [[ "$VERBOSE" -eq 1 ]]; then
        g++ -std=c++11 -o "$out" "$src"
    else
        g++ -std=c++11 -o "$out" "$src" 2>/dev/null
    fi

    local rc=$?
    if [[ "$rc" -ne 0 ]]; then
        echo "    g++    FAILED (exit $rc)"
        GPP_FAIL=$((GPP_FAIL + 1))
        return 1
    else
        echo "    g++    OK → $(basename "$out")"
        GPP_PASS=$((GPP_PASS + 1))
        return 0
    fi
}

# ── transpile with MaiaCpp ────────────────────────────────────────────────────
build_maiacpp() {
    local src="$1"
    local dir
    dir="$(dirname "$src")"
    local stem
    stem="$(basename "${src%.cpp}")"
    local c_out="${src%.cpp}.c"
    local wat_out="${src%.cpp}.wat"
    local dist_dir="$dir/dist"

    if [[ ! -x "$WEBCPP" && ! -f "$WEBCPP" ]]; then
        echo "    MaiaCpp SKIP — webcpp.sh not found"
        MAIACPP_SKIP=$((MAIACPP_SKIP + 1))
        return
    fi

    local cmd=("$WEBCPP" "$src"
        --c-out   "$c_out"
        --wat-out "$wat_out"
        --dist    --out-dir "$dist_dir"
        --name    "$stem")

    if [[ "$VERBOSE" -eq 1 ]]; then
        "${cmd[@]}"
    else
        if ! "${cmd[@]}" 2>&1 | grep -E '^\[(webcpp|webc)\]|ERROR|error:' ; then
            true
        fi
    fi

    local rc="${PIPESTATUS[0]:-${?}}"
    if [[ "$rc" -ne 0 ]]; then
        echo "    MaiaCpp FAILED (exit $rc)"
        MAIACPP_FAIL=$((MAIACPP_FAIL + 1))
        return
    fi

    local size_wasm=0
    local wasm_file="$dist_dir/$stem.wasm"
    if [[ -f "$wasm_file" ]]; then
        size_wasm="$(wc -c < "$wasm_file")"
    fi

    echo "    MaiaCpp OK — C: $(basename "$c_out")  WASM: ${size_wasm}B"
    MAIACPP_PASS=$((MAIACPP_PASS + 1))
}

# ── main loop ─────────────────────────────────────────────────────────────────
for test_dir in "$SCRIPT_DIR"/*/; do
    [[ -d "$test_dir" ]] || continue
    dir_name="$(basename "$test_dir")"
    [[ -z "$FILTER" || "$dir_name" == *"$FILTER"* ]] || continue

    for src in "$test_dir"*.cpp; do
        [[ -f "$src" ]] || continue
        stem="$(basename "${src%.cpp}")"
        echo ""
        echo "==> $dir_name/$stem.cpp"
        build_gpp    "$src"
        build_maiacpp "$src"
    done
done

echo ""
echo "========================================"
echo "g++     : $GPP_PASS built, $GPP_FAIL failed"
echo "MaiaCpp : $MAIACPP_PASS built, $MAIACPP_FAIL failed, $MAIACPP_SKIP skipped"
echo "========================================"

if [[ "$GPP_FAIL" -gt 0 || "$MAIACPP_FAIL" -gt 0 ]]; then
    exit 1
fi
