# Local plain struct types in AST-strict C89 lowering

The AST-strict path gives each named local plain struct its own deterministic C
identity. The semantic analyzer attaches that identity to the generated class
node; declarations use it without flattening the class body into a type spelling.
Local names are resolved at their lexical declaration point and restored when
leaving compounds/for scopes. A local struct cannot overwrite a global or another
function's layout. C identities avoid every identifier present in the source.

The selected scope covers public data-only named structs, scalar/fixed-array
members, supported pointers (including self references), inline/separate/multiple
object declarators, existing scalar aggregate initialization and constant local
static storage. Types can refer to visible outer local types. sizeof resolves
local type names without rewriting ordinary values, member names or literals;
a value hiding the injected class name retains its own sizeof behavior.
Unsupported anonymous/nested/behavior-bearing local classes and dynamic local
static initialization receive explicit diagnostics. Out-of-scope local type uses
must not silently become int. Local typedefs/enums, templates, inheritance and
full C++98 local-class semantics remain outside this increment. The legacy
non-strict semantic path is retained; this is a scoped AST-strict correction.
No generated parser, grammar, compiler public API or guest/host ABI changes.

Original failures: `struct Local{int value;} object` was lowered as int object;
two function-local Local types with different fields shared one global layout.
Both native C++98 cases pass; their pre-correction emitted C89 fails independently
of MaiaC. Regression coverage preserves both and requires executable parity.

`npm run check:local-struct-types` is also the first step of npm test. Its latest
focused run passes 16 independent programs,6explicit unsupported forms and2invalid
out-of-scope uses. Checks include generated AST identities, deterministic output,
native C++98 and emitted C89 ASan/UBSan, strict wasm32 C89 and actual own
MaiaC/MaiaWASM execution,40repetitions per guest with stable stack/memory and
absence of runtime allocation. Both original regressions are preserved.

`npm test` and `npm run ready:full` pass, including 103 tier checks, 46 fixtures,
release equivalence/stress/distribution/native/browser-host gates. The complete
`npm run check:maiaCC-selfhost-wasm` passes three stable C++98 -> C89 -> WASM
bootstrap generations, exact frontend XML and both C++98/JavaScript emitters on
seven grammars, invalid-input rejection and27actual memory growths. This is
reference WASM validation, not MaiaWASMVM/device conformance. Logs:
/tmp/maiacpp-local-struct-{npm,ready,selfhost,final-focused}.log.
No npm publication or version change is included. Consumers must receive this
correction through tested, published submodule commits under the protocol.
