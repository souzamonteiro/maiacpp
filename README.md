# MaiaCpp (WebCpp)
Maia C++ Compiler.

<img src="images/TheWebCpp.png" style="width: 512px; height: auto;">

## Wrapper CLI

Use `bin/webcpp.sh` to generate AST and compilation artifacts:

```bash
./bin/webcpp.sh --file ./compiler/test.cpp --all --out-dir ./out
```

Basic usage:

```bash
./bin/webcpp.sh <input.cpp> [options]
./bin/webcpp.sh --file <input.cpp> [options]
```

Supported outputs:

- AST in XML (`--ast-xml-out`)
- AST in JSON (`--ast-json-out`)
- AST in a tree in the terminal (`--ast-show`)

- Generated C (`--c-out`)

- Generated JS wrapper (`--js-out`, via MaiaC `webc`)

- WAT (`--wat-out`, via MaiaC)

- WASM (`--wasm-out`, via MaiaC)

Extended MaiaC `webc` options (forwarded by `webcpp.sh`):

- `--wat`
- `--no-validate`
- `--resolve-system-includes` / `--no-system-includes`
- `--dist`
- `--dist-run`
- `--name <appName>`
- `--webc-out-base <base>` (maps to `webc -o`)
- `--run`

Examples:

```bash
# C + WASM + WAT + JS in explicit paths
./bin/webcpp.sh ./compiler/test.cpp \
	--c-out ./out/test.c \
	--wasm-out ./out/test.wasm \
	--wat-out ./out/test.wat \
	--js-out ./out/test.js

# Generate distribution package (browser + Node runner)
./bin/webcpp.sh ./compiler/test.cpp \
	--dist --out-dir ./dist --name testapp

# Generate dist and run node runner immediately
./bin/webcpp.sh ./compiler/test.cpp \
	--dist-run --out-dir ./dist --name testapp
```

Note:

- The default path is `C++ -> C (MaiaCpp) -> WAT/WASM (MaiaC)`.

- The `cpp-compiler.js` file only outputs C code.

- The only WAT file maintained in the project is `compiler/runtime.wat`, reserved for the runtime that covers what doesn't fit in the target C code, such as support for `try`/`catch`.

## Execution of test.cpp (console, Node, browser)

## Migration Guide (old → new)

Quick command mapping after the wrapper updates:

```bash
# Old
bash ./bin/run-test-node.sh ./compiler/test.cpp
# New (equivalent legacy behavior)
bash ./bin/run-test-node.sh --file ./compiler/test.cpp --mode legacy

# Old
bash ./bin/run-wasm-browser.sh ./compiler/test.cpp
# New (equivalent legacy behavior)
bash ./bin/run-wasm-browser.sh --file ./compiler/test.cpp --mode legacy

# New dist-oriented Node flow
bash ./bin/run-test-node.sh --file ./compiler/test.cpp --mode dist --out-dir ./out/node-dist --name testapp

# New dist-oriented Browser flow
bash ./bin/run-wasm-browser.sh --file ./compiler/test.cpp --mode dist --out-dir ./out/browser-dist --name testapp

# Passing extra MaiaC/webc flags through wrappers
bash ./bin/run-test-node.sh --file ./compiler/test.cpp --mode dist -- --resolve-system-includes --wat
```

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
node ./compiler/tests/run_tiered_cpp98.js
```

Semantic comparison of native C++ vs. generated C pipeline:

```bash
node ./compiler/tests/compare_cpp_vs_pipeline.js --file ./compiler/test_cpp98_extended.cpp
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

# Node + WASM (legacy mode)
bash ./bin/run-test-node.sh --file ./compiler/test.cpp --mode legacy

# Node dist pipeline (webc --dist-run)
bash ./bin/run-test-node.sh --file ./compiler/test.cpp --mode dist --out-dir ./out/node-dist --name testapp

# Browser + WASM (legacy mode; starts local server)
bash ./bin/run-wasm-browser.sh --file ./compiler/test.cpp --mode legacy

# Browser dist runner (starts local server in dist folder)
bash ./bin/run-wasm-browser.sh --file ./compiler/test.cpp --mode dist --out-dir ./out/browser-dist --name testapp
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