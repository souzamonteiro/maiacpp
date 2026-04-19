#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
MAIACPP_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"
DEFAULT_PARENT="$(cd "$MAIACPP_ROOT/.." && pwd -P)"

usage() {
  cat <<'EOF'
Usage: run-cross-repo-validation.sh [options]

Runs a fail-fast cross-repo validation gate for:
- MaiaCpp
- MaiaC
- MaiaWASM
- MaiaCC

Options:
  --maiacpp-root PATH   MaiaCpp repo root (default: auto)
  --maiac-root PATH     MaiaC repo root (default: ../maiac or ./maiac)
  --maiawasm-root PATH  MaiaWASM repo root (default: ../maiawasm or ./maiawasm)
  --maiacc-root PATH    MaiaCC repo root (default: ../maiacc or ./maiacc)
  --log-dir PATH        Output log directory (default: out/reports/cross-repo-validation-<timestamp>)
  -h, --help            Show this help
EOF
}

MAIACPP_ROOT_OVERRIDE=""
MAIAC_ROOT_OVERRIDE=""
MAIAWASM_ROOT_OVERRIDE=""
MAIACC_ROOT_OVERRIDE=""
LOG_DIR=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    --maiacpp-root)
      [[ $# -ge 2 ]] || { echo "Error: missing value for --maiacpp-root" >&2; exit 2; }
      MAIACPP_ROOT_OVERRIDE="$2"
      shift 2
      ;;
    --maiac-root)
      [[ $# -ge 2 ]] || { echo "Error: missing value for --maiac-root" >&2; exit 2; }
      MAIAC_ROOT_OVERRIDE="$2"
      shift 2
      ;;
    --maiawasm-root)
      [[ $# -ge 2 ]] || { echo "Error: missing value for --maiawasm-root" >&2; exit 2; }
      MAIAWASM_ROOT_OVERRIDE="$2"
      shift 2
      ;;
    --maiacc-root)
      [[ $# -ge 2 ]] || { echo "Error: missing value for --maiacc-root" >&2; exit 2; }
      MAIACC_ROOT_OVERRIDE="$2"
      shift 2
      ;;
    --log-dir)
      [[ $# -ge 2 ]] || { echo "Error: missing value for --log-dir" >&2; exit 2; }
      LOG_DIR="$2"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Error: unknown option $1" >&2
      usage
      exit 2
      ;;
  esac
done

if [[ -n "$MAIACPP_ROOT_OVERRIDE" ]]; then
  MAIACPP_ROOT="$MAIACPP_ROOT_OVERRIDE"
fi
MAIACPP_ROOT="$(cd "$MAIACPP_ROOT" && pwd -P)"

resolve_repo_root() {
  local override="$1"
  local sibling="$2"
  local submodule="$3"

  if [[ -n "$override" ]]; then
    if [[ -d "$override" ]]; then
      cd "$override" && pwd -P
      return 0
    fi
    return 1
  fi

  if [[ -d "$sibling" ]]; then
    cd "$sibling" && pwd -P
    return 0
  fi

  if [[ -d "$submodule" ]]; then
    cd "$submodule" && pwd -P
    return 0
  fi

  return 1
}

if ! MAIAC_ROOT="$(resolve_repo_root "$MAIAC_ROOT_OVERRIDE" "$DEFAULT_PARENT/maiac" "$MAIACPP_ROOT/maiac")"; then
  echo "Error: could not resolve MaiaC root. Use --maiac-root." >&2
  exit 3
fi

if ! MAIAWASM_ROOT="$(resolve_repo_root "$MAIAWASM_ROOT_OVERRIDE" "$DEFAULT_PARENT/maiawasm" "$MAIACPP_ROOT/maiawasm")"; then
  echo "Error: could not resolve MaiaWASM root. Use --maiawasm-root." >&2
  exit 3
fi

if ! MAIACC_ROOT="$(resolve_repo_root "$MAIACC_ROOT_OVERRIDE" "$DEFAULT_PARENT/maiacc" "$MAIACPP_ROOT/maiacc")"; then
  echo "Error: could not resolve MaiaCC root. Use --maiacc-root." >&2
  exit 3
fi

if [[ -z "$LOG_DIR" ]]; then
  TS="$(date +%Y%m%d-%H%M%S)"
  LOG_DIR="$MAIACPP_ROOT/out/reports/cross-repo-validation-$TS"
fi
mkdir -p "$LOG_DIR"

SUMMARY_FILE="$LOG_DIR/summary.txt"
: > "$SUMMARY_FILE"

run_check() {
  local name="$1"
  local root="$2"
  local cmd="$3"
  local log_file="$LOG_DIR/${name}.log"

  echo "[check] $name"
  echo "repo: $root"
  echo "cmd:  $cmd"
  echo

  {
    echo "name=$name"
    echo "repo=$root"
    echo "cmd=$cmd"
    echo "started=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
    echo
    (cd "$root" && bash -lc "$cmd")
  } > "$log_file" 2>&1

  local rc=$?
  if [[ $rc -ne 0 ]]; then
    echo "FAIL $name (rc=$rc)"
    echo "log: $log_file"
    {
      echo "FAIL $name rc=$rc"
      echo "log=$log_file"
    } >> "$SUMMARY_FILE"
    return $rc
  fi

  echo "PASS $name"
  echo "log: $log_file"
  {
    echo "PASS $name rc=0"
    echo "log=$log_file"
  } >> "$SUMMARY_FILE"
  return 0
}

echo "Cross-repo validation"
echo "maiacpp=$MAIACPP_ROOT"
echo "maiac=$MAIAC_ROOT"
echo "maiawasm=$MAIAWASM_ROOT"
echo "maiacc=$MAIACC_ROOT"
echo "log_dir=$LOG_DIR"
echo

run_check "maiacpp" "$MAIACPP_ROOT" "cd compiler/examples && ./build_test_dist.sh"
run_check "maiac" "$MAIAC_ROOT" "node tools/webc.js compiler/examples/test.c -o out/test --run"
run_check "maiawasm" "$MAIAWASM_ROOT" "node assembler/tests/run-tests.js"
run_check "maiacc" "$MAIACC_ROOT" "if [ -f parser-generator/tests/run-tests.js ]; then node parser-generator/tests/run-tests.js; else npm test; fi"

echo
echo "All checks passed."
echo "Summary: $SUMMARY_FILE"
