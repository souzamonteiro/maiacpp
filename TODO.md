# MaiaCpp TODO (AST-First 100 Percent)

Main plan:
- See docs/AST_100_IMPLEMENTATION_TODO.md

Current conformance target input:
- compiler/test_ebnf_full.cpp

Current observed gap from target:
- Parser fails on full target file with:
  - Expected 'EOF', got 'TOKEN_template'

Immediate next execution steps:
1. Fix parser/semantic path for full templateDeclaration and explicitInstantiation/specialization flow.
2. Remove normalizeForParser/source-heuristic fallback from production path.
3. Implement AST-driven statement/expression lowering for global/member functions.
4. Classify all Cpp.ebnf families into Tier 1 (run), Tier 2 (transpile), Tier 3 (parse-only).
5. Gate completion by MaiaCpp -> C -> MaiaC runtime tests.
