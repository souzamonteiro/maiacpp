# MaiaCpp Support Roadmap

This roadmap tracks the gap between grammar breadth and effective compiler support.

## Current Snapshot

- Grammar (`grammar/Cpp.ebnf`) is broad and includes many C++98 constructs.
- Parser/codegen support is still partial in combined real-world files.
- Comprehensive baseline for day-to-day validation is `compiler/test.cpp`.

## Proven Working Baseline (Parser: ok)

- Class declaration with constructor and const method.
- Template class declaration with `operator[]`.
- Function pointer typedef and indirect call.
- `dynamic_cast` and `static_cast`.
- `new`, `delete`, placement new, explicit destructor call.
- `printf`-based output for browser/node runtime portability.

## Known Gaps Observed During Integration

- Some combinations that include namespace blocks plus additional declarations still fall back to simple analyzer.
- Semantic/codegen coverage is narrower than grammar coverage (many features parse but are not fully lowered).
- WAT backend is currently skeletal (module/import/stub generation) and not yet full lowering.

## Next Priorities

1. Parser reliability for mixed translation units:
   - Add focused fixtures for combinations (namespace + class + template + functions).
   - Remove fallback in positive-path compilation for those fixtures.

## Phase 1 Progress (Current)

- Added positive integration fixture:
   - `compiler/tests/fixtures/009_integration_core_subset.cpp`
   - Covers class + template + function-pointer typedef + casts + new/delete/placement-new.
- Added regression tracker for known parser gap:
   - `compiler/tests/fixtures/104_negative_namespace_combo_regression.cpp`
   - Documents current fallback when namespace blocks are combined with additional global declarations.

## Phase 2 Progress (Current)

- Implemented parser retry path for mixed units with namespace blocks in `compiler/cpp-compiler.js`.
- Promoted namespace handling to a parser-front normalization step (syntactic preprocessing):
   - flatten namespace blocks
   - strip `using namespace` directives in normalized source
   - strip qualifiers for known namespace names (e.g. `B::f()` -> `f()`)
- `104_negative_namespace_combo_regression` moved to positive expectation (`shouldParse: true`).
- Removed error-message heuristic branch previously used for this case.
- Goal: keep compilation in parser path (`Parser: ok`) while full namespace semantics are expanded.

2. Semantic coverage expansion:
   - Global function collection and symbol resolution.
   - Local declarations and initialization handling consistency.

## Phase 3 Progress (Current)

- Semantic analyzer now tracks namespace scope while walking parse tree.
- Class metadata now carries `namespacePath` and defines fully-qualified class symbols.
- C/WAT stub emission now passes namespace path into mangling for class method symbols.
- Added source-backed semantic hints to fill class members/methods/constructors when parse-tree metadata is partial.
- Added source-backed extraction for global functions with namespace path and stub emission in C/WAT backends.
- Added namespace mangling regression fixture:
   - `compiler/tests/fixtures/010_namespace_class_mangling.cpp`
   - Validates namespace-prefixed class symbols (`N_C_init`, `N_C_destroy`, `N_C_m`) in compiler output.
- Added global namespace function regression fixture:
   - `compiler/tests/fixtures/011_namespace_global_function.cpp`
   - Validates namespace-prefixed global function symbol (`N_add__ii`) in compiler output.

3. Codegen C expansion:
   - Emit non-stub bodies for core expressions/statements used by baseline tests.
   - Tighten behavior for parser failures (strict mode option).

## Phase 4 Progress (Current)

- Added source-backed global function extraction with body hints.
- Implemented safe incremental lowering in C backend for simple global returns:
   - pattern: `return <arith_expr_using_params>;`
   - no function-call lowering yet (still stubbed for safety).
- Added lowering regression fixture:
   - `compiler/tests/fixtures/012_global_simple_return_lowering.cpp`
   - Validates emitted body contains `return a + b;` for `add__ii`.

## Phase 5 Progress (Current)

- Implemented safe lowering for simple global call returns:
   - pattern: `return foo(arg1, arg2, ...);`
   - resolves and rewrites callee to mangled global symbol when available.
- Added call-lowering regression fixture:
   - `compiler/tests/fixtures/013_global_call_lowering.cpp`
   - Validates emitted call `return add__ii(x, y);` inside `sum2__ii`.

## Phase 6 Progress (Current)

- Extended safe lowering for namespace-qualified global calls:
   - pattern: `return N::foo(arg1, arg2, ...);`
   - resolves namespace-qualified callee to the mangled global symbol.
- Added qualified-call lowering regression fixture:
   - `compiler/tests/fixtures/014_namespace_qualified_call_lowering.cpp`
   - Validates emitted call `return N_add__ii(x, y);` inside `sum2__ii`.

## Phase 7 Progress (Current)

- Extended qualified-call resolution for deeper namespace paths and relative qualification:
   - nested qualifier pattern: `return A::B::foo(...);`
   - relative qualifier pattern inside namespace scope: `return B::foo(...);`
- Added regression fixtures:
   - `compiler/tests/fixtures/015_nested_namespace_qualified_call_lowering.cpp`
   - Validates emitted call `return A_B_add__ii(x, y);`.
   - `compiler/tests/fixtures/016_partial_namespace_qualified_call_lowering.cpp`
   - Validates relative qualification in namespace scope resolves to `A_B_add__ii` (not global `add`).

## Phase 8 Progress (Current)

- Added deterministic overload selection for simple call lowering when argument types are inferable:
   - Uses caller parameter types and numeric literals to infer argument type hints.
   - Prefers exact signature match by normalized type before namespace/arity fallback.
- Added overload resolution fixtures:
   - `compiler/tests/fixtures/017_overload_type_resolution.cpp`
   - Validates `return add__ii(x, y);` when `add(float,float)` appears before `add(int,int)`.
   - `compiler/tests/fixtures/018_namespace_overload_type_resolution.cpp`
   - Validates namespace-qualified overload lowers to `return N_add__ii(x, y);`.

## Phase 9 Progress (Current)

- Extended call-arg type inference used by simple call lowering:
   - accepts explicit C-style casts in arguments (e.g. `(long)x`)
   - accepts numeric suffixes for integer and floating literals (e.g. `1L`, `1.0f`)
- Added overload regression fixtures for these cases:
   - `compiler/tests/fixtures/019_cast_overload_type_resolution.cpp`
   - Validates cast-driven lowering to `return add__ll((long)x, (long)y);`.
   - `compiler/tests/fixtures/020_literal_suffix_overload_type_resolution.cpp`
   - Validates suffix-driven lowering to `return add__ff(1.0f, y);`.

## Phase 10 Progress (Current)

- Extended simple-call validation and arg-type inference with:
   - unary argument handling (e.g. `-x`, `+x`)
   - `static_cast<T>(expr)` argument handling
   - safer argument splitting with nesting awareness for parentheses/angle brackets.
- Added overload regression fixtures:
   - `compiler/tests/fixtures/021_unary_arg_overload_resolution.cpp`
   - Validates unary-arg lowering to `return neg__i(-x);`.
   - `compiler/tests/fixtures/022_static_cast_overload_resolution.cpp`
   - Validates cast-driven lowering to `return add__ll(static_cast<long>(x), static_cast<long>(y));`.

## Phase 11 Progress (Current)

- Added simple implicit conversion ranking for overload selection when exact match is unavailable:
   - prefers `int -> long` over broader alternatives
   - prefers `float -> double` over incompatible alternatives
   - keeps deterministic fallback when no supported conversion path exists.
- Added conversion-focused regression fixtures:
   - `compiler/tests/fixtures/023_implicit_int_to_long_overload_resolution.cpp`
   - Validates lowering to `return add__ll(x, y);`.
   - `compiler/tests/fixtures/024_implicit_float_to_double_overload_resolution.cpp`
   - Validates lowering to `return add__dd(x, y);`.

## Phase 12 Progress (Current)

- Added explicit deterministic tie-break for overload candidates with equal conversion cost:
   - prefers candidate whose namespace path is closest to current function scope
   - then prefers deeper namespace match
   - then applies stable lexical key fallback.
- Added scope tie-break regression fixtures:
   - `compiler/tests/fixtures/025_scope_tie_break_namespace_priority.cpp`
   - Validates namespace-local function is preferred over global candidate in tie scenario.
   - `compiler/tests/fixtures/026_scope_tie_break_proximity.cpp`
   - Validates nearest namespace-prefix candidate is selected in fallback tie scenario.

## Phase 13 Progress (Current)

- Added paramwise tie-break before scope tie-break for equal total conversion cost:
   - compares conversion cost vectors lexicographically (parameter order).
   - ensures deterministic preference for signatures with earlier exact/cheaper matches.
- Added regression fixtures for real equal-sum ties:
   - `compiler/tests/fixtures/027_paramwise_cost_tie_break.cpp`
   - Validates call lowers to `return mix__il(x, y);` over `mix__li` for `(int,int)` args.
   - `compiler/tests/fixtures/028_paramwise_cost_tie_break_namespace.cpp`
   - Validates same paramwise rule under namespace lowering (`N_mix__il`).

## Phase 14 Progress (Current)

- Added explicit ambiguity diagnostic when overload resolution falls back to stable lexical key after full tie:
   - diagnostic is emitted in generated C as a comment before the lowered return call.
   - includes selected candidate signature key for traceability.
- Added regression fixture:
   - `compiler/tests/fixtures/029_ambiguity_stable_fallback_diagnostic.cpp`
   - Validates diagnostic marker and deterministic selected callee (`A_B_add__ll`) in an ambiguous tie scenario.

## Phase 15 Progress (Current)

- Added per-file ambiguity summary in generated C output:
   - emits total ambiguity case count for the source file
   - emits one line per unique caller/selected-callee ambiguity event.
- Added regression fixture:
   - `compiler/tests/fixtures/030_ambiguity_summary_count.cpp`
   - Validates summary header/count and detailed caller/selection line.

## Phase 16 Progress (Current)

- Added practical execution workflow for `compiler/test.cpp` across targets (MaiaC-like DX):
   - console native execution script (`clang++`/`g++`)
   - Node WASM execution script with host runtime imports
   - browser WASM runner page + local server script.
- Added orchestration script for one-command execution by target (`console|node|browser|all`).

## Phase 17 Progress (Current)

- Extended WAT backend global function lowering beyond stubs for safe subsets:
   - arithmetic `return` expressions on params/literals
   - simple global call returns
   - simple `if (x == 0) return A; else return B;` pattern
- Added focused fixture:
   - `compiler/tests/fixtures/031_wat_simple_if_return.cpp`
   - Tracks parser/codegen path while WAT output is validated via direct WAT inspection.

## Phase 18 Progress (Current)

- Generalized WAT simple-if lowering for integer comparisons in global function bodies:
   - supports `==`, `!=`, `<`, `>`, `<=`, `>=` against integer constants.
   - maps compare operators to signed integer WAT ops (`i32.eq`, `i32.ne`, `i32.lt_s`, `i32.gt_s`, `i32.le_s`, `i32.ge_s`).
- Added focused fixture:
   - `compiler/tests/fixtures/032_wat_if_compare_return.cpp`
   - Validates compare-op lowering path (example: `x > 0`) in emitted WAT.

## Phase 19 Progress (Current)

- Extended WAT simple-if compare lowering to support parameter-vs-parameter comparisons:
   - pattern: `if (x OP y) { return A; } return B;` and `if/else` equivalent
   - enforces compatible integer WAT parameter types before emitting compare op.
- Added focused fixture:
   - `compiler/tests/fixtures/033_wat_if_param_compare_return.cpp`
   - Validates emitted sequence `local.get $x`, `local.get $y`, `i32.gt_s`, and structured `if (result i32)`.

## Phase 20 Progress (Current)

- Extended conditional return inference to include ternary form in global functions:
   - pattern: `return (x OP y_or_const) ? A : B;`
   - reuses the same structured compare + `if (result i32)` WAT emission path.
- Added focused fixture:
   - `compiler/tests/fixtures/034_wat_ternary_compare_return.cpp`
   - Validates ternary compare lowering to signed integer compare op and structured `if` result.

4. WAT backend expansion:
   - Generate function stubs for global functions systematically.
   - Incremental lowering for arithmetic, calls, and local variable flow.

5. Regression strategy:
   - Keep `compiler/test.cpp` as mandatory minimum compile target (`Parser: ok`).
   - Add progressive integration fixtures for each priority above.
