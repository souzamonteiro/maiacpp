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
- WAT (`--wat-out`)
- WASM (`--wasm-out`)
