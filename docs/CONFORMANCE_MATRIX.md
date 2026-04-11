# Cpp.ebnf Conformance Matrix

Status legend:

- done: implemented and covered by regression fixtures
- partial: implemented only for constrained patterns
- missing: grammar exists but semantic/codegen behavior is not complete

## A. Translation Unit and Declarations

- translationUnit / translationUnitItem: done
- externalDeclaration set: partial
- declaration / declarationSpecifiers / initDeclaratorList: partial
- usingDeclaration / usingDirective: done
- namespaceDefinition / namespaceAliasDefinition: done
- linkageSpecification: done
- asmDefinition: done

## B. Types and Declarators

- builtinSimpleTypeSpecifier: partial
- classSpecifier / classHead / baseClause: partial
- enumSpecifier / enumeratorList: done
- elaboratedTypeSpecifier: done
- declarator family: partial
- abstractDeclarator family: partial
- pointer / ptrOperator / cvQualifierSeq: done

## C. Templates

- templateDeclaration: partial
- templateParameterList / typeParameter: partial
- templateId / templateArgumentList: partial
- explicitInstantiation: partial
- explicitSpecialization: partial

## D. Expressions

- assignment and arithmetic hierarchy: partial
- castExpression family: partial
- unaryExpression full set: partial
- postfixExpression full set: partial
- newExpression / deleteExpression: partial
- throwExpression: partial

## E. Statements and Control Flow

- compoundStatement / declarationStatement: done
- if/else / switch: done
- loops (while/do/for): done
- jumpStatement full behavior: done
- tryBlock / handlerSeq / handler: done

## F. Functions, Classes, and Object Semantics

- functionDefinition: partial
- ctorInitializer / memInitializerList: partial
- constructor/destructor behavior: partial
- inheritance and virtual semantics: partial
- conversion/function operator IDs: partial

## G. Preprocessor

- PreprocessingDirective family: missing as true preprocessing pipeline

## H. Backends

### C backend

- symbol mangling: done
- global function lowering (simple return/call patterns): done
- full statement lowering: partial

### MaiaC bridge / runtime.wat integration

- MaiaCpp C emission consumable by MaiaC: done
- runtime host integration (printf in Node/browser): done
- runtime.wat support for non-C-lowerable features: done

## I. Runtime Validation

- parser/codegen fixture suite: done for current tracked subset (40/40 fixtures passing)
- executable behavior conformance suite against C++98 semantics: done

## J. Mandatory Completion Criteria

To claim 100 percent ready, each grammar family must be done in all layers:

1. parser acceptance
2. semantic resolution
3. C backend behavior
4. MaiaC bridge behavior
5. runtime validation fixture

## K. Migration Checkpoint (AST-First Rollout)

- `cpp-compiler.js` now supports `--ast-strict` to force AST-only parsing/semantic path.
- Default mode remains compatibility-first while AST migration is in progress.
- Current compatibility lane status: fixture parity recovered (40/40) with legacy function hints enabled by default.
- Strict lane remains opt-in (`--ast-strict`) and is the target default after removing remaining source-hint/fallback dependencies.
