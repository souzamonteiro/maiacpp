# MaiaCpp Host Wrapper Convention

## Purpose

This document defines how MaiaCpp exposes host-environment functionality
(Node.js / browser) that is not expressible in C89 / MaiaC alone.

---

## Decision tree (implementation priority order)

Before adding any new implementation, follow this order:

```
1. Is it already in MaiaC C89 stdlib?
   YES → use the MaiaC C header directly. No wrapper needed.
         (stdio, math, time, string, stdlib, locale, signal, setjmp)

2. Is it host-only (browser API / Node.js module), but the generated
   JS host environment from MaiaC/webc already provides it?
   YES → declare a __namespace__funcName host import and add a thin
         wrapper in include/maiacpp_host_bridge.h.
         Do NOT reimplement in C, C++, or WAT.

3. Is it C++-specific semantics (RAII, templates, overloads, exceptions)?
   YES → implement in src/ in C++98, delegating to step 1 or 2 for I/O.

4. Only if none of the above apply: implement in C89 or WAT.
```

---

## Host import naming convention

MaiaC/webc treats function names matching `__<namespace>__<funcName>` as
WAT imports wired automatically to the JS host environment.

| C++ declaration                         | JS host binding                  |
|----------------------------------------|----------------------------------|
| `void __console__log(const char*)`     | `env.__console__log`  → `console.log` |
| `void __console__error(const char*)`   | `env.__console__error` → `console.error` |
| `void __console__warn(const char*)`    | `env.__console__warn`  → `console.warn` |
| `double __performance__now(void)`      | `env.__performance__now` → `performance.now()` |

Rules:

- Namespace segment: lowercase or PascalCase matching the JS object name
  (`console`, `Math`, `performance`, `Date`, `Crypto`…).
- Function segment: camelCase matching the JS method name.
- Always use `extern "C"` linkage so the C89/WAT ABI matches.
- Declare in `include/maiacpp_host_bridge.h` under
  `#ifdef MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS`.

---

## Wrapper structure in `maiacpp_host_bridge.h`

```cpp
#ifdef MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS
extern "C" {
ReturnType __namespace__funcName(ParamTypes...);
}
#endif

namespace std { namespace __maiacpp {

inline ReturnType host_wrapper_name(ParamTypes... args) {
#ifdef MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS
    return __namespace__funcName(args...);
#else
    (void)args...;  // or return a safe default
    return DefaultValue;
#endif
}

}} // namespace std::__maiacpp
```

Key rules:

- Use `#ifdef MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS` guards so native builds
  compile cleanly without unresolved symbols.
- Provide a no-op or safe default for the non-host path.
- Keep wrapper functions `inline` in the header; no separate .cpp needed.

---

## Build flags

| Flag                                   | Meaning                                   |
|----------------------------------------|-------------------------------------------|
| `MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS`  | Activates host import declarations and wiring. Set by MaiaCpp → MaiaC → webc build pipeline. Not set in standalone native host builds. |

---

## What MaiaC C89 stdlib already handles (no wrappers needed)

These are fully wired by MaiaC/webc and require only `#include` of the
corresponding MaiaCpp C header:

| Feature            | MaiaC C header  | Key functions                                 |
|--------------------|-----------------|-----------------------------------------------|
| Console I/O        | `<cstdio>`      | `printf`, `fprintf`, `putchar`, `puts`        |
| File I/O           | `<cstdio>`      | `fopen`, `fclose`, `fread`, `fwrite`, `fseek` |
| Math               | `<cmath>`       | `sin`, `cos`, `sqrt`, `pow`, `floor`, `ceil`  |
| Memory             | `<cstdlib>`     | `malloc`, `free`, `calloc`, `realloc`         |
| String             | `<cstring>`     | `strlen`, `strcpy`, `memcpy`, `memmove`       |
| Time               | `<ctime>`       | `time`, `clock`, `localtime`, `strftime`      |
| Locale             | `<clocale>`     | `setlocale`, `localeconv`                     |
| Process control    | `<cstdlib>`     | `abort`, `exit`, `getenv`                     |
| Long jump          | `<csetjmp>`     | `setjmp`, `longjmp`                           |

---

## What needs host wrappers (not in C89)

| Feature                        | Declaration                              | Reason                              |
|--------------------------------|------------------------------------------|-------------------------------------|
| Direct console channel         | `__console__log` / `__console__error`    | Bypasses stdio buffering, maps to `console.log` in browser and Node |
| High-resolution timer          | `__performance__now`                     | `performance.now()` is not in C89 `time.h` |
| Crypto random                  | (future) `__crypto__getRandomValues`     | Not in C89, WASM requires host import |
| Fetch / network                | (future) `__fetch__url`                  | Not expressible in C89              |

---

## Current wrappers in `include/maiacpp_host_bridge.h`

| Wrapper function                      | Host binding                  | Used by              |
|---------------------------------------|-------------------------------|----------------------|
| `std::__maiacpp::host_console_log`    | `__console__log`              | `std::terminate`, `std::unexpected` |
| `std::__maiacpp::host_console_error`  | `__console__error`            | `std::terminate`, `std::unexpected` |
| `std::__maiacpp::host_console_warn`   | `__console__warn`             | (available for future use) |
| `std::__maiacpp::host_performance_now`| `__performance__now`          | (available for future use) |

---

## What does NOT need a wrapper

- Anything already mapped through `MaiaC`'s `printf` / `fprintf` chain.
- `std::cout`, `std::cerr`: route through `src/iostream.cpp` → MaiaC stdio → host write.
- `std::fstream`: routes through MaiaC `fopen`/`fread`/`fwrite`.
- `std::abort`, `std::exit`: routed through MaiaC C89 stdlib host builtins.
