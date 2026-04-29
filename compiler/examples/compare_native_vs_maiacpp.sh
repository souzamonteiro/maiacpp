#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd -P)"
WEBCPP="$ROOT_DIR/bin/webcpp.sh"
EXAMPLE_ROOT="$SCRIPT_DIR/cpp98-parity"

if [[ ! -x "$WEBCPP" && ! -f "$WEBCPP" ]]; then
  echo "Error: MaiaCpp driver not found: $WEBCPP" >&2
  exit 1
fi

if ! command -v g++ >/dev/null 2>&1; then
  echo "Error: g++ is required for native comparison." >&2
  exit 1
fi

if ! command -v node >/dev/null 2>&1; then
  echo "Error: node is required to run MaiaCpp wrappers." >&2
  exit 1
fi

SOURCES=()
while IFS= read -r source_path; do
  SOURCES+=("$source_path")
done < <(find "$EXAMPLE_ROOT" -type f -name "*.cpp" | sort)
if [[ ${#SOURCES[@]} -eq 0 ]]; then
  echo "No C++ examples found under $EXAMPLE_ROOT" >&2
  exit 1
fi

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

pass_count=0
fail_count=0
native_build_fail_count=0
maiacpp_build_fail_count=0
runtime_fail_count=0

echo "Comparing g++ vs MaiaCpp for ${#SOURCES[@]} examples..."
echo

for src in "${SOURCES[@]}"; do
  rel="${src#$SCRIPT_DIR/}"
  stem="$(echo "$rel" | tr '/' '_' | sed 's/\.cpp$//')"

  native_bin="$TMP_DIR/${stem}.native"
  native_out="$TMP_DIR/${stem}.native.out"
  maia_out="$TMP_DIR/${stem}.maia.out"
  maia_js="$TMP_DIR/${stem}.js"
  maia_wasm="$TMP_DIR/${stem}.wasm"

  native_args=(-std=c++98 -Wall -Wextra -pedantic)
  if grep -q '#include <pthread.h>' "$src"; then
    native_args+=(-pthread)
  fi

  set +e
  g++ "${native_args[@]}" "$src" -o "$native_bin" >"$TMP_DIR/${stem}.native.build.log" 2>&1
  native_build_code=$?
  set -e
  if [[ $native_build_code -ne 0 ]]; then
    printf "NATIVE_BUILD_FAIL   %s\n" "$rel"
    cat "$TMP_DIR/${stem}.native.build.log"
    echo
    native_build_fail_count=$((native_build_fail_count + 1))
    fail_count=$((fail_count + 1))
    continue
  fi

  set +e
  "$WEBCPP" "$src" --js-out "$maia_js" --wasm-out "$maia_wasm" >"$TMP_DIR/${stem}.maia.build.log" 2>&1
  maiacpp_build_code=$?
  set -e
  if [[ $maiacpp_build_code -ne 0 ]]; then
    printf "MAIACPP_BUILD_FAIL %s\n" "$rel"
    cat "$TMP_DIR/${stem}.maia.build.log"
    echo
    maiacpp_build_fail_count=$((maiacpp_build_fail_count + 1))
    fail_count=$((fail_count + 1))
    continue
  fi

  set +e
  "$native_bin" >"$native_out" 2>&1
  native_code=$?
  set -e

  set +e
  node -e "const mod=require(process.argv[1]); mod.run(process.argv[2]).then(code=>process.exit(code||0)).catch(err=>{console.error(err&&err.stack?err.stack:String(err));process.exit(1);});" \
    "$maia_js" "$maia_wasm" >"$maia_out" 2>&1
  maia_code=$?
  set -e

  if [[ $native_code -ne 0 || $maia_code -ne 0 ]]; then
    runtime_fail_count=$((runtime_fail_count + 1))
  fi

  if [[ $native_code -eq $maia_code ]] && cmp -s "$native_out" "$maia_out"; then
    printf "PASS               %s\n" "$rel"
    pass_count=$((pass_count + 1))
  else
    printf "FAIL               %s\n" "$rel"
    echo "  native exit: $native_code"
    echo "  maia   exit: $maia_code"
    echo "  output diff:"
    diff -u "$native_out" "$maia_out" || true
    fail_count=$((fail_count + 1))
  fi
  echo
done

echo "Summary: PASS=$pass_count FAIL=$fail_count NATIVE_BUILD_FAIL=$native_build_fail_count MAIACPP_BUILD_FAIL=$maiacpp_build_fail_count RUNTIME_FAIL=$runtime_fail_count"

if [[ $fail_count -ne 0 ]]; then
  exit 1
fi