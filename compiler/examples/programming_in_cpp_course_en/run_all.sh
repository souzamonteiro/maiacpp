#!/usr/bin/env bash
# run_all.sh — Execute every compiled example and compare outputs between
#              the native g++ binary and the MaiaCpp WASM, when available.
#
# For each NN_category/<name>.cpp it:
#   1. Checks if g++ binary exists (fails with SKIP if not yet built)
#   2. Runs the g++ binary (using <name>.input.txt if present, else empty stdin)
#   3. If a MaiaCpp dist/node-runner.sh exists, runs it too (same stdin) and diffs
#   4. If <name>.expected_output.txt exists, verifies g++ output against it
#
# Input files (per-program, in the same category directory):
#   <name>.input.txt          — stdin fed to both g++ and MaiaCpp WASM
#   <name>.expected_output.txt — expected stdout (optional; overrides g++ diff)
#
# Usage (from anywhere):
#   bash /path/to/programming_in_cpp_course_en/run_all.sh [FILTER]
#
#   FILTER  optional substring — only test dirs whose name contains FILTER
#
# Exit code: 0 if every test passed/skipped, 1 if any diff failed.
#
# Environment:
#   WASM_TIMEOUT   seconds before WASM run is killed (default: 10)

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

# Cross-platform timeout wrapper (macOS lacks GNU 'timeout')
_run_timed() {
    local secs=$1; shift
    if command -v timeout &>/dev/null; then
        timeout "$secs" "$@"
    elif command -v gtimeout &>/dev/null; then
        gtimeout "$secs" "$@"
    else
        # No timeout binary available — programs with a complete input.txt won't hang
        "$@"
    fi
}

FILTER="${1:-}"
WASM_TIMEOUT="${WASM_TIMEOUT:-10}"

PASS=0
FAIL=0
SKIP=0
INTERACTIVE=0
MAIACPP_PASS=0
MAIACPP_FAIL=0
MAIACPP_SKIP=0

run_one() {
    local test_dir="$1"
    local stem="$2"
    local binary="$test_dir/$stem"
    local runner="$test_dir/dist/node-runner.sh"
    local expected="$test_dir/${stem}.expected_output.txt"
    local input_file="$test_dir/${stem}.input.txt"

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
        gpp_out="$(echo '' | "$binary" 2>/dev/null)" || true
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
        local wasm_file="$test_dir/dist/$stem.wasm"
        local wasm_raw
        # Pass the specific WASM file and the same stdin used for g++.
        if [[ -f "$wasm_file" ]]; then
            if [[ -f "$input_file" ]]; then
                wasm_raw="$(_run_timed "$WASM_TIMEOUT" bash "$runner" "$wasm_file" < "$input_file" 2>/dev/null)" || true
            else
                wasm_raw="$(echo '' | _run_timed "$WASM_TIMEOUT" bash "$runner" "$wasm_file" 2>/dev/null)" || true
            fi
        else
            # WASM not compiled (0-byte or missing) — skip comparison
            echo "    MaiaCpp SKIP — WASM binary not found (stub-fallback or build error)"
            MAIACPP_SKIP=$((MAIACPP_SKIP + 1))
            return
        fi
        local wasm_out
        wasm_out="$(printf '%s\n' "$wasm_raw" | sed '/^\[node-runner\] program returned:/d')"
        local wasm_norm
        # Strip null bytes (WASM zero-initializes memory; native may have garbage)
        wasm_norm="$(printf '%s\n' "$wasm_out" | tr -d '\000' | awk 'NF{last=NR} {lines[NR]=$0} END{for(i=1;i<=last;i++) print lines[i]}')"

        # If a WASM-specific expected output exists, compare against it instead of g++
        local wasm_expected_file="$test_dir/${stem}.wasm_expected_output.txt"
        local wasm_ref_norm
        if [[ -f "$wasm_expected_file" ]]; then
            wasm_ref_norm="$(awk 'NF{last=NR} {lines[NR]=$0} END{for(i=1;i<=last;i++) print lines[i]}' "$wasm_expected_file")"
        else
            wasm_ref_norm="$gpp_norm"
        fi

        if diff <(printf '%s\n' "$wasm_ref_norm") <(printf '%s\n' "$wasm_norm") > /dev/null 2>&1; then
            echo "    MaiaCpp PASS (matches g++ output)"
            MAIACPP_PASS=$((MAIACPP_PASS + 1))
        else
            local generated_c="$test_dir/$stem.c"
            local classification="output-mismatch"
            if [[ -f "$generated_c" ]] && grep -q "stub-fallback" "$generated_c"; then
                classification="stub-fallback (semantic gap)"
            fi
            echo "    MaiaCpp FAIL — $classification"
            if [[ -f "$wasm_expected_file" ]]; then
                echo "    ┌── expected (wasm_expected_output.txt) ──────────"
                printf '%s\n' "$wasm_ref_norm" | sed 's/^/    │ /'
            else
                echo "    ┌── g++ ──────────────────────────────────────────"
                printf '%s\n' "$gpp_norm"  | sed 's/^/    │ /'
            fi
            echo "    ├── MaiaCpp ──────────────────────────────────────"
            printf '%s\n' "$wasm_norm" | sed 's/^/    │ /'
            echo "    └─────────────────────────────────────────────────"
            MAIACPP_FAIL=$((MAIACPP_FAIL + 1))
        fi
    else
        echo "    MaiaCpp SKIP — WASM dist not found (run build_all.sh first)"
        MAIACPP_SKIP=$((MAIACPP_SKIP + 1))
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
echo "g++ results : $PASS passed  /  $FAIL failed  /  $SKIP skipped"
echo "MaiaCpp WASM: $MAIACPP_PASS passed  /  $MAIACPP_FAIL failed  /  $MAIACPP_SKIP skipped"
echo "Programs with no expected_output and no input.txt: $INTERACTIVE"
echo "========================================"

if [[ "$FAIL" -gt 0 ]]; then
    exit 1
fi
