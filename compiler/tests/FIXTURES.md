# Fixture Test Suite

A suíte formal usa pares de arquivos em `tests/fixtures`:

- `NNN_nome.cpp`: entrada do parser
- `NNN_nome.expect.json`: expectativa do teste

## Campos aceitos em `.expect.json`

- `shouldParse` (bool): se o parse deve ter sucesso (`true`) ou falhar (`false`)
- `mustContain` (array): snippets obrigatórios no XML de saída
- `minTagCount` (objeto): contagem mínima por tag XML

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

## Execução

- Completa (build + fixtures + smoke): `./test_grammar.sh`
- Só fixtures: `python3 tests/run_fixtures.py`
