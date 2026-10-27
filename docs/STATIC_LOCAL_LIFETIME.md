# Static locals in the AST C89 backend

Local `static` declarations retain static storage duration in generated C89.
Zero initialization and supported constant initializers stay on the declaration;
they are not rewritten into assignments executed on each function entry.
The correction covers scalars, fixed arrays and plain aggregates without bases,
constructors or methods. Existing block boundaries and identifier scopes remain.
Non-static declarations retain their existing lowering.

The bounded constant-initializer path accepts literal/string constants, nested
initializer lists, arithmetic/logical constant expressions and supported casts.
It preserves represented const qualification. Symbolic initializers, calls,
runtime reads, parenthesized direct initialization and nontrivial static objects
are outside this path and receive an explicit unsupported-static-initialization
diagnostic. This is not an implementation of C++ dynamic initialization guards,
thread-safe initialization or static constructors/destructors.

The original defect turned `static S g={19,77};` into automatic `S g;` followed
by field assignments on every call. Two calls consequently failed a native
C++98 lifetime expectation even when the emitted C89 was compiled by Clang,
independently of MaiaC. Keep this regression alongside the direct-global-member
cases that exercise the published MaiaC addressable-storage correction.

`node compiler/tests/test-static-struct-members.js` compares native C++98,
independently compiled emitted C89 and actual own-submodule WASM. Native paths
use ASan/UBSan. The guest links the repository-local MaiaC allocator, with host
imports forwarding only allocation/memory operations to that guest; no allocation
or static-initialization policy is implemented by the host. It checks repeated
calls, static arrays/structs/scalars, zero-fill, aliases, nested blocks, unsigned
updates, callbacks and const reads, plus a valid dynamic C++ initializer that
must be explicitly rejected by this bounded backend.

No generated grammar/parser, compiler API or runtime ABI is changed.
