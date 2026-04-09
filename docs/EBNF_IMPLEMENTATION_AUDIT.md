# Cpp.ebnf Implementation Audit

Date: 2026-04-09

This audit defines what is currently not working (or only partially working) in MaiaCpp relative to grammar/Cpp.ebnf, and what must be implemented next to reach full semantic/codegen behavior.

## 1. Scope and Method

Inputs used:

- grammar/Cpp.ebnf (216 nonterminal/token rules)
- compiler/cpp-compiler.js
- compiler/tests/fixtures (34 fixtures)
- docs/SUPPORT_ROADMAP.md

Important interpretation:

- Parser support means the parser can often accept syntax.
- Compiler support means semantics + codegen produce behaviorally correct output.
- Current gap is mostly semantic/codegen, not only parser syntax.

## 2. Current Reality (Short Version)

What is proven to work now:

- Parser succeeds for current fixture subset.
- Namespace-aware symbol mangling and incremental call lowering for selected global-return patterns.
- Overload tie-break rules (exact, conversion ranking, paramwise tie, scope tie, stable fallback).
- Runtime target scripts for console, Node, browser.
- WASM runner now prints via host printf integration.
- Default executable path is `C++ -> C (MaiaCpp) -> WAT/WASM (MaiaC)`.

What is not fully working:

- C backend is also partial (many constructs still emitted as stubs/default values).
- Large parts of C++98 grammar are parse-level only and not translated correctly.

## 3. Non-Working / Partial Areas (Must Fix)

### 3.1 Function and Statement Semantics

Status: partial

Not fully implemented:

- Full statement lowering in function bodies (general if/else, switch, loops, labels, goto).
- Correct lowering into MaiaC-consumable C for arbitrary blocks.
- Correct data-flow/local variable lifetime and initialization semantics.
- Full return-path correctness for non-trivial expressions.

Impact:

- Programs may parse but execute incorrectly in generated outputs.

### 3.2 Exception Model

Status: missing/partial

From grammar area:

- functionTryBlock
- tryBlock
- handlerSeq
- handler
- exceptionDeclaration
- throwExpression
- exceptionSpecification

Not fully implemented:

- Real try/catch/throw lowering and runtime propagation semantics.
- Type-based catch matching across inheritance.

### 3.3 Class/Object Semantics

Status: partial

From grammar area:

- classSpecifier/classHead/baseClause/memberDeclaration
- ctorInitializer/memInitializerList
- destructor semantics
- virtual dispatch and full vtable behavior

Not fully implemented:

- Correct constructor/destructor side effects in generated code.
- Full base-class layout and virtual call semantics.
- Member initialization list semantics in all cases.

### 3.4 Templates

Status: very partial

From grammar area:

- templateDeclaration
- templateParameterList/typeParameter
- templateId/templateArgumentList
- explicitInstantiation/explicitSpecialization

Not fully implemented:

- Real template instantiation model.
- Specialization resolution.
- Template argument-dependent symbol generation beyond narrow cases.

### 3.5 Declarators and Types

Status: partial

From grammar area:

- declarator/directDeclarator/directDeclaratorSuffix
- abstractDeclarator/directAbstractDeclarator
- pointer/ptrOperator/typeQualifierList
- conversionFunctionId/conversionTypeId

Not fully implemented:

- Full declarator graph lowering (especially nested/function-pointer/array combinations).
- Complete type canonicalization and qualifier propagation.

### 3.6 Operator and Expression Coverage

Status: partial

From grammar area:

- unaryExpression/postfixExpression/pmExpression/castExpression
- assignmentExpression/conditionalExpression
- dynamic_cast/static_cast/reinterpret_cast/const_cast/typeid

Not fully implemented:

- General expression lowering in the C backend.
- Full operator semantics and precedence-preserving translation for all forms.

### 3.7 New/Delete and Placement New

Status: partial

From grammar area:

- newExpression/newPlacement/newDeclarator/newInitializer
- deleteExpression

Not fully implemented:

- Complete allocation/deallocation semantics and object lifecycle integration.
- Correct behavior for arrays and custom placement forms beyond tested subset.

### 3.8 Namespaces and Using Constructs

Status: partial

From grammar area:

- namespaceAliasDefinition
- usingDeclaration
- usingDirective
- linkageSpecification

Not fully implemented:

- Full semantic resolution with aliases and transitive using scopes across modules.
- Correct behavior in all mixed translation-unit combinations.

### 3.9 Enums and Elaborated Type Specifiers

Status: partial

From grammar area:

- enumSpecifier/enumeratorList
- elaboratedTypeSpecifier

Not fully implemented:

- Robust enum value propagation and cross-scope type resolution in codegen.

### 3.10 Preprocessing Directives

Status: mostly parse-level only

From grammar area:

- PreprocessingDirective family (define/ifdef/include/etc.)

Not fully implemented:

- Real preprocessor pipeline integrated with semantic/codegen stages.

### 3.11 MaiaC Bridge and runtime.wat Correctness

Status: partial (major gap)

Not fully implemented:

- Full lowering of AST/semantic model into MaiaC-consumable C for all executable constructs.
- Reliable runtime semantics equivalent to source behavior after the MaiaC handoff.
- runtime.wat coverage for features that cannot be represented in target C, especially exception support.

## 4. Coverage vs Grammar Breadth

- Grammar rules: 216
- Fixture files: 34

Conclusion:

- Current validated subset is significantly smaller than grammar breadth.
- The main blocker is semantic and backend completeness, not only parser syntax.

## 5. What To Fix First (Execution-Critical)

Priority A (must-do for real compiler usability):

1. Expand C backend statement lowering (`if/else`, loops, `return`) so MaiaC receives semantically faithful C.
2. Expand local variable + expression lowering in the C backend.
3. Define and isolate what stays in runtime.wat because the C bridge cannot express it cleanly.
4. Add behavioral execution fixtures (assert runtime output/return values), not only textual marker fixtures.

Priority B:

1. Implement full object lifecycle (constructors/destructors/base init).
2. Implement exception runtime and catch matching behavior.
3. Tighten MaiaC bridge compatibility guarantees in generated C.

Priority C:

1. Template instantiation/specialization model.
2. Full declarator/type system completeness.
3. Preprocessor integration.

## 6. Acceptance Criteria for "EBNF Implemented Correctly"

"Implemented" should require all three layers to pass:

1. Parse: syntax accepted according to grammar.
2. Semantic: symbols/types/scopes resolved correctly.
3. Codegen behavior: generated C and downstream MaiaC WAT/WASM execute equivalent behavior on conformance fixtures.

Without layer 3, grammar compliance is only superficial.

## 7. Immediate Next Deliverable

Create a conformance matrix file mapping each major grammar family to:

- parser status
- semantic status
- C codegen status
- MaiaC bridge/runtime status
- runtime fixture status

and gate each family with at least one executable fixture.
