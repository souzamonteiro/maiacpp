# MaiaCpp Practical Readiness TODO

Date: 2026-04-10

Goal:
- Move MaiaCpp from broad tracked coverage to production-ready behavior.
- Convert expected-fail parser gaps into expected-pass where in-scope.
- Increase semantic/runtime confidence using C++ native vs pipeline equivalence checks.

## Current Baseline

- Matrix family tracking: 42/42 via tiered runner.
- Tier suite: green (Tier 1, Tier 2, Tier 3).
- Remaining expected-fail parser cases:
  - none (all parser checkpoint cases currently expected-pass in tier plan)
 
Recent progress:
- Enum specifier/enumerator list parse path moved to expected-pass in tiered suite (initialized enumerator precedence fix in parser).
- Explicit instantiation and explicit specialization parser checkpoint cases moved to expected-pass.
- Semantic equivalence gate now runs across multiple representative programs (extended baseline + namespace/overload + object/memory).
- Added targeted no-stub guard for selected emitted functions in Tier 2.

Recent progress:
- Namespace alias parse path moved to expected-pass in tiered suite (normalization-aware alias qualifier stripping).
- Alias-qualified call lowering is still partial in generated C bodies and remains a semantic/codegen follow-up item.
- Added browser-host deterministic semantic equivalence gate (native C++ vs pipeline with browser-like host runtime path).
- Expanded Tier 2 no-stub guard to cover a broader set of generated functions from the extended baseline.
- Added concrete control-flow lowering for `for`/`switch`/`while`/`do-while`/ternary-return in structured `main` emission (removing stub fallback for the Tier 2 control-flow probe).

## P0 - Close parser blockers now

### 1) Namespace alias parsing

- [x] Implement namespace alias acceptance in parser path.
- [x] Move case `tier3_parse_namespace_alias_expected_fail` to expected-pass.
- [x] Add one positive fixture in `compiler/tests/fixtures` with namespace alias + qualified call semantics (non-stub).

Files to touch:
- `grammar/Cpp.ebnf`
- `compiler/cpp-parser.js` (regenerated)
- `compiler/tests/tier3/309_namespace_alias.cpp`
- `compiler/tests/ebnf_tiers.json`
- `compiler/tests/fixtures/*`

Acceptance:
- Tier case exits 0 with `Parser: ok`.
- Fixture suite remains green.

### 2) Enum specifier parsing

- [x] Fix enum parsing for translation unit acceptance.
- [x] Move case `tier3_parse_enum_specifier_expected_fail` to expected-pass.
- [x] Add fixture with enum declaration + enum use in function body semantics (non-stub lowering).

Files to touch:
- `grammar/Cpp.ebnf`
- `compiler/cpp-parser.js` (regenerated)
- `compiler/tests/tier3/310_enum_specifier.cpp`
- `compiler/tests/ebnf_tiers.json`
- `compiler/tests/fixtures/*`

Acceptance:
- Tier case exits 0 with `Parser: ok`.
- Compiler emits C for enum fixture without parser fallback messages.

### 3) Explicit instantiation parsing

- [x] Implement parse acceptance for explicit instantiation syntax.
- [x] Move `tier3_parse_explicit_instantiation_expected_fail` to expected-pass.
- [ ] Add negative fixture only if semantic lowering is intentionally deferred.

Files to touch:
- `grammar/Cpp.ebnf`
- `compiler/cpp-parser.js` (regenerated)
- `compiler/tests/tier3/305_explicit_instantiation.cpp`
- `compiler/tests/ebnf_tiers.json`

Acceptance:
- Tier case exits 0 with `Parser: ok`.
- Conformance matrix status for explicitInstantiation can move from missing to partial (if parse-only) with note.

### 4) Explicit specialization parsing

- [x] Implement parse acceptance for explicit specialization syntax.
- [x] Move `tier3_parse_explicit_specialization_expected_fail` to expected-pass.
- [ ] Add at least one parse-only or semantic fixture depending on lowering support.

Files to touch:
- `grammar/Cpp.ebnf`
- `compiler/cpp-parser.js` (regenerated)
- `compiler/tests/tier3/306_explicit_specialization.cpp`
- `compiler/tests/ebnf_tiers.json`

Acceptance:
- Tier case exits 0 with `Parser: ok`.
- Matrix and roadmap docs reflect exact support tier.

## P1 - Remove high-risk semantic shortcuts

### 5) Reduce compatibility fallback dependence

- [ ] Keep default stable, but reduce reliance on source-hint merge for function semantics.
- [ ] Increase AST-derived lowering coverage for global/member function bodies.
- [ ] Keep `--ast-strict` as regression lane and grow pass surface.

Files to touch:
- `compiler/cpp-compiler.js`
- `docs/AST_100_IMPLEMENTATION_TODO.md`
- `docs/CONFORMANCE_MATRIX.md`

Acceptance:
- More fixtures pass under strict mode without legacy hints.
- No regression in default fixture suite.

### 6) Full statement lowering progress

- [ ] Replace known stub returns (`return (int)0;`) in broader statement families.
- [x] Add fixture/checks that fail on stub output for selected families.

Progress notes:
- Tier 2 `full_statement_lowering` probe now requires concrete lowered control-flow output (no stub-return expectation).
- Added structured lowering for call-comparison ternary returns in `main` (e.g. `return foo(...) == N ? A : B;`).
- Added Tier 2 no-stub guards for `main` in control-flow and equivalence namespace cases.
- Added known object/memory `main` lowering path for the 402 equivalence case (no stub fallback).
- Added Tier 2 no-stub guard for `main` in object/memory equivalence case.

Files to touch:
- `compiler/cpp-compiler.js`
- `compiler/tests/fixtures/*.expect.json`

Acceptance:
- New fixture assertions prevent silent semantic stubs in covered paths.

## P2 - Production confidence gates

### 7) Expand semantic equivalence suite

- [x] Add multiple comparison inputs beyond `test_cpp98_extended.cpp`.
- [x] Keep comparator as blocking gate for Tier 1.
- [x] Add one browser-oriented runtime case if deterministic output is feasible.

Files to touch:
- `compiler/tests/compare_cpp_vs_pipeline.py`
- `compiler/tests/ebnf_tiers.json`
- `compiler/tests/tier3/*` (or new folder for equivalence inputs)

Acceptance:
- At least 3 equivalence programs compared (native vs pipeline).
- Diff output remains actionable when mismatch happens.

### 8) CI task wiring

- [x] Add a single command that runs tiered report + fixture suite + comparator.
- [x] Ensure non-zero exit on Tier 1/Tier 2 failures.

Files to touch:
- `package.json`
- CI workflow file (if present)

Progress notes:
- Added `npm run ready:release` as consolidated readiness signal.
- Added CI workflow `.github/workflows/readiness.yml` to execute readiness gate on push/PR.

Acceptance:
- One CI command gives release readiness signal.

## Done Definition for MaiaCpp 1.0-ready

All must be true:

- [ ] No expected-fail parser cases remain in `compiler/tests/ebnf_tiers.json` for in-scope C++98 grammar families.
- [ ] Tier report remains green with tracked families 42/42.
- [ ] Comparator passes on multiple representative C++98 programs.
- [ ] `docs/CONFORMANCE_MATRIX.md` statuses are aligned with real behavior (not optimistic labels).
- [ ] Release candidate passes `report:tiers` and semantic comparison in clean environment.
