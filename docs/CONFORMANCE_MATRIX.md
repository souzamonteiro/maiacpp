# Cpp.ebnf Conformance Matrix

Status legend:

- done: implemented and covered by regression fixtures
- partial: implemented only for constrained patterns
- missing: grammar exists but semantic/codegen behavior is not complete

## A. Translation Unit and Declarations

- translationUnit / translationUnitItem: partial
- externalDeclaration set: partial
- declaration / declarationSpecifiers / initDeclaratorList: partial
- usingDeclaration / usingDirective: partial
- namespaceDefinition / namespaceAliasDefinition: partial
- linkageSpecification: missing
- asmDefinition: missing

## B. Types and Declarators

- builtinSimpleTypeSpecifier: partial
- classSpecifier / classHead / baseClause: partial
- enumSpecifier / enumeratorList: partial
- elaboratedTypeSpecifier: partial
- declarator family: partial
- abstractDeclarator family: missing
- pointer / ptrOperator / cvQualifierSeq: partial

## C. Templates

- templateDeclaration: partial
- templateParameterList / typeParameter: partial
- templateId / templateArgumentList: partial
- explicitInstantiation: missing
- explicitSpecialization: missing

## D. Expressions

- assignment and arithmetic hierarchy: partial
- castExpression family: partial
- unaryExpression full set: partial
- postfixExpression full set: partial
- newExpression / deleteExpression: partial
- throwExpression: missing

## E. Statements and Control Flow

- compoundStatement / declarationStatement: partial
- if/else / switch: partial
- loops (while/do/for): partial
- jumpStatement full behavior: partial
- tryBlock / handlerSeq / handler: missing

## F. Functions, Classes, and Object Semantics

- functionDefinition: partial
- ctorInitializer / memInitializerList: partial
- constructor/destructor behavior: partial
- inheritance and virtual semantics: partial
- conversion/function operator IDs: missing

## G. Preprocessor

- PreprocessingDirective family: missing as true preprocessing pipeline

## H. Backends

### C backend

- symbol mangling: done
- global function lowering (simple return/call patterns): partial
- full statement lowering: missing

### MaiaC bridge / runtime.wat integration

- MaiaCpp C emission consumable by MaiaC: partial
- runtime host integration (printf in Node/browser): partial
- runtime.wat support for non-C-lowerable features: partial

## I. Runtime Validation

- parser/codegen fixture suite: done for current subset
- executable behavior conformance suite against C++98 semantics: missing

## J. Mandatory Completion Criteria

To claim 100 percent ready, each grammar family must be done in all layers:

1. parser acceptance
2. semantic resolution
3. C backend behavior
4. MaiaC bridge behavior
5. runtime validation fixture
