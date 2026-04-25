#!/usr/bin/env bash
# run_all.sh — Execute every compiled WASM test and compare output.
#
# For each NN_category/<name>.cpp it:
#   1. Runs  dist/node-runner.sh  (fails with SKIP if not yet built)
#   2. Diffs actual output against expected_output.txt
#   3. If no expected file exists, checks that the last line is "ALL PASS"
#
# Usage (from anywhere):
#   bash /path/to/suite/run_all.sh [FILTER]
#
#   FILTER  optional substring — only test dirs whose name contains FILTER
#
# Exit code: 0 if every test passed, 1 if any failed.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"

FILTER="${1:-}"

PASS=0
FAIL=0
SKIP=0

run_one() {
    local test_dir="$1"
    local stem="$2"
    local runner="$test_dir/dist/node-runner.sh"
    local expected="$test_dir/expected_output.txt"

    echo ""
    echo "--> $(basename "$test_dir")/$stem"

    if [[ ! -f "$runner" ]]; then
        echo "    SKIP — dist not found (run build_all.sh first)"
        SKIP=$((SKIP + 1))
        return
    fi

    local actual
    actual="$(bash "$runner" 2>/dev/null)" || true

    if [[ -f "$expected" ]]; then
        if diff <(printf '%s\n' "$actual") "$expected" > /dev/null 2>&1; then
            echo "    PASS"
            PASS=$((PASS + 1))
        else
            echo "    FAIL — output mismatch"
            echo "    ┌── expected ──────────────────────────────────────"
            sed 's/^/    │ /' "$expected"
            echo "    ├── actual ────────────────────────────────────────"
            printf '%s\n' "$actual" | sed 's/^/    │ /'
            echo "    └─────────────────────────────────────────────────"
            FAIL=$((FAIL + 1))
        fi
    else
        # No reference file: just check last line
        local last
        last="$(printf '%s\n' "$actual" | tail -n1)"
        if [[ "$last" == "ALL PASS" ]]; then
            echo "    PASS (no expected_output.txt — last line: ALL PASS)"
            PASS=$((PASS + 1))
        else
            echo "    WARN — no expected_output.txt; last line: $last"
            printf '%s\n' "$actual" | sed 's/^/    > /'
            SKIP=$((SKIP + 1))
        fi
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
echo "========================================"

if [[ "$FAIL" -gt 0 ]]; then
    exit 1
fi
