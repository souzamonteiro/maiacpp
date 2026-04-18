# MaiaCpp Architecture

This document describes the current architecture of MaiaCpp, an experimental C++98 compiler pipeline focused on practical transpilation and WebAssembly workflows.

## 1. Goals

- Parse C++98 source files using the generated parser.
- Build semantic metadata for classes, methods, namespaces, and global functions.
- Transpile C++98 into C (stub-first, with incremental lowering for selected patterns).
- Reuse MaiaC for all WAT/WASM generation in Node and browser execution workflows.
- Keep a fixture-driven regression suite for incremental compiler evolution.

## 2. High-Level Pipeline

```text
C++98 source (.cpp)
  -> Parser + ParseTreeCollector
  -> Semantic analysis + source-backed hints
  -> C transpilation (primary path)
  -> runtime.wat linkage for features beyond C lowering (for example try/catch support)
  -> MaiaC C compiler
  -> WAT emission / WASM assembly
  -> Runtime execution (console | Node | browser)
```

## 3. Core Components

### 3.1 Compiler Entry Point

- File: compiler/cpp-compiler.js
- Class: Cpp98Compiler
- Responsibilities:
  - Load source file.
  - Parse source with fallback strategy (normalized + original source).
  - Build analysis model.
  - Emit C output only.

### 3.2 Parsing Layer

- Files:
  - compiler/cpp-parser.js
  - compiler/parse-tree-collector.js
- Responsibilities:
  - Run grammar-generated parser.
  - Build parse-tree model.
  - Support XML/JSON/tree debug outputs.

### 3.3 Semantic and Source-Hint Layer

- Main logic in compiler/cpp-compiler.js.
- Responsibilities:
  - Track namespace scope.
  - Extract class and global function metadata.
  - Infer signatures and symbols used by code generation.
  - Fill sparse semantic data with source-backed hints.

### 3.4 C Backend

- Class: CppToCTranspiler in compiler/cpp-compiler.js.
- Responsibilities:
  - Emit headers and runtime interface declarations.
  - Emit class stubs and global function stubs.
  - Apply conservative lowering for selected return-call patterns.
  - Resolve overloads with deterministic tie-break rules.
  - Emit ambiguity diagnostics and per-file ambiguity summary.

### 3.5 Runtime WAT

- File: compiler/runtime.wat
- Responsibilities:
  - Provide handwritten runtime support that is not naturally expressed in the C bridge.
  - Remain the only WAT artifact kept inside MaiaCpp.

## 4. Name Resolution and Overload Strategy

The compiler currently implements deterministic, conservative call lowering for simple global-call returns.

Resolution model includes:

- Exact signature match when argument types are inferable.
- Basic implicit conversion ranking for selected paths (for example int to long, float to double).
- Lexicographic per-parameter cost tie-break before scope tie-break.
- Scope-aware tie-break based on namespace proximity.
- Stable lexical fallback when all previous criteria tie.

When stable fallback is used, the C output includes:

- Inline diagnostic comment near the lowered return.
- File-level ambiguity summary block with selected candidates.

## 5. Runtime Targets

### 5.1 Native Console

- Script: bin/run-test-console.sh
- Flow:
  - Compile .cpp with clang++ or g++ (C++98 mode).
  - Execute native binary.

### 5.2 Node + WASM

- Scripts:
  - bin/run-test-node.sh
  - tools/node/run-wasm-node.js
- Flow:
  - Compile .cpp to C via MaiaCpp.
  - Compile generated C to WASM via MaiaC.
  - Instantiate in Node with env imports.
  - Execute main (or _start fallback).

### 5.3 Browser + WASM

- Scripts/UI:
  - bin/run-wasm-browser.sh
  - tools/browser/run-wasm.html
- Flow:
  - Compile .cpp to C via MaiaCpp.
  - Compile generated C to WASM via MaiaC.
  - Start local HTTP server.
  - Load runner page and execute main in browser.

### 5.4 Unified Orchestration

- Script: bin/run-test-cpp.sh
- Targets:
  - console
  - node
  - browser
  - all

## 6. Tooling and Wrapper CLI

- Script: bin/webcpp.sh
- Supports:
  - AST show
  - AST XML/JSON output
  - C output
  - WAT output through MaiaC
  - WASM output through MaiaC
  - all outputs in one command

## 7. Testing and Validation

### 7.1 Fixture Suite

- Directory: compiler/tests/fixtures
- Runner: compiler/tests/run_fixtures.js
- Focus:
  - Parser success/failure behavior.
  - Codegen expectations in stdout markers.
  - Incremental regressions (namespaces, overload resolution, diagnostics).

### 7.2 Grammar/Smoke Runner

- Script: compiler/test_grammar.sh
- Flow:
  - Execute fixtures.
  - Execute smoke cases.

### 7.3 Baseline Program

- File: compiler/test.cpp
- Purpose:
  - Mandatory baseline target.
  - Runtime-friendly sample for parser/codegen evolution.
  - Cross-target execution checks (console/node/browser).

## 8. Current Boundaries

- C backend is still largely stub-oriented outside conservative lowered patterns.
- MaiaCpp no longer owns a direct WAT backend; executable code generation beyond C is delegated to MaiaC plus runtime.wat support.
- Runtime imports are currently minimal host stubs for execution workflows.
- Full C++98 semantic parity is not complete yet.

## 9. Evolution Strategy

Architecture evolution follows fixture-first increments:

1. Add a focused regression fixture.
2. Implement smallest safe compiler change.
3. Re-run full fixture and smoke suites.
4. Commit/push one narrow phase at a time.

Tracking document: docs/SUPPORT_ROADMAP.md
