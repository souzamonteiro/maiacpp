# UTF-8 narrow source strings

Ordinary narrow C++98 string literals accept raw Unicode scalar characters.
The generated frontend retains their source spelling through AST-strict C89
emission; the repository-local MaiaC backend encodes raw scalars as UTF-8 bytes.
No normalization occurs. Numeric byte escapes remain explicit bytes, so raw `é`
and `\xE9` have different sizes and byte contents.

Regenerate from the owning grammar with:

```sh
./maiacc/bin/tREx.sh --ebnf --to-xml ./grammar/Cpp.xml ./grammar/Cpp.ebnf ./compiler/cpp-parser.js
```

`node compiler/tests/test-utf8-string-literals.js` compares native C++98, emitted
strict C89 (both with ASan/UBSan), and actual WASM with independent numeric byte
expectations. It exercises pointers, local/static/global arrays, aggregate fields,
sizeof, canaries, combining marks, supplementary/boundary scalars, numeric escapes,
embedded NULs, adjacent fragments, generated token spans and isolated surrogate
rejection. The regression is part of `npm test`.

Wide strings, raw Unicode character constants, new universal-character-name
semantics, execution locale selection and CLI file-decoder changes are outside
this selected scope. No fallback parser or host-side string storage is added.

Local narrow arrays initialized by literal fragments retain those fragments in
the C89 declaration, including inferred sizes. They must not become assignments
to arrays. This also repairs the equivalent ASCII case; the same byte/sizeof
regression exercises both explicit and inferred bounds.

The plain-struct helpers retain the standard __malloc/__free declarations.
The focused WASM harness supplies rejecting callbacks and asserts exactly those
imports, proving this literal-storage corpus never invokes host allocation.
The native emitted-C check runs with sanitizers and no replacement literal
implementation. U+007F is exercised through its existing octal spelling; raw
ASCII-control acceptance is unchanged.

Validation (2026-10-27): `npm test`, `npm run ready:full` (103 tier checks,
46 fixtures and parity/stress/dist checks), and `npm run check:maiaCC-selfhost-wasm`
all pass. The complete MaiaCC guest produces three stable bootstrap generations
and byte-identical C++/JavaScript emitters on seven grammars, with 27 memory
growths. Parser/XML regeneration reproduces both hashes exactly. All 15 recursive
gitlinks resolve to published commits, including MaiaC 4fc3ee9. Reference WASM
validation does not imply MaiaWASMVM or device conformance.
