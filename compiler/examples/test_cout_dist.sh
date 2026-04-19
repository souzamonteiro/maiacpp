#!/usr/bin/env bash
# Debug helper for MaiaCpp transpiler regression:
# - Variant A: final status line with printf
# - Variant B: final status line with cout
#
# It generates both variants from compiler/examples/test.cpp, runs webcpp,
# captures logs, extracts the generated C main(), and diffs the results.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd -P)"
WEBCPP="$REPO_ROOT/bin/webcpp.sh"
BASE_TEST="$SCRIPT_DIR/test.cpp"

if [[ ! -f "$WEBCPP" ]]; then
	echo "ERROR: webcpp not found at: $WEBCPP" >&2
	exit 1
fi

if [[ ! -f "$BASE_TEST" ]]; then
	echo "ERROR: test.cpp not found at: $BASE_TEST" >&2
	exit 1
fi

STAMP="$(date +%Y%m%d_%H%M%S)"
OUT_DIR="$SCRIPT_DIR/dist/debug_main_regression_$STAMP"
mkdir -p "$OUT_DIR"

PRINTF_CPP="$OUT_DIR/test_printf_last.cpp"
COUT_CPP="$OUT_DIR/test_cout_last.cpp"
PRINTF_C="$OUT_DIR/test_printf_last.c"
COUT_C="$OUT_DIR/test_cout_last.c"
PRINTF_LOG="$OUT_DIR/webcpp_printf_last.log"
COUT_LOG="$OUT_DIR/webcpp_cout_last.log"
PRINTF_MAIN="$OUT_DIR/main_printf_last.c"
COUT_MAIN="$OUT_DIR/main_cout_last.c"
DIFF_MAIN="$OUT_DIR/main_diff.txt"

make_variant_printf_last() {
	# Keep printf enabled and force cout final line commented.
	sed -E \
		-e 's@^([[:space:]]*)//[[:space:]]*printf\("TESTS FAILED: %d\\n", failures\);@\1printf("TESTS FAILED: %d\\n", failures);@' \
		-e 's@^([[:space:]]*)cout << "TESTS FAILED: " << failures << endl;@\1//cout << "TESTS FAILED: " << failures << endl;@' \
		"$BASE_TEST" > "$PRINTF_CPP"
}

make_variant_cout_last() {
	# Keep cout enabled and force printf final line commented.
	sed -E \
		-e 's@^([[:space:]]*)printf\("TESTS FAILED: %d\\n", failures\);@\1//printf("TESTS FAILED: %d\\n", failures);@' \
		-e 's@^([[:space:]]*)//[[:space:]]*cout << "TESTS FAILED: " << failures << endl;@\1cout << "TESTS FAILED: " << failures << endl;@' \
		"$BASE_TEST" > "$COUT_CPP"
}

extract_main() {
	local in_file="$1"
	local out_file="$2"

	awk '
		BEGIN { in_main = 0; depth = 0 }
		/^[[:space:]]*int[[:space:]]+main[[:space:]]*\(/ {
			in_main = 1
		}
		{
			if (in_main) {
				print
				opens = gsub(/\{/, "&")
				closes = gsub(/\}/, "&")
				depth += opens - closes
				if (depth == 0 && opens > 0) {
					exit
				}
			}
		}
	' "$in_file" > "$out_file"
}

transpile_variant() {
	local in_cpp="$1"
	local out_c="$2"
	local log_file="$3"

	if "$WEBCPP" "$in_cpp" --c-out "$out_c" >"$log_file" 2>&1; then
		echo "OK: $(basename "$in_cpp")"
	else
		echo "FAIL: $(basename "$in_cpp") (see $log_file)"
		return 1
	fi
}

echo "==> Output directory: $OUT_DIR"
echo "==> Creating variants from: $BASE_TEST"
make_variant_printf_last
make_variant_cout_last

echo "==> Transpiling variants"
transpile_variant "$PRINTF_CPP" "$PRINTF_C" "$PRINTF_LOG"
transpile_variant "$COUT_CPP" "$COUT_C" "$COUT_LOG"

echo "==> Extracting generated main()"
extract_main "$PRINTF_C" "$PRINTF_MAIN"
extract_main "$COUT_C" "$COUT_MAIN"

echo "==> Diff main() (printf-last vs cout-last)"
{
	echo "--- printf-last main"
	cat "$PRINTF_MAIN"
	echo
	echo "--- cout-last main"
	cat "$COUT_MAIN"
	echo
	echo "--- unified diff"
	diff -u "$PRINTF_MAIN" "$COUT_MAIN" || true
} > "$DIFF_MAIN"

cat "$DIFF_MAIN"

echo
echo "==> Quick diagnostics"
if grep -Eq '^\s*int\s+main\s*\(void\)\s*\{\s*$' "$COUT_MAIN" && grep -Eq '^\s*return\s*\(int\)0;\s*$' "$COUT_MAIN"; then
	echo "[WARN] cout-last generated an effectively empty main()."
else
	echo "[INFO] cout-last main() is not the trivial empty form."
fi

echo
echo "Artifacts:"
echo "- $PRINTF_CPP"
echo "- $COUT_CPP"
echo "- $PRINTF_C"
echo "- $COUT_C"
echo "- $PRINTF_MAIN"
echo "- $COUT_MAIN"
echo "- $DIFF_MAIN"
echo "- $PRINTF_LOG"
echo "- $COUT_LOG"