# MaiaCpp Lowering Diagnostics Report

**Date**: 2026-04-25  
**Compiler**: MaiaCpp (C++98 → C89 transpiler)  
**Test File**: `diagnostic-refined.cpp`  

## Executive Summary

MaiaCpp successfully parses valid C++98 code but **fails to lower (transpile) multiple standard constructs to C89**, resulting in `stub-fallback` behavior (functions return 0 instead of executing).

### Supported Constructs

✅ **Work with `structured-local-return`:**
- Simple variable declarations with literal assignments
- Variable returns without modification
- Unused arithmetic expressions (folded away by optimizer)

Example:
```cpp
int t1() {
    int x = 5;
    return 1;              // ✅ Works
}

int t2() {
    int x = 5;
    return x;              // ✅ Works
}
```

### Unsupported Constructs (⚠️ Critical)

#### 1. **Ternary operator (conditional expression)**
- **Fails**: `(condition) ? true_val : false_val`
- **Status**: `stub-fallback (no-supported-lowering)`
- **Impact**: Cannot implement conditional logic without if-statements

```cpp
int t7() {
    int a = 10;
    int b = (a > 5) ? 1 : 0;  // ❌ FAILS: stub-fallback
    return 1;
}
```

**Generated C:**
```c
int t7(void) {
  return (int)0;  // Stubified
}
```

---

#### 2. **If-statements (control flow)**
- **Fails**: `if (condition) { ... }`
- **Status**: `stub-fallback (no-supported-lowering)`
- **Impact**: Cannot implement conditional logic whatsoever

```cpp
int t8() {
    int a = 10;
    if (a > 5) {          // ❌ FAILS: stub-fallback
        return 1;
    }
    return 0;
}
```

**Generated C:**
```c
int t8(void) {
  return (int)0;  // Stubified
}
```

---

#### 3. **Function calls (all external/library calls)**
- **Fails**: `printf(...)`, `foo(...)`, etc.
- **Status**: `stub-fallback (no-supported-lowering)`
- **Impact**: Cannot call ANY functions, including I/O

```cpp
int t9() {
    printf("test\n");     // ❌ FAILS: stub-fallback
    return 1;
}

int t10() {
    int x = 42;
    printf("x=%d\n", x);  // ❌ FAILS: stub-fallback
    return 1;
}

int t11() {
    int a = 10;
    if (a > 5) printf("ok\n");  // ❌ FAILS: stub-fallback
    return 1;
}
```

**Generated C:**
```c
int t9(void) {
  return (int)0;  // Stubified
}

int t10(void) {
  return (int)0;  // Stubified
}

int t11(void) {
  return (int)0;  // Stubified
}
```

---

## Impact on Test Suite

The existing test suite attempts to use these unsupported constructs:

| Feature | Code Pattern | Status |
|---------|--------------|--------|
| I/O Testing | `printf("PASS test\n")` | ❌ NO LOWERING |
| Control Flow | `if (condition) { ... }` | ❌ NO LOWERING |
| Conditional Logic | `(expr) ? val1 : val2` | ❌ NO LOWERING |
| Any Function Call | `func(args)` | ❌ NO LOWERING |

**Result**: All 10 test programs are completely stubified. No test logic executes.

---

## What DOES Work

From diagnostic testing, these patterns **do transpile**:

✅ Local variable declarations  
✅ Literal assignments  
✅ Return values (simple variable returns)  
✅ Class/template definitions (parsing only)  

Example of working code:
```cpp
int working() {
    int x = 5;
    int y = 10;
    return x;  // This will transpile
}
```

**Transpiles to:**
```c
int working(void) {
  int x = 5;
  int y = 10;
  return x;
}
```

---

## Recommendations

### For MaiaCpp Developers

1. **Priority 1**: Implement function call lowering
   - Required for: `printf()`, `malloc()`, user functions
   - Current limitation: Function calls trigger `no-supported-lowering`

2. **Priority 2**: Implement if-statement lowering
   - Required for: All control flow
   - Current limitation: Control structures not lowered to C

3. **Priority 3**: Implement ternary operator lowering
   - Required for: Conditional expressions
   - Fallback: Convert to if-else chains

### For Test Suite Strategy

**Option A**: Wait for MaiaCpp fixes (recommended)
- Current: Tests cannot validate anything
- Future: Once lowering is implemented, tests will work as designed

**Option B**: Use introspection-based testing (temporary)
- Create minimal test programs that only use supported constructs
- Use constant folding to verify arithmetic (if fully implemented)
- Example: `int x = 10 + 20; return x;` would verify addition works
- **Limitation**: Cannot verify control flow, I/O, or function calls

**Option C**: Test MaiaC directly instead
- Skip MaiaCpp entirely
- Write C89 test programs
- Validates MaiaC→WASM pipeline independently
- Does not test C++98→C89 transpilation

---

## Test Results Summary

| Test | Function | Status | Issue |
|------|----------|--------|-------|
| 01_operators | Main + helpers | ❌ STUB | printf() calls |
| 02_control_flow | If/for/while | ❌ STUB | Control flow + printf |
| 03_functions | Recursion | ❌ STUB | Function calls + printf |
| 04_classes | Member methods | ❌ STUB | Method calls + printf |
| 05_templates | Template functions | ❌ STUB | printf calls |
| 06_inheritance | Virtual methods | ❌ STUB | printf calls |
| 07_memory | new/delete | ❌ STUB | printf calls |
| 08_arrays_pointers | Array operations | ❌ STUB | printf calls |
| 09_strings | strcpy/strcmp | ❌ STUB | Function calls |
| 10_casts | Type casts | ❌ STUB | printf calls |

**All 10 tests fail at lowering stage.**

---

## Conclusion

MaiaCpp's C++98 parser is robust and accepts valid code, but the **code generation backend has fundamental gaps**:

1. ❌ No function call lowering
2. ❌ No control flow lowering
3. ❌ No ternary/conditional expression lowering

Until these are implemented, **no meaningful C++98→C89 transpilation is possible**. The test suite correctly identifies these limitations and documents them for compiler development.
