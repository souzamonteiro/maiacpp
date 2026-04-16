# C++98 Standard Library Implementation Proposal for WASM Transpiler

## Overview

This document outlines how the C++98 standard library surface in MaiaCpp should be modeled for a `C++98 -> C89 -> WASM` toolchain.

The core principle is:

- MaiaCpp is a transpiler.
- The final C headers and runtime libraries for the C subset should reuse MaiaC.
- C++ headers in `include/` should provide the C++98-facing declarations/macros/types expected by source programs.
- Whenever possible, implementations should be provided in:
  - C, when the feature maps naturally to the MaiaC runtime,
  - C++, when a thin C++ wrapper is enough,
  - WAT, when stack/runtime/ABI control is required,
  - JavaScript, when the functionality is host-facing and MaiaC already uses JS imports.

## Current Snapshot (2026-04-16)

### Header Inventory in `include/`

The following C++98 headers are currently present:

- C compatibility headers: `cassert`, `cctype`, `cerrno`, `cfloat`, `ciso646`, `climits`, `clocale`, `cmath`, `complex`, `csetjmp`, `csignal`, `cstdarg`, `cstddef`, `cstdio`, `cstdlib`, `cstring`, `ctime`, `cwchar`, `cwctype`
- Core/runtime headers: `exception`, `new`, `stdexcept`, `typeinfo`
- I/O headers: `ios`, `iosfwd`, `iostream`, `istream`, `ostream`, `streambuf`, `iomanip`, `sstream`, `fstream`
- Containers/algorithms/utilities: `algorithm`, `bitset`, `deque`, `functional`, `iterator`, `limits`, `list`, `locale`, `map`, `memory`, `numeric`, `queue`, `set`, `stack`, `string`, `utility`, `valarray`, `vector`

### Completeness Summary

#### 1. File set completeness

At the level of **header names**, the `include/` directory is broadly aligned with the expected C++98 library surface.

#### 2. Semantic completeness

At the level of **usable declarations and implementation-defined values**, the headers were incomplete.

Main issues found during audit:

- many `#define` entries were left as empty `/* implementation-defined */` placeholders,
- several `typedef /* implementation-defined */ name;` declarations were not valid C++ declarations,
- some headers did not yet reflect the MaiaC runtime profile used by the generated C/WASM pipeline,
- wide-character and locale-related areas remain only partially modeled and should stay provisional until the corresponding library/runtime is created.

## WASM / MaiaC Compatibility Profile

For C-derived compatibility headers, MaiaCpp should follow the MaiaC runtime profile unless there is a strong C++-specific reason not to.

### Adopted values from MaiaC-compatible profile

#### Numeric limits

From the current MaiaC compatibility model:

- `CHAR_BIT = 8`
- `CHAR_MAX = 127`
- `CHAR_MIN = -128`
- `INT_MAX = 32767`
- `INT_MIN = -32767`
- `LONG_MAX = 2147483647L`
- `LONG_MIN = -2147483647L`
- `MB_LEN_MAX = 1`
- `UINT_MAX = 65535U`
- `ULONG_MAX = 4294967295UL`
- `USHRT_MAX = 65535U`

#### Floating-point model

Current MaiaC-compatible profile:

- `FLT_RADIX = 2`
- `FLT_ROUNDS = 1`
- `FLT_DIG = 6`
- `DBL_DIG = 10`
- `LDBL_DIG = 10`
- `FLT_MAX = 1e37`
- `DBL_MAX = 1e37`
- `LDBL_MAX = 1e37`
- `FLT_MIN = 1e-37`
- `DBL_MIN = 1e-37`
- `LDBL_MIN = 1e-37`
- `HUGE_VAL = 1e37`

These values should be treated as the current runtime contract for MaiaCpp while it targets MaiaC.

#### Locale/signal/stdio/stdlib/time profile

Current chosen profile:

- locale categories identical to MaiaC (`LC_ALL = 0`, etc.),
- `sig_atomic_t = int`,
- signal constants identical to MaiaC (`SIGABRT = 1`, ... `SIGTERM = 6`),
- stdio constants identical to MaiaC (`BUFSIZ = 1024`, `EOF = -1`, `SEEK_SET = 0`, etc.),
- `EXIT_SUCCESS = 0`, `EXIT_FAILURE = 1`, `RAND_MAX = 32767`, `MB_CUR_MAX = 1`,
- `CLOCKS_PER_SEC = 1000000`.

## Header Status Matrix

| Header | Present | Audit status | Current interpretation |
|---|---|---|---|
| `cassert` | ✅ | not yet audited in detail | likely wrapper/header-only |
| `cctype` | ✅ | not yet audited in detail | should track MaiaC `ctype` |
| `cerrno` | ✅ | adjusted | now bound to MaiaC-style errno profile |
| `cfloat` | ✅ | adjusted | concrete WASM values assigned |
| `ciso646` | ✅ | not yet audited in detail | likely header-only |
| `climits` | ✅ | adjusted | concrete WASM values assigned |
| `clocale` | ✅ | adjusted | category constants assigned; runtime still library-backed |
| `cmath` | ✅ | adjusted | `HUGE_VAL` assigned; functions should map to MaiaC math/runtime |
| `complex` | ✅ | pending | likely library work needed |
| `csetjmp` | ✅ | adjusted | mapped to MaiaC-compatible `jmp_buf` profile |
| `csignal` | ✅ | adjusted | concrete signal model assigned |
| `cstdarg` | ✅ | adjusted | MaiaC-compatible `va_list`/macro model adopted |
| `cstddef` | ✅ | adjusted | `ptrdiff_t`, `size_t`, `NULL`, `offsetof` assigned |
| `cstdio` | ✅ | adjusted | constants/types/macros aligned with MaiaC profile |
| `cstdlib` | ✅ | adjusted | constants/types aligned with MaiaC profile |
| `cstring` | ✅ | adjusted | `NULL` assigned; functions should reuse MaiaC string runtime |
| `ctime` | ✅ | adjusted | `clock_t`, `time_t`, constants aligned with MaiaC profile |
| `cwchar` | ✅ | provisional | wide-char model still pending runtime/library decisions |
| `cwctype` | ✅ | provisional | wide-char classification model still pending |
| `string` | ✅ | adjusted/provisional | declaration layer improved; still needs real runtime + `char_traits`/STL support |
| `exception` | ✅ | audited | declaration-only header is acceptable as a first-layer interface |
| `new` | ✅ | audited | declaration-only header is acceptable; still needs runtime allocation/handler semantics |
| `stdexcept` | ✅ | audited | declarations are structurally fine; depends on `string` + exception runtime |
| `typeinfo` | ✅ | audited | declarations are structurally fine; RTTI runtime still pending |
| `limits` | ✅ | adjusted | practical `numeric_limits` specializations added for built-in scalar types |
| `iosfwd` | ✅ | adjusted/provisional | now provides foundational `fpos` and basic `char_traits` specializations |
| `ios` | ✅ | adjusted/provisional | placeholder scalar/types sanitized; full stream semantics still pending |
| `locale` | ✅ | adjusted/provisional | placeholder token sanitized; facets/traits/runtime still pending |
| `memory` | ✅ | audited/provisional | allocator declarations are coherent; allocation semantics still pending |
| `streambuf` | ✅ | audited/provisional | declaration layer looks coherent; runtime semantics still missing |
| `istream` | ✅ | adjusted/provisional | declaration layer coherent after fixing dependency on `ostream` |
| `ostream` | ✅ | audited/provisional | declaration layer looks coherent; output semantics still missing |
| `sstream` | ✅ | audited/provisional | declaration layer looks coherent; depends on `string` and streams runtime |
| `iostream` | ✅ | audited/provisional | standard object declarations acceptable as interface layer |
| `fstream` | ✅ | audited/provisional | declaration layer coherent; filesystem/host/runtime still missing |
| `cassert` | ✅ | audited/provisional | only remaining issue is implementation-defined assert expansion |
| `cctype` | ✅ | audited | declaration layer acceptable and aligned with MaiaC-style C wrappers |
| `ciso646` | ✅ | audited | acceptable compatibility header; no action needed |
| `complex` | ✅ | audited/provisional | declaration layer acceptable; math/runtime implementation still pending |
| `deque` | ✅ | audited/pending | still contains spec placeholders for iterator and size-related typedefs |
| `list` | ✅ | audited/pending | still contains spec placeholders for iterator and size-related typedefs |
| `vector` | ✅ | audited/pending | still contains spec placeholders for iterator/size typedefs and `vector<bool>` proxy types |
| `map` | ✅ | audited/pending | still contains spec placeholders for iterator and size-related typedefs |
| `set` | ✅ | audited/pending | still contains spec placeholders for iterator and size-related typedefs |
| `iomanip` | ✅ | audited/pending | still uses `unspecified` proxy manipulator return types from the standard text |
| container/IO/STL headers | ✅ | largely unaudited | need staged review and implementation planning |

## What Was Adjusted Immediately

The following headers were updated to remove empty implementation-defined placeholders and make them usable as an actual profile:

- `include/cerrno.h`
- `include/cfloat.h`
- `include/climits.h`
- `include/clocale.h`
- `include/cmath.h`
- `include/csetjmp.h`
- `include/csignal.h`
- `include/cstdarg.h`
- `include/cstddef.h`
- `include/cstdio.h`
- `include/cstdlib.h`
- `include/cstring.h`
- `include/ctime.h`
- `include/cwchar.h`
- `include/cwctype.h`

Additional second-pass improvements:

- `include/string.h` — made syntactically consistent as a declaration-only interface by adding `<iterator>` and giving `iterator`/`const_iterator` a provisional contiguous-storage interpretation.
- `include/limits.h` — added practical `numeric_limits` specializations for built-in scalar types aligned with the current MaiaC/WASM profile.

Additional third-pass improvements:

- `include/ios.h` — replaced abstract placeholder scalar aliases and category typedefs (`OFF_T`, `SZ_T`, `T1`...`T4`) with a provisional integral WASM/MaiaC-compatible profile for `streamoff`, `streamsize`, `fmtflags`, `iostate`, `openmode`, and `seekdir`.
- `include/locale.h` — replaced the remaining `implementation_defined` token in `ctype<char>::table_size` with a provisional concrete value (`256`).

Additional fourth-pass improvements:

- `include/istream.h` — added `<ostream>` so `basic_iostream` no longer derives from an incompletely defined `basic_ostream`.

Additional fifth-pass improvements:

- `include/iosfwd.h` — added a foundational `fpos` template and concrete `char_traits<char>` / `char_traits<wchar_t>` specializations with provisional WASM/MaiaC-compatible scalar types and basic inline trait operations.
- `include/string.h` — removed redundant local `char_traits` forward declarations and now relies on the centralized `iosfwd` trait layer.

## Audit Closure: remaining unresolved specification-style headers

At this point, the audit of the header set is functionally complete.

The main remaining unresolved files are no longer broad unknowns; they are a specific set of headers whose outstanding issues are now well identified:

### 1. Container headers still carrying spec placeholders

These headers still contain raw standard-text placeholders such as `typedef implementation defined iterator;` and similar entries:

- `include/deque.h`
- `include/list.h`
- `include/vector.h`
- `include/map.h`
- `include/set.h`

Interpretation:

- these headers are present and useful as a design surface,
- but they are not yet valid implementation-oriented C++ headers,
- they should be finalized together with the concrete container implementations.

### 2. Manipulator header still using `unspecified`

`include/iomanip.h` still exposes specification-style placeholders such as:

- `typedef unspecified T1;`
- `typedef unspecified T2;`
- ...

Interpretation:

- this is standard-spec wording, not implementable header wording,
- it should be replaced during implementation with concrete lightweight manipulator wrapper types.

### 3. `cassert`

`include/cassert.h` is almost complete, but the active `assert(expression)` expansion is still left as `implementation-defined`.

Interpretation:

- this is a small remaining runtime/header task,
- it can likely be implemented as a MaiaC-compatible abort/report macro during the implementation phase.

### 4. Audited and acceptable as declaration layers

The following headers were reviewed in this closing pass and do not currently present additional audit blockers beyond missing runtime semantics:

- `include/cctype.h`
- `include/ciso646.h`
- `include/complex.h`

## Final audit summary

### Headers now reasonably normalized for audit purposes

These headers are no longer blocked by empty placeholders or abstract spec tokens and can be treated as audited interface layers:

- C compatibility core: `cerrno`, `cfloat`, `climits`, `clocale`, `cmath`, `csetjmp`, `csignal`, `cstdarg`, `cstddef`, `cstdio`, `cstdlib`, `cstring`, `ctime`, `cwchar`, `cwctype`
- foundational runtime/STL support: `iosfwd`, `ios`, `locale`, `memory`, `limits`, `string`, `exception`, `new`, `stdexcept`, `typeinfo`
- stream chain: `streambuf`, `istream`, `ostream`, `sstream`, `iostream`, `fstream`
- simple surface headers: `cctype`, `ciso646`, `complex`

### Headers still intentionally deferred to implementation phase

- `cassert`
- `deque`
- `list`
- `vector`
- `map`
- `set`
- `iomanip`

These are now known work items rather than unknown audit gaps.

## Second-Pass Audit: `string`, `exception`, `new`, `stdexcept`, `typeinfo`, `limits`

### `string`

Status: **provisional but improved**.

Findings:

- the header had invalid placeholder syntax for `iterator` and `const_iterator`,
- it relied on `reverse_iterator` without including `<iterator>`,
- it is still declaration-only and does not yet define storage, ownership, small-string policy, allocator interaction, or `char_traits` behavior.

Immediate action taken:

- included `<iterator>`,
- mapped `iterator` to `pointer` and `const_iterator` to `const_pointer` as a provisional contiguous-storage model.

Remaining dependency blockers:

- functioning allocator/runtime semantics,
- stream integration,
- exception behavior for bounds/length failures.

### `exception`

Status: **acceptable as declaration layer**.

Findings:

- the header is structurally reasonable for a first phase,
- no implementation-defined placeholders were blocking use,
- real behavior still depends on the MaiaCpp/MaiaC exception model and runtime support.

Recommended implementation direction:

- implement the base class in C++ with static message storage,
- map termination/unexpected hooks either to C/WAT runtime or host-assisted runtime logic.

### `new`

Status: **acceptable as declaration layer**.

Findings:

- declarations are structurally coherent,
- operational semantics still depend on the allocator strategy and the `operator new`/`operator delete` bridge to MaiaC allocation.

Recommended implementation direction:

- route global allocation operators to MaiaC `malloc`/`free`,
- keep `nothrow` and `new_handler` behavior as a thin C++ runtime layer.

### `stdexcept`

Status: **structurally fine, runtime-pending**.

Findings:

- declarations are fine,
- the header depends on a working `string` representation and exception object model.

Recommended implementation direction:

- implement these classes as thin wrappers over `exception` with stored message payloads,
- prefer simple heap/string ownership semantics first; optimize later.

### `typeinfo`

Status: **structurally fine, RTTI-pending**.

Findings:

- declarations are fine,
- practical use depends on RTTI metadata generation and object model support in the transpiler/runtime.

Recommended implementation direction:

- keep the current declaration layer,
- defer full semantics until RTTI emission is specified in the transpiler.

### `limits`

Status: **improved significantly**.

Findings:

- the header originally exposed only the generic template skeleton,
- real-world code needs concrete specializations for built-in types.

Immediate action taken:

- added `numeric_limits` specializations for:
  - `bool`
  - `char`, `signed char`, `unsigned char`
  - `wchar_t`
  - `short`, `int`, `long`
  - `unsigned short`, `unsigned int`, `unsigned long`
  - `float`, `double`, `long double`

These specializations currently reflect the MaiaC/WASM compatibility profile, not a host-native C++ ABI.

## Transitive Blockers Found During This Audit

While auditing the six priority headers, additional upstream blockers became visible:

- the iostream family is still not semantically usable even where forward declarations exist,
- allocator, stream, locale, and exception semantics are still mostly declarative.

This means `string`, `sstream`, `bitset`, and the stream family can be declared, but not considered implementation-ready yet.

## Third-Pass Audit: `ios` and `locale`

### `ios`

Status: **provisional but materially improved**.

Findings before adjustment:

- the header still contained abstract specification placeholders for core scalar categories,
- `streamoff`, `streamsize`, `fmtflags`, `iostate`, `openmode`, and `seekdir` were not usable as C++ declarations.

Immediate action taken:

- assigned `streamoff` and `streamsize` to a provisional `long`-based model,
- assigned `fmtflags`, `iostate`, `openmode`, and `seekdir` to provisional `int`-based models,
- assigned concrete bitmask values for the common `ios_base` categories.

Remaining blockers:

- no actual implementation behind `ios_base` state management,
- stream buffer and stream class semantics still only declarative.

### `locale`

Status: **provisional but syntactically sanitized**.

Findings before adjustment:

- `ctype<char>::table_size` still used unresolved specification text.

Immediate action taken:

- assigned `table_size = 256` as a provisional WASM/MaiaC-compatible character table size.

Remaining blockers:

- locale facets still lack implementations,
- codecvt interaction is still unresolved at runtime level,
- the header remains declaration-heavy and implementation-light.

## Fifth-Pass Audit: `iosfwd`, `char_traits`, `fpos`, and allocator foundation

### General result

Status: **foundational layer now materially stronger**.

Findings before adjustment:

- `iosfwd.h` only forward-declared `char_traits` specializations,
- `streampos` and `wstreampos` depended on `char_traits<...>::state_type` without a concrete visible definition,
- multiple stream/string headers depended on trait members such as `int_type`, `pos_type`, `off_type`, and `eof()`.

Immediate action taken:

- added a concrete `fpos` template in `iosfwd.h`,
- added concrete `char_traits<char>` and `char_traits<wchar_t>` specializations,
- provided provisional scalar/profile choices for:
  - `int_type`
  - `off_type`
  - `state_type`
  - `pos_type`
- provided basic inline trait operations such as:
  - `assign`
  - `eq` / `lt`
  - `compare`
  - `length`
  - `find`
  - `move`
  - `copy`
  - `to_char_type`
  - `to_int_type`
  - `eq_int_type`
  - `eof` / `not_eof`

### `iosfwd`

Status: **improved from pure forward-declaration header to usable foundation layer**.

This is important because it allows downstream headers to refer to `traits::int_type`, `traits::pos_type`, `traits::off_type`, and `traits::eof()` without being structurally hollow.

### `memory` / allocator

Status: **still declaration-only but structurally acceptable**.

Findings:

- allocator declarations are coherent,
- no placeholder cleanup was needed in this pass,
- actual allocation semantics still need implementation and integration with MaiaC `malloc`/`free`.

### Remaining blockers after the foundation pass

- `basic_string` still lacks a real storage/ownership implementation,
- `ios_base`, `basic_streambuf`, `basic_istream`, and `basic_ostream` still lack semantic/runtime implementations,
- locale facets remain declaration-heavy,
- allocator behavior is still not wired to the MaiaC runtime.

## Fourth-Pass Audit: stream chain (`streambuf`, `istream`, `ostream`, `sstream`, `iostream`, `fstream`)

### General result

Status: **structurally better than earlier layers**.

Findings:

- no remaining abstract tokens like `OFF_T`, `SZ_T`, `T1`...`T4`, or `implementation_defined` were found in the audited stream headers,
- the main structural issue found was header dependency ordering: `basic_iostream` was defined in `istream.h` while inheriting from `basic_ostream` without including `ostream.h`,
- after adding `<ostream>` to `istream.h`, the chain became more coherent as a declaration-only surface.

### `streambuf`

Status: **acceptable as declaration layer**.

Findings:

- the header is structurally coherent,
- it still depends on missing runtime implementation for buffer pointers, get/put areas, and locale interaction.

### `istream`

Status: **improved structurally**.

Immediate action taken:

- included `<ostream>` so `basic_iostream` has a complete `basic_ostream` base class at declaration time.

Remaining blockers:

- extraction semantics,
- sentry behavior,
- interaction with `num_get`, `streambuf`, and `ios_base` state.

### `ostream`

Status: **acceptable as declaration layer**.

Findings:

- declarations are structurally coherent,
- practical use still depends on `streambuf`, `num_put`, width/fill/flags behavior, and host/file output layers.

### `sstream`

Status: **acceptable as declaration layer**.

Findings:

- declarations are coherent,
- functionality depends on a real `basic_string` implementation and working `streambuf`/`ios_base` semantics.

### `iostream`

Status: **acceptable as declaration layer**.

Findings:

- it is basically an object declaration header,
- real usefulness depends entirely on the underlying stream runtime.

### `fstream`

Status: **acceptable as declaration layer**.

Findings:

- declarations are coherent,
- real implementation will require either:
  - a MaiaC-backed stdio/file abstraction,
  - or host-backed filesystem bridges in JavaScript for Node/browser environments.

## Remaining Work

### 1. Audit all remaining headers for invalid placeholders

The next pass should cover:

- `cassert`, `cctype`, `ciso646`
- `complex`
- semantic/runtime layer for `ios`, `streambuf`, `istream`, `ostream`, `sstream`, `fstream`
- allocator/runtime wiring layer
- containers/algorithms/iterators/utilities

### 2. Reconcile C++ wrappers with MaiaC headers

Where a `c*` header is just a C++ view of a C89 header, the preferred long-term approach is:

- keep declarations/types/macros ABI-compatible with MaiaC,
- expose names inside `namespace std`,
- avoid duplicating runtime policy in multiple places when a single shared definition can be generated or included.

### 3. Wide-character policy

`cwchar` and `cwctype` still require a real design decision.

Recommended current policy:

- keep the provisional values already assigned so headers are syntactically usable,
- document them as provisional,
- finalize only when the corresponding wide-character library/runtime is implemented.

Questions still open:

- should `wchar_t` semantics follow UTF-32-like code units?
- should conversion functions be minimal UTF-8 adapters in C?
- should classification/transcoding be delegated partly to JavaScript host helpers?

### 4. I/O and STL layers

The STL-like headers (`string`, containers, streams, locale, iterators, allocators) should be implemented in phases.

Recommended order:

1. `string`
2. `exception`, `new`, `stdexcept`, `typeinfo`
3. `iosfwd`, `ios`, `streambuf`, `istream`, `ostream`, `iostream`
4. containers: `vector`, `list`, `deque`, `map`, `set`, `queue`, `stack`
5. algorithms/utilities: `algorithm`, `functional`, `iterator`, `memory`, `numeric`, `utility`
6. `complex`, `valarray`, `locale`, `bitset`, `sstream`, `fstream`

## Implementation Strategy

### Tier 1: Reuse MaiaC directly

These should map directly onto MaiaC C89 headers/libraries/runtime behavior:

- `cerrno`, `cfloat`, `climits`, `clocale`, `cmath`, `csetjmp`, `csignal`, `cstdarg`, `cstddef`, `cstdio`, `cstdlib`, `cstring`, `ctime`

### Tier 2: Thin C++ wrappers over MaiaC/runtime pieces

These should be implemented mostly in C++ headers/source using MaiaC functionality underneath:

- `string`
- `exception`, `new`, `stdexcept`, `typeinfo`
- stream base types and iostream glue
- `limits` specializations for concrete built-in types

### Tier 3: Special/host-backed areas

These may need mixed implementation in C/WAT/JavaScript:

- wide-char support (`cwchar`, `cwctype`)
- locale-heavy behavior
- filesystem-backed stream functionality
- exception/runtime support that depends on MaiaC WASM lowering details

## Recommendation

Short term:

1. Treat the MaiaC C89 headers/runtime as the source of truth for C compatibility.
2. Keep filling MaiaCpp `c*` headers with MaiaC-compatible values and declarations.
3. Mark genuinely unresolved pieces as provisional instead of leaving empty placeholders.
4. Audit remaining non-`c*` headers and classify each as:
   - header-only now,
   - wrapper over MaiaC,
   - requires new C/C++ runtime code,
   - requires WAT/JS host support.

Medium term:

- generate part of the `c*` header layer from MaiaC metadata/templates to avoid drift,
- centralize implementation-defined target values in one profile document or generated config header.

## Conclusion

The `include/` directory is reasonably complete in terms of file coverage, but it was not yet complete in terms of usable C++98 header semantics. The immediate blocking issue was the presence of empty `implementation-defined` placeholders and invalid placeholder typedefs.

Those core C-compatibility headers have now been normalized to a concrete WASM/MaiaC profile. The next step is a staged semantic audit of the remaining STL/iostream/runtime headers and the creation of the corresponding library layers on top of MaiaC.
