# MaiaCpp Compiler Implementation Gaps and Correction Plan

## Goal

Eliminate semantic drift caused by heuristic/stub lowering in `compiler/cpp-compiler.js`, replacing it progressively with AST-first lowering while preserving pipeline stability.

## Problem Summary

Current codegen still depends on heuristic extraction for many constructs:

- Global function lowering can collapse bodies to constants or stubs.
- Class methods/constructors are frequently emitted as stubs unless narrow patterns match.
- `main` lowering is mostly regex/program-shape based, with hardcoded source probes.
- AST and fallback (`SimpleAnalyzer`) metadata are merged in ways that can override AST intent.

This can produce output-equivalent behavior in some files while still losing source semantics in helper functions.

## Implementation Hotspots

### 1) Global Function Heuristic Collapse

- `inferSimpleReturnExprFromBody` / `inferSimpleReturnCallFromBody` / `inferSimpleIfReturnFromBody`
- `inferSimpleReturnExpr` / `inferSimpleReturnCall` / `inferSimpleIfReturn`
- `inferDeterministicNoParamI32Return`
- Emission decision chain in `emitGlobalFunctionStubs`
- Stub fallback in `emitStubReturn`

Risk:

- Non-trivial functions can be folded to constants.
- Unsupported bodies can silently become `return (type)0;`.

### 2) Class Stub Emission

- `emitClassStubs`

Risk:

- Constructor/method semantics are dropped unless minimal lowering patterns are recognized.

### 3) Regex-Driven Main Lowering

- `extractMainStructuredPlan` and helper parsers
- Hardcoded probes: `matchTryThrowCatchMainReturn`, `isDeclarationsMain`, `isElaboratedTypeMain`, `isObjectMemoryMain`

Risk:

- Behavior depends on shape/text markers, not AST semantics.

### 4) AST/Fallback Merge Behavior

- `applySourceClassHints`

Risk:

- Fallback heuristics can inject semantic metadata into AST analysis.

### 5) Call-Lowering Sanitization

- `lowerSimpleReturnCall`

Risk:

- Sanitization (for unsupported forms) can alter semantics silently.

## Correction Strategy

### Phase A: Safety Gates and Observability

1. Add a deterministic-folding gate option in compiler options.
   - Keep current behavior as default for compatibility in short term.
   - Add strict mode path (`--ast-strict`) that disables deterministic function folding for non-main.

2. Emit explicit diagnostics when fallback stub return is used.
   - Record function name and reason (`unsupported lowering path`).
   - Make diagnostics visible in CI logs.

3. Extend regression coverage:
   - Add targeted tests to detect constant-collapsed helper functions in known samples.
   - Keep existing `assert_no_stub_returns` checks and add a "no-unexpected-constant-fold" guard for selected functions.

Acceptance criteria:

- Compiler reports where stubs/folds happen.
- CI catches new unexpected function collapses.

### Phase B: AST-First Function Body Lowering (Global Functions)

1. Implement statement-level lowering from AST for a constrained subset:
   - local declarations/initializers
   - assignments
   - `if`/`else`
   - `return`
   - function calls
   - arithmetic/comparison expressions

2. Switch global-function emission order:
   - Prefer AST body lowering.
   - Use heuristic lowering only as explicit fallback mode.

3. Keep fallback path behind option flags.

Acceptance criteria:

- Selected equivalence files no longer rely on deterministic folding.
- Number of functions emitted via stubs decreases materially.

### Phase C: Class/Method AST Lowering

1. Lower constructor bodies, method bodies, and simple member access from AST.
2. Keep existing micro-lowerings as optimization only.
3. Remove broad method fallback to `return 0` when AST body exists.

Acceptance criteria:

- Class-heavy examples stop using placeholder method stubs.

### Phase D: Replace Regex Main Planner with AST Main Lowering

1. AST lowering for `main` statements used today in regex path.
2. Drop hardcoded source-marker probes.
3. Keep regex path as temporary compatibility fallback only.

Acceptance criteria:

- Existing `main`-equivalence suite passes with AST main path.

### Phase E: Merge Cleanup and Legacy Path Decommission

1. Restrict `applySourceClassHints` to namespace/signature recovery only.
2. Disable semantic metadata injection from `SimpleAnalyzer` by default.
3. Mark legacy fallback options as deprecated.

Acceptance criteria:

- AST semantics are canonical.
- Legacy behavior is opt-in and isolated.

## Test Plan Updates

1. Keep tiered semantic comparisons (`compare_cpp_vs_pipeline.js`).
2. Expand `assert_no_stub_returns.js` usage for more files/functions.
3. Add a new guard script:
   - fail when selected functions collapse to constant return unexpectedly.
4. Add per-phase target suites:
   - Phase B: global function body corpus
   - Phase C: class/method corpus
   - Phase D: main control-flow and iostream corpus

## Recommended Execution Order

1. Land Phase A first (non-invasive guardrails + diagnostics).
2. Implement Phase B on a small function subset and iterate.
3. Proceed to Phase C and D after B is stable.
4. Finish with Phase E and remove hidden semantic overrides.

## Rollback Strategy

- Keep compatibility flags for deterministic/legacy paths until Phase D stabilizes.
- If a phase regresses CI, fallback flags can be re-enabled temporarily while keeping new diagnostics.

## Notes

- Parser fixes must remain grammar-first (`grammar/Cpp.ebnf` + regeneration).
- Avoid direct edits to generated `compiler/cpp-parser.js`.
- Prioritize AST as source of truth for semantics.
