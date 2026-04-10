# MaiaCpp (WebCpp)
Maia C++ Compiler.

<img src="images/TheWebCpp.png" style="width: 512px; height: auto;" />

## Wrapper CLI

Use `bin/webcpp.sh` para gerar AST e artefatos de compilacao:

```bash
./bin/webcpp.sh --file ./compiler/test.cpp --all --out-dir ./out
```

Saidas suportadas:

- AST em XML (`--ast-xml-out`)
- AST em JSON (`--ast-json-out`)
- AST em arvore no terminal (`--ast-show`)
- C gerado (`--c-out`)
- WAT (`--wat-out`, via MaiaC)
- WASM (`--wasm-out`, via MaiaC)

Observacao:

- O caminho padrao e `C++ -> C (MaiaCpp) -> WAT/WASM (MaiaC)`.
- O `cpp-compiler.js` emite apenas C.
- O unico WAT mantido no projeto e o `compiler/runtime.wat`, reservado ao runtime que cobre o que nao cabe no C alvo, como suporte a `try`/`catch`.

## Execucao do test.cpp (console, Node, browser)

Fluxo unificado (default: `compiler/test.cpp`):

```bash
bash ./bin/run-test-cpp.sh --target all
```

Baseline estendido C++98 (maior cobertura no estado atual):

```bash
bash ./bin/run-test-cpp.sh --file ./compiler/test_cpp98_extended.cpp --target all
```

Suíte tierizada C++98 + relatório EBNF (produção):

```bash
python3 ./compiler/tests/run_tiered_cpp98.py
```

Comparação semântica C++ nativo vs pipeline C gerado:

```bash
python3 ./compiler/tests/compare_cpp_vs_pipeline.py --file ./compiler/test_cpp98_extended.cpp
```

Esse comando:

- compila e executa o C++98 nativo
- gera C via MaiaCpp e executa o pipeline MaiaCpp -> MaiaC -> WASM no Node
- compara stdout e código de retorno
- salva o C gerado em `out/reports/cpp-vs-c/<arquivo>.generated.c`

Saída machine-readable:

- `out/reports/ebnf-tiered-report.json`

Tiers atuais:

- Tier 1: runtime Node/WASM (cenário end-to-end)
- Tier 2: regressão de compilação (fixtures)
- Tier 3: rastreamento parse-only para famílias ainda em lacuna (ex.: linkage/asm/try-catch)

O relatório também informa quantas famílias da matriz EBNF já estão rastreadas por casos tierizados (`matrixTracking`).
No estado atual, esse rastreamento já cobre a maior parte da matriz e deve ser atualizado a cada execução do runner tierizado.

Alvos individuais:

```bash
# Console nativo (clang++/g++)
bash ./bin/run-test-console.sh ./compiler/test.cpp

# Node + WASM
bash ./bin/run-test-node.sh ./compiler/test.cpp

# Browser + WASM (inicia servidor local)
bash ./bin/run-wasm-browser.sh ./compiler/test.cpp
```

Runner no browser:

- `tools/browser/run-wasm.html`
- Aceita query `?wasm=/caminho/para/arquivo.wasm`

## Exemplo minimo com classe (novo)

Arquivo: `compiler/example_class_plus_one.cpp`

```bash
# Node + WASM
bash ./bin/run-test-node.sh ./compiler/example_class_plus_one.cpp

# Browser + WASM (porta 8080)
bash ./bin/run-wasm-browser.sh ./compiler/example_class_plus_one.cpp ./out/browser 8080

# Atalhos dedicados para o exemplo simples
bash ./bin/run-test-node-simple.sh
bash ./bin/run-wasm-browser-simple.sh ./out/browser 8080
```

O `test.cpp` continua como baseline completo e nao foi alterado.

## Architecture

- See `docs/ARCHITECTURE.md` for an English architecture overview.
- See `docs/EBNF_IMPLEMENTATION_AUDIT.md` for a detailed gap definition against `grammar/Cpp.ebnf`.
- See `docs/CONFORMANCE_MATRIX.md` for implementation status by grammar family.
- See `docs/PRACTICAL_READINESS_TODO.md` for execution-focused tasks to make MaiaCpp production-ready.
