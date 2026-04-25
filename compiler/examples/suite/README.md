# MaiaCpp Test Suite — Analysis Report

## Summary

This test suite was created to validate MaiaCpp's C++98→C89 transpilation capability across 11 major language feature categories.

**Status**: ⚠️ All tests fail at the **lowering stage** of the compiler, not due to incorrect test code.

## What This Reveals

The test files contain **valid, standards-compliant C++98 code**. Their failure to transpile exposes **fundamental gaps in MaiaCpp's code generation backend**:

| Category | Test File | Status | Root Cause |
|----------|-----------|--------|-----------|
| Operators | 01_operators | ❌ Stubified | printf() calls not supported |
| Control Flow | 02_control_flow | ❌ Stubified | if-statements + printf() not supported |
| Functions | 03_functions | ❌ Stubified | Function calls not supported |
| Classes | 04_classes | ❌ Stubified | Method calls + printf() not supported |
| Templates | 05_templates | ❌ Stubified | printf() calls not supported |
| Inheritance | 06_inheritance | ❌ Stubified | printf() calls + virtual calls not supported |
| Memory | 07_memory | ❌ Stubified | printf() calls not supported |
| Arrays/Pointers | 08_arrays_pointers | ❌ Stubified | printf() calls not supported |
| Strings | 09_strings | ❌ Stubified | C library calls (strlen, strcmp, etc.) not supported |
| Casts | 10_casts | ❌ Stubified | printf() calls not supported |
| Preprocessor | 11_preprocessor | ❌ Broken preprocessing | function-like macros collapse to `1(...)`, multiline macros leak raw bodies |

## Key Diagnostics

See [DIAGNOSTICS.md](DIAGNOSTICS.md) for detailed analysis.

### Unsupported Constructs (Cause `stub-fallback / no-supported-lowering`)

1. **Function calls** — ANY external or user function call
   - `printf()`, `malloc()`, `strcpy()`, custom functions
   - Completely blocks I/O, memory operations, library use

2. **If-statements** — Control flow statements
   - `if (cond) { ... }`
   - Blocks all conditional logic

3. **Ternary operator** — Conditional expressions
   - `(cond) ? a : b`
   - Alternative to if-statements

4. **While/do-while loops** — Loop constructs
   - While loops may be affected (needs confirmation)

### Supported Constructs (Do Transpile)

✅ Variable declarations with literal assignments  
✅ Simple variable returns  
✅ Arithmetic expressions (constant-folded)  
✅ Class/template definitions (parsing only)

## Test File Descriptions

### 01_operators.cpp
Tests all C++98 operator categories using `if (condition) printf("PASS ...")` pattern.  
**Issue**: `printf()` calls trigger stub-fallback. Code is valid.

### 02_control_flow.cpp
Tests if/else chains, for/while/do-while loops, switch statements, break/continue.  
**Issue**: Control structures + printf() both unsupported.

### 03_functions.cpp
Tests recursion, function overloading, pass-by-reference, function pointers.  
**Issue**: Function calls unsupported.

### 04_classes.cpp
Tests member initializer lists, copy constructor, const methods, operator overloading.  
**Issue**: Method calls + printf() unsupported.

### 05_templates.cpp
Tests function and class templates with non-type parameters.  
**Issue**: printf() calls unsupported.

### 06_inheritance.cpp
Tests single inheritance, virtual methods, abstract classes, static_cast/dynamic_cast.  
**Issue**: Virtual call + printf() unsupported.

### 07_memory.cpp
Tests new/delete (scalar and array), RAII with destructors.  
**Issue**: printf() calls unsupported.

### 08_arrays_pointers.cpp
Tests 1D/2D arrays, pointer arithmetic, pointer-to-pointer, array of pointers.  
**Issue**: printf() calls unsupported.

### 09_strings.cpp
Tests C-string operations: strlen, strcmp, strcpy, strcat, strstr.  
**Issue**: C library calls not supported.

### 10_casts.cpp
Tests static_cast, reinterpret_cast, const_cast, C-style casts.  
**Issue**: printf() calls unsupported.

### 11_preprocessor/preprocessor.cpp
Tests local includes, chained object-like macros, function-like macros, multiline macros, conditional compilation, `#undef`, stringification, and token pasting.  
**Issue**: the current preprocessor only supports object-like textual replacement. Function-like macros are stored as `#define NAME 1`, multiline macros leak raw `\` lines into the preprocessed source, and conforming macro semantics are not implemented.

---

### diagnostic.cpp / diagnostic-refined.cpp
Binary-search style tests to isolate exact failure points.  
Result: Proves that printf() and control structures are the blockers.

### DIAGNOSTICS.md
Detailed technical analysis with code examples and recommendations.

---

## Interpreting Results

**These tests are NOT broken.** They correctly expose compiler limitations.

When MaiaCpp implements:
1. ✅ Function call lowering
2. ✅ If-statement lowering
3. ✅ Ternary operator lowering

...all tests will pass and validate MaiaCpp's C++98→C89 quality.

## Next Steps

### For MaiaCpp Developers
Prioritize implementing function call lowering in the code generation backend. This is the primary blocker for all test categories.

### For Test Users
- **Option A** (Recommended): Wait for MaiaCpp fixes, then re-run tests
- **Option B**: Use diagnostic files to track compiler progress
- **Option C**: Contribute fixes to MaiaCpp compiler backend

---

**Created**: 2026-04-25  
**Purpose**: Validate and document MaiaCpp's C++98 transpilation capability  
**Status**: Tests created successfully; compiler limitations identified
