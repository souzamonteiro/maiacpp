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
