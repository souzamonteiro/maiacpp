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
- Added namespace mangling regression fixture:
   - `compiler/tests/fixtures/010_namespace_class_mangling.cpp`
   - Validates namespace-prefixed class symbols (`N_C_init`, `N_C_destroy`) in compiler output.

3. Codegen C expansion:
   - Emit non-stub bodies for core expressions/statements used by baseline tests.
   - Tighten behavior for parser failures (strict mode option).

4. WAT backend expansion:
   - Generate function stubs for global functions systematically.
   - Incremental lowering for arithmetic, calls, and local variable flow.

5. Regression strategy:
   - Keep `compiler/test.cpp` as mandatory minimum compile target (`Parser: ok`).
   - Add progressive integration fixtures for each priority above.
