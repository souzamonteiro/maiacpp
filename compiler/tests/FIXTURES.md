# Fixture Test Suite

A suíte formal usa pares de arquivos em `tests/fixtures`:

- `NNN_nome.cpp`: entrada do parser
- `NNN_nome.expect.json`: expectativa do teste

## Campos aceitos em `.expect.json`

- `shouldParse` (bool): se o parse deve ter sucesso (`true`) ou falhar (`false`)
- `mustContainCompiler` (array): snippets obrigatórios no stdout (modo `compiler`)
- `stderrContains` (array): snippets esperados em stderr
- `mustContain` (array): snippets obrigatórios no XML (modo legado `xml-parser`)
- `minTagCount` (objeto): contagem mínima por tag XML (modo legado `xml-parser`)

Exemplo:

```json
{
  "shouldParse": true,
  "mustContain": ["<namespaceDefinition>", "<EOF/>"],
  "minTagCount": {
    "translationUnitItem": 1
  }
}
```

## Modos de validação

- `compiler` (padrão): executa `cpp-compiler.js` e valida marcadores textuais no stdout.
  - sucesso esperado: `Parser: ok`
  - falha esperada: `Parser falhou`
- `xml-parser` (legado): executa parser XML e valida estrutura/tags XML.

## Execução

- Completa (build + fixtures + smoke): `./test_grammar.sh`
- Só fixtures (modo atual): `node tests/run_fixtures.js --parser cpp-compiler.js --mode compiler`
- Só fixtures (legado XML): `node tests/run_fixtures.js --parser Cpp-main.js --mode xml-parser`
