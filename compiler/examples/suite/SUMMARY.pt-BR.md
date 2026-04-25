# Test Suite Diagnostic Summary

## Objetivo da Suite

Criar uma bateria de testes para validar que o MaiaCpp consegue transpilar construções C++98 válidas para C89 executável.

## O Que Foi Descoberto

**MaiaCpp tem BUG CRÍTICO**: O compiler não consegue fazer "lowering" (tradução) de construções C++98 absolutamente essenciais:

### ❌ FALHA (Stub-Fallback / No Lowering Support)
```
1. Function calls      → printf(), strcpy(), ANY function call → stubified
2. If-statements       → if (x) { ... } → stubified  
3. Ternary operator    → (x) ? a : b → stubified
4. While/do-while      → Possibly affected
```

### ✅ FUNCIONA
```
- Variáveis com literais: int x = 5;
- Return simples: return x;
- Expressions aritméticas (constant-folded)
- Definições de classe/template (parsing only)
```

## Arquivos Criados

### Testes Originais (10 categorias)
```
suite/01_operators/operators.cpp          — Todos operadores C++98
suite/02_control_flow/control_flow.cpp    — if/for/while/switch
suite/03_functions/functions.cpp          — Recursão, overload, ptrs
suite/04_classes/classes.cpp              — Ctors, operators, const
suite/05_templates/templates.cpp          — Templates genéricos
suite/06_inheritance/inheritance.cpp      — Virtual, abstract, casts
suite/07_memory/memory.cpp                — new/delete, RAII
suite/08_arrays_pointers/arrays_pointers.cpp — Arrays, pointers
suite/09_strings/strings.cpp              — C-strings
suite/10_casts/casts.cpp                  — Type casts
```

### Diagnóstico
```
suite/diagnostic.cpp              — Teste 1: Isolamento sistemática
suite/diagnostic-refined.cpp      — Teste 2: Binary search de falhas
suite/DIAGNOSTICS.md              — Análise técnica completa
suite/README.md                   — Guia da suite
```

## Resultado do Teste

**Transpilação**: ✅ 10/10 testes compilam para .wasm (parsingworks)  
**Lowering**: ❌ 0/10 testes fazem lowering (compiler fails)  
**Execução**: ❌ Nenhum teste produz output (todos stubificados)

## Exemplo: O Que Acontece

**Código C++98 válido:**
```cpp
int test() {
    int a = 10, b = 5;
    if (a > b) printf("PASS\n");
    return 1;
}
```

**Transpilado por MaiaCpp:**
```c
int test(void) {
  return (int)0;   // ← STUB! Printf + if não foram traduzidos
}
```

**Diagnóstico no log:**
```
/* - test: stub-fallback (no-supported-lowering) */
```

## Conclusão

Os testes **estão corretos**. Expõem bugs reais no MaiaCpp:

| Severidade | Problema | Impacto |
|-----------|----------|--------|
| CRÍTICA | Sem suporte a function calls | Nenhum programa pode funcionar |
| CRÍTICA | Sem suporte a if-statements | Sem lógica condicional |
| ALTA | Sem suporte a ternário | Sem expressões condicionais |

**MaiaCpp não consegue transpilar QUALQUER programa C++98 útil no momento.**

---

### Próximos Passos

1. **Para MaiaCpp devs**: Implementar lowering para function calls (prioridade 1)
2. **Para validação**: Re-rodar testes após fixes do compiler
3. **Para debugging**: Usar diagnostic.cpp e DIAGNOSTICS.md como referência

