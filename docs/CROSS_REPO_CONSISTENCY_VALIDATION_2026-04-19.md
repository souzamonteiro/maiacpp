# Cross-Repo Consistency Validation

Date: 2026-04-19

Scope:
- MaiaCpp
- MaiaC
- MaiaWASM
- MaiaCC (tREx parser generator)

Goal:
Confirm that the current end-to-end toolchain is consistent for representative workflows and currently published test entry points.

## Executed Checks

### 1) MaiaCpp
Repository: /Volumes/External_SSD/Documentos/Projects/maiacpp
Command:
- cd compiler/examples
- ./build_test_dist.sh

Result:
- Exit code: 0
- Key evidence:
  - ALL TESTS PASSED
  - [node-runner] program returned: 0

Log:
- /tmp/maiacpp_validate.log

### 2) MaiaC
Repository: /Volumes/External_SSD/Documentos/Projects/maiac
Command:
- node tools/webc.js compiler/examples/test.c -o out/test --run

Result:
- Exit code: 0
- Key evidence:
  - === TEST COMPLETED SUCCESSFULLY ===
  - [webc] program returned: 0

Log:
- /tmp/maiac_validate.log

### 3) MaiaWASM
Repository: /Volumes/External_SSD/Documentos/Projects/maiawasm
Command:
- node assembler/tests/run-tests.js

Result:
- Exit code: 0
- Key evidence:
  - Summary: 10 passed, 0 failed
  - Fixtures with hash mismatch are still explicitly marked PASSED (valid wasm, hash mismatch), so the suite status is green by design.

Log:
- /tmp/maiawasm_validate.log

### 4) MaiaCC
Repository: /Volumes/External_SSD/Documentos/Projects/maiacc
Command:
- node parser-generator/tests/run-tests.js

Result:
- Exit code: 0
- Key evidence:
  - tests 96
  - pass 96
  - fail 0

Log:
- /tmp/maiacc_validate.log

## Consolidated Status

All four repositories passed the selected validation entry points with exit code 0.

This supports a practical statement of cross-repo consistency for:
- MaiaCpp C++ input transpilation and dist execution
- MaiaC compile/run flow from C to wasm/js runtime
- MaiaWASM assembler/disassembler test suite baseline
- MaiaCC parser-generator test baseline

## Important Boundaries

This is a strong integration signal, not a formal proof of global correctness.
It does not automatically guarantee:
- full feature completeness across all language corners
- absence of regressions outside the tested fixtures
- identical behavior under all environment/toolchain combinations

## Recommended Next Step (Optional)

If you want a stricter release gate, add one orchestrated script that runs these four checks in sequence and fails on the first non-zero status, then publish this as CI in the MaiaCpp root pipeline.

## Automation Gate Script

A fail-fast orchestrated script is now available:

- `bin/run-cross-repo-validation.sh`

Default behavior:

- Resolves repo roots from sibling folders (`../maiac`, `../maiawasm`, `../maiacc`) with fallback to MaiaCpp submodules (`./maiac`, `./maiawasm`, `./maiacc`).
- Runs checks in this order: MaiaCpp -> MaiaC -> MaiaWASM -> MaiaCC.
- Stops immediately on first failure.
- Stores logs under `out/reports/cross-repo-validation-<timestamp>/`.

Usage:

```bash
bash ./bin/run-cross-repo-validation.sh
```

Optional root overrides:

```bash
bash ./bin/run-cross-repo-validation.sh \
  --maiac-root /path/to/maiac \
  --maiawasm-root /path/to/maiawasm \
  --maiacc-root /path/to/maiacc
```

Validated run example:

- `out/reports/cross-repo-validation-20260419-192429/summary.txt`
