# MaiaCpp Diagnostics — Programming in C++ Course Examples

**Date**: 2026-04-30  
**Compiler**: MaiaCpp (C++98/C++11 → C89 transpiler)  
**Test Set**: `programming_in_cpp_course_en/` (48 programs across 13 categories)

---

## Executive Summary

These programs are valid, idiomatic C++ covering basic to intermediate language features. Their MaiaCpp compatibility is determined by the same backend limitations documented in the `suite/` diagnostics: **function calls**, **if-statements**, and **loops** all trigger `stub-fallback` behavior in the current MaiaCpp code generation backend.

### Compatibility Matrix by Category

| Category | Programs | g++ | MaiaCpp | Limiting Factor |
|----------|----------|-----|---------|-----------------|
| 01_basics | 4 | ✅ All | ⚠️ Partial | printf/cout calls stubified |
| 02_control_flow | 3 | ✅ All | ❌ None | if-statements + cin not supported |
| 03_functions | 8 | ✅ All | ❌ None | Function calls stub-fallback |
| 04_classes | 4 | ✅ All | ❌ None | Method calls not lowered |
| 05_inheritance | 6 | ✅ All | ❌ None | Virtual dispatch not lowered |
| 06_polymorphism | 2 | ✅ All | ❌ None | Virtual + function calls |
| 07_overloading | 4 | ✅ All | ❌ None | Operator/function calls not lowered |
| 08_templates | 1 | ✅ All | ⚠️ Partial | Ternary operator stubified |
| 09_pointers | 4 | ✅ All | ❌ None | printf/cout + pointer ops |
| 10_strings | 8 | ✅ All | ❌ None | strlen + conditionals |
| 11_exceptions | 1 | ✅ All | ❌ None | throw/catch not supported |
| 12_namespaces | 2 | ✅ All | ⚠️ Partial | Namespace declarations parse OK; printf stubified |
| 13_misc | 1 | ✅ All | ❌ None | while loop + cout |

---

## Known MaiaCpp Limitations (Reproduced Here)

These are the same critical gaps documented in `suite/DIAGNOSTICS.md`, confirmed to affect this program set:

### 1. Function Calls (`stub-fallback`)
Any call to an external or user-defined function causes `stub-fallback`:
- `printf()`, `cout <<`, `cin >>` — blocks all I/O
- User-defined functions (`celsius()`, `square()`, etc.)
- Standard library: `strlen()`, `toupper()`, `system()`

**Impact**: Every program that prints output fails.

### 2. If-Statements (`stub-fallback`)
```cpp
if (condition) { ... }   // ❌ not lowered
```
Blocks all conditional logic. Affects 03_functions through 13_misc heavily.

### 3. Ternary Operator (`stub-fallback`)
```cpp
T result = (a > b) ? a : b;   // ❌ not lowered
```
Used in `08_templates/templates.cpp` (tmax function).

### 4. While / Do-While Loops (`stub-fallback`)
```cpp
while (condition) { ... }   // ❌ not lowered
do { ... } while (condition);
```
Affects all loop-heavy programs: `loop_structures.cpp`, vowel counters, palindrome checkers, `pointers.cpp`, `game.cpp`.

### 5. Exception Handling (parsing/lowering failure)
```cpp
try { throw string("Oops!"); } catch (string e) { ... }
```
`throw` and `catch` keywords are either not parsed or not lowered.

---

## Programs Most Likely to Transpile (Partially)

These programs contain constructs that MaiaCpp *can* lower (`structured-local-return`):

| Program | Transpilable Subset | What Fails |
|---------|---------------------|------------|
| `01_basics/data_types.cpp` | Variable declarations | All cout calls stubified |
| `12_namespaces/namespace.cpp` | Namespace + var declarations | cout calls stubified |
| `08_templates/templates.cpp` | Template parsing | Ternary in tmax stubified |

---

## Programs Guaranteed to Stub-Fallback Entirely

The following categories use patterns that entirely prevent lowering:

- All programs using `cout <<` or `cin >>` (all 48)
- All programs using `if`, `while`, `for`, or `do` (45/48)
- All programs calling any function (45/48)

---

## Constructs That DO Work

✅ Variable declarations with literal values  
✅ Class and namespace declarations (parsing)  
✅ Template type parameter declarations (parsing)  
✅ Simple return statements with literals or declared variables  

---

## Constructs That DON'T Work

❌ Any function call (including cout, cin, printf)  
❌ if / else if / else  
❌ switch / case  
❌ while, do-while, for  
❌ Ternary operator `? :`  
❌ throw / try / catch  
❌ Member function calls on objects  
❌ Virtual dispatch  
❌ Pointer dereference in expressions  

---

## Comparison With suite/ Diagnostics

The limitations found in this course set are **identical** to those found in the `suite/` test programs. This confirms that MaiaCpp's backend gaps are systematic, not specific to any single test case:

| Finding | suite/ | course_en/ |
|---------|--------|------------|
| printf stubified | ✅ | ✅ |
| if-statements stubified | ✅ | ✅ |
| while loops stubified | ✅ | ✅ |
| Function calls stubified | ✅ | ✅ |
| Preprocessor issues | ✅ | N/A |

---

## Recommendations

1. **Fix function call lowering first** — it unblocks all I/O and unlocks most programs
2. **Fix if-statement lowering** — required by virtually all non-trivial programs
3. **Fix loop lowering** — required by iterative algorithms (vowels, palindromes, Fibonacci)
4. These course examples make excellent regression tests once the backend gaps are fixed
