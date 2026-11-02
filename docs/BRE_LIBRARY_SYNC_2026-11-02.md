# Additive BRE dependency propagation

2026-11-02. All four updated root gitlinks (MaiaC3007495, MaiaCCfe00596,
MaiaWASM8f5d02b, MaiaRegex8a1dcbf) were committed after completed git_pull and
before downstream tests. No compiler/grammar/generated parser change.

npm test passed the default suite: generated parser/lexer/resources in actual
WASM, pointer members, local structs, UTF-8/static lifetime/unsigned operations,
AST/C89 contracts and the unchanged38-check ordered Regex guest. Independently,
npm run check:maiaCC-selfhost-wasm passed three complete stable guest generations,
exact XML/source parity for both emitters/seven grammars, invalid input checks,
frame restoration and27 memory growths. Both suites completed successfully.
They used independent temporary directories while running concurrently.

The unchanged ordered runtime remains the generator runtime; this synchronization
does not claim additional BRE execution coverage. The owner records native/
sanitized/own-WASM BRE acceptance separately. Node reference WASM evidence is
separate from MaiaWASMVM/device conformance. Regex development tools/maiacpp is
intentionally uninitialized here (`update=none`), preserving its historical lock.
No npm/version/history or root MaiaOS publication change.
