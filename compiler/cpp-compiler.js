#!/usr/bin/env node

const fs = require('fs');
const os = require('os');
const path = require('path');
const { execFileSync } = require('child_process');
const Parser = require('./cpp-parser');
const { ParseTreeCollector } = require('./parse-tree-collector');

class Type {
  constructor(kind, name) {
    this.kind = kind;
    this.name = name || kind;
  }
}

class PointerType extends Type {
  constructor(base) {
    super('pointer', `${base.name}*`);
    this.base = base;
  }
}

class ReferenceType extends Type {
  constructor(base) {
    super('reference', `${base.name}&`);
    this.base = base;
  }
}

class ArrayType extends Type {
  constructor(base, size) {
    super('array', `${base.name}[${size}]`);
    this.base = base;
    this.size = size;
  }
}

class ClassType extends Type {
  constructor(name) {
    super('class', name);
    this.bases = [];
    this.members = [];
    this.methods = [];
    this.constructors = [];
    this.destructor = null;
    this.hasVtable = false;
  }
}

class FunctionType extends Type {
  constructor(returnType, paramTypes) {
    super('function', `${returnType.name}(${paramTypes.map((t) => t.name).join(',')})`);
    this.returnType = returnType;
    this.paramTypes = paramTypes;
  }
}

const BUILTIN_TYPES = {
  int: new Type('int', 'int'),
  unsigned: new Type('unsigned', 'unsigned'),
  long: new Type('long', 'long'),
  short: new Type('short', 'short'),
  char: new Type('char', 'char'),
  bool: new Type('bool', 'bool'),
  float: new Type('float', 'float'),
  double: new Type('double', 'double'),
  void: new Type('void', 'void')
};

const SymbolKind = {
  VARIABLE: 'variable',
  FUNCTION: 'function',
  CLASS: 'class',
  NAMESPACE: 'namespace',
  TYPEDEF: 'typedef'
};

class Symbol {
  constructor(kind, name, type) {
    this.kind = kind;
    this.name = name;
    this.type = type;
  }
}

class SymbolTable {
  constructor(parent = null) {
    this.parent = parent;
    this.symbols = new Map();
  }

  define(name, symbol) {
    this.symbols.set(name, symbol);
    return symbol;
  }

  lookup(name) {
    return this.symbols.get(name) || (this.parent ? this.parent.lookup(name) : null);
  }

  child() {
    return new SymbolTable(this);
  }
}

function typeCode(type) {
  if (!type) return 'v';
  const map = {
    int: 'i',
    unsigned: 'u',
    long: 'l',
    short: 's',
    char: 'c',
    bool: 'b',
    float: 'f',
    double: 'd',
    void: 'v'
  };
  if (map[type.kind]) return map[type.kind];
  if (type.kind === 'pointer') return `p${typeCode(type.base)}`;
  if (type.kind === 'reference') return `r${typeCode(type.base)}`;
  if (type.kind === 'array') return `a${type.size}${typeCode(type.base)}`;
  if (type.kind === 'class') return `N${type.name.length}${type.name}`;
  return 'x';
}

function mangle(name, paramTypes = [], className = null, namespace = []) {
  const ns = namespace.length ? `${namespace.join('_')}_` : '';
  const cl = className ? `${className}_` : '';
  const args = paramTypes.length ? `__${paramTypes.map(typeCode).join('')}` : '';
  return `${ns}${cl}${name}${args}`;
}

function isIdentChar(ch) {
  return /[A-Za-z0-9_]/.test(ch || '');
}

function findMatchingBrace(text, openIndex) {
  let depth = 0;
  for (let i = openIndex; i < text.length; i += 1) {
    const ch = text[i];
    if (ch === '{') depth += 1;
    if (ch === '}') {
      depth -= 1;
      if (depth === 0) return i;
    }
  }
  return -1;
}

function flattenSimpleNamespaces(source, maxPasses = 4) {
  let current = source;

  for (let pass = 0; pass < maxPasses; pass += 1) {
    let changed = false;
    let out = '';

    for (let i = 0; i < current.length;) {
      const prev = i > 0 ? current[i - 1] : '';
      if (prev && isIdentChar(prev)) {
        out += current[i];
        i += 1;
        continue;
      }

      if (current.slice(i, i + 9) !== 'namespace') {
        out += current[i];
        i += 1;
        continue;
      }

      const after = current[i + 9] || '';
      if (after && isIdentChar(after)) {
        out += current[i];
        i += 1;
        continue;
      }

      let j = i + 9;
      while (j < current.length && /\s/.test(current[j])) j += 1;

      if (current[j] === '=') {
        // namespace alias definition: keep as-is
        out += current[i];
        i += 1;
        continue;
      }

      // Skip namespace identifier if present.
      if (/[A-Za-z_]/.test(current[j] || '')) {
        j += 1;
        while (j < current.length && isIdentChar(current[j])) j += 1;
      }

      while (j < current.length && /\s/.test(current[j])) j += 1;
      if (current[j] !== '{') {
        out += current[i];
        i += 1;
        continue;
      }

      const close = findMatchingBrace(current, j);
      if (close < 0) {
        out += current[i];
        i += 1;
        continue;
      }

      // Flatten one namespace layer by keeping only body content.
      out += `${current.slice(j + 1, close)}\n`;
      i = close + 1;
      changed = true;
    }

    if (!changed) return current;
    current = out;
  }

  return current;
}

function stripUsingNamespaceDirectives(source) {
  return String(source || '').replace(/\busing\s+namespace\s+[^;]+;/g, '');
}

function extractNamespaceNames(source) {
  const names = new Set();
  const rx = /\bnamespace\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/g;
  let m;
  while ((m = rx.exec(String(source || ''))) !== null) {
    names.add(m[1]);
  }
  return names;
}

function stripNamespaceQualifiers(source, namespaceNames) {
  let out = String(source || '');
  for (const ns of namespaceNames || []) {
    const escaped = ns.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
    const rx = new RegExp(`\\b${escaped}\\s*::`, 'g');
    out = out.replace(rx, '');
  }
  return out;
}

function normalizeForParser(source) {
  const names = extractNamespaceNames(source);
  const flattened = flattenSimpleNamespaces(source);
  const noUsing = stripUsingNamespaceDirectives(flattened);
  return stripNamespaceQualifiers(noUsing, names);
}

function inferClassNamespaceMap(source) {
  const map = new Map();
  const text = String(source || '');

  function scanSegment(segment, nsPath = []) {
    const nsRx = /\bnamespace\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/g;
    let m;
    while ((m = nsRx.exec(segment)) !== null) {
      const nsName = m[1];
      const braceIndex = nsRx.lastIndex - 1;
      const close = findMatchingBrace(segment, braceIndex);
      if (close < 0) continue;

      const body = segment.slice(braceIndex + 1, close);
      const nextPath = [...nsPath, nsName];

      const classRx = /\b(class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?::\s*[^\{]+)?\{/g;
      let cm;
      while ((cm = classRx.exec(body)) !== null) {
        const className = cm[2];
        if (!map.has(className)) {
          map.set(className, nextPath);
        }
      }

      // Recurse for nested namespaces.
      scanSegment(body, nextPath);
      nsRx.lastIndex = close + 1;
    }
  }

  scanSegment(text, []);
  return map;
}

class CEmitter {
  constructor() {
    this.lines = [];
    this.level = 0;
  }

  line(text = '') {
    if (!text) {
      this.lines.push('');
      return;
    }
    this.lines.push(`${'  '.repeat(this.level)}${text}`);
  }

  block(head, cb) {
    this.line(head);
    this.level += 1;
    cb();
    this.level -= 1;
  }

  code() {
    return this.lines.join('\n');
  }
}

class SemanticAnalyzer {
  constructor(parseTree) {
    this.parseTree = parseTree;
    this.globals = new SymbolTable();
    this.classes = new Map();
    this.functions = [];
    this.namespaceStack = [];
  }

  analyze() {
    this.walk(this.parseTree);
    return {
      symbols: this.globals,
      classes: this.classes,
      functions: this.functions
    };
  }

  walk(node) {
    if (!node) return;

    const enteredNamespace = this.enterNamespaceIfNeeded(node);

    if (node.kind === 'nonterminal' && (node.name === 'classDefinition' || node.name === 'classSpecifier')) {
      const className = this.extractClassName(node);
      if (className) {
        const cls = new ClassType(className);
        cls.namespacePath = [...this.namespaceStack];
        this.classes.set(className, cls);
        this.globals.define(className, new Symbol(SymbolKind.CLASS, className, cls));
        const fqName = cls.namespacePath.length ? `${cls.namespacePath.join('::')}::${className}` : className;
        this.globals.define(fqName, new Symbol(SymbolKind.CLASS, fqName, cls));
      }
    }

    for (const child of node.children || []) {
      this.walk(child);
    }

    if (enteredNamespace) {
      this.namespaceStack.pop();
    }
  }

  extractClassName(node) {
    const classNameNode = this.findFirstNonterminal(node, 'className');
    const direct = this.text(classNameNode).trim();
    if (direct) return direct;

    const classHead = this.findFirstNonterminal(node, 'classHead');
    const identifier = this.findFirstTerminal(classHead, 'Identifier');
    if (identifier) return String(identifier.value || '').trim();

    // Fallback: scan terminals up to '{' and pick the first Identifier after class/struct/union.
    const terminals = [];
    this.collectTerminals(node, terminals);
    let sawClassKey = false;
    let inBody = false;

    for (let i = 0; i < terminals.length; i += 1) {
      const t = terminals[i];
      if (!t) continue;
      if (t.token === 'TOKEN_class' || t.token === 'TOKEN_struct' || t.token === 'TOKEN_union') {
        sawClassKey = true;
        continue;
      }
      if (sawClassKey && t.token === 'TOKEN__7B_') {
        inBody = true;
        continue;
      }
      if (inBody && t.token === 'TOKEN__7D_') {
        break;
      }
      if (sawClassKey && t.token === 'Identifier') {
        const value = String(t.value || '').trim();
        if (value) return value;
      }
    }

    return '';
  }

  findFirstNonterminal(node, name) {
    if (!node) return null;
    if (node.kind === 'nonterminal' && node.name === name) return node;
    for (const child of node.children || []) {
      const found = this.findFirstNonterminal(child, name);
      if (found) return found;
    }
    return null;
  }

  findFirstTerminal(node, token) {
    if (!node) return null;
    if (node.kind === 'terminal' && (!token || node.token === token)) return node;
    for (const child of node.children || []) {
      const found = this.findFirstTerminal(child, token);
      if (found) return found;
    }
    return null;
  }

  collectTerminals(node, out) {
    if (!node) return;
    if (node.kind === 'terminal') {
      out.push(node);
      return;
    }
    for (const child of node.children || []) {
      this.collectTerminals(child, out);
    }
  }

  enterNamespaceIfNeeded(node) {
    if (!node || node.kind !== 'nonterminal') return false;
    const namespaceNodeNames = new Set([
      'namespaceDefinition',
      'namedNamespaceDefinition',
      'originalNamespaceDefinition',
      'extensionNamespaceDefinition',
      'unnamedNamespaceDefinition'
    ]);
    if (!namespaceNodeNames.has(node.name)) return false;

    const ns = this.extractNamespaceName(node);
    if (!ns) return false;

    this.namespaceStack.push(ns);
    return true;
  }

  extractNamespaceName(node) {
    if (!node) return null;
    if (node.name === 'unnamedNamespaceDefinition') {
      return '__anon';
    }

    if (node.kind === 'terminal' && node.token === 'Identifier') {
      return (node.value || '').trim() || null;
    }

    for (const child of node.children || []) {
      if (child.kind === 'terminal' && child.token === 'Identifier') {
        return (child.value || '').trim() || null;
      }
    }

    for (const child of node.children || []) {
      const nested = this.extractNamespaceName(child);
      if (nested) return nested;
    }
    return null;
  }

  text(node) {
    if (!node) return '';
    if (node.kind === 'terminal') return node.value || '';
    return (node.children || []).map((c) => this.text(c)).join('');
  }
}

class SimpleAnalyzer {
  constructor(filePath) {
    this.content = fs.readFileSync(filePath, 'utf8');
  }

  analyze() {
    const classes = this.extractClasses();
    const symbols = new SymbolTable();
    for (const [className, cls] of classes) {
      symbols.define(className, new Symbol(SymbolKind.CLASS, className, cls));
    }
    return {
      symbols,
      classes,
      functions: []
    };
  }

  extractClasses() {
    const classes = new Map();
    const classHeaderRegex = /(class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?::\s*(?:public|private|protected)?\s*([A-Za-z_][A-Za-z0-9_]*))?\s*\{/g;
    let m;

    while ((m = classHeaderRegex.exec(this.content)) !== null) {
      const kind = m[1];
      const className = m[2];
      const baseName = m[3] || null;
      const bodyStart = classHeaderRegex.lastIndex - 1;
      const bodyEnd = this.findMatchingBrace(this.content, bodyStart);
      if (bodyEnd < 0) continue;

      const body = this.content.slice(bodyStart + 1, bodyEnd);
      const cls = new ClassType(className);
      cls.defaultAccess = kind === 'struct' ? 'public' : 'private';
      if (baseName) {
        cls.bases.push({ name: baseName, access: 'public' });
      }

      this.parseClassBody(body, cls);
      classes.set(className, cls);
      classHeaderRegex.lastIndex = bodyEnd + 1;
    }

    return classes;
  }

  findMatchingBrace(text, openBraceIndex) {
    let depth = 0;
    for (let i = openBraceIndex; i < text.length; i += 1) {
      const ch = text[i];
      if (ch === '{') depth += 1;
      if (ch === '}') {
        depth -= 1;
        if (depth === 0) return i;
      }
    }
    return -1;
  }

  parseClassBody(body, cls) {
    const cleanedBody = body
      .replace(/\/\/.*$/gm, '')
      .replace(/\/\*[\s\S]*?\*\//g, '');

    const normalized = cleanedBody.replace(/\r\n/g, '\n');
    const sections = [];
    const accessRegex = /\b(public|private|protected)\s*:/g;
    let cursor = 0;
    let currentAccess = cls.defaultAccess;
    let am;

    while ((am = accessRegex.exec(normalized)) !== null) {
      sections.push({ access: currentAccess, text: normalized.slice(cursor, am.index) });
      currentAccess = am[1];
      cursor = accessRegex.lastIndex;
    }
    sections.push({ access: currentAccess, text: normalized.slice(cursor) });

    const ctorRegex = new RegExp(`(?:explicit\\s+)?${cls.name}\\s*\\(([^)]*)\\)\\s*(?::[^{};]*)?\\s*\\{[\\s\\S]*?\\}`, 'g');
    const dtorRegex = new RegExp(`(?:virtual\\s+)?~${cls.name}\\s*\\(([^)]*)\\)\\s*\\{[\\s\\S]*?\\}`, 'g');
    const methodRegex = /(virtual\s+)?([A-Za-z_][A-Za-z0-9_:<>\s\*&]+?)\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)\s*(const)?\s*(?:\{[\s\S]*?\}|;)/g;
    const memberRegex = /([A-Za-z_][A-Za-z0-9_:<>\s\*&]+?)\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?:=[^;]+)?;/g;

    for (let s = 0; s < sections.length; s += 1) {
      const access = sections[s].access;
      const text = sections[s].text;
      let fm;

      while ((fm = ctorRegex.exec(text)) !== null) {
        cls.constructors.push({
          name: cls.name,
          params: this.parseParams(fm[1]),
          access
        });
      }
      ctorRegex.lastIndex = 0;

      const dtor = dtorRegex.exec(text);
      if (dtor) {
        cls.destructor = { name: `~${cls.name}`, params: [], access };
      }
      dtorRegex.lastIndex = 0;

      while ((fm = methodRegex.exec(text)) !== null) {
        const isVirtual = !!fm[1];
        const returnType = this.normalizeType(fm[2]);
        const name = fm[3];
        if (name === cls.name || name === `~${cls.name}`) continue;

        cls.methods.push({
          name,
          returnType,
          params: this.parseParams(fm[4]),
          isVirtual,
          isConst: !!fm[5],
          access
        });

        if (isVirtual) cls.hasVtable = true;
      }
      methodRegex.lastIndex = 0;

      const textWithoutMethods = text
        .replace(ctorRegex, '\n')
        .replace(dtorRegex, '\n')
        .replace(methodRegex, '\n');

      while ((fm = memberRegex.exec(textWithoutMethods)) !== null) {
        const type = this.normalizeType(fm[1]);
        const name = fm[2];
        if (!type || !name) continue;
        cls.members.push({ name, type, access });
      }
      memberRegex.lastIndex = 0;
    }
  }

  parseParams(paramListText) {
    const text = (paramListText || '').trim();
    if (!text || text === 'void') return [];
    const raw = text.split(',').map((s) => s.trim()).filter(Boolean);
    const params = [];
    for (let i = 0; i < raw.length; i += 1) {
      const p = raw[i].replace(/\s+/g, ' ').trim();
      const match = p.match(/^(.*?)([A-Za-z_][A-Za-z0-9_]*)$/);
      if (!match) {
        params.push({ type: this.normalizeType(p), name: `p${i + 1}` });
        continue;
      }
      params.push({
        type: this.normalizeType(match[1].trim()),
        name: match[2].trim()
      });
    }
    return params;
  }

  normalizeType(type) {
    const t = (type || '')
      .replace(/\b(public|private|protected)\b\s*:/g, '')
      .replace(/\bconst\b/g, '')
      .replace(/\s+/g, ' ')
      .trim();
    if (!t) return 'int';
    if (t.endsWith('&')) {
      return `${t.slice(0, -1).trim()}*`;
    }
    if (t === 'bool') return 'int';
    if (t === 'std::string') return 'char*';
    return t;
  }
}

class CppToCTranspiler {
  constructor(analysis) {
    this.analysis = analysis;
    this.em = new CEmitter();
  }

  transpile() {
    this.emitHeaders();
    this.emitClasses();
    return this.em.code();
  }

  emitHeaders() {
    this.em.line('/* Generated from C++98 source */');
    this.em.line('/* Target: C89 */');
    this.em.line();
    this.em.line('#include <stddef.h>');
    this.em.line('#include <stdint.h>');
    this.em.line('#include <stdlib.h>');
    this.em.line('#include <string.h>');
    this.em.line();
    this.em.line('/* Runtime interface */');
    this.em.line('extern void   __exc_push(void);');
    this.em.line('extern void   __exc_pop(void);');
    this.em.line('extern int    __exc_active(void);');
    this.em.line('extern int    __exc_type(void);');
    this.em.line('extern void*  __exc_data(void);');
    this.em.line('extern void   __exc_throw(int type, void* data);');
    this.em.line('extern void   __exc_clear(void);');
    this.em.line('extern int    __exc_matches(int thrown_type, int catch_type);');
    this.em.line('extern void*  __malloc(size_t size);');
    this.em.line('extern void   __free(void* ptr);');
    this.em.line();
  }

  emitClasses() {
    let id = 1;
    for (const [name] of this.analysis.classes) {
      this.em.line(`#define EXC_${name} ${id++}`);
    }
    if (this.analysis.classes.size) this.em.line();

    for (const [name, cls] of this.analysis.classes) {
      const nsPath = Array.isArray(cls.namespacePath) ? cls.namespacePath : [];
      this.em.line(`typedef struct ${name} {`);
      this.em.level += 1;
      for (const base of cls.bases) {
        this.em.line(`${base.name} __base;`);
      }
      for (const member of cls.members) {
        this.em.line(`${member.type} ${member.name};`);
      }
      if (cls.hasVtable) {
        this.em.line(`void* __vptr;`);
      }
      this.em.level -= 1;
      this.em.line(`} ${name};`);
      this.em.line();

      if (cls.constructors.length === 0) {
        this.em.line(`void ${mangle('init', [], name, nsPath)}(${name}* self);`);
      }
      for (const ctor of cls.constructors) {
        const sigTypes = ctor.params.map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
        const paramsText = this.formatParams(ctor.params, true);
        this.em.line(`void ${mangle('init', sigTypes, name, nsPath)}(${name}* self${paramsText ? `, ${paramsText}` : ''});`);
      }

      this.em.line(`void ${mangle('destroy', [], name, nsPath)}(${name}* self);`);

      for (const method of cls.methods) {
        const sigTypes = method.params.map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
        const paramsText = this.formatParams(method.params, true);
        this.em.line(`${method.returnType} ${mangle(method.name, sigTypes, name, nsPath)}(${name}* self${paramsText ? `, ${paramsText}` : ''});`);
      }

      this.em.line();
      this.emitClassStubs(name, cls, nsPath);
    }
  }

  typeKindFromText(typeText) {
    const t = (typeText || '').trim();
    if (t.endsWith('*')) return 'pointer';
    if (BUILTIN_TYPES[t]) return t;
    return 'class';
  }

  formatParams(params, includeType) {
    return (params || []).map((p, idx) => {
      const pname = p.name || `p${idx + 1}`;
      return includeType ? `${p.type} ${pname}` : pname;
    }).join(', ');
  }

  emitClassStubs(name, cls, nsPath = []) {
    if (cls.constructors.length === 0) {
      this.em.line(`void ${mangle('init', [], name, nsPath)}(${name}* self) {`);
      this.em.level += 1;
      this.em.line('(void)self;');
      this.em.level -= 1;
      this.em.line('}');
      this.em.line();
    }

    for (const ctor of cls.constructors) {
      const sigTypes = ctor.params.map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
      const paramsText = this.formatParams(ctor.params, true);
      this.em.line(`void ${mangle('init', sigTypes, name, nsPath)}(${name}* self${paramsText ? `, ${paramsText}` : ''}) {`);
      this.em.level += 1;
      this.em.line('(void)self;');
      for (let i = 0; i < ctor.params.length; i += 1) {
        this.em.line(`(void)${ctor.params[i].name};`);
      }
      this.em.level -= 1;
      this.em.line('}');
      this.em.line();
    }

    this.em.line(`void ${mangle('destroy', [], name, nsPath)}(${name}* self) {`);
    this.em.level += 1;
    this.em.line('(void)self;');
    this.em.level -= 1;
    this.em.line('}');
    this.em.line();

    for (const method of cls.methods) {
      const sigTypes = method.params.map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
      const paramsText = this.formatParams(method.params, true);
      this.em.line(`${method.returnType} ${mangle(method.name, sigTypes, name, nsPath)}(${name}* self${paramsText ? `, ${paramsText}` : ''}) {`);
      this.em.level += 1;
      this.em.line('(void)self;');
      for (let i = 0; i < method.params.length; i += 1) {
        this.em.line(`(void)${method.params[i].name};`);
      }
      if (method.returnType !== 'void') {
        this.em.line(`return (${method.returnType})0;`);
      }
      this.em.level -= 1;
      this.em.line('}');
      this.em.line();
    }
  }
}

class WatEmitter {
  constructor() {
    this.lines = [];
    this.level = 0;
  }

  line(text = '') {
    if (!text) {
      this.lines.push('');
      return;
    }
    this.lines.push(`${'  '.repeat(this.level)}${text}`);
  }

  code() {
    return this.lines.join('\n');
  }
}

class CppToWatTranspiler {
  constructor(analysis, options = {}) {
    this.analysis = analysis;
    this.options = options;
    this.em = new WatEmitter();
  }

  transpile() {
    this.em.line('(module');
    this.em.level += 1;
    this.emitHeader();
    this.emitImports();
    this.emitMemory();
    this.emitClassMethodStubs();
    this.emitMainStub();
    this.em.level -= 1;
    this.em.line(')');
    return this.em.code();
  }

  emitHeader() {
    this.em.line(';; Generated from C++98 source (MaiaCpp experimental WAT backend)');
    this.em.line(`;; Source: ${this.options.filePath || '<unknown>'}`);
    this.em.line();
  }

  emitImports() {
    this.em.line(';; Runtime imports expected from browser/node host');
    this.em.line('(import "env" "printf" (func $printf (param i32 i32) (result i32)))');
    this.em.line('(import "env" "__malloc" (func $__malloc (param i32) (result i32)))');
    this.em.line('(import "env" "__free" (func $__free (param i32)))');
    this.em.line('(import "env" "__exc_push" (func $__exc_push))');
    this.em.line('(import "env" "__exc_pop" (func $__exc_pop))');
    this.em.line('(import "env" "__exc_throw" (func $__exc_throw (param i32 i32)))');
    this.em.line('(import "env" "__exc_active" (func $__exc_active (result i32)))');
    this.em.line('(import "env" "__exc_clear" (func $__exc_clear))');
    this.em.line();
  }

  emitMemory() {
    this.em.line('(memory (export "memory") 1)');
    this.em.line();
  }

  mapCTypeToWat(typeText) {
    const t = (typeText || '').replace(/\s+/g, ' ').trim();
    if (!t || t === 'void') return null;
    if (t === 'float') return 'f32';
    if (t === 'double') return 'f64';
    if (t === 'long') return 'i64';
    return 'i32';
  }

  emitClassMethodStubs() {
    if (!this.analysis || !this.analysis.classes || this.analysis.classes.size === 0) return;

    this.em.line(';; Class/method stubs extracted from semantic analysis');
    for (const [className, cls] of this.analysis.classes) {
      const nsPath = Array.isArray(cls.namespacePath) ? cls.namespacePath : [];
      this.em.line(`;; class ${className}`);
      for (const method of cls.methods || []) {
        const methodName = mangle(method.name, [], className, nsPath);
        const resultType = this.mapCTypeToWat(method.returnType);
        const params = ['(param $self i32)'];
        for (let i = 0; i < (method.params || []).length; i += 1) {
          const p = method.params[i];
          params.push(`(param $${p.name || `p${i + 1}`} ${this.mapCTypeToWat(p.type) || 'i32'})`);
        }

        this.em.line(`(func $${methodName} (export "${methodName}") ${params.join(' ')}${resultType ? ` (result ${resultType})` : ''}`);
        this.em.level += 1;
        if (resultType === 'f32') {
          this.em.line('f32.const 0');
        } else if (resultType === 'f64') {
          this.em.line('f64.const 0');
        } else if (resultType === 'i64') {
          this.em.line('i64.const 0');
        } else if (resultType === 'i32') {
          this.em.line('i32.const 0');
        } else {
          this.em.line('nop');
        }
        this.em.level -= 1;
        this.em.line(')');
      }
    }
    this.em.line();
  }

  emitMainStub() {
    const hasMain = /\b(?:int|void)\s+main\s*\(/.test(this.options.source || '');
    if (hasMain) {
      this.em.line('(func $main (export "main") (result i32)');
      this.em.level += 1;
      this.em.line('i32.const 0');
      this.em.level -= 1;
      this.em.line(')');
      this.em.line('(func (export "_start")');
      this.em.level += 1;
      this.em.line('call $main');
      this.em.line('drop');
      this.em.level -= 1;
      this.em.line(')');
    } else {
      this.em.line('(func (export "_start")');
      this.em.level += 1;
      this.em.line('nop');
      this.em.level -= 1;
      this.em.line(')');
    }
  }
}

class Cpp98Compiler {
  constructor(filePath, options = {}) {
    this.filePath = filePath;
    this.options = options;
  }

  compile() {
    const source = fs.readFileSync(this.filePath, 'utf8');
    const analysis = this.analyze(source);
    const transpiler = new CppToCTranspiler(analysis);
    return transpiler.transpile();
  }

  compileWat() {
    const source = fs.readFileSync(this.filePath, 'utf8');
    const analysis = this.analyze(source);
    const transpiler = new CppToWatTranspiler(analysis, { filePath: this.filePath, source });
    return transpiler.transpile();
  }

  analyze(source) {
    console.log(`Parsing: ${this.filePath}`);
    const parseSources = [];
    const normalized = normalizeForParser(source);

    if (normalized !== source) {
      parseSources.push({ text: normalized, label: 'Parser: ok (namespace normalized)' });
    }
    parseSources.push({ text: source, label: 'Parser: ok' });

    let lastErr = null;

    for (let i = 0; i < parseSources.length; i += 1) {
      const candidate = parseSources[i];
      try {
        const collector = new ParseTreeCollector();
        const parser = new Parser(candidate.text, collector);
        parser.parse();

        if (!collector.root) {
          throw new Error('Nenhuma árvore de parse disponível');
        }

        console.log(candidate.label);
        const sema = new SemanticAnalyzer(collector.root);
        const analysis = sema.analyze();
        this.applySourceClassHints(analysis, source);
        return analysis;
      } catch (err) {
        lastErr = err;
      }
    }

    console.log(`Parser falhou (${lastErr ? lastErr.message : 'erro desconhecido'}). Usando fallback simples.`);
    return new SimpleAnalyzer(this.filePath).analyze();
  }

  applySourceClassHints(analysis, source) {
    if (!analysis || !analysis.classes || analysis.classes.size === 0) return;

    const nsMap = inferClassNamespaceMap(source);
    const fallback = new SimpleAnalyzer(this.filePath).analyze();

    for (const [className, cls] of analysis.classes) {
      if (!cls) continue;

      const inferredNs = nsMap.get(className);
      if ((!Array.isArray(cls.namespacePath) || cls.namespacePath.length === 0) && inferredNs && inferredNs.length > 0) {
        cls.namespacePath = [...inferredNs];
      }

      const hinted = fallback.classes.get(className);
      if (!hinted) continue;

      if ((!cls.members || cls.members.length === 0) && hinted.members && hinted.members.length > 0) {
        cls.members = hinted.members.map((m) => ({ ...m }));
      }

      if ((!cls.methods || cls.methods.length === 0) && hinted.methods && hinted.methods.length > 0) {
        cls.methods = hinted.methods.map((m) => ({ ...m }));
      }

      if ((!cls.constructors || cls.constructors.length === 0) && hinted.constructors && hinted.constructors.length > 0) {
        cls.constructors = hinted.constructors.map((c) => ({ ...c, params: (c.params || []).map((p) => ({ ...p })) }));
      }

      if (!cls.destructor && hinted.destructor) {
        cls.destructor = { ...hinted.destructor };
      }

      if (!cls.hasVtable && hinted.hasVtable) {
        cls.hasVtable = true;
      }
    }
  }
}

if (require.main === module) {
  const args = process.argv.slice(2);
  if (!args.length) {
    console.log('Uso: node cpp-compiler.js <arquivo.cpp> [--output arquivo.c] [--wat-output arquivo.wat] [--wasm-output arquivo.wasm] [--verbose]');
    process.exit(1);
  }

  const input = path.resolve(args[0]);
  const outIdx = args.indexOf('--output');
  const watIdx = args.indexOf('--wat-output');
  const wasmIdx = args.indexOf('--wasm-output');
  const outFile = outIdx >= 0 && args[outIdx + 1] ? path.resolve(args[outIdx + 1]) : null;
  const watFile = watIdx >= 0 && args[watIdx + 1] ? path.resolve(args[watIdx + 1]) : null;
  const wasmFile = wasmIdx >= 0 && args[wasmIdx + 1] ? path.resolve(args[wasmIdx + 1]) : null;

  try {
    const compiler = new Cpp98Compiler(input, { verbose: args.includes('--verbose') });
    const code = compiler.compile();

    if (outFile) {
      fs.writeFileSync(outFile, code, 'utf8');
      console.log(`C gerado em: ${outFile}`);
    } else if (!watFile && !wasmFile) {
      console.log(code);
    }

    if (watFile || wasmFile) {
      const watCode = compiler.compileWat();
      const effectiveWatPath = watFile || path.join(fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-wat-')), `${path.basename(input, path.extname(input))}.wat`);
      fs.writeFileSync(effectiveWatPath, watCode, 'utf8');
      if (watFile) {
        console.log(`WAT gerado em: ${effectiveWatPath}`);
      }

      if (wasmFile) {
        execFileSync('wat2wasm', [effectiveWatPath, '-o', wasmFile], { stdio: 'inherit' });
        console.log(`WASM gerado em: ${wasmFile}`);
      }
    }
  } catch (err) {
    console.error(`Erro: ${err.message}`);
    process.exit(1);
  }
}

module.exports = {
  Type,
  PointerType,
  ReferenceType,
  ArrayType,
  ClassType,
  FunctionType,
  BUILTIN_TYPES,
  Symbol,
  SymbolKind,
  SymbolTable,
  typeCode,
  mangle,
  CEmitter,
  SemanticAnalyzer,
  SimpleAnalyzer,
  CppToCTranspiler,
  CppToWatTranspiler,
  Cpp98Compiler
};
