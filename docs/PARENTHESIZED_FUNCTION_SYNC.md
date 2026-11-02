# MaiaC parenthesized function identity — consumer validation

2026-11-02. MaiaC9793ab9 is consumed through this repository's own maiac
submodule. Its gitlink was recorded before downstream tests. No MaiaCpp
compiler or generated parser change was needed.

`npm test` passed the complete default suite, including generated C++98
parser/lexer/resources through strict C89 and actual WASM, pointer members,
local structs, UTF-8, static lifetime, unsigned arithmetic and the existing
38-check ordered Regex guest.

`npm run check:maiaCC-selfhost-wasm` passed: three stable complete guest
generations, exact XML/source parity for both emitters across seven grammars,
invalid-input checks, frame restoration and 27 memory growths. The owning MaiaC
regression independently passed 39 positive and five negative native C89,
ASan/UBSan and WASM cases, followed by its full suite and example validation.

This is Node reference WebAssembly evidence, not MaiaWASMVM/device conformance.
No npm, version, public API or history changes; Regex/OS publication is excluded.
