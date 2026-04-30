#!/usr/bin/env bash
# run_all.sh — Execute every compiled example and compare outputs between
#              the native g++ binary and the MaiaCpp WASM, when available.
#
# For each NN_category/<name>.cpp it:
#   1. Checks if g++ binary exists (fails with SKIP if not yet built)
#   2. Runs the g++ binary (using input.txt if present, else no stdin)
#   3. If a MaiaCpp dist/node-runner.sh exists, runs it too and diffs outputs
#   4. If expected_output.txt exists, verifies g++ output against it
#
# Usage (from anywhere):
#   bash /path/to/programming_in_cpp_course_en/run_all.sh [FILTER]
#
#   FILTER  optional substring — only test dirs whose name contains FILTER
#
# Exit code: 0 if every test passed/skipped, 1 if any diff failed.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

FILTER="${1:-}"

PASS=0
FAIL=0
SKIP=0
INTERACTIVE=0

run_one() {
    local test_dir="$1"
    local stem="$2"
    local binary="$test_dir/$stem"
    local runner="$test_dir/dist/node-runner.sh"
    local expected="$test_dir/expected_output.txt"
    local input_file="$test_dir/input.txt"

    echo ""
    echo "--> $(basename "$test_dir")/$stem"

    # ── check g++ binary ──────────────────────────────────────────────────────
    if [[ ! -x "$binary" ]]; then
        echo "    SKIP — g++ binary not found (run build_all.sh first)"
        SKIP=$((SKIP + 1))
        return
    fi

    # ── run g++ binary ────────────────────────────────────────────────────────
    local gpp_out
    if [[ -f "$input_file" ]]; then
        gpp_out="$("$binary" < "$input_file" 2>/dev/null)" || true
    else
        # Programs that read stdin will exit immediately with empty input
        gpp_out="$(echo "" | timeout 3 "$binary" 2>/dev/null)" || true
    fi

    local gpp_norm
    gpp_norm="$(printf '%s\n' "$gpp_out" | awk 'NF{last=NR} {lines[NR]=$0} END{for(i=1;i<=last;i++) print lines[i]}')"

    # ── compare with expected_output.txt ─────────────────────────────────────
    if [[ -f "$expected" ]]; then
        local expected_norm
        expected_norm="$(awk 'NF{last=NR} {lines[NR]=$0} END{for(i=1;i<=last;i++) print lines[i]}' "$expected")"
        if diff <(printf '%s\n' "$gpp_norm") <(printf '%s\n' "$expected_norm") > /dev/null 2>&1; then
            echo "    g++    PASS (matches expected_output.txt)"
            PASS=$((PASS + 1))
        else
            echo "    g++    FAIL — output mismatch vs expected_output.txt"
            echo "    ┌── expected ──────────────────────────────────────"
            printf '%s\n' "$expected_norm" | sed 's/^/    │ /'
            echo "    ├── actual ────────────────────────────────────────"
            printf '%s\n' "$gpp_norm"      | sed 's/^/    │ /'
            echo "    └─────────────────────────────────────────────────"
            FAIL=$((FAIL + 1))
        fi
    else
        echo "    g++    OUTPUT (no expected_output.txt):"
        if [[ -n "$gpp_norm" ]]; then
            printf '%s\n' "$gpp_norm" | sed 's/^/    > /'
        else
            echo "    > (no output — program may require interactive input)"
            INTERACTIVE=$((INTERACTIVE + 1))
        fi
    fi

    # ── compare g++ vs MaiaCpp WASM ──────────────────────────────────────────
    if [[ -f "$runner" ]]; then
        local wasm_raw
        wasm_raw="$(bash "$runner" 2>/dev/null)" || true
        local wasm_out
        wasm_out="$(printf '%s\n' "$wasm_raw" | sed '/^\[node-runner\] program returned:/d')"
        local wasm_norm
        wasm_norm="$(printf '%s\n' "$wasm_out" | awk 'NF{last=NR} {lines[NR]=$0} END{for(i=1;i<=last;i++) print lines[i]}')"

        if diff <(printf '%s\n' "$gpp_norm") <(printf '%s\n' "$wasm_norm") > /dev/null 2>&1; then
            echo "    MaiaCpp PASS (matches g++ output)"
        else
            local generated_c="$test_dir/$stem.c"
            local classification="output-mismatch"
            if [[ -f "$generated_c" ]] && grep -q "main: stub-fallback" "$generated_c"; then
                classification="stub-fallback (semantic gap)"
            fi
            echo "    MaiaCpp FAIL — $classification"
            echo "    ┌── g++ ──────────────────────────────────────────"
            printf '%s\n' "$gpp_norm"  | sed 's/^/    │ /'
            echo "    ├── MaiaCpp ──────────────────────────────────────"
            printf '%s\n' "$wasm_norm" | sed 's/^/    │ /'
            echo "    └─────────────────────────────────────────────────"
        fi
    else
        echo "    MaiaCpp SKIP — WASM dist not found (run build_all.sh first)"
    fi
}

for test_dir in "$SCRIPT_DIR"/*/; do
    [[ -d "$test_dir" ]] || continue
    dir_name="$(basename "$test_dir")"
    [[ -z "$FILTER" || "$dir_name" == *"$FILTER"* ]] || continue

    for src in "$test_dir"*.cpp; do
        [[ -f "$src" ]] || continue
        stem="$(basename "${src%.cpp}")"
        run_one "$test_dir" "$stem"
    done
done

echo ""
echo "========================================"
echo "Results: $PASS passed  /  $FAIL failed  /  $SKIP skipped"
echo "Programs needing interactive input: $INTERACTIVE"
echo "========================================"

if [[ "$FAIL" -gt 0 ]]; then
    exit 1
fi
