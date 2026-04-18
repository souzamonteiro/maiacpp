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

## MaiaC-first policy (mandatory)

From this point on, implementation work must follow this order of preference:

1. Reuse MaiaC as-is.
2. Add thin C++ wrappers/adapters over MaiaC behavior.
3. Reuse host functions already exposed by MaiaC/webc (Node/browser) via wrappers.
4. Implement only the remaining delta that cannot be expressed in C89/MaiaC or host wrappers.

Practical rule:

- If a feature already exists in MaiaC C headers/runtime, MaiaCpp must not reimplement it independently.
- If a feature is representable in C and can be lowered to MaiaC, prefer that route.
- If equivalent host functionality already exists in browser/node runtime bridges, prefer wrapper declarations over new C/C++/WAT implementations.
- Only C++-specific semantics (templates, overload sets, RAII object model, exceptions/RTTI semantics, stream/class behaviors) should have MaiaCpp-specific runtime code.

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
| `new` | ✅ | adjusted/provisional | global `new/delete` and `new_handler` now wired to MaiaC-style `malloc/free` backend |
| `stdexcept` | ✅ | audited | declarations are structurally fine; depends on `string` + exception runtime |
| `typeinfo` | ✅ | audited | declarations are structurally fine; RTTI runtime still pending |
| `limits` | ✅ | adjusted | practical `numeric_limits` specializations added for built-in scalar types |
| `iosfwd` | ✅ | adjusted/provisional | now provides foundational `fpos` and basic `char_traits` specializations |
| `ios` | ✅ | adjusted/provisional | placeholder scalar/types sanitized; full stream semantics still pending |
| `locale` | ✅ | adjusted/provisional | placeholder token sanitized; facets/traits/runtime still pending |
| `memory` | ✅ | adjusted/provisional | `allocator<T>` core operations now mapped to global `operator new/delete` |
| `streambuf` | ✅ | audited/provisional | declaration layer looks coherent; runtime semantics still missing |
| `istream` | ✅ | adjusted/provisional | declaration layer coherent after fixing dependency on `ostream` |
| `ostream` | ✅ | audited/provisional | declaration layer looks coherent; output semantics still missing |
| `sstream` | ✅ | audited/provisional | declaration layer looks coherent; depends on `string` and streams runtime |
| `iostream` | ✅ | audited/provisional | standard object declarations acceptable as interface layer |
| `fstream` | ✅ | audited/provisional | declaration layer coherent; filesystem/host/runtime still missing |
| `cassert` | ✅ | adjusted/provisional | assert macro now has concrete debug/release behavior |
| `cctype` | ✅ | audited | declaration layer acceptable and aligned with MaiaC-style C wrappers |
| `ciso646` | ✅ | audited | acceptable compatibility header; no action needed |
| `complex` | ✅ | audited/provisional | declaration layer acceptable; math/runtime implementation still pending |
| `deque` | ✅ | adjusted/provisional | typedef placeholders replaced with concrete provisional aliases |
| `list` | ✅ | adjusted/provisional | typedef placeholders replaced with concrete provisional aliases |
| `vector` | ✅ | adjusted/provisional | typedef placeholders replaced with concrete provisional aliases |
| `map` | ✅ | adjusted/provisional | typedef placeholders replaced with concrete provisional aliases |
| `set` | ✅ | adjusted/provisional | typedef placeholders replaced with concrete provisional aliases |
| `iomanip` | ✅ | adjusted/provisional | unspecified manipulator placeholders replaced with concrete wrapper structs |
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

## Implementation kick-off update (2026-04-16)

First implementation step after audit closure:

- `include/cassert.h` was converted from specification placeholder to a usable macro form:
  - `assert(expression)` now aborts in debug mode and compiles away under `NDEBUG`.
- `include/iomanip.h` was converted from `unspecified` placeholders to concrete manipulator wrapper structs and inline factories:
  - `resetiosflags`
  - `setiosflags`
  - `setbase`
  - `setfill`
  - `setprecision`
  - `setw`

After subsequent implementation passes, the remaining placeholder set was reduced to zero.

Current status of placeholder markers in `include/*.h`:

- `typedef implementation defined` → none
- `typedef unspecified` → none
- `/* implementation-defined */` in active placeholder form → none

This means the header cleanup/normalization phase is complete.

## Implementation pass: allocation bridge (`new` + `memory`) (2026-04-16)

Additional MaiaC-first implementation completed:

- added runtime definitions for global `operator new/delete` and array forms,
- wired allocation/deallocation to `std::malloc` / `std::free`,
- implemented `std::nothrow`, `std::set_new_handler`, and `std::bad_alloc::what()`,
- implemented `allocator<T>` core semantics in `memory.h`:
  - `allocate` -> `::operator new`
  - `deallocate` -> `::operator delete`
  - `construct` -> placement new
  - `destroy` -> explicit destructor call

Interpretation:

- this keeps backend allocation delegated to the C/MaiaC-compatible layer,
- while adding only the C++ interface semantics required by C++98.

## Implementation pass: exception/typeinfo/stdexcept runtime stubs (2026-04-18)

Additional MaiaC-first implementation completed:

- added `src/exception.cpp` with concrete base runtime behavior for:
  - `std::exception` / `std::bad_exception` constructors, assignment, destructors, and `what()`,
  - `set_terminate` / `terminate`,
  - `set_unexpected` / `unexpected`,
  - `uncaught_exception` (provisional behavior).
- added `src/typeinfo.cpp` with concrete runtime definitions for:
  - `std::type_info` core operators and naming stub,
  - `std::bad_cast` and `std::bad_typeid` lifecycle + `what()`.
- added `src/stdexcept.cpp` with constructor definitions for:
  - `logic_error`, `domain_error`, `invalid_argument`, `length_error`, `out_of_range`,
  - `runtime_error`, `range_error`, `overflow_error`, `underflow_error`.

Interpretation:

- this pass converts three previously declaration-only headers into linkable runtime surface,
- behavior remains intentionally minimal/provisional while the full exception lowering model is completed,
- no independent allocator/runtime stack was introduced; implementation remains aligned with MaiaC-first policy.

## Implementation pass: host-wrapper bridge policy in runtime (2026-04-18)

Additional policy-aligned implementation completed:

- added internal bridge header `include/maiacpp_host_bridge.h` to centralize opt-in wrappers for host imports already modeled by MaiaC/webc,
- connected `src/exception.cpp` runtime hooks to host-side console error wrappers (`std::unexpected` / `std::terminate` paths),
- kept host wrappers opt-in through `MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS` so native host builds can still compile without unresolved host imports.

Interpretation:

- this keeps MaiaCpp runtime thin and avoids reimplementing functionality already available in Node/browser hosts,
- wrapper ownership remains in MaiaCpp surface while transport/runtime plumbing remains in MaiaC/webc,
- linker behavior in MaiaWASM remains unchanged for this pass.

## Implementation pass: iostream runtime + host-wrapper convention guide (2026-04-18)

Work completed:

- created `src/iostream.cpp` implementing the standard stream objects (`cin`, `cout`, `cerr`, `clog`, `wcin`, `wcout`, `wcerr`, `wclog`):
  - introduced internal class `__MaiacppStdioBuf` — a concrete `basic_streambuf<char>` backed by MaiaC stdio file handles 1 (stdout), 2 (stderr), 3 (stdin),
  - `xsputn` and `overflow` delegate to MaiaC `fwrite`/`putchar` (MaiaC/webc routes these to the JS host write function),
  - `xsgetn` and `underflow` delegate to MaiaC `getchar`/`fread`,
  - wide-stream objects (`wcout`, etc.) are stub-initialized with null buffer (wide I/O not supported in WASM profile),
- extended `include/maiacpp_host_bridge.h` with additional host import wrappers:
  - `__console__warn` → `std::__maiacpp::host_console_warn`,
  - `__performance__now` → `std::__maiacpp::host_performance_now` (sub-ms timer not available in C89 `time.h`),
  - added policy documentation header comment listing which C89 stdlib areas MaiaC already covers natively,
- created `docs/HOST_WRAPPER_CONVENTION.md` as the authoritative conventions guide covering:
  - 4-step implementation priority decision tree,
  - `__namespace__funcName` naming convention and WAT import wiring,
  - `MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS` guard pattern and wrapper template,
  - table of what MaiaC C89 stdlib handles vs. what needs host wrappers,
  - current wrapper inventory with usage references.

Interpretation:

- iostream is now a complete runtime layer (not just declaration-only): cout/cerr/clog route to MaiaC stdio, which MaiaC/webc routes to the JS host `write` function; no WAT/C reimplementation was needed,
- the host bridge stays thin: two new wrappers added, all other I/O remains via MaiaC C89 stdlib path,
- fstream runtime completed in same pass (see below).

## Implementation pass: fstream runtime (2026-04-18)

Work completed:

- created `src/fstream.cpp` with explicit `char` specialisations for all four fstream classes:
  - `basic_filebuf<char>`: constructor/destructor, `is_open`, `open`, `close`; virtuals `underflow`/`uflow`/`xsgetn`/`pbackfail` (input) and `overflow`/`xsputn` (output) delegate to MaiaC `fgetc`/`fread`/`ungetc`/`fputc`/`fwrite`; `seekoff`/`seekpos` delegate to MaiaC `fseek`/`ftell`; `sync` delegates to MaiaC `fflush`,
  - `basic_ifstream<char>` / `basic_ofstream<char>` / `basic_fstream<char>`: constructors (default + filename/mode), `rdbuf()`, `is_open()`, `open()`, `close()` — all wired to the contained `sb` filebuf member; `setstate`/`clear` used for failure propagation,
  - `ios_base::openmode` → C `fopen` mode string mapping covers all 8 canonical C++98 mode combinations and the binary flag,
  - wide-char stream classes (`wifstream`, `wofstream`, `wfstream`) are not specialised; they are typedef stubs only (wide I/O unsupported in WASM profile),
- updated `include/fstream.h`: added `#include <cstdio>` and declared `FILE* _file` as a private member of `basic_filebuf` so specialisation bodies in `src/fstream.cpp` can access the file handle.

Interpretation:

- no WAT, C, or JS was written; the entire fstream layer delegates to MaiaC's `createStdioHosts` which is already fully implemented in `maiac/src/runtime/c89-js-hosts.js`,
- MaiaWASM linker is unchanged,
- the host-wrapper-first + MaiaC-first policy is now fully applied to the three major stream headers: iostream, fstream, and the host bridge.

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

Execution rule for Tier 1:

- Always bind these headers/types/macros/functions to MaiaC-compatible ABI and values first.
- Prefer forwarding wrappers over new implementations.

### Tier 2: Thin C++ wrappers over MaiaC/runtime pieces

These should be implemented mostly in C++ headers/source using MaiaC functionality underneath:

- `string`
- `exception`, `new`, `stdexcept`, `typeinfo`
- stream base types and iostream glue
- `limits` specializations for concrete built-in types

Execution rule for Tier 2:

- Use MaiaC primitives (`malloc/free`, C string/memory routines, stdio/time/math hosts) as backend.
- Keep C++ layer focused on interface semantics and type system requirements.

### Tier 3: Special/host-backed areas

These may need mixed implementation in C/WAT/JavaScript:

- wide-char support (`cwchar`, `cwctype`)
- locale-heavy behavior
- filesystem-backed stream functionality
- exception/runtime support that depends on MaiaC WASM lowering details

Execution rule for Tier 3:

- Implement only what cannot be mapped through MaiaC C-level behavior.
- Prefer host/JS integration where MaiaC already has established host mechanisms.

## Decision checklist before adding new runtime code

Before implementing any function/class runtime path in MaiaCpp:

1. Does MaiaC already expose equivalent behavior in C89 headers/runtime?
2. Can the C++ feature be lowered to existing MaiaC calls with a wrapper?
3. Is the remaining gap strictly C++-specific (not expressible in C89)?

Only if (1) and (2) are both no, implement new MaiaCpp runtime behavior.

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

## Checkpoint de Continuidade (2026-04-16)

Estado em que paramos:

- Fase de auditoria dos headers `include/*.h` concluída (sem placeholders pendentes).
- Bridge de alocação C++98 concluída com política MaiaC-first:
  - `src/new.cpp` implementa `operator new/delete` (simples + array), versões `nothrow`, `set_new_handler`, `std::nothrow`, `std::bad_alloc::what()`.
  - backend de alocação delegado para `std::malloc` / `std::free`.
- `include/memory.h` com semântica essencial de `allocator<T>`:
  - `allocate` -> `::operator new`
  - `deallocate` -> `::operator delete`
  - `construct` -> placement new
  - `destroy` -> destrutor explícito

Validação no ponto de parada:

- Verificação de erros do workspace nos arquivos alterados: sem erros.
- Teste de compilação com `g++` do host apresentou conflito de ABI/perfil (esperado fora do pipeline MaiaC), portanto não é critério de bloqueio para este projeto.

Próximo passo imediato (retomada):

1. Implementar núcleo mínimo de `exception` em modo MaiaC-first:
   - `std::exception`, `std::bad_exception`
   - `set_terminate` / `terminate`
   - `set_unexpected` / `unexpected`
   - `uncaught_exception` com comportamento provisório documentado.
2. Em seguida, fechar `stdexcept` sobre esse núcleo (thin wrappers).

## Implementation Pass Log

### 2026-04-18: Container runtime profile completed

Implemented the first usable STL container/runtime layer directly in the MaiaCpp headers with a WASM-oriented pragmatic profile:

- `vector` now has contiguous storage with inline allocation/growth semantics.
- `deque` uses a contiguous-storage profile with front headroom and inline growth.
- `list` now has a real doubly-linked node model and bidirectional iterators.
- `map` and `set` use a sorted-array backend instead of a tree, which keeps the implementation small and deterministic for the current MaiaC/WASM target profile.
- `multimap` and `multiset` follow the same sorted-array model with duplicate-key support.
- `queue`, `stack`, and `priority_queue` are now usable as container adaptors over the implemented sequence containers.

Supporting utility/runtime work completed in the same pass:

- `functional` now provides inline bodies for the core arithmetic/comparison/logical function objects and binder helpers.
- `utility` now provides inline `pair` and `rel_ops` helpers.
- `iterator` now provides `reverse_iterator`, `advance`, and `distance` inline bodies.

This pass intentionally favors correctness, portability through MaiaC lowering, and small implementation surface over strict data-structure fidelity to a native-host STL.

### 2026-04-18: Generic algorithm and numeric layer completed

Implemented a header-only generic algorithm layer in `include/algorithm.h` and a header-only numeric layer in `include/numeric.h`.

Completed areas include:

- non-modifying algorithms: `for_each`, `find`, `find_if`, `find_end`, `find_first_of`, `adjacent_find`, `count`, `count_if`, `mismatch`, `equal`, `search`, `search_n`
- mutating algorithms: `copy`, `copy_backward`, `swap`, `swap_ranges`, `iter_swap`, `transform`, `replace*`, `fill*`, `generate*`, `remove*`, `unique*`, `reverse*`, `rotate*`, `random_shuffle`, `partition`, `stable_partition`
- ordering/search/set algorithms: `sort`, `stable_sort`, `partial_sort`, `partial_sort_copy`, `nth_element`, `lower_bound`, `upper_bound`, `equal_range`, `binary_search`, `merge`, `inplace_merge`, `includes`, `set_union`, `set_intersection`, `set_difference`, `set_symmetric_difference`
- heap and permutation algorithms: `push_heap`, `pop_heap`, `make_heap`, `sort_heap`, `min`, `max`, `min_element`, `max_element`, `lexicographical_compare`, `next_permutation`, `prev_permutation`
- numeric algorithms: `accumulate`, `inner_product`, `partial_sum`, `adjacent_difference`

Implementation notes for this pass:

- the algorithms are intentionally simple, header-only, and template-local so they lower cleanly through MaiaCpp into MaiaC without additional runtime objects
- several random-access algorithms use straightforward insertion-sort or full-sort strategies rather than more complex host-STL-grade implementations
- this is acceptable for the current target because the primary goal is semantic availability and predictable lowering, not peak native throughput

Next recommended layer after this pass:

1. complete remaining iterator adapters and function-object wrappers still declared but not yet defined
2. revisit `locale` and wide-character policy boundaries
3. add focused transpiler/runtime tests that exercise containers plus algorithms together
