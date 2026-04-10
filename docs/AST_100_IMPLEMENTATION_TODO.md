# MaiaCpp AST-First 100 Percent TODO

Date: 2026-04-10

Goal:
- Remove all regex/source heuristics from the compilation path.
- Make semantic analysis and C emission driven only by AST nodes generated from Cpp.ebnf.
- Reach full grammar conformance with explicit tiering where C89/MaiaC cannot represent full C++98 runtime semantics.

## 1) Ground Rules

- Keep parser as single source of syntax truth: grammar/Cpp.ebnf -> compiler/cpp-parser.js.
- For every grammar family, implement three layers:
  - Parse acceptance
  - AST semantic extraction
  - C89 emission behavior (or explicit parse-only classification)
- Eliminate fallback behavior from production path:
  - normalizeForParser namespace flattening
  - inferGlobalFunctions regex extraction
  - SimpleAnalyzer regex class/function parsing
  - hardcoded run_* lowering as semantic substitute

## 2) Current Gaps (Observed)

- AST use is partial for declarations/classes/functions.
- Function body lowering is still pattern-based and incomplete.
- Namespaces are normalized/flattened instead of resolved from scope tree.
- Templates are parsed but not semantically instantiated.
- Exceptions are declared in runtime bridge but not fully lowered from AST try/throw/catch.
- Multiple grammar families are parse-level only, not semantic/codegen complete.

## 3) Implementation Phases

## Phase 0 - Migration Safety Rail (introduced)

- [x] Add an opt-in AST-first strict mode in compiler path (`--ast-strict`) to validate AST-only behavior without breaking the current baseline.
- [x] Keep legacy normalized/fallback behavior as default while fixture parity is incomplete.
- [x] Recover fixture baseline parity after AST migration checkpoints (compiler fixtures: 40/40 passing).
- [x] Keep compatibility lane explicit for function-level hints (`--legacy-function-hints`, default on; opt-out via `--no-legacy-function-hints`).
- [ ] Flip default to AST-strict after parity for fixture families currently relying on source-hint/fallback paths.

Acceptance:
- AST-only path is executable today via explicit flag.
- Default path remains stable for existing fixtures while migration work is in progress.
- Regression suite currently passes fully in compatibility mode (40/40 fixtures).

## Phase A - AST Semantic Core

- [ ] Build a full scope tree from AST:
  - translation unit
  - namespace scopes
  - class scopes
  - block scopes
- [ ] Replace regex declaration extraction with AST declaration extraction.
- [ ] Extract typedefs and function-pointer typedefs from AST only.
- [ ] Track storage/function/cv specifiers in symbol table.

Acceptance:
- No calls in compile path to regex extractors for declarations/functions/classes.
- Compiler output for existing baseline fixtures remains stable.

## Phase B - Class/Object Model From AST

- [ ] Extract full class model from classSpecifier/memberSpecification:
  - fields (including arrays/pointers)
  - methods
  - constructors
  - destructor
  - base classes + access
- [ ] Extract ctorInitializer/memInitializerList as semantic actions.
- [ ] Keep mangling source solely from AST symbols.

Acceptance:
- Generated C structs/method signatures match AST structure without source text hints.
- Constructor member initialization comes from AST initializer list, not heuristics.

## Phase C - Statements and Expressions From AST

- [ ] Build AST visitors for:
  - declaration statements
  - expression statements
  - selection/iteration/jump statements
  - try/catch/throw statements
- [ ] Build typed expression lowering for assignment/conditional/logical/arithmetic/casts.
- [ ] Remove simpleReturnExpr/simpleReturnCall/simpleIfReturn regex inference path.

Acceptance:
- Global and member function bodies are emitted from AST statement/expression trees.
- No hardcoded run_* lowering required for baseline behavior.

## Phase D - Name/Type Resolution and Overload

- [ ] Namespace-aware symbol resolution without flattening input source.
- [ ] Overload resolution from AST argument types and conversion ranking.
- [ ] Operator function ID and conversion function ID resolution from AST.

Acceptance:
- Qualified/unqualified lookup resolution is deterministic and scope-correct.
- Overload choice trace is reproducible from AST symbol/type data.

## Phase E - Templates and Advanced C++98

- [ ] Parse and represent template declarations/parameters/ids in semantic model.
- [ ] Implement explicit instantiation/specialization lowering strategy.
- [ ] Define policy for template features that cannot be emitted to C89 directly.

Acceptance:
- Template declarations are semantically represented and validated.
- Explicit instantiations produce concrete C symbols where supported.

## Phase F - Exception and Runtime Bridge

- [ ] Lower try/catch/throw AST nodes to runtime bridge calls:
  - __exc_push/__exc_pop
  - __exc_throw
  - __exc_type/__exc_matches
- [ ] Define object lifetime rules during exception flow (destructor calls where supported).

Acceptance:
- Exception fixtures compile and execute with expected catch behavior for supported subset.

## Phase G - Conformance and Testing

- [ ] Introduce tiered conformance suite:
  - Tier 1: must parse + compile-to-C + run
  - Tier 2: must parse + compile-to-C (behavior constrained)
  - Tier 3: parse-only (documented out-of-scope runtime in C89 bridge)
- [ ] For each grammar family in Cpp.ebnf, add at least one fixture and status.
- [ ] Publish machine-readable conformance report per run.

Acceptance:
- 100 percent of grammar families are classified and tested.
- No unclassified grammar family remains.

## 4) C89/MaiaC Constraint Ledger

These are not parser gaps; they are backend/language-boundary constraints and must be tracked explicitly:

- C89 has no classes/templates/exceptions/overloading as native constructs.
- Template semantics require compile-time monomorphization before C emission.
- Virtual dispatch requires explicit vtable lowering strategy.
- Some C++98 constructs may be parse-only in first complete conformance pass:
  - asmDefinition
  - linkageSpecification (extern "C")
  - advanced RTTI/typeid patterns
  - complex template specialization patterns

Policy:
- Do not silently stub unsupported runtime behavior.
- Emit explicit diagnostics + conformance status tag.

## 5) Completion Definition

The compiler is considered AST-first complete when:

1. Production compile path has no regex semantic fallback.
2. Every major Cpp.ebnf family has semantic status + tests.
3. Tier 1 and Tier 2 suites pass in MaiaCpp -> C -> MaiaC pipeline.
4. Tier 3 items are documented with explicit rationale and migration plan.
