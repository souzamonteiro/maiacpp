#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

./build.sh >/tmp/cpp_build.log 2>&1

echo "[ok] build.sh"

python3 tests/run_fixtures.py

echo ""
echo "Running legacy smoke cases..."

legacy_failures=0
for case_file in tests/cases/*.cpp; do
  out_file="/tmp/$(basename "${case_file%.cpp}").xml"
  err_file="/tmp/$(basename "${case_file%.cpp}").err"

  if node Cpp-main.js "$case_file" >"$out_file" 2>"$err_file"; then
    echo "[ok] smoke: $(basename "$case_file")"
  else
    echo "[fail] smoke: $(basename "$case_file")"
    sed -n '1,20p' "$err_file"
    legacy_failures=$((legacy_failures + 1))
  fi
done

if [[ "$legacy_failures" -gt 0 ]]; then
  echo ""
  echo "Legacy smoke failures: $legacy_failures"
  exit 1
fi

echo ""
echo "All grammar tests passed (fixtures + smoke)."
