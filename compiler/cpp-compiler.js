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

function normalizeTypeText(type) {
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

function parseParamList(paramListText) {
  const text = (paramListText || '').trim();
  if (!text || text === 'void') return [];
  const raw = text.split(',').map((s) => s.trim()).filter(Boolean);
  const params = [];
  for (let i = 0; i < raw.length; i += 1) {
    const p = raw[i].replace(/\s+/g, ' ').trim();
    const match = p.match(/^(.*?)([A-Za-z_][A-Za-z0-9_]*)$/);
    if (!match) {
      params.push({ type: normalizeTypeText(p), name: `p${i + 1}` });
      continue;
    }
    params.push({
      type: normalizeTypeText(match[1].trim()),
      name: match[2].trim()
    });
  }
  return params;
}

function stripClassLikeBlocks(segment) {
  const text = String(segment || '');
  let out = '';
  for (let i = 0; i < text.length;) {
    const classMatch = text.slice(i).match(/^\s*(class|struct|union)\s+[A-Za-z_][A-Za-z0-9_]*[^\{]*\{/);
    if (!classMatch) {
      out += text[i];
      i += 1;
      continue;
    }
    const offset = classMatch.index || 0;
    out += text.slice(i, i + offset);
    const open = i + offset + classMatch[0].lastIndexOf('{');
    const close = findMatchingBrace(text, open);
    if (close < 0) {
      // If malformed, keep original tail.
      out += text.slice(i + offset);
      break;
    }
    out += '\n';
    i = close + 1;
  }
  return out;
}

function stripNamespaceBlocks(segment) {
  const text = String(segment || '');
  let out = '';
  for (let i = 0; i < text.length;) {
    const nsMatch = text.slice(i).match(/^\s*namespace\s+[A-Za-z_][A-Za-z0-9_]*\s*\{/);
    if (!nsMatch) {
      out += text[i];
      i += 1;
      continue;
    }
    const offset = nsMatch.index || 0;
    out += text.slice(i, i + offset);
    const open = i + offset + nsMatch[0].lastIndexOf('{');
    const close = findMatchingBrace(text, open);
    if (close < 0) {
      out += text.slice(i + offset);
      break;
    }
    out += '\n';
    i = close + 1;
  }
  return out;
}

function inferGlobalFunctions(source) {
  const functions = [];

  function stripUnaryPrefix(text) {
    let t = String(text || '').trim();
    while (/^[-+]/.test(t)) {
      t = t.slice(1).trim();
    }
    return t;
  }

  function splitSimpleArgs(argsText) {
    const out = [];
    let depthParen = 0;
    let depthAngle = 0;
    let cur = '';
    const text = String(argsText || '');
    for (let i = 0; i < text.length; i += 1) {
      const ch = text[i];
      if (ch === '(') depthParen += 1;
      else if (ch === ')' && depthParen > 0) depthParen -= 1;
      else if (ch === '<') depthAngle += 1;
      else if (ch === '>' && depthAngle > 0) depthAngle -= 1;

      if (ch === ',' && depthParen === 0 && depthAngle === 0) {
        if (cur.trim()) out.push(cur.trim());
        cur = '';
        continue;
      }
      cur += ch;
    }
    if (cur.trim()) out.push(cur.trim());
    return out;
  }

  function inferSimpleReturnExpr(body, params) {
    const clean = String(body || '')
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\/\/.*$/gm, '')
      .trim();

    const m = clean.match(/^return\s+([^;]+);\s*$/);
    if (!m) return null;
    const expr = m[1].trim();
    if (!expr) return null;

    // Keep lowering conservative: no function calls, no namespace qualifiers.
    if (/\b[A-Za-z_][A-Za-z0-9_]*\s*\(/.test(expr)) return null;
    if (/::/.test(expr)) return null;

    // Allow only identifiers from params and arithmetic/logic tokens.
    const ids = expr.match(/[A-Za-z_][A-Za-z0-9_]*/g) || [];
    const allowed = new Set((params || []).map((p) => p.name));
    for (const id of ids) {
      if (!allowed.has(id)) return null;
    }

    return expr;
  }

  function inferSimpleReturnCall(body, params) {
    const clean = String(body || '')
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\/\/.*$/gm, '')
      .trim();

    const m = clean.match(/^return\s+((?:::)?(?:[A-Za-z_][A-Za-z0-9_]*\s*::\s*)*[A-Za-z_][A-Za-z0-9_]*)\s*\((.*)\)\s*;\s*$/);
    if (!m) return null;

    const calleeText = (m[1] || '').replace(/\s+/g, '');
    const absolute = calleeText.startsWith('::');
    const calleeNormalized = absolute ? calleeText.slice(2) : calleeText;
    const calleeParts = calleeNormalized.split('::').filter(Boolean);
    if (calleeParts.length === 0) return null;
    const callee = calleeParts[calleeParts.length - 1];
    const calleeNamespacePath = calleeParts.slice(0, -1);
    const argsText = (m[2] || '').trim();
    const args = argsText ? splitSimpleArgs(argsText) : [];

    const allowed = new Set((params || []).map((p) => p.name));
    const castRx = /^\(\s*([A-Za-z_][A-Za-z0-9_:\s\*]*)\s*\)\s*(.+)$/;
    const staticCastRx = /^static_cast\s*<\s*([^>]+)\s*>\s*\((.+)\)$/;

    function isAllowedSimpleValue(valueText) {
      const raw = String(valueText || '').trim();
      if (!raw) return false;

      const unaryStripped = stripUnaryPrefix(raw);
      if (!unaryStripped) return false;

      if (/^[-+]?\d+(?:[uU]|[lL]|[uU][lL]|[lL][uU])?$/.test(unaryStripped)) return true;
      if (/^[-+]?(?:\d+\.\d*|\d*\.\d+)(?:[eE][-+]?\d+)?[fFlL]?$/.test(unaryStripped)) return true;

      const scMatch = unaryStripped.match(staticCastRx);
      if (scMatch) {
        const castType = normalizeTypeText(scMatch[1] || '');
        if (!castType) return false;
        return isAllowedSimpleValue(scMatch[2]);
      }

      const castMatch = unaryStripped.match(castRx);
      if (castMatch) {
        const castType = normalizeTypeText(castMatch[1] || '');
        if (!castType) return false;
        return isAllowedSimpleValue(castMatch[2]);
      }

      if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(unaryStripped)) {
        return allowed.has(unaryStripped);
      }

      return false;
    }

    for (const arg of args) {
      if (!isAllowedSimpleValue(arg)) return null;
    }

    return {
      callee,
      calleeNamespacePath,
      absolute,
      args
    };
  }

  function inferSimpleIfReturn(body, params) {
    const clean = String(body || '')
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\/\/.*$/gm, '')
      .trim();

    const withElse = clean.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*==\s*0\s*\)\s*\{\s*return\s+([-+]?\d+)\s*;\s*\}\s*else\s*\{\s*return\s+([-+]?\d+)\s*;\s*\}\s*$/);
    const noElse = clean.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*==\s*0\s*\)\s*\{\s*return\s+([-+]?\d+)\s*;\s*\}\s*return\s+([-+]?\d+)\s*;\s*$/);
    const m = withElse || noElse;
    if (!m) return null;

    const varName = m[1];
    const allowed = new Set((params || []).map((p) => p.name));
    if (!allowed.has(varName)) return null;

    return {
      kind: 'eq_zero',
      varName,
      thenValue: Number.parseInt(m[2], 10) | 0,
      elseValue: Number.parseInt(m[3], 10) | 0
    };
  }

  function scanSegment(segment, nsPath = []) {
    const nsRx = /\bnamespace\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/g;
    let m;
    while ((m = nsRx.exec(segment)) !== null) {
      const nsName = m[1];
      const braceIndex = nsRx.lastIndex - 1;
      const close = findMatchingBrace(segment, braceIndex);
      if (close < 0) continue;
      const body = segment.slice(braceIndex + 1, close);
      scanSegment(body, [...nsPath, nsName]);
      nsRx.lastIndex = close + 1;
    }

    const plain = stripNamespaceBlocks(stripClassLikeBlocks(segment));
    const fnRx = /(^|[;\n])\s*([A-Za-z_][A-Za-z0-9_:<>\s\*&]+?)\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^()]*)\)\s*(?:const\s*)?\{/g;
    let fm;
    while ((fm = fnRx.exec(plain)) !== null) {
      const returnType = normalizeTypeText(fm[2]);
      const name = (fm[3] || '').trim();
      if (!name) continue;
      if (['if', 'for', 'while', 'switch', 'catch'].includes(name)) continue;
      const params = parseParamList(fm[4]);

      const open = fm.index + fm[0].lastIndexOf('{');
      const close = findMatchingBrace(plain, open);
      const bodyText = close > open ? plain.slice(open + 1, close) : '';
      const simpleReturnExpr = inferSimpleReturnExpr(bodyText, params);
      const simpleReturnCall = inferSimpleReturnCall(bodyText, params);
      const simpleIfReturn = inferSimpleIfReturn(bodyText, params);

      functions.push({
        name,
        returnType,
        params,
        namespacePath: [...nsPath],
        simpleReturnExpr,
        simpleReturnCall,
        simpleIfReturn
      });

      if (close > open) {
        fnRx.lastIndex = close + 1;
      }
    }
  }

  scanSegment(String(source || ''), []);

  // De-duplicate by namespace + name + normalized parameter signature.
  const unique = new Map();
  for (const fn of functions) {
    const sig = (fn.params || []).map((p) => normalizeTypeText(p.type || '')).join(',');
    const key = `${(fn.namespacePath || []).join('::')}::${fn.name}(${sig})`;
    if (!unique.has(key)) unique.set(key, fn);
  }
  return Array.from(unique.values());
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
    const functions = inferGlobalFunctions(this.content);
    const symbols = new SymbolTable();
    for (const [className, cls] of classes) {
      symbols.define(className, new Symbol(SymbolKind.CLASS, className, cls));
    }
    for (const fn of functions) {
      symbols.define(fn.name, new Symbol(SymbolKind.FUNCTION, fn.name, new FunctionType(BUILTIN_TYPES.int, [])));
    }
    return {
      symbols,
      classes,
      functions
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
  constructor(analysis, options = {}) {
    this.analysis = analysis;
    this.options = options;
    this.em = new CEmitter();
    this.ambiguityEvents = [];
  }

  transpile() {
    this.emitHeaders();
    this.emitClasses();
    this.emitGlobalFunctionStubs();
    this.emitAmbiguitySummary();
    return this.em.code();
  }

  emitAmbiguitySummary() {
    if (!Array.isArray(this.ambiguityEvents) || this.ambiguityEvents.length === 0) return;

    const byCaller = new Map();
    for (const ev of this.ambiguityEvents) {
      const key = `${ev.caller || ''}|${ev.selected || ''}`;
      if (!byCaller.has(key)) byCaller.set(key, ev);
    }

    const sourceName = path.basename(this.options.filePath || 'input.cpp');
    const items = Array.from(byCaller.values()).sort((a, b) => {
      const ak = `${a.caller || ''}|${a.selected || ''}`;
      const bk = `${b.caller || ''}|${b.selected || ''}`;
      return ak.localeCompare(bk);
    });

    this.em.line(`/* Overload ambiguity summary (source: ${sourceName}): ${items.length} case(s) */`);
    for (const ev of items) {
      const count = Array.isArray(ev.candidates) ? ev.candidates.length : 0;
      this.em.line(`/* - caller ${ev.caller}: selected ${ev.selected} among ${count} candidate(s) */`);
    }
    this.em.line();
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

  emitGlobalFunctionStubs() {
    const fns = Array.isArray(this.analysis.functions) ? this.analysis.functions : [];
    if (fns.length === 0) return;

    this.em.line('/* Global function stubs */');
    for (const fn of fns) {
      const sigTypes = (fn.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
      const paramsText = this.formatParams(fn.params || [], true);
      const mangled = mangle(fn.name, sigTypes, null, fn.namespacePath || []);
      this.em.line(`${fn.returnType} ${mangled}(${paramsText || 'void'});`);
    }
    this.em.line();

    for (const fn of fns) {
      const sigTypes = (fn.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
      const paramsText = this.formatParams(fn.params || [], true);
      const mangled = mangle(fn.name, sigTypes, null, fn.namespacePath || []);
      this.em.line(`${fn.returnType} ${mangled}(${paramsText || 'void'}) {`);
      this.em.level += 1;
      if (fn.simpleReturnExpr && fn.returnType !== 'void') {
        this.em.line(`return ${fn.simpleReturnExpr};`);
      } else if (fn.simpleReturnCall && fn.returnType !== 'void') {
        const lowered = this.lowerSimpleReturnCall(fn, fns);
        if (lowered.diagnostic) {
          this.em.line(`/* ${lowered.diagnostic} */`);
        }
        if (lowered.ambiguity) {
          this.ambiguityEvents.push({
            caller: lowered.ambiguity.caller,
            selected: lowered.ambiguity.selected,
            candidates: lowered.ambiguity.candidates || []
          });
        }
        this.em.line(`return ${lowered.expr};`);
      } else {
        for (const p of fn.params || []) {
          this.em.line(`(void)${p.name};`);
        }
        if (fn.returnType !== 'void') {
          this.em.line(`return (${fn.returnType})0;`);
        }
      }
      this.em.level -= 1;
      this.em.line('}');
      this.em.line();
    }
  }

  lowerSimpleReturnCall(fn, allFns) {
    const call = fn.simpleReturnCall;
    if (!call || !call.callee) return { expr: '(int)0', diagnostic: null };
    const argTypes = (call.args || []).map((arg) => this.inferCallArgType(arg, fn));

    const qualifiedNs = Array.isArray(call.calleeNamespacePath) ? call.calleeNamespacePath : [];
    const resolution = this.resolveGlobalFunctionDetailed(
      call.callee,
      (call.args || []).length,
      fn.namespacePath || [],
      allFns,
      qualifiedNs,
      Boolean(call.absolute),
      argTypes
    );
    const match = resolution ? resolution.match : null;
    if (!match) {
      const fallbackCallee = qualifiedNs.length ? `${qualifiedNs.join('::')}::${call.callee}` : call.callee;
      return { expr: `${fallbackCallee}(${(call.args || []).join(', ')})`, diagnostic: null };
    }

    const sigTypes = (match.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
    const calleeMangled = mangle(match.name, sigTypes, null, match.namespacePath || []);
    const ambiguity = resolution && resolution.ambiguity
      ? {
        caller: this.functionStableKey(fn),
        selected: resolution.ambiguity.selected,
        candidates: resolution.ambiguity.candidates || []
      }
      : null;
    const diagnostic = ambiguity
      ? `Overload ambiguity resolved by stable key: ${ambiguity.selected}`
      : null;
    return { expr: `${calleeMangled}(${(call.args || []).join(', ')})`, diagnostic, ambiguity };
  }

  inferCallArgType(arg, currentFn) {
    const text = String(arg || '').trim();
    if (!text) return null;

    const unary = text.match(/^([+-]+)\s*(.+)$/);
    if (unary) {
      return this.inferCallArgType(unary[2], currentFn);
    }

    const staticCastMatch = text.match(/^static_cast\s*<\s*([^>]+)\s*>\s*\((.+)\)$/);
    if (staticCastMatch) {
      return normalizeTypeText(staticCastMatch[1] || '');
    }

    const castMatch = text.match(/^\(\s*([A-Za-z_][A-Za-z0-9_:\s\*]*)\s*\)\s*(.+)$/);
    if (castMatch) {
      return normalizeTypeText(castMatch[1] || '');
    }

    const intLiteral = text.match(/^[-+]?\d+([uU]|[lL]|[uU][lL]|[lL][uU])?$/);
    if (intLiteral) {
      const suffix = (intLiteral[1] || '').toLowerCase();
      if (suffix.includes('l')) return 'long';
      if (suffix.includes('u')) return 'unsigned';
      return 'int';
    }

    const floatLiteral = text.match(/^[-+]?(?:\d+\.\d*|\d*\.\d+)(?:[eE][-+]?\d+)?([fFlL]?)$/);
    if (floatLiteral) {
      const suffix = (floatLiteral[1] || '').toLowerCase();
      if (suffix === 'f') return 'float';
      return 'double';
    }

    if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(text)) {
      const params = Array.isArray(currentFn?.params) ? currentFn.params : [];
      const found = params.find((p) => p && p.name === text);
      if (found) return normalizeTypeText(found.type || '');
    }

    return null;
  }

  resolveByArgTypes(candidates, argTypes, options = {}) {
    const detailed = this.resolveByArgTypesDetailed(candidates, argTypes, options);
    return detailed.match;
  }

  resolveByArgTypesDetailed(candidates, argTypes, options = {}) {
    const list = Array.isArray(candidates) ? candidates : [];
    if (list.length === 0) return { match: null, ambiguity: null };
    const currentNamespacePath = Array.isArray(options.currentNamespacePath) ? options.currentNamespacePath : [];

    const types = Array.isArray(argTypes) ? argTypes : [];
    const hasTypeHints = types.some((t) => Boolean(t));
    if (!hasTypeHints) return { match: list[0], ambiguity: null };

    const exact = list.find((fn) => {
      const params = Array.isArray(fn.params) ? fn.params : [];
      if (params.length !== types.length) return false;
      for (let i = 0; i < params.length; i += 1) {
        const expected = normalizeTypeText(params[i]?.type || '');
        const actual = normalizeTypeText(types[i] || '');
        if (!actual) continue;
        if (expected !== actual) return false;
      }
      return true;
    });

    if (exact) return { match: exact, ambiguity: null };

    let best = null;
    let bestCost = Number.POSITIVE_INFINITY;
    let bestCostsByParam = null;
    const stableTieKeys = new Set();
    for (const fn of list) {
      const params = Array.isArray(fn.params) ? fn.params : [];
      if (params.length !== types.length) continue;

      let totalCost = 0;
      let valid = true;
      const costsByParam = [];
      for (let i = 0; i < params.length; i += 1) {
        const expected = normalizeTypeText(params[i]?.type || '');
        const actual = normalizeTypeText(types[i] || '');
        const cost = this.conversionCost(actual, expected);
        if (!Number.isFinite(cost)) {
          valid = false;
          break;
        }
        costsByParam.push(cost);
        totalCost += cost;
      }

      if (!valid) continue;

      if (totalCost < bestCost) {
        bestCost = totalCost;
        best = fn;
        bestCostsByParam = costsByParam;
        continue;
      }

      if (totalCost === bestCost) {
        const paramCmp = this.compareParamwiseCost(costsByParam, bestCostsByParam);
        if (paramCmp < 0) {
          best = fn;
          bestCostsByParam = costsByParam;
          continue;
        }
        if (paramCmp > 0) continue;

        const nsCmp = this.compareNamespaceRank(fn, best, currentNamespacePath);
        if (nsCmp > 0) {
          best = fn;
          bestCostsByParam = costsByParam;
          continue;
        }
        if (nsCmp < 0) continue;

        const candKey = this.functionStableKey(fn);
        const bestKey = this.functionStableKey(best);
        stableTieKeys.add(candKey);
        stableTieKeys.add(bestKey);
        if (candKey < bestKey) {
          best = fn;
          bestCostsByParam = costsByParam;
        }
      }
    }

    const match = best || list[0];
    const ambiguity = stableTieKeys.size > 1
      ? {
        reason: 'stable-key-fallback',
        selected: this.functionStableKey(match),
        candidates: Array.from(stableTieKeys).sort()
      }
      : null;
    return { match, ambiguity };
  }

  compareParamwiseCost(candidateCosts, currentBestCosts) {
    if (!Array.isArray(candidateCosts)) return 1;
    if (!Array.isArray(currentBestCosts)) return -1;

    const n = Math.min(candidateCosts.length, currentBestCosts.length);
    for (let i = 0; i < n; i += 1) {
      if (candidateCosts[i] < currentBestCosts[i]) return -1;
      if (candidateCosts[i] > currentBestCosts[i]) return 1;
    }

    if (candidateCosts.length < currentBestCosts.length) return -1;
    if (candidateCosts.length > currentBestCosts.length) return 1;
    return 0;
  }

  compareNamespaceRank(candidate, currentBest, currentNamespacePath) {
    if (!currentBest) return 1;

    const candNs = Array.isArray(candidate?.namespacePath) ? candidate.namespacePath : [];
    const bestNs = Array.isArray(currentBest?.namespacePath) ? currentBest.namespacePath : [];
    const currentNs = Array.isArray(currentNamespacePath) ? currentNamespacePath : [];

    const candPrefix = this.commonNamespacePrefixLen(candNs, currentNs);
    const bestPrefix = this.commonNamespacePrefixLen(bestNs, currentNs);
    if (candPrefix !== bestPrefix) return candPrefix > bestPrefix ? 1 : -1;

    if (candNs.length !== bestNs.length) return candNs.length > bestNs.length ? 1 : -1;
    return 0;
  }

  commonNamespacePrefixLen(a, b) {
    const left = Array.isArray(a) ? a : [];
    const right = Array.isArray(b) ? b : [];
    const n = Math.min(left.length, right.length);
    let i = 0;
    while (i < n && left[i] === right[i]) i += 1;
    return i;
  }

  functionStableKey(fn) {
    const ns = Array.isArray(fn?.namespacePath) ? fn.namespacePath.join('::') : '';
    const name = String(fn?.name || '');
    const params = (fn?.params || []).map((p) => normalizeTypeText(p?.type || '')).join(',');
    return `${ns}::${name}(${params})`;
  }

  conversionCost(actualType, expectedType) {
    const actual = normalizeTypeText(actualType || '');
    const expected = normalizeTypeText(expectedType || '');

    if (!actual || !expected) return 0;
    if (actual === expected) return 0;

    // Simple implicit conversions prioritized for current lowering scope.
    if (actual === 'int' && expected === 'long') return 1;
    if (actual === 'float' && expected === 'double') return 1;

    if (actual === 'int' && expected === 'double') return 3;
    if (actual === 'float' && expected === 'long') return 4;

    return Number.POSITIVE_INFINITY;
  }

  resolveGlobalFunction(name, arity, namespacePath, allFns, qualifiedNamespacePath = [], isAbsoluteQualified = false, argTypes = []) {
    return this.resolveGlobalFunctionDetailed(name, arity, namespacePath, allFns, qualifiedNamespacePath, isAbsoluteQualified, argTypes).match;
  }

  resolveGlobalFunctionDetailed(name, arity, namespacePath, allFns, qualifiedNamespacePath = [], isAbsoluteQualified = false, argTypes = []) {
    const list = Array.isArray(allFns) ? allFns : [];

    const selectDetailed = (predicate) => this.resolveByArgTypesDetailed(
      list.filter((f) => predicate(f) && f.name === name && (f.params || []).length === arity),
      argTypes,
      { currentNamespacePath: namespacePath || [] }
    );

    if (Array.isArray(qualifiedNamespacePath) && qualifiedNamespacePath.length > 0) {
      const qualified = selectDetailed((f) => this.sameNs(f.namespacePath || [], qualifiedNamespacePath));
      if (qualified.match) return qualified;

      if (!isAbsoluteQualified) {
        const current = Array.isArray(namespacePath) ? namespacePath : [];
        for (let cut = current.length; cut >= 0; cut -= 1) {
          const candidateNs = [...current.slice(0, cut), ...qualifiedNamespacePath];
          const relative = selectDetailed((f) => this.sameNs(f.namespacePath || [], candidateNs));
          if (relative.match) return relative;
        }
      }
    }
    const sameNs = selectDetailed((f) => this.sameNs(f.namespacePath || [], namespacePath || []));
    if (sameNs.match) return sameNs;

    const currentNs = Array.isArray(namespacePath) ? namespacePath : [];
    for (let cut = currentNs.length - 1; cut > 0; cut -= 1) {
      const ancestorNs = currentNs.slice(0, cut);
      const ancestor = selectDetailed((f) => this.sameNs(f.namespacePath || [], ancestorNs));
      if (ancestor.match) return ancestor;
    }

    const globalNs = selectDetailed((f) => (f.namespacePath || []).length === 0);
    if (globalNs.match) return globalNs;
    return selectDetailed(() => true);
  }

  sameNs(a, b) {
    if (a.length !== b.length) return false;
    for (let i = 0; i < a.length; i += 1) {
      if (a[i] !== b[i]) return false;
    }
    return true;
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
    this.dataOffset = 2048;
    this.dataStrings = new Map();
  }

  transpile() {
    this.em.line('(module');
    this.em.level += 1;
    this.emitHeader();
    this.emitImports();
    this.emitMemory();
    this.emitClassMethodStubs();
    this.emitGlobalFunctionStubs();
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

  emitGlobalFunctionStubs() {
    const fns = Array.isArray(this.analysis.functions) ? this.analysis.functions : [];
    if (fns.length === 0) return;

    this.em.line(';; Global function stubs extracted from analysis');
    for (const fn of fns) {
      if (fn.name === 'main' && (fn.namespacePath || []).length === 0) {
        continue;
      }
      const sigTypes = (fn.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
      const fnName = mangle(fn.name, sigTypes, null, fn.namespacePath || []);
      const resultType = this.mapCTypeToWat(fn.returnType);
      const params = [];
      for (let i = 0; i < (fn.params || []).length; i += 1) {
        const p = fn.params[i];
        params.push(`(param $${p.name || `p${i + 1}`} ${this.mapCTypeToWat(p.type) || 'i32'})`);
      }

      this.em.line(`(func $${fnName} (export "${fnName}") ${params.join(' ')}${resultType ? ` (result ${resultType})` : ''}`);
      this.em.level += 1;
      if (fn.simpleIfReturn && resultType) {
        if (!this.emitWatForSimpleIfReturn(fn, resultType)) {
          this.emitWatZero(resultType);
        }
      } else if (fn.simpleReturnExpr && resultType) {
        if (!this.emitWatForSimpleReturnExpr(fn, resultType)) {
          this.emitWatZero(resultType);
        }
      } else if (fn.simpleReturnCall && resultType) {
        if (!this.emitWatForSimpleReturnCall(fn, fns, resultType)) {
          this.emitWatZero(resultType);
        }
      } else if (resultType) {
        this.emitWatZero(resultType);
      } else {
        this.em.line('nop');
      }
      this.em.level -= 1;
      this.em.line(')');
    }
    this.em.line();
  }

  emitWatForSimpleIfReturn(fn, resultType) {
    const info = fn.simpleIfReturn;
    if (!info || info.kind !== 'eq_zero') return false;
    if (resultType !== 'i32' && resultType !== 'i64') return false;

    const params = Array.isArray(fn.params) ? fn.params : [];
    const param = params.find((p) => p.name === info.varName);
    if (!param) return false;

    const paramType = this.mapCTypeToWat(param.type) || 'i32';
    const eqzOp = paramType === 'i64' ? 'i64.eqz' : 'i32.eqz';
    const constOp = resultType === 'i64' ? 'i64.const' : 'i32.const';
    const resultSig = resultType === 'i64' ? ' (result i64)' : ' (result i32)';

    this.em.line(`local.get $${info.varName}`);
    this.em.line(eqzOp);
    this.em.line(`if${resultSig}`);
    this.em.level += 1;
    this.em.line(`${constOp} ${info.thenValue | 0}`);
    this.em.level -= 1;
    this.em.line('else');
    this.em.level += 1;
    this.em.line(`${constOp} ${info.elseValue | 0}`);
    this.em.level -= 1;
    this.em.line('end');
    return true;
  }

  typeKindFromText(typeText) {
    const t = (typeText || '').trim();
    if (t.endsWith('*')) return 'pointer';
    if (BUILTIN_TYPES[t]) return t;
    return 'class';
  }

  emitWatZero(resultType) {
    if (resultType === 'f32') this.em.line('f32.const 0');
    else if (resultType === 'f64') this.em.line('f64.const 0');
    else if (resultType === 'i64') this.em.line('i64.const 0');
    else this.em.line('i32.const 0');
  }

  emitWatForSimpleReturnExpr(fn, resultType) {
    if (resultType !== 'i32' && resultType !== 'i64') return false;
    const expr = String(fn.simpleReturnExpr || '').trim();
    if (!expr) return false;

    // Tokenize a conservative arithmetic subset.
    const tokens = [];
    const rx = /([A-Za-z_][A-Za-z0-9_]*|\d+|\+|\-|\*|\/|%|\(|\))/g;
    let m;
    while ((m = rx.exec(expr)) !== null) tokens.push(m[1]);
    if (tokens.join('') !== expr.replace(/\s+/g, '')) return false;

    const output = [];
    const ops = [];
    const precedence = { '+': 1, '-': 1, '*': 2, '/': 2, '%': 2 };
    for (const t of tokens) {
      if (/^\d+$/.test(t) || /^[A-Za-z_][A-Za-z0-9_]*$/.test(t)) {
        output.push(t);
      } else if (t in precedence) {
        while (ops.length > 0) {
          const top = ops[ops.length - 1];
          if (!(top in precedence) || precedence[top] < precedence[t]) break;
          output.push(ops.pop());
        }
        ops.push(t);
      } else if (t === '(') {
        ops.push(t);
      } else if (t === ')') {
        let found = false;
        while (ops.length > 0) {
          const top = ops.pop();
          if (top === '(') {
            found = true;
            break;
          }
          output.push(top);
        }
        if (!found) return false;
      } else {
        return false;
      }
    }
    while (ops.length > 0) {
      const top = ops.pop();
      if (top === '(') return false;
      output.push(top);
    }

    const paramNames = new Set((fn.params || []).map((p) => p.name));
    for (const t of output) {
      if (/^\d+$/.test(t)) {
        const constOp = resultType === 'i64' ? 'i64.const' : 'i32.const';
        this.em.line(`${constOp} ${t}`);
      } else if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(t)) {
        if (!paramNames.has(t)) return false;
        this.em.line(`local.get $${t}`);
      } else {
        const opPrefix = resultType === 'i64' ? 'i64' : 'i32';
        if (t === '+') this.em.line(`${opPrefix}.add`);
        else if (t === '-') this.em.line(`${opPrefix}.sub`);
        else if (t === '*') this.em.line(`${opPrefix}.mul`);
        else if (t === '/') this.em.line(`${opPrefix}.div_s`);
        else if (t === '%') this.em.line(`${opPrefix}.rem_s`);
        else return false;
      }
    }

    return true;
  }

  emitWatForSimpleReturnCall(fn, allFns, resultType) {
    const call = fn.simpleReturnCall;
    if (!call || !call.callee) return false;
    const args = Array.isArray(call.args) ? call.args : [];

    const target = this.resolveWatGlobalFunction(call.callee, args.length, fn.namespacePath || [], allFns, call.calleeNamespacePath || [], Boolean(call.absolute));
    if (!target) return false;

    const sigTypes = (target.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
    const callee = mangle(target.name, sigTypes, null, target.namespacePath || []);

    const params = Array.isArray(target.params) ? target.params : [];
    for (let i = 0; i < args.length; i += 1) {
      const arg = String(args[i] || '').trim();
      const expected = this.mapCTypeToWat(params[i]?.type || '') || 'i32';
      if (/^[-+]?\d+$/.test(arg)) {
        const instr = expected === 'i64' ? 'i64.const' : 'i32.const';
        this.em.line(`${instr} ${Number.parseInt(arg, 10) | 0}`);
      } else if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(arg)) {
        this.em.line(`local.get $${arg}`);
      } else {
        return false;
      }
    }

    this.em.line(`call $${callee}`);
    if ((this.mapCTypeToWat(target.returnType) || null) !== resultType) {
      // Keep conservative: mismatched return types not lowered yet.
      this.em.line('drop');
      this.emitWatZero(resultType);
    }
    return true;
  }

  resolveWatGlobalFunction(name, arity, namespacePath, allFns, qualifiedNamespacePath = [], isAbsoluteQualified = false) {
    const list = Array.isArray(allFns) ? allFns : [];
    const by = (ns) => list.find((f) => f.name === name && (f.params || []).length === arity && this.sameNs(f.namespacePath || [], ns || []));

    if (qualifiedNamespacePath.length > 0) {
      const qualified = by(qualifiedNamespacePath);
      if (qualified) return qualified;

      if (!isAbsoluteQualified) {
        const current = Array.isArray(namespacePath) ? namespacePath : [];
        for (let cut = current.length; cut >= 0; cut -= 1) {
          const candidateNs = [...current.slice(0, cut), ...qualifiedNamespacePath];
          const rel = by(candidateNs);
          if (rel) return rel;
        }
      }
    }

    const same = by(namespacePath || []);
    if (same) return same;
    const global = by([]);
    if (global) return global;
    return list.find((f) => f.name === name && (f.params || []).length === arity) || null;
  }

  sameNs(a, b) {
    const left = Array.isArray(a) ? a : [];
    const right = Array.isArray(b) ? b : [];
    if (left.length !== right.length) return false;
    for (let i = 0; i < left.length; i += 1) {
      if (left[i] !== right[i]) return false;
    }
    return true;
  }

  emitMainStub() {
    const hasMain = /\b(?:int|void)\s+main\s*\(/.test(this.options.source || '');
    if (hasMain) {
      const structured = this.extractMainStructuredPlan(this.options.source || '');
      if (structured) {
        this.emitStructuredMain(structured);
      } else {
        const printCalls = this.extractMainPrintfCalls(this.options.source || '');
        for (let i = 0; i < printCalls.length; i += 1) {
          const call = printCalls[i];
          this.em.line(`(data (i32.const ${call.offset}) "${call.watData}")`);
        }
        if (printCalls.length > 0) this.em.line();

        this.em.line('(func $main (export "main") (result i32)');
        this.em.level += 1;
        for (let i = 0; i < printCalls.length; i += 1) {
          const call = printCalls[i];
          this.em.line(`i32.const ${call.offset}`);
          this.em.line(`i32.const ${call.arg0}`);
          this.em.line('call $printf');
          this.em.line('drop');
        }
        this.em.line('i32.const 0');
        this.em.level -= 1;
        this.em.line(')');
      }

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

  emitStructuredMain(plan) {
    for (const item of plan.data || []) {
      this.em.line(`(data (i32.const ${item.offset}) "${item.watData}")`);
    }
    if ((plan.data || []).length > 0) this.em.line();

    this.em.line('(func $main (export "main") (result i32)');
    this.em.level += 1;

    for (const local of plan.locals || []) {
      this.em.line(`(local $${local.name} i32)`);
    }

    for (const local of plan.locals || []) {
      this.em.line(`i32.const ${local.init | 0}`);
      this.em.line(`local.set $${local.name}`);
    }

    this.emitMainOps(plan.ops || []);
    this.em.line('i32.const 0');

    this.em.level -= 1;
    this.em.line(')');
  }

  emitMainOps(ops) {
    for (const op of ops || []) {
      if (op.kind === 'printf') {
        this.em.line(`i32.const ${op.offset}`);
        if (op.arg == null) {
          this.em.line('i32.const 0');
        } else if (op.arg.type === 'int') {
          this.em.line(`i32.const ${op.arg.value | 0}`);
        } else if (op.arg.type === 'var') {
          this.em.line(`local.get $${op.arg.name}`);
        } else {
          this.em.line('i32.const 0');
        }
        this.em.line('call $printf');
        this.em.line('drop');
      } else if (op.kind === 'inc') {
        this.em.line(`local.get $${op.varName}`);
        this.em.line('i32.const 1');
        this.em.line('i32.add');
        this.em.line(`local.set $${op.varName}`);
      } else if (op.kind === 'if_call') {
        this.em.line(`call $${op.callee}`);
        this.em.line('if');
        this.em.level += 1;
        this.emitMainOps(op.thenOps || []);
        this.em.level -= 1;
        if (Array.isArray(op.elseOps) && op.elseOps.length > 0) {
          this.em.line('else');
          this.em.level += 1;
          this.emitMainOps(op.elseOps);
          this.em.level -= 1;
        }
        this.em.line('end');
      } else if (op.kind === 'if_eq_zero') {
        this.em.line(`local.get $${op.varName}`);
        this.em.line('i32.eqz');
        this.em.line('if');
        this.em.level += 1;
        this.emitMainOps(op.thenOps || []);
        this.em.level -= 1;
        if (Array.isArray(op.elseOps) && op.elseOps.length > 0) {
          this.em.line('else');
          this.em.level += 1;
          this.emitMainOps(op.elseOps);
          this.em.level -= 1;
        }
        this.em.line('end');
      } else if (op.kind === 'return') {
        this.em.line(`i32.const ${op.value | 0}`);
        this.em.line('return');
      }
    }
  }

  extractMainStructuredPlan(sourceText) {
    const source = String(sourceText || '');
    const body = this.extractMainBodyText(source);
    if (!body) return null;

    const locals = [];
    const ops = [];
    const data = [];
    let rest = this.stripComments(body).trim();

    const addData = (fmtRaw) => {
      const key = String(fmtRaw || '');
      if (this.dataStrings.has(key)) return this.dataStrings.get(key);
      const dataText = this.decodeCStringLiteral(key);
      const bytes = [...Buffer.from(dataText, 'utf8'), 0];
      const watData = this.bytesToWatString(bytes);
      const entry = { offset: this.dataOffset, watData };
      this.dataOffset += bytes.length + 8;
      this.dataStrings.set(key, entry);
      data.push(entry);
      return entry;
    };

    const parseArg = (text) => {
      const t = String(text || '').trim();
      if (!t) return null;
      if (/^[-+]?\d+$/.test(t)) return { type: 'int', value: Number.parseInt(t, 10) | 0 };
      if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(t)) return { type: 'var', name: t };
      return null;
    };

    const parsePrintf = (text) => {
      const m = text.match(/^printf\s*\(\s*"((?:\\.|[^"\\])*)"\s*(?:,\s*([^\)]+))?\)\s*;\s*/);
      if (!m) return null;
      const ds = addData(m[1] || '');
      const arg = parseArg(m[2] || '');
      return { consumed: m[0].length, op: { kind: 'printf', offset: ds.offset, arg } };
    };

    const parseInc = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\+\+\s*;\s*/);
      if (!m) return null;
      return { consumed: m[0].length, op: { kind: 'inc', varName: m[1] } };
    };

    const parseReturn = (text) => {
      const m = text.match(/^return\s+([-+]?\d+)\s*;\s*/);
      if (!m) return null;
      return { consumed: m[0].length, op: { kind: 'return', value: Number.parseInt(m[1], 10) | 0 } };
    };

    const parseLocal = (text) => {
      const m = text.match(/^int\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([-+]?\d+)\s*;\s*/);
      if (!m) return null;
      return {
        consumed: m[0].length,
        local: { name: m[1], init: Number.parseInt(m[2], 10) | 0 }
      };
    };

    const parseBlockOps = (blockText) => {
      const out = [];
      let t = String(blockText || '').trim();
      while (t.length > 0) {
        const p = parsePrintf(t) || parseInc(t) || parseReturn(t);
        if (!p) return null;
        out.push(p.op);
        t = t.slice(p.consumed).trim();
      }
      return out;
    };

    while (rest.length > 0) {
      const local = parseLocal(rest);
      if (local) {
        locals.push(local.local);
        rest = rest.slice(local.consumed).trim();
        continue;
      }

      const p = parsePrintf(rest) || parseInc(rest) || parseReturn(rest);
      if (p) {
        ops.push(p.op);
        rest = rest.slice(p.consumed).trim();
        continue;
      }

      const ifCall = rest.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*\)\s*\)\s*\{([\s\S]*?)\}\s*else\s*\{([\s\S]*?)\}\s*/);
      if (ifCall) {
        const calleeName = ifCall[1];
        const thenOps = parseBlockOps(ifCall[2]);
        const elseOps = parseBlockOps(ifCall[3]);
        if (!thenOps || !elseOps) return null;
        const resolved = this.resolveWatMainCallee(calleeName, 0);
        ops.push({ kind: 'if_call', callee: resolved, thenOps, elseOps });
        rest = rest.slice(ifCall[0].length).trim();
        continue;
      }

      const ifEqZero = rest.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*==\s*0\s*\)\s*\{([\s\S]*?)\}\s*/);
      if (ifEqZero) {
        const thenOps = parseBlockOps(ifEqZero[2]);
        if (!thenOps) return null;
        ops.push({ kind: 'if_eq_zero', varName: ifEqZero[1], thenOps, elseOps: [] });
        rest = rest.slice(ifEqZero[0].length).trim();
        continue;
      }

      return null;
    }

    return { locals, ops, data };
  }

  resolveWatMainCallee(name, arity) {
    const list = Array.isArray(this.analysis?.functions) ? this.analysis.functions : [];
    const fn = list.find((f) => f.name === name && (f.params || []).length === arity && (f.namespacePath || []).length === 0);
    if (!fn) return name;
    const sigTypes = (fn.params || []).map((p) => ({ kind: 'int', name: p.type }));
    return mangle(fn.name, sigTypes, null, fn.namespacePath || []);
  }

  extractMainBodyText(source) {
    const text = String(source || '');
    const m = text.match(/\b(?:int|void)\s+main\s*\([^)]*\)\s*\{/);
    if (!m) return null;
    const open = (m.index || 0) + m[0].lastIndexOf('{');
    const close = findMatchingBrace(text, open);
    if (close < 0) return null;
    return text.slice(open + 1, close);
  }

  stripComments(source) {
    return String(source || '')
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\/\/.*$/gm, '');
  }

  extractMainPrintfCalls(sourceText) {
    const source = String(sourceText || '');
    const mainMatch = source.match(/\b(?:int|void)\s+main\s*\([^)]*\)\s*\{/);
    if (!mainMatch) return [];

    const open = (mainMatch.index || 0) + mainMatch[0].lastIndexOf('{');
    const close = findMatchingBrace(source, open);
    if (close < 0) return [];

    const body = source.slice(open + 1, close);
    const calls = [];
    const rx = /printf\s*\(\s*"((?:\\.|[^"\\])*)"\s*(?:,\s*([^\)]+))?\)\s*;/g;
    let m;
    let offset = 2048;
    let scanPos = 0;
    while ((m = rx.exec(body)) !== null) {
      const between = body.slice(scanPos, m.index);
      if (/\breturn\b/.test(between)) {
        scanPos = m.index + m[0].length;
        continue;
      }

      const before = body.slice(Math.max(0, m.index - 48), m.index);
      if (/else\s*\{\s*$/m.test(before)) {
        scanPos = m.index + m[0].length;
        continue;
      }

      const fmtRaw = m[1] || '';
      const argText = (m[2] || '').trim();
      const arg0 = this.parseSimpleIntArg(argText);
      const data = this.decodeCStringLiteral(fmtRaw);
      const bytes = [...Buffer.from(data, 'utf8'), 0];
      const watData = this.bytesToWatString(bytes);

      calls.push({
        offset,
        arg0,
        watData
      });
      offset += bytes.length + 8;
      scanPos = m.index + m[0].length;
    }

    return calls;
  }

  parseSimpleIntArg(argText) {
    if (!argText) return 0;
    const t = String(argText).trim();
    if (/^[-+]?\d+$/.test(t)) return Number.parseInt(t, 10) | 0;
    return 0;
  }

  decodeCStringLiteral(raw) {
    const text = String(raw || '');
    let out = '';
    for (let i = 0; i < text.length; i += 1) {
      const ch = text[i];
      if (ch !== '\\') {
        out += ch;
        continue;
      }
      const n = text[i + 1] || '';
      if (n === 'n') {
        out += '\n';
        i += 1;
      } else if (n === 't') {
        out += '\t';
        i += 1;
      } else if (n === 'r') {
        out += '\r';
        i += 1;
      } else if (n === '0') {
        out += '\0';
        i += 1;
      } else if (n === '\\') {
        out += '\\';
        i += 1;
      } else if (n === '"') {
        out += '"';
        i += 1;
      } else {
        out += n;
        i += 1;
      }
    }
    return out;
  }

  bytesToWatString(bytes) {
    const data = Array.isArray(bytes) ? bytes : [];
    let out = '';
    for (let i = 0; i < data.length; i += 1) {
      const b = data[i] & 0xff;
      if (b === 34 || b === 92 || b < 32 || b > 126) {
        out += `\\${b.toString(16).padStart(2, '0')}`;
      } else {
        out += String.fromCharCode(b);
      }
    }
    return out;
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
    const transpiler = new CppToCTranspiler(analysis, { filePath: this.filePath });
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
    if (!analysis) return;

    const nsMap = inferClassNamespaceMap(source);
    const fallback = new SimpleAnalyzer(this.filePath).analyze();

    if ((!analysis.functions || analysis.functions.length === 0) && fallback.functions && fallback.functions.length > 0) {
      analysis.functions = fallback.functions.map((f) => ({
        ...f,
        params: (f.params || []).map((p) => ({ ...p })),
        namespacePath: [...(f.namespacePath || [])]
      }));
    }

    if (!analysis.classes || analysis.classes.size === 0) {
      return;
    }

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
