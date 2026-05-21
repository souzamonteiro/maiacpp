# MaiaCpp Plan for Parser Timeout with MaiaJS Stress Output (2026-05-17)

## 1. Contexto

O baseline nativo de MaiaCpp em `compiler/examples/test.cpp` está passando (`build_test_dist.sh` verde).

O problema aparece quando MaiaCpp (`webcpp`) recebe o C++ gerado por MaiaJS a partir de `maiajs/compiler/examples/test.js` com bateria robusta de arrays.

## 2. Reprodução

Comando observado:

- `cd /Volumes/External_SSD/Documentos/Projects/maiajs && bash compiler/examples/build_test_dist.sh`

Falha na etapa `webcpp`:

- `Parsing: .../maiajs/compiler/examples/test.cpp`
- `Parser timeout (180000ms) during Parser: ok`
- fallback simples acionado.
- Em seguida, no backend C (`webc`), foi observado:
  - `[webc] Compilation error: Unknown base symbol 'this' (C_getValue)`.

## 3. Evidência técnica no C++ gerado

Arquivo de entrada para MaiaCpp parser:

- `maiajs/compiler/examples/test.cpp`

Pontos críticos:

1. Cadeias de chamadas muito profundas em array literal builder
- expressões extensas aninhadas com `__maia_arr_builder_push_value(...)`.
- risco de backtracking explosivo no parser Cpp.

2. Construções incompatíveis com o backend atual
- padrões como `array.length` e membro em `void*` (`lastPoint.x`) aumentam caminhos de erro semântico e podem amplificar custo de parse/recuperação.

3. Placeholders de loop no código emitido
- `// [for loop with unexpected semicolon count: 1]`.
- esse padrão cria árvores sintáticas incompletas para etapas seguintes.

4. Falha semântica explícita de member receiver
- símbolo base `this` não resolvido no caminho que compila `C_getValue`.
- evidencia incompatibilidade de lowering entre código gerado por MaiaJS e backend MaiaCpp/webc.

## 4. Responsabilidade por camada

### MaiaJS (fonte primária)

- Deve reduzir profundidade de expressão emitida.
- Deve emitir formas C++ consumíveis por MaiaCpp (sem acesso direto JS-like em `void*`).

### MaiaCpp (robustez necessária)

Mesmo com input imperfeito, o parser não deve degradar para timeout prolongado.

## 5. Plano de correção detalhado (MaiaCpp)

## Fase A - Parser grammar-first

1. Revisar `grammar/Cpp.ebnf` para pontos de ambiguidade/backtracking em:
- nested function-call expressions;
- chains de postfix/member access;
- cast + call + member combinações.
2. Reduzir ambiguidade com fatoração/precedência explícita nas regras de expressão.
3. Regenerar parser exclusivamente via MaiaCC:
- `./maiacc/bin/tREx.sh --ebnf --to-xml ./grammar/Cpp.xml ./grammar/Cpp.ebnf ./compiler/cpp-parser.js`

## Fase B - Guardrails de performance de parse

1. Introduzir testes de stress sintático com expressão profundamente aninhada.
2. Medir tempo de parse e impor limite máximo aceitável por fixture.
3. Se limite excedido:
- retornar diagnóstico estruturado rápido (sem timeout longo),
- identificar regra de maior custo no trace parser.

## Fase C - Hardening do pipeline webcpp

1. Melhorar mensagem de erro quando fallback simples é acionado, incluindo:
- função/regra sintática onde o parse estourou custo,
- trecho de código reduzido (snippet).
2. Evitar encadeamento de etapas caras após timeout sem sinalizar causa raiz.

## Fase D - Testes de integração cruzada MaiaJS -> MaiaCpp

1. Adicionar fixture no conjunto MaiaCpp contendo padrão de array-builder profundo similar ao gerado por MaiaJS.
2. Rodar `compiler/examples/build_test_dist.sh` e também caso cross-repo com input do MaiaJS.
3. Validar não apenas parse/sucesso, mas também tempo de execução do parser.
4. Adicionar verificação dedicada para erros `Unknown base symbol 'this'` em métodos de classe gerados por MaiaJS.

## 6. Protocolo obrigatório da suite

Caso mudanças atinjam parser/gerador compartilhado, aplicar na ordem:

1. MaiaCC
2. MaiaWASM
3. MaiaC
4. MaiaCpp
5. MaiaJS

Regras:

- Não editar `compiler/cpp-parser.js` manualmente.
- Toda mudança de parser deve nascer em `grammar/Cpp.ebnf` e ser regenerada com tREx.

## 7. Critérios de aceite

1. Parser Cpp não entra em timeout com o `test.cpp` gerado pelo stress test de MaiaJS.
2. `webcpp` retorna erro sintático objetivo em tempo curto quando o input é inválido semanticamente.
3. Build padrão de MaiaCpp permanece verde.
4. Integração MaiaJS -> MaiaCpp com stress arrays deixa de depender de fallback simples.

## 8. Comandos de validação

- Parser regen MaiaCpp:
  - `cd /Volumes/External_SSD/Documentos/Projects/maiacpp`
  - `./maiacc/bin/tREx.sh --ebnf --to-xml ./grammar/Cpp.xml ./grammar/Cpp.ebnf ./compiler/cpp-parser.js`

- Smoke tests:
  - `bash compiler/test_grammar.sh`
  - `bash compiler/examples/build_test_dist.sh`

- Integração com output do MaiaJS:
  - `cd /Volumes/External_SSD/Documentos/Projects/maiajs`
  - `bash compiler/examples/build_test_dist.sh`
