/**
 * Class Analysis Module for C++98 → C Transpiler
 * 
 * Responsibilities:
 *   - Extract class definitions from AST
 *   - Compute memory layout (with inheritance offsets)
 *   - Identify virtual methods and generate vtables
 *   - Track constructors and destructors
 */

class ClassAnalyzer {
  constructor(symbolTable, parseTree) {
    this.symbolTable = symbolTable;
    this.parseTree = parseTree;
    this.classes = new Map();
    this.classOrder = [];  // topological sort for dependencies
  }

  analyze() {
    this.findAllClasses(this.parseTree);
    this.resolveInheritance();
    this.computeLayouts();
    return {
      classes: this.classes,
      order: this.classOrder,
    };
  }

  findAllClasses(node) {
    if (!node) return;
    if (node.kind === 'nonterminal') {
      if (node.name === 'classSpecifier' || node.name === 'classDefinition') {
        this.extractClass(node);
      }
      // Recursively search children
      if (node.children) {
        for (const child of node.children) {
          this.findAllClasses(child);
        }
      }
    }
  }

  extractClass(classNode) {
    // Extract class name
    const nameNode = this.findChild(classNode, 'className');
    const className = nameNode ? this.extractText(nameNode) : null;

    if (!className) return;
    if (this.classes.has(className)) {
      return;  // Already defined
    }

    const classInfo = {
      name: className,
      kind: 'class',
      members: [],       // { name, type, access, offset }
      methods: [],       // { name, returnType, params, isVirtual, access }
      constructors: [],  // { params, access, body }
      destructor: null,  // { access, body }
      copyConstructor: null,
      assignmentOp: null,
      bases: [],         // { name, access, offset }
      virtuals: [],      // list of virtual methods
      size: 0,
      hasVtable: false,
      access: 'public',
    };

    // Extract base classes (if any)
    const baseSpecList = this.findChild(classNode, 'baseSpecifierList');
    if (baseSpecList) {
      this.extractBases(baseSpecList, classInfo);
    }

    // Extract members and methods
    const body = this.findChild(classNode, 'classBody');
    if (body) {
      this.extractClassBody(body, classInfo);
    }

    this.classes.set(className, classInfo);
    this.classOrder.push(className);
  }

  extractBases(baseSpecList, classInfo) {
    const baseSpecifiers = this.findAllChildren(baseSpecList, 'baseSpecifier');
    let offset = 0;

    for (const spec of baseSpecifiers) {
      const baseName = this.extractText(spec).trim();
      const baseClass = this.classes.get(baseName);

      classInfo.bases.push({
        name: baseName,
        access: 'public',  // TODO: Extract actual access specifier
        offset: offset,
      });

      // Accumulate offset
      if (baseClass) {
        offset += baseClass.size;
      }
    }
  }

  extractClassBody(bodyNode, classInfo) {
    if (!bodyNode || !bodyNode.children) return;

    const members = this.findAllChildren(bodyNode, 'memberDeclaration');
    let currentAccess = 'private';  // default for classes
    let fieldOffset = 0;

    for (const member of members) {
      // Check for access specifier
      const accessNode = this.findChild(member, 'accessSpecifier');
      if (accessNode) {
        currentAccess = this.extractText(accessNode).toLowerCase();
        continue;
      }

      // Member variable
      const varDecl = this.findChild(member, 'memberVariableDeclaration');
      if (varDecl) {
        const varInfo = this.extractMemberVariable(varDecl);
        if (varInfo) {
          varInfo.access = currentAccess;
          varInfo.offset = fieldOffset;
          classInfo.members.push(varInfo);
          fieldOffset += 4;  // TODO: compute actual size based on type
        }
      }

      // Member function
      const funcDecl = this.findChild(member, 'memberFunctionDeclaration');
      if (funcDecl) {
        const funcInfo = this.extractMemberFunction(funcDecl);
        if (funcInfo) {
          funcInfo.access = currentAccess;
          classInfo.methods.push(funcInfo);

          if (funcInfo.name === classInfo.name) {
            classInfo.constructors.push(funcInfo);
          } else if (funcInfo.name === '~' + classInfo.name) {
            classInfo.destructor = funcInfo;
          } else if (funcInfo.isVirtual) {
            classInfo.virtuals.push(funcInfo);
            classInfo.hasVtable = true;
          }
        }
      }
    }

    // Layout size: all members + vtable pointer if needed
    classInfo.size = fieldOffset;
    if (classInfo.hasVtable) {
      classInfo.size += 8;  // vtable pointer (64-bit)
    }

    // Add size from bases
    for (const base of classInfo.bases) {
      const baseClass = this.classes.get(base.name);
      if (baseClass) {
        classInfo.size += baseClass.size;
      }
    }
  }

  extractMemberVariable(varNode) {
    if (!varNode) return null;

    const typeStr = this.extractText(this.findChild(varNode, 'type'));
    const name = this.extractText(this.findChild(varNode, 'identifier'));

    if (!name || !typeStr) return null;

    return {
      kind: 'variable',
      name: name,
      type: typeStr,
    };
  }

  extractMemberFunction(funcNode) {
    if (!funcNode) return null;

    const isVirtual = !!this.findChild(funcNode, 'VIRTUAL');
    const isConst = !!this.findChild(funcNode, 'CONST');
    const isPureVirtual = !!this.findChild(funcNode, 'EQUALS') &&
                          !!this.findChild(funcNode, 'LITERAL_0');

    const returnType = this.extractText(this.findChild(funcNode, 'type')) || 'void';
    const name = this.extractText(this.findChild(funcNode, 'declarator'));
    const params = this.extractParameters(this.findChild(funcNode, 'parameterList'));

    return {
      kind: 'method',
      name: name,
      returnType: returnType,
      params: params,
      isVirtual: isVirtual,
      isConst: isConst,
      isPureVirtual: isPureVirtual,
      body: this.findChild(funcNode, 'functionBody'),
    };
  }

  extractParameters(paramList) {
    if (!paramList) return [];

    const params = [];
    const decls = this.findAllChildren(paramList, 'parameterDeclaration');

    for (const decl of decls) {
      const typeStr = this.extractText(this.findChild(decl, 'type'));
      const nameStr = this.extractText(
        this.findChild(decl, 'declarator') ||
        this.findChild(decl, 'identifier')
      );

      if (nameStr) {
        params.push({
          name: nameStr,
          type: typeStr,
        });
      }
    }

    return params;
  }

  resolveInheritance() {
    // TODO: Check for circular inheritance, resolve diamond problem
  }

  computeLayouts() {
    // TODO: Compute final memory layout with multiple inheritance offsets
  }

  findChild(node, name) {
    if (!node || !node.children) return null;
    for (const child of node.children) {
      if (child.kind === 'nonterminal' && child.name === name) {
        return child;
      }
    }
    return null;
  }

  findAllChildren(node, name) {
    if (!node || !node.children) return [];
    return node.children.filter(ch => ch.kind === 'nonterminal' && ch.name === name);
  }

  extractText(node) {
    if (!node) return '';
    if (node.kind === 'terminal') return node.value || '';
    if (node.children) {
      return node.children.map(ch => this.extractText(ch)).join(' ');
    }
    return '';
  }
}

module.exports = { ClassAnalyzer };
