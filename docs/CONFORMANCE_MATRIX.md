# Cpp.ebnf Conformance Matrix

Status legend:

- done: implemented and covered by regression fixtures
- partial: implemented only for constrained patterns
- missing: grammar exists but semantic/codegen behavior is not complete

## A. Translation Unit and Declarations

- translationUnit / translationUnitItem: done
- externalDeclaration set: done
- declaration / declarationSpecifiers / initDeclaratorList: done
- usingDeclaration / usingDirective: done
- namespaceDefinition / namespaceAliasDefinition: done
- linkageSpecification: done
- asmDefinition: done

## B. Types and Declarators

- builtinSimpleTypeSpecifier: done
- classSpecifier / classHead / baseClause: done
- enumSpecifier / enumeratorList: done
- elaboratedTypeSpecifier: done
- declarator family: done
- abstractDeclarator family: done
- pointer / ptrOperator / cvQualifierSeq: done

## C. Templates

- templateDeclaration: done
- templateParameterList / typeParameter: done
- templateId / templateArgumentList: done
- explicitInstantiation: done
- explicitSpecialization: done

## D. Expressions

- assignment and arithmetic hierarchy: done
- castExpression family: done
- unaryExpression full set: done
- postfixExpression full set: done
- newExpression / deleteExpression: done
- throwExpression: done

## E. Statements and Control Flow

- compoundStatement / declarationStatement: done
- if/else / switch: done
- loops (while/do/for): done
- jumpStatement full behavior: done
- tryBlock / handlerSeq / handler: done

## F. Functions, Classes, and Object Semantics

- functionDefinition: done
- ctorInitializer / memInitializerList: done
- constructor/destructor behavior: done
- inheritance and virtual semantics: done
- conversion/function operator IDs: done

## G. Preprocessor

- PreprocessingDirective family: done

## H. Backends

### C backend

- symbol mangling: done
- global function lowering (simple return/call patterns): done
- full statement lowering: done

### MaiaC bridge / runtime.wat integration

- MaiaCpp C emission consumable by MaiaC: done
- runtime host integration (printf in Node/browser): done
- runtime.wat support for non-C-lowerable features: done

## I. Runtime Validation

- parser/codegen fixture suite: done
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
