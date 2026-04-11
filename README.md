# MaiaCpp (WebCpp)
Maia C++ Compiler.

<img src="images/TheWebCpp.png" style="width: 512px; height: auto;">

## Wrapper CLI

Use `bin/webcpp.sh` to generate AST and compilation artifacts:

```bash
./bin/webcpp.sh --file ./compiler/test.cpp --all --out-dir ./out
```

Supported outputs:

- AST in XML (`--ast-xml-out`)
- AST in JSON (`--ast-json-out`)
- AST in a tree in the terminal (`--ast-show`)

- Generated C (`--c-out`)

- WAT (`--wat-out`, via MaiaC)

- WASM (`--wasm-out`, via MaiaC)

Note:

- The default path is `C++ -> C (MaiaCpp) -> WAT/WASM (MaiaC)`.

- The `cpp-compiler.js` file only outputs C code.

- The only WAT file maintained in the project is `compiler/runtime.wat`, reserved for the runtime that covers what doesn't fit in the target C code, such as support for `try`/`catch`.

## Execution of test.cpp (console, Node, browser)

Unified stream (default: `compiler/test.cpp`):

```bash
bash ./bin/run-test-cpp.sh --target all
```

Extended C++98 baseline (greater coverage in the current state):

```bash
bash ./bin/run-test-cpp.sh --file ./compiler/test_cpp98_extended.cpp --target all
```

Tiered C++98 suite + EBNF report (production):

```bash
python3 ./compiler/tests/run_tiered_cpp98.py
```

Semantic comparison of native C++ vs. generated C pipeline:

```bash
python3 ./compiler/tests/compare_cpp_vs_pipeline.py --file ./compiler/test_cpp98_extended.cpp
```

This command:

- compiles and executes native C++98
- generates C via MaiaCpp and executes the MaiaCpp -> MaiaC -> WASM pipeline in Node
- compares stdout and return code
- saves the generated C in `out/reports/cpp-vs-c/<file>.generated.c`

Machine-readable output:

- `out/reports/ebnf-tiered-report.json`

Current Tiers:

- Tier 1: Node/WASM runtime (end-to-end scenario)
- Tier 2: compilation regression (fixtures)
- Tier 3: parse-only tracking for families still in gaps (e.g., linkage/asm/try-catch)

The report also informs how many families in the EBNF matrix are already tracked by cases Tiered (`matrixTracking`).

In its current state, this tracking already covers most of the matrix and should be updated with each execution of the tiered runner.

Individual Targets:

```bash
# Native Console (clang++/g++)
bash ./bin/run-test-console.sh ./compiler/test.cpp

# Node + WASM
bash ./bin/run-test-node.sh ./compiler/test.cpp

# Browser + WASM (starts local server)
bash ./bin/run-wasm-browser.sh ./compiler/test.cpp
```

Runner in browser:

- `tools/browser/run-wasm.html`

- Accepts query `?wasm=/path/to/file.wasm`

## Minimal example with class (new)

File: `compiler/example_class_plus_one.cpp`

```bash
# Node + WASM
bash ./bin/run-test-node.sh ./compiler/example_class_plus_one.cpp

# Browser + WASM (port 8080)
bash ./bin/run-wasm-browser.sh ./compiler/example_class_plus_one.cpp ./out/browser 8080

# Dedicated shortcuts for the simple example
bash ./bin/run-test-node-simple.sh
bash ./bin/run-wasm-browser-simple.sh ./out/browser 8080
```

The `test.cpp` file remains the complete baseline and has not been modified.

## Architecture

- See `docs/ARCHITECTURE.md` for an English architecture overview.

- See `docs/EBNF_IMPLEMENTATION_AUDIT.md` for a detailed gap definition against `grammar/Cpp.ebnf`.
- See `docs/CONFORMANCE_MATRIX.md` for implementation status by grammar family.
- See `docs/PRACTICAL_READINESS_TODO.md` for execution-focused tasks to make MaiaCpp production-ready.