# Legacy example C89 diagnostics

2026-10-22. Owner-approved second slice after multi-file cat. Principal MaiaCpp
baseline 1a15d4c (1.6.1); implementation and local validation, without a new
release version, npm package or implied publication authorization.

## Corrections

The C89 bridge prelude declares printf with its standard variadic signature,
including semantic-token detection in AST-strict mode. Legacy mode already emits
a bridge prelude and now includes printf alongside sprintf. No generated parser
or emitted example is manually edited.

Void fallthrough guards compare the sanitized C return type. Source metadata such
as `static void` must not append `return (void)0;`, which strict C89 rejects.

Object constructors previously emitted executable calls between local declarations.
Later declarations now open nested declaration regions; constructor/scalar
initialization order and earlier object visibility are preserved. The legacy
C-style emitter likewise scopes later declarations using identities from the
generated C++ AST and semantic type table. It does not hoist declarations across
initializers or earlier global-variable uses. Signature-hint merging retains the
generated body AST, instead of discarding that structural information. Grammar,
parser ownership and strict AST lowering remain intact.

## Regression evidence

`compiler/tests/test-legacy-c89-output-contracts.js` is part of `npm test`:
- Four real fixtures (functions/classes/inheritance/casts) generated in the same
  legacy-hint mode as the documented CLI, accepted by strict Clang C89, and
  compared against original native C++98 and actual MaiaC-produced WASM output.
- Static void function, native C++98/C89 execution and actual WASM printf ABI.
- Constructor/scalar initializer effects and earlier object visibility in C89
  and WASM; late local shadowing preserves an earlier global binding.
- The small WASM test adapter implements printf; unused legacy bridge imports
  throw if invoked. This is unit execution, not exception/allocator validation.

The suite had two stale expected_output files. Original native C++98 execution
matches current generated guest output for classes and casts. The expected files
were corrected from that independent native oracle, and the regression now
requires native C++98, generated C89, WASM and reference fixture equality. Input
examples and their source checks were not deleted or weakened. This does not
claim support for dynamic_cast cases absent from the actual casts source.

`npm test` PASS. Targeted contracts PASS. Versioned examples were regenerated with
`bash compiler/examples/suite/build_all.sh` (11 built) and executed with
`bash compiler/examples/suite/run_all.sh`: 11/11 PASS, no failures/skips/drift.
`npm run check:maiaCC-selfhost-wasm` PASS: three stable complete bootstrap
generations, REx/XML/sample/WAT/shell exact XML and C++/JavaScript backend parity,
invalid-input rejection, stack restoration and 24 actual memory growths.
The isolated final-generation WAT frontend succeeded with status 0 and 143711
XML characters, taking 1671746 ms in this run. This is consistency evidence,
not a satisfactory performance claim. Dense regex memo allocation/zeroing on
each attempted token match is a concrete performance concern to profile in the
owning MaiaRegex/MaiaCC before proposing a separate optimization. No recursion
overflow was observed. Final parity-phase timing logs were added for subsequent
runs; they do not change assertions or guest execution.
All tracked generated example outputs follow the owner's GitHub/mobile review
policy and must accompany the reviewed compiler fix when publication is approved.
