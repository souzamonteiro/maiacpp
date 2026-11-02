# Published BRE prerequisites — consumer validation

2026-11-02. This repository consumes MaiaCC4cf1083, MaiaWASM9a1c1ce and
MaiaC6c01465 through its own synchronized submodules. Owner implementation edits
remain in those principals; MaiaCpp adds integration tests, not another fix.

`compiler/tests/test-parser-resource-wasm.js` generates syntax using its local
published MaiaCC and compiles that owner's exact resource-test fixture through
MaiaCpp AST-strict C++98, strict wasm32 C89 and MaiaC/MaiaWASM. C89 validation
supplies the standard stddef.h declaration for sizeof(size_t); actual linkage
uses repository-local C89 stdlib/string and the unchanged ordered Regex runtime.
Only memory/allocator transport is imported. The guest passes32 checks x50 calls,
including real size_t memo-size overflow, NOMEM injection, valid/invalid syntax,
retry/reset, collector rollback and frame restoration. No host parser/matcher
performs those operations.

`compiler/tests/test-indexed-pointer-members.js` compares native C++98/ASan/UBSan
with emitted strict C89 and actual WASM for char*, int* and struct-pointer members
of indexed structs, including negative offsets and derived additive expressions.
Its allocator/memory imports delegate to the linked C89 library, without mocks.
Both tests join npm test. Native C89 coverage remains the owning MaiaC regression
(20 positive and2 negative cases); the owning MaiaCC native fixture has32 checks.

Validation passed: npm test and npm run check:maiaCC-selfhost-wasm. The complete
guest bootstrap has three stable generations, both backends/seven grammars with
exact source parity, invalid-input checks and27 real memory growths. This is reference WebAssembly execution, not a claim
about MaiaWASMVM/device conformance or a new npm/version release.
