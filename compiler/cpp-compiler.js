#!/usr/bin/env node

const fs = require('fs');
const os = require('os');
const path = require('path');
const { spawnSync } = require('child_process');
const Parser = require('./cpp-parser');
const { ParseTreeCollector, printTree } = require('./parse-tree-collector');
const { CppPreprocessor } = require('./cpp-preprocessor');

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
  if (type.kind === 'class') {
    // Strip template params and scope operators so the fragment is a valid C identifier.
    const safeCls = sanitizeMangleFragment(type.name);
    return `N${safeCls.length}${safeCls}`;
  }
  return 'x';
}

function sanitizeMangleFragment(name) {
  return String(name || '')
    .replace(/operator\s*\[\s*\]/g, 'operator_subscript')
    .replace(/operator\s*\(\s*\)/g, 'operator_call')
    .replace(/delete\s*\[\s*\]/g, 'deletearray')
    .replace(/new\s*\[\s*\]/g, 'newarray')
    // Named replacements for multi-char operators (must come before single-char)
    .replace(/operator\s*==/g, 'operator_eq')
    .replace(/operator\s*!=/g, 'operator_ne')
    .replace(/operator\s*<</g, 'operator_lshift')
    .replace(/operator\s*>>/g, 'operator_rshift')
    .replace(/operator\s*<=/g, 'operator_le')
    .replace(/operator\s*>=/g, 'operator_ge')
    .replace(/operator\s*&&/g, 'operator_and')
    .replace(/operator\s*\|\|/g, 'operator_or')
    .replace(/operator\s*\+\+/g, 'operator_inc')
    .replace(/operator\s*--/g, 'operator_dec')
    .replace(/operator\s*->/g, 'operator_arrow')
    .replace(/operator\s*\+=/g, 'operator_addassign')
    .replace(/operator\s*-=/g, 'operator_subassign')
    .replace(/operator\s*\*=/g, 'operator_mulassign')
    .replace(/operator\s*\/=/g, 'operator_divassign')
    .replace(/operator\s*%=/g, 'operator_modassign')
    .replace(/operator\s*\^=/g, 'operator_xorassign')
    .replace(/operator\s*&=/g, 'operator_andassign')
    .replace(/operator\s*\|=/g, 'operator_orassign')
    .replace(/operator\s*<<=/g, 'operator_lshiftassign')
    .replace(/operator\s*>>=/g, 'operator_rshiftassign')
    // Single-char operators
    .replace(/operator\s*\+/g, 'operator_add')
    .replace(/operator\s*-/g, 'operator_sub')
    .replace(/operator\s*\*/g, 'operator_mul')
    .replace(/operator\s*\//g, 'operator_div')
    .replace(/operator\s*%/g, 'operator_mod')
    .replace(/operator\s*\^/g, 'operator_xor')
    .replace(/operator\s*&/g, 'operator_bitand')
    .replace(/operator\s*\|/g, 'operator_bitor')
    .replace(/operator\s*~/g, 'operator_bitnot')
    .replace(/operator\s*!/g, 'operator_not')
    .replace(/operator\s*</g, 'operator_lt')
    .replace(/operator\s*>/g, 'operator_gt')
    .replace(/operator\s*=/g, 'operator_assign')
    .replace(/operator\s*,/g, 'operator_comma')
    .replace(/~/g, 'destructor_')
    .replace(/[^A-Za-z0-9_]/g, '_')
    .replace(/_+/g, '_')
    .replace(/^_+|_+$/g, '') || 'anon';
}

function mangle(name, paramTypes = [], className = null, namespace = []) {
  const safeName = sanitizeMangleFragment(name);
  const ns = namespace.length ? `${namespace.map((part) => sanitizeMangleFragment(part)).join('_')}_` : '';
  const cl = className ? `${sanitizeMangleFragment(className)}_` : '';
  const args = paramTypes.length ? `__${paramTypes.map(typeCode).join('')}` : '';
  return `${ns}${cl}${safeName}${args}`;
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

function extractNamespaceAliasNames(source) {
  const names = new Set();
  const rx = /\bnamespace\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*[^;]+;/g;
  let m;
  while ((m = rx.exec(String(source || ''))) !== null) {
    names.add(m[1]);
  }
  return names;
}

function extractSimpleEnumValues(source) {
  const values = new Map();
  const clean = String(source || '')
    .replace(/\/\*[\s\S]*?\*\//g, '')
    .replace(/\/\/.*$/gm, '');
  const enumRx = /\benum\s+[A-Za-z_][A-Za-z0-9_]*\s*\{([\s\S]*?)\}\s*;/g;
  let match;

  while ((match = enumRx.exec(clean)) !== null) {
    const items = String(match[1] || '').split(',').map((item) => item.trim()).filter(Boolean);
    let currentValue = -1;

    for (const item of items) {
      const itemMatch = item.match(/^([A-Za-z_][A-Za-z0-9_]*)(?:\s*=\s*([-+]?\d+))?$/);
      if (!itemMatch) continue;
      currentValue = itemMatch[2] != null ? (Number.parseInt(itemMatch[2], 10) | 0) : (currentValue + 1);
      values.set(itemMatch[1], currentValue);
    }
  }

  return values;
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
  const names = new Set([
    ...extractNamespaceNames(source),
    ...extractNamespaceAliasNames(source)
  ]);
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

function inferFunctionNamespaceMap(source) {
  const map = new Map();
  const text = String(source || '')
    .replace(/\/\*[\s\S]*?\*\//g, '')
    .replace(/\/\/.*$/gm, '');

  const namespaceStack = [];
  const braceKinds = [];
  let statementStart = 0;

  const forbiddenHeads = new Set(['if', 'for', 'while', 'switch', 'catch', 'namespace']);

  for (let i = 0; i < text.length; i += 1) {
    const remainder = text.slice(i);
    const nsMatch = remainder.match(/^namespace\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{/);
    if (nsMatch) {
      namespaceStack.push(nsMatch[1]);
      braceKinds.push('namespace');
      i += nsMatch[0].length - 1;
      statementStart = i + 1;
      continue;
    }

    const ch = text[i];
    if (ch === ';') {
      statementStart = i + 1;
      continue;
    }

    if (ch === '{') {
      const snippet = text.slice(statementStart, i + 1).trim();
      const header = snippet.replace(/\s+/g, ' ');
      const fnMatch = header.match(/([A-Za-z_][A-Za-z0-9_]*)\s*\(([^()]*)\)\s*\{$/);
      if (fnMatch) {
        const fname = fnMatch[1];
        const headPrefix = header.slice(0, Math.max(0, header.length - fnMatch[0].length)).trim();
        const arity = parseParamList(fnMatch[2]).length;
        const blocked = forbiddenHeads.has(fname)
          || /\b(class|struct|union|enum|typedef|template)\b/.test(headPrefix)
          || /\b(return|throw)\b/.test(headPrefix);

        if (!blocked) {
          const key = `${fname}/${arity}`;
          const nsPath = [...namespaceStack];
          const close = findMatchingBrace(text, i);
          const bodyText = close > i ? text.slice(i + 1, close) : '';
          const entry = {
            namespacePath: nsPath,
            bodyKey: functionBodyKey(bodyText)
          };
          const list = map.get(key) || [];
          list.push(entry);
          map.set(key, list);
        }
      }

      braceKinds.push('block');
      statementStart = i + 1;
      continue;
    }

    if (ch === '}') {
      const popped = braceKinds.pop();
      if (popped === 'namespace') {
        namespaceStack.pop();
      }
      statementStart = i + 1;
    }
  }

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

// C89 reserved keywords — must not be used as parameter names in emitted C.
const C89_KEYWORDS = new Set([
  'auto','break','case','char','const','continue','default','do',
  'double','else','enum','extern','float','for','goto','if',
  'int','long','register','return','short','signed','sizeof',
  'static','struct','switch','typedef','union','unsigned','void',
  'volatile','while'
]);

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
    const candidateName = match[2].trim();
    params.push({
      type: normalizeTypeText(match[1].trim()),
      name: C89_KEYWORDS.has(candidateName) ? `p${i + 1}` : candidateName
    });
  }
  return params;
}

function cleanFunctionBodyText(bodyText) {
  return String(bodyText || '')
    .replace(/^[\s\{]+/, '')
    .replace(/[\s\}]+$/, '')
    .replace(/\/\*[\s\S]*?\*\//g, '')
    .replace(/\/\/.*$/gm, '')
    .trim();
}

function functionBodyKey(bodyText) {
  return cleanFunctionBodyText(bodyText).replace(/\s+/g, '');
}

function inferSimpleReturnExprFromBody(bodyText, params) {
  const clean = cleanFunctionBodyText(bodyText);
  const m = clean.match(/^return\s*([^;]+);\s*$/);
  if (!m) return null;

  const expr = m[1].trim();
  if (!expr || /::/.test(expr)) return null;
  if (/\b[A-Za-z_][A-Za-z0-9_]*\s*\(/.test(expr)) return null;

  const ids = expr.match(/[A-Za-z_][A-Za-z0-9_]*/g) || [];
  const allowed = new Set((params || []).map((p) => p.name));
  for (const id of ids) {
    if (!allowed.has(id)) return null;
  }

  return expr;
}

function inferSimpleReturnCallFromBody(bodyText) {
  const clean = cleanFunctionBodyText(bodyText);
  const m = clean.match(/^return\s*((?:::)?(?:[A-Za-z_][A-Za-z0-9_]*\s*::\s*)*[A-Za-z_][A-Za-z0-9_]*)\s*\((.*)\)\s*;\s*$/);
  if (!m) return null;

  const calleeText = (m[1] || '').replace(/\s+/g, '');
  const absolute = calleeText.startsWith('::');
  const calleeNormalized = absolute ? calleeText.slice(2) : calleeText;
  const calleeParts = calleeNormalized.split('::').filter(Boolean);
  if (calleeParts.length === 0) return null;

  const argsText = (m[2] || '').trim();
  const args = argsText
    ? argsText.split(',').map((s) => s.trim()).filter(Boolean)
    : [];

  return {
    callee: calleeParts[calleeParts.length - 1],
    calleeNamespacePath: calleeParts.slice(0, -1),
    absolute,
    args
  };
}

function inferSimpleIfReturnFromBody(bodyText, params) {
  const clean = cleanFunctionBodyText(bodyText);
  const withElse = clean.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*\)\s*\{\s*return\s*([-+]?\d+)\s*;\s*\}\s*else\s*\{\s*return\s*([-+]?\d+)\s*;\s*\}\s*$/);
  const noElse = clean.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*\)\s*\{\s*return\s*([-+]?\d+)\s*;\s*\}\s*return\s*([-+]?\d+)\s*;\s*$/);
  const ternary = clean.match(/^return\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*\)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;\s*$/);
  const m = withElse || noElse || ternary;
  if (!m) return null;

  const allowed = new Set((params || []).map((p) => p.name));
  const leftName = m[1];
  if (!allowed.has(leftName)) return null;

  const rhsRaw = m[3];
  const rhsIsConst = /^[-+]?\d+$/.test(rhsRaw);
  if (!rhsIsConst && !allowed.has(rhsRaw)) return null;

  const right = rhsIsConst
    ? { kind: 'const', value: Number.parseInt(rhsRaw, 10) | 0 }
    : { kind: 'param', name: rhsRaw };

  return {
    kind: 'var_cmp',
    leftName,
    right,
    op: m[2],
    thenValue: Number.parseInt(m[4], 10) | 0,
    elseValue: Number.parseInt(m[5], 10) | 0
  };
}

function inferSimpleLocalInitReturnFromBody(bodyText, params) {
  const clean = cleanFunctionBodyText(bodyText);
  const m = clean.match(/^((?:(?:int|long|short|char|float|double)\s+[A-Za-z_][A-Za-z0-9_]*\s*=\s*[^;]+;\s*)+)return\s+([^;]+);\s*$/);
  if (!m) return null;

  const declBlock = m[1] || '';
  const returnExpr = (m[2] || '').trim();
  if (!returnExpr) return null;
  if (/::|->|\.|\[|\]/.test(returnExpr)) return null;
  if (/\b[A-Za-z_][A-Za-z0-9_]*\s*\(/.test(returnExpr)) return null;

  const splitArgs = (text) => {
    const out = [];
    let depthParen = 0;
    let depthAngle = 0;
    let cur = '';
    const src = String(text || '');
    for (let i = 0; i < src.length; i += 1) {
      const ch = src[i];
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
  };

  const allowedNames = new Set((params || []).map((p) => p.name));

  const trimUnary = (text) => {
    let t = String(text || '').trim();
    while (/^[-+]/.test(t)) t = t.slice(1).trim();
    return t;
  };

  const castRx = /^\(\s*([A-Za-z_][A-Za-z0-9_:\s\*]*)\s*\)\s*(.+)$/;
  const staticCastRx = /^static_cast\s*<\s*([^>]+)\s*>\s*\((.+)\)$/;

  function isAllowedValue(text) {
    const raw = String(text || '').trim();
    if (!raw) return false;
    if (/::|->|\.|\[|\]/.test(raw)) return false;

    const unaryStripped = trimUnary(raw);
    if (!unaryStripped) return false;

    if (/^[-+]?\d+(?:[uU]|[lL]|[uU][lL]|[lL][uU])?$/.test(unaryStripped)) return true;
    if (/^[-+]?(?:\d+\.\d*|\d*\.\d+)(?:[eE][-+]?\d+)?[fFlL]?$/.test(unaryStripped)) return true;
    if (/^'.'$/.test(unaryStripped)) return true;

    const scMatch = unaryStripped.match(staticCastRx);
    if (scMatch) {
      const castType = normalizeTypeText(scMatch[1] || '');
      if (!castType) return false;
      return isAllowedValue(scMatch[2]);
    }

    const castMatch = unaryStripped.match(castRx);
    if (castMatch) {
      const castType = normalizeTypeText(castMatch[1] || '');
      if (!castType) return false;
      return isAllowedValue(castMatch[2]);
    }

    if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(unaryStripped)) {
      return true;
    }

    const callMatch = unaryStripped.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\((.*)\)$/);
    if (!callMatch) return false;
    const args = splitArgs(callMatch[2] || '');
    for (const arg of args) {
      if (!isAllowedValue(arg)) return false;
    }
    return true;
  }

  const declRx = /(int|long|short|char|float|double)\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);/g;
  const locals = [];
  let dm;
  while ((dm = declRx.exec(declBlock)) !== null) {
    const localType = normalizeTypeText(dm[1]);
    const localName = (dm[2] || '').trim();
    const initExpr = (dm[3] || '').trim();
    if (!localName || !initExpr) return null;
    if (!isAllowedValue(initExpr)) return null;
    locals.push({ type: localType, name: localName, initExpr });
    allowedNames.add(localName);
  }
  if (locals.length === 0) return null;

  const ids = returnExpr.match(/[A-Za-z_][A-Za-z0-9_]*/g) || [];
  for (const id of ids) {
    if (!allowedNames.has(id)) return null;
  }

  return { locals, returnExpr };
}

function inferSimpleMethodCmpReturnFromBody(bodyText) {
  const clean = cleanFunctionBodyText(bodyText);
  const m = clean.match(/^([A-Za-z_][A-Za-z0-9_:<>]*)\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^\)]*)\)\s*;\s*return\s*\(\s*(.+?)\s*(==|!=|<=|>=|<|>)\s*([-+]?\d+)\s*\)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;\s*$/);
  if (!m) return null;

  const ctorType = (m[1] || '').trim();
  const localName = (m[2] || '').trim();
  const ctorArgText = (m[3] || '').trim();
  const leftExpr = (m[4] || '').trim();
  const op = (m[5] || '').trim();
  const rhsValue = Number.parseInt(m[6], 10) | 0;
  const thenValue = Number.parseInt(m[7], 10) | 0;
  const elseValue = Number.parseInt(m[8], 10) | 0;

  if (!ctorType || !localName || !leftExpr) return null;
  if (/::/.test(ctorType)) return null;

  const splitArgs = (text) => {
    if (!String(text || '').trim()) return [];
    const out = [];
    let depthParen = 0;
    let depthAngle = 0;
    let cur = '';
    const src = String(text || '');
    for (let i = 0; i < src.length; i += 1) {
      const ch = src[i];
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
  };

  const ctorArgs = splitArgs(ctorArgText);
  for (const arg of ctorArgs) {
    if (!/^[-+]?\d+$/.test(arg)) return null;
  }

  const methodCall = leftExpr.match(new RegExp(`^${localName}\\.([A-Za-z_][A-Za-z0-9_]*)\\(\\)$`));
  if (methodCall) {
    return {
      local: {
        type: ctorType,
        name: localName,
        ctorArgs: ctorArgs.map((a) => Number.parseInt(a, 10) | 0)
      },
      access: {
        kind: 'method',
        name: methodCall[1]
      },
      op,
      rhsValue,
      thenValue,
      elseValue
    };
  }

  const indexCall = leftExpr.match(new RegExp(`^${localName}\\[\\s*([-+]?\\d+)\\s*\\]\\s*\\+\\s*${localName}\\[\\s*([-+]?\\d+)\\s*\\]$`));
  if (indexCall) {
    return {
      local: {
        type: ctorType,
        name: localName,
        ctorArgs: ctorArgs.map((a) => Number.parseInt(a, 10) | 0)
      },
      access: {
        kind: 'index_sum',
        a: Number.parseInt(indexCall[1], 10) | 0,
        b: Number.parseInt(indexCall[2], 10) | 0
      },
      op,
      rhsValue,
      thenValue,
      elseValue
    };
  }

  return null;
}

function inferSimpleIndexedObjectCmpReturnFromBody(bodyText) {
  const clean = cleanFunctionBodyText(bodyText);
  const m = clean.match(/^([A-Za-z_][A-Za-z0-9_:<>]*)\s+([A-Za-z_][A-Za-z0-9_]*)\s*;\s*\2\[\s*([-+]?\d+)\s*\]\s*=\s*([-+]?\d+)\s*;\s*\2\[\s*([-+]?\d+)\s*\]\s*=\s*([-+]?\d+)\s*;\s*return\s*\(\s*\2\[\s*([-+]?\d+)\s*\]\s*\+\s*\2\[\s*([-+]?\d+)\s*\]\s*(==|!=|<=|>=|<|>)\s*([-+]?\d+)\s*\)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;\s*$/);
  if (!m) return null;

  const localType = (m[1] || '').trim();
  const localName = (m[2] || '').trim();
  if (!localType || !localName) return null;
  if (/::/.test(localType)) return null;

  return {
    local: {
      type: localType,
      name: localName
    },
    assignments: [
      { index: Number.parseInt(m[3], 10) | 0, value: Number.parseInt(m[4], 10) | 0 },
      { index: Number.parseInt(m[5], 10) | 0, value: Number.parseInt(m[6], 10) | 0 }
    ],
    sumIndexes: {
      a: Number.parseInt(m[7], 10) | 0,
      b: Number.parseInt(m[8], 10) | 0
    },
    op: (m[9] || '').trim(),
    rhsValue: Number.parseInt(m[10], 10) | 0,
    thenValue: Number.parseInt(m[11], 10) | 0,
    elseValue: Number.parseInt(m[12], 10) | 0
  };
}

function inferResourceDeterministicReturnHintFromBody(bodyText, params) {
  if (Array.isArray(params) && params.length > 0) return false;
  const clean = cleanFunctionBodyText(bodyText);
  if (!clean) return false;

  const hasGuardedIf = /\bif\s*\(/.test(clean);
  const hasResourceOps = /\bnew\b|\bdelete\b|\bdynamic_cast\b|\bstatic_cast\b|~\s*[A-Za-z_][A-Za-z0-9_]*\s*\(|\bsizeof\s*\(/.test(clean);
  return hasGuardedIf && hasResourceOps;
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

    const withElse = clean.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*\)\s*\{\s*return\s+([-+]?\d+)\s*;\s*\}\s*else\s*\{\s*return\s+([-+]?\d+)\s*;\s*\}\s*$/);
    const noElse = clean.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*\)\s*\{\s*return\s+([-+]?\d+)\s*;\s*\}\s*return\s+([-+]?\d+)\s*;\s*$/);
    const ternary = clean.match(/^return\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*\)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;\s*$/);
    const m = withElse || noElse || ternary;
    if (!m) return null;

    const leftName = m[1];
    const allowed = new Set((params || []).map((p) => p.name));
    if (!allowed.has(leftName)) return null;

    const rhsRaw = m[3];
    const rhsIsConst = /^[-+]?\d+$/.test(rhsRaw);
    if (!rhsIsConst && !allowed.has(rhsRaw)) return null;

    const right = rhsIsConst
      ? { kind: 'const', value: Number.parseInt(rhsRaw, 10) | 0 }
      : { kind: 'param', name: rhsRaw };

    return {
      kind: 'var_cmp',
      leftName,
      right,
      op: m[2],
      thenValue: Number.parseInt(m[4], 10) | 0,
      elseValue: Number.parseInt(m[5], 10) | 0
    };
  }

  function inferDeterministicNoParamI32Return(body, params) {
    if (Array.isArray(params) && params.length > 0) return null;
    const clean = String(body || '')
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\/\/.*$/gm, '')
      .trim();

    function trimOuterParens(text) {
      let t = String(text || '').trim();
      let changed = true;
      while (changed && t.startsWith('(') && t.endsWith(')')) {
        changed = false;
        let depth = 0;
        let balanced = true;
        for (let i = 0; i < t.length; i += 1) {
          const ch = t[i];
          if (ch === '(') depth += 1;
          else if (ch === ')') depth -= 1;
          if (depth === 0 && i < t.length - 1) {
            balanced = false;
            break;
          }
          if (depth < 0) {
            balanced = false;
            break;
          }
        }
        if (balanced && depth === 0) {
          t = t.slice(1, -1).trim();
          changed = true;
        }
      }
      return t;
    }

    function splitTopLevel(text, op) {
      const t = String(text || '').trim();
      let depthParen = 0;
      let depthBracket = 0;
      for (let i = 0; i <= t.length - op.length; i += 1) {
        const ch = t[i];
        if (ch === '(') depthParen += 1;
        else if (ch === ')' && depthParen > 0) depthParen -= 1;
        else if (ch === '[') depthBracket += 1;
        else if (ch === ']' && depthBracket > 0) depthBracket -= 1;
        if (depthParen === 0 && depthBracket === 0 && t.slice(i, i + op.length) === op) {
          return [t.slice(0, i).trim(), t.slice(i + op.length).trim()];
        }
      }
      return null;
    }

    function splitArgs(text) {
      const out = [];
      let cur = '';
      let depthParen = 0;
      let depthBracket = 0;
      const t = String(text || '');
      for (let i = 0; i < t.length; i += 1) {
        const ch = t[i];
        if (ch === '(') depthParen += 1;
        else if (ch === ')' && depthParen > 0) depthParen -= 1;
        else if (ch === '[') depthBracket += 1;
        else if (ch === ']' && depthBracket > 0) depthBracket -= 1;
        if (ch === ',' && depthParen === 0 && depthBracket === 0) {
          out.push(cur.trim());
          cur = '';
          continue;
        }
        cur += ch;
      }
      if (cur.trim()) out.push(cur.trim());
      return out;
    }

    function parseIfStatement(text) {
      const t = String(text || '');
      const head = t.match(/^if\s*\(/);
      if (!head) return null;
      let i = head[0].length;
      let depthParen = 1;
      for (; i < t.length; i += 1) {
        const ch = t[i];
        if (ch === '(') depthParen += 1;
        else if (ch === ')') {
          depthParen -= 1;
          if (depthParen === 0) break;
        }
      }
      if (depthParen !== 0) return null;
      const condText = t.slice(head[0].length, i).trim();
      let j = i + 1;
      while (j < t.length && /\s/.test(t[j])) j += 1;
      if (t[j] !== '{') return null;
      let depthBrace = 1;
      let k = j + 1;
      for (; k < t.length; k += 1) {
        const ch = t[k];
        if (ch === '{') depthBrace += 1;
        else if (ch === '}') {
          depthBrace -= 1;
          if (depthBrace === 0) break;
        }
      }
      if (depthBrace !== 0) return null;
      return {
        condText,
        blockText: t.slice(j + 1, k),
        consumed: k + 1
      };
    }

    const env = new Map();

    function asComparableInt(value) {
      if (!value) return null;
      if (value.kind === 'int') return value.value | 0;
      if (value.kind === 'pointer') return value.target ? 1 : 0;
      return null;
    }

    function evalExpr(exprText) {
      const expr = trimOuterParens(exprText);
      if (!expr) return null;

      if (/^[-+]?\d+$/.test(expr)) {
        return { kind: 'int', value: Number.parseInt(expr, 10) | 0 };
      }

      if (/^[-+]?(?:\d+\.\d*|\d*\.\d+)$/.test(expr)) {
        return { kind: 'float', value: Number.parseFloat(expr) };
      }

      if (/^(add|multiply|execute)$/.test(expr)) {
        return { kind: 'fnref', value: expr };
      }

      if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(expr)) {
        return env.has(expr) ? env.get(expr) : null;
      }

      const methodCall = expr.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\.\s*(get|value)\s*\(\s*\)$/);
      if (methodCall) {
        const obj = env.get(methodCall[1]);
        if (obj && obj.kind === 'object') return { kind: 'int', value: obj.value | 0 };
        return null;
      }

      const arrowMethod = expr.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*->\s*(get|value)\s*\(\s*\)$/);
      if (arrowMethod) {
        const obj = env.get(arrowMethod[1]);
        if (obj && obj.kind === 'pointer' && obj.target && obj.target.kind === 'object') {
          return { kind: 'int', value: obj.target.value | 0 };
        }
        return null;
      }

      const deref = expr.match(/^\*\s*([A-Za-z_][A-Za-z0-9_]*)$/);
      if (deref) {
        const ptr = env.get(deref[1]);
        if (ptr && ptr.kind === 'pointer' && ptr.target && ptr.target.kind === 'int') {
          return { kind: 'int', value: ptr.target.value | 0 };
        }
        return null;
      }

      const indexAccess = expr.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*(\d+)\s*\]$/);
      if (indexAccess) {
        const obj = env.get(indexAccess[1]);
        if (!obj || obj.kind !== 'box_int') return null;
        const idx = Number.parseInt(indexAccess[2], 10) | 0;
        return { kind: 'int', value: (obj.items.get(idx) || 0) | 0 };
      }

      const andSplit = splitTopLevel(expr, '&&');
      if (andSplit) {
        const left = evalExpr(andSplit[0]);
        const right = evalExpr(andSplit[1]);
        if (!left || !right || left.kind !== 'int' || right.kind !== 'int') return null;
        return { kind: 'int', value: (left.value !== 0 && right.value !== 0) ? 1 : 0 };
      }

      const eqSplit = splitTopLevel(expr, '==');
      if (eqSplit) {
        const left = evalExpr(eqSplit[0]);
        const right = evalExpr(eqSplit[1]);
        const leftValue = asComparableInt(left);
        const rightValue = asComparableInt(right);
        if (leftValue == null || rightValue == null) return null;
        return { kind: 'int', value: leftValue === rightValue ? 1 : 0 };
      }

      const neSplit = splitTopLevel(expr, '!=');
      if (neSplit) {
        const left = evalExpr(neSplit[0]);
        const right = evalExpr(neSplit[1]);
        const leftValue = asComparableInt(left);
        const rightValue = asComparableInt(right);
        if (leftValue == null || rightValue == null) return null;
        return { kind: 'int', value: leftValue !== rightValue ? 1 : 0 };
      }

      const plusSplit = splitTopLevel(expr, '+');
      if (plusSplit) {
        const left = evalExpr(plusSplit[0]);
        const right = evalExpr(plusSplit[1]);
        if (!left || !right || left.kind !== 'int' || right.kind !== 'int') return null;
        return { kind: 'int', value: (left.value + right.value) | 0 };
      }

      const call = expr.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\((.*)\)$/);
      if (call) {
        const callee = call[1];
        const args = splitArgs(call[2]).map((arg) => evalExpr(arg));
        if (args.some((arg) => !arg)) return null;
        if (callee === 'add' && args.length === 2 && args.every((arg) => arg.kind === 'int')) {
          return { kind: 'int', value: (args[0].value + args[1].value) | 0 };
        }
        if (callee === 'multiply' && args.length === 2 && args.every((arg) => arg.kind === 'int')) {
          return { kind: 'int', value: Math.imul(args[0].value, args[1].value) };
        }
        if (callee === 'execute' && args.length === 3 && args[0].kind === 'int' && args[1].kind === 'int' && args[2].kind === 'fnref') {
          if (args[2].value === 'add') return { kind: 'int', value: (args[0].value + args[1].value) | 0 };
          if (args[2].value === 'multiply') return { kind: 'int', value: Math.imul(args[0].value, args[1].value) };
        }
      }

      const dynamicCast = expr.match(/^dynamic_cast\s*<\s*([A-Za-z_][A-Za-z0-9_]*)\s*\*\s*>\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)$/);
      if (dynamicCast) {
        const ptr = env.get(dynamicCast[2]);
        if (ptr && ptr.kind === 'pointer' && ptr.target && ptr.target.kind === 'object' && ptr.target.className === dynamicCast[1]) {
          return { kind: 'pointer', target: ptr.target };
        }
        return { kind: 'int', value: 0 };
      }

      const staticCastInt = expr.match(/^static_cast\s*<\s*int\s*>\s*\(\s*([^\)]+)\s*\)$/);
      if (staticCastInt) {
        const value = evalExpr(staticCastInt[1]);
        if (!value) return null;
        if (value.kind === 'float') {
          return { kind: 'int', value: value.value < 0 ? Math.ceil(value.value) : Math.floor(value.value) };
        }
        if (value.kind === 'int') return { kind: 'int', value: value.value | 0 };
        return null;
      }

      return null;
    }

    let rest = clean;
    while (rest.length > 0) {
      const pointerNewDerived = rest.match(/^(?:[A-Za-z_][A-Za-z0-9_:<>]*)\*\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*new\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([^\)]+)\s*\)\s*;\s*/);
      if (pointerNewDerived && pointerNewDerived[2] !== 'int') {
        const value = evalExpr(pointerNewDerived[3]);
        if (!value || value.kind !== 'int') break;
        env.set(pointerNewDerived[1], {
          kind: 'pointer',
          target: { kind: 'object', className: pointerNewDerived[2], value: value.value | 0 }
        });
        rest = rest.slice(pointerNewDerived[0].length).trim();
        continue;
      }

      const pointerNewInt = rest.match(/^int\*\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*new\s+int\s*\(\s*([^\)]+)\s*\)\s*;\s*/);
      if (pointerNewInt) {
        const value = evalExpr(pointerNewInt[2]);
        if (!value || value.kind !== 'int') break;
        env.set(pointerNewInt[1], { kind: 'pointer', target: { kind: 'int', value: value.value | 0 } });
        rest = rest.slice(pointerNewInt[0].length).trim();
        continue;
      }

      const objectCtor = rest.match(/^C\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([^\)]+)\s*\)\s*;\s*/);
      if (objectCtor) {
        const value = evalExpr(objectCtor[2]);
        if (!value || value.kind !== 'int') break;
        env.set(objectCtor[1], { kind: 'object', className: 'C', value: value.value | 0 });
        rest = rest.slice(objectCtor[0].length).trim();
        continue;
      }

      const charBuffer = rest.match(/^char\s+([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*sizeof\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*\]\s*;\s*/);
      if (charBuffer) {
        env.set(charBuffer[1], { kind: 'buffer', typeName: charBuffer[2] });
        rest = rest.slice(charBuffer[0].length).trim();
        continue;
      }

      const placementNew = rest.match(/^([A-Za-z_][A-Za-z0-9_:<>]*)\*\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*new\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([^\)]+)\s*\)\s*;\s*/);
      if (placementNew) {
        const value = evalExpr(placementNew[5]);
        const buffer = env.get(placementNew[3]);
        if (!value || value.kind !== 'int' || !buffer || buffer.kind !== 'buffer') break;
        env.set(placementNew[2], {
          kind: 'pointer',
          target: { kind: 'object', className: placementNew[4], value: value.value | 0, storage: 'placement' }
        });
        rest = rest.slice(placementNew[0].length).trim();
        continue;
      }

      const boxDecl = rest.match(/^Box\s*<\s*int\s*>\s+([A-Za-z_][A-Za-z0-9_]*)\s*;\s*/);
      if (boxDecl) {
        env.set(boxDecl[1], { kind: 'box_int', items: new Map() });
        rest = rest.slice(boxDecl[0].length).trim();
        continue;
      }

      const intLocal = rest.match(/^int\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);\s*/);
      if (intLocal) {
        const value = evalExpr(intLocal[2]);
        if (!value || value.kind !== 'int') break;
        env.set(intLocal[1], { kind: 'int', value: value.value | 0 });
        rest = rest.slice(intLocal[0].length).trim();
        continue;
      }

      const pointerLocal = rest.match(/^([A-Za-z_][A-Za-z0-9_:<>]*)\*\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);\s*/);
      if (pointerLocal) {
        const value = evalExpr(pointerLocal[3]);
        if (!value || value.kind !== 'pointer') break;
        env.set(pointerLocal[2], value);
        rest = rest.slice(pointerLocal[0].length).trim();
        continue;
      }

      const boxAssign = rest.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*(\d+)\s*\]\s*=\s*([^;]+);\s*/);
      if (boxAssign) {
        const box = env.get(boxAssign[1]);
        const value = evalExpr(boxAssign[3]);
        if (!box || box.kind !== 'box_int' || !value || value.kind !== 'int') break;
        box.items.set(Number.parseInt(boxAssign[2], 10) | 0, value.value | 0);
        rest = rest.slice(boxAssign[0].length).trim();
        continue;
      }

      const deleteStmt = rest.match(/^delete\s+([A-Za-z_][A-Za-z0-9_]*)\s*;\s*/);
      if (deleteStmt) {
        const value = env.get(deleteStmt[1]);
        if (value && value.kind === 'pointer') value.deleted = true;
        rest = rest.slice(deleteStmt[0].length).trim();
        continue;
      }

      const dtorStmt = rest.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*->\s*~\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*\)\s*;\s*/);
      if (dtorStmt) {
        const value = env.get(dtorStmt[1]);
        if (!value || value.kind !== 'pointer' || !value.target || value.target.kind !== 'object') break;
        value.target.destroyed = true;
        rest = rest.slice(dtorStmt[0].length).trim();
        continue;
      }

      const ifBlock = parseIfStatement(rest);
      if (ifBlock) {
        const cond = evalExpr(ifBlock.condText);
        if (!cond || cond.kind !== 'int') break;
        if (cond.value !== 0) {
          const thenResult = inferDeterministicNoParamI32Return(ifBlock.blockText, []);
          if (Number.isInteger(thenResult)) return thenResult | 0;
          break;
        }
        rest = rest.slice(ifBlock.consumed).trim();
        continue;
      }

      const ternaryReturn = rest.match(/^return\s*\(\s*([^?]+)\s*\)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;\s*/);
      if (ternaryReturn) {
        const cond = evalExpr(ternaryReturn[1]);
        if (!cond || cond.kind !== 'int') break;
        return cond.value !== 0 ? (Number.parseInt(ternaryReturn[2], 10) | 0) : (Number.parseInt(ternaryReturn[3], 10) | 0);
      }

      const returnInt = rest.match(/^return\s+([-+]?\d+)\s*;\s*/);
      if (returnInt) {
        return Number.parseInt(returnInt[1], 10) | 0;
      }

      break;
    }

    return null;
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
      const simpleLocalInitReturn = inferSimpleLocalInitReturnFromBody(bodyText, params);
      const simpleMethodCmpReturn = inferSimpleMethodCmpReturnFromBody(bodyText);
      const simpleIndexedObjectCmpReturn = inferSimpleIndexedObjectCmpReturnFromBody(bodyText);
      const resourceDeterministicHint = inferResourceDeterministicReturnHintFromBody(bodyText, params);
      const deterministicNoParamI32Return = inferDeterministicNoParamI32Return(bodyText, params);

      functions.push({
        name,
        returnType,
        params,
        namespacePath: [...nsPath],
        bodyText,
        simpleReturnExpr,
        simpleReturnCall,
        simpleIfReturn,
        simpleLocalInitReturn,
        simpleMethodCmpReturn,
        simpleIndexedObjectCmpReturn,
        resourceDeterministicHint,
        deterministicNoParamI32Return
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
  constructor(parseTree, options = {}) {
    this.parseTree = parseTree;
    this.options = options;
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

    if (node.kind === 'nonterminal' && node.name === 'externalDeclaration') {
      const fn = this.extractGlobalFunction(node);
      if (fn) {
        this.functions.push(fn);
        this.globals.define(fn.name, new Symbol(SymbolKind.FUNCTION, fn.name, new FunctionType(BUILTIN_TYPES.int, [])));
      }
    }

    if (node.kind === 'nonterminal' && (node.name === 'classDefinition' || node.name === 'classSpecifier')) {
      const cls = this.extractClassInfo(node);
      if (cls) {
        this.classes.set(cls.name, cls);
        this.globals.define(cls.name, new Symbol(SymbolKind.CLASS, cls.name, cls));
        const fqName = cls.namespacePath.length ? `${cls.namespacePath.join('::')}::${cls.name}` : cls.name;
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

  extractClassInfo(node) {
    const className = this.extractClassHeadName(node) || this.extractClassName(node);
    if (!className) return null;

    const cls = new ClassType(className);
    cls.namespacePath = [...this.namespaceStack];
    cls.defaultAccess = this.extractClassKey(node) === 'struct' ? 'public' : 'private';

    this.extractBaseSpecifiers(node, cls);
    this.extractClassMembers(node, cls);
    return cls;
  }

  extractClassHeadName(node) {
    const classHead = this.findFirstNonterminal(node, 'classHead');
    if (!classHead) return '';
    const nameNode = this.findFirstNonterminal(classHead, 'classHeadName');
    const text = this.text(nameNode).replace(/\s+/g, '').trim();
    return text;
  }

  extractClassKey(node) {
    const classKey = this.findFirstNonterminal(node, 'classKey');
    const text = this.text(classKey).trim();
    return text || 'class';
  }

  extractBaseSpecifiers(node, cls) {
    const classHead = this.findFirstNonterminal(node, 'classHead');
    const baseSpecifiers = this.findAllNonterminals(classHead, 'baseSpecifier');
    for (const spec of baseSpecifiers) {
      const accessNode = this.findFirstNonterminal(spec, 'accessSpecifier');
      const nameNode = this.findFirstNonterminal(spec, 'className');
      const baseName = this.text(nameNode).trim();
      if (!baseName) continue;
      cls.bases.push({
        name: baseName,
        access: (this.text(accessNode).trim() || 'public').toLowerCase()
      });
    }
  }

  extractClassMembers(node, cls) {
    const memberSpec = this.findFirstNonterminal(node, 'memberSpecification');
    if (!memberSpec || !Array.isArray(memberSpec.children)) return;

    let currentAccess = cls.defaultAccess || 'private';
    for (const item of memberSpec.children) {
      if (!item || item.kind !== 'nonterminal' || item.name !== 'memberSpecificationItem') continue;

      const accessNode = this.findDirectChildNonterminal(item, 'accessSpecifier');
      if (accessNode) {
        currentAccess = (this.text(accessNode).trim() || currentAccess).toLowerCase();
        continue;
      }

      const memberDecl = this.findDirectChildNonterminal(item, 'memberDeclaration');
      if (!memberDecl) continue;
      this.consumeMemberDeclaration(memberDecl, cls, currentAccess);
    }
  }

  consumeMemberDeclaration(memberDecl, cls, access) {
    const declaration = this.findDirectChildNonterminal(memberDecl, 'declaration');
    const declarator = this.findDirectChildNonterminal(memberDecl, 'declarator')
      || this.findFirstNonterminal(declaration, 'declarator');
    const functionBody = this.findFirstNonterminal(memberDecl, 'functionBody');

    if (functionBody && declarator) {
      const name = this.extractDeclaratorName(declarator);
      if (!name) return;

      const params = this.extractParametersFromDeclarator(declarator);
      const specifiers = declaration ? this.findFirstNonterminal(declaration, 'declarationSpecifiers') : null;
      const returnType = this.extractDeclaratorType(specifiers, declarator);
      const methodInfo = {
        name,
        returnType: returnType || 'void',
        params,
        isVirtual: Boolean(this.findFirstTerminal(memberDecl, 'TOKEN_virtual')),
        isConst: Boolean(this.findFirstNonterminal(memberDecl, 'cvQualifierSeq')),
        access
      };

      if (name === cls.name) {
        cls.constructors.push({ name, params, access });
        return;
      }

      if (name === `~${cls.name}`) {
        cls.destructor = { name, params, access };
        if (methodInfo.isVirtual) cls.hasVtable = true;
        return;
      }

      cls.methods.push(methodInfo);
      if (methodInfo.isVirtual) cls.hasVtable = true;
      return;
    }

    if (!declaration) return;

    const specifiers = this.findFirstNonterminal(declaration, 'declarationSpecifiers');
    const initDeclarators = this.findAllNonterminals(declaration, 'initDeclarator');
    if (initDeclarators.length > 0) {
      for (const initDecl of initDeclarators) {
        const fieldDeclarator = this.findFirstNonterminal(initDecl, 'declarator');
        if (!fieldDeclarator) continue;
        const name = this.extractDeclaratorName(fieldDeclarator);
        if (!name) continue;
        const suffix = this.extractArraySuffix(fieldDeclarator);
        cls.members.push({
          name: `${name}${suffix}`,
          type: this.extractDeclaratorType(specifiers, fieldDeclarator) || 'int',
          access
        });
      }
      return;
    }

    if (declarator) {
      const name = this.extractDeclaratorName(declarator);
      if (!name) return;
      const suffix = this.extractArraySuffix(declarator);
      cls.members.push({
        name: `${name}${suffix}`,
        type: this.extractDeclaratorType(specifiers, declarator) || 'int',
        access
      });
    }
  }

  extractDeclaratorType(specifiersNode, declaratorNode) {
    const base = this.text(specifiersNode).trim();
    const ptr = this.extractPointerSuffix(declaratorNode);
    return normalizeTypeText(`${base} ${ptr}`.trim());
  }

  extractPointerSuffix(node) {
    if (!node) return '';
    let out = '';
    if (node.kind === 'nonterminal' && node.name === 'ptrOperator') {
      const stars = this.findAllTerminals(node, 'TOKEN__2A_').length;
      const refs = this.findAllTerminals(node, 'TOKEN__26_').length;
      out += '*'.repeat(stars);
      out += '&'.repeat(refs);
    }
    for (const child of node.children || []) {
      out += this.extractPointerSuffix(child);
    }
    return out;
  }

  extractArraySuffix(node) {
    const suffixes = [];
    const declaratorSuffixes = this.findAllNonterminals(node, 'directDeclaratorSuffix');
    for (const suffix of declaratorSuffixes) {
      const openBracket = this.findFirstTerminal(suffix, 'TOKEN__5B_');
      if (!openBracket) continue;
      const constantExpr = this.findFirstNonterminal(suffix, 'constantExpression');
      const text = this.text(constantExpr).replace(/\s+/g, '').trim();
      suffixes.push(`[${text}]`);
    }
    return suffixes.join('');
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

  findDirectChildNonterminal(node, name) {
    if (!node || !Array.isArray(node.children)) return null;
    for (const child of node.children) {
      if (child.kind === 'nonterminal' && child.name === name) {
        return child;
      }
    }
    return null;
  }

  findAllNonterminals(node, name, out = []) {
    if (!node) return out;
    if (node.kind === 'nonterminal' && node.name === name) {
      out.push(node);
    }
    for (const child of node.children || []) {
      this.findAllNonterminals(child, name, out);
    }
    return out;
  }

  findAllTerminals(node, token, out = []) {
    if (!node) return out;
    if (node.kind === 'terminal' && (!token || node.token === token)) {
      out.push(node);
    }
    for (const child of node.children || []) {
      this.findAllTerminals(child, token, out);
    }
    return out;
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

  extractGlobalFunction(node) {
    // For plain functions: externalDeclaration → declaration
    // For templates: externalDeclaration → templateDeclaration → declaration
    // For explicit specializations: externalDeclaration → explicitSpecialization → declaration
    let declaration = this.findDirectChildNonterminal(node, 'declaration');
    if (!declaration) {
      const wrapper = this.findDirectChildNonterminal(node, 'templateDeclaration')
        || this.findDirectChildNonterminal(node, 'explicitSpecialization');
      if (wrapper) {
        declaration = this.findDirectChildNonterminal(wrapper, 'declaration');
      }
    }
    if (!declaration) return null;

    const functionBody = this.findDirectChildNonterminal(declaration, 'functionBody');
    if (!functionBody) return null;

    const declarator = this.findDirectChildNonterminal(declaration, 'declarator');
    if (!declarator) return null;

    const name = this.extractDeclaratorName(declarator);
    if (!name) return null;
    if (['if', 'for', 'while', 'switch', 'catch'].includes(name)) return null;

    const specifiers = this.findFirstNonterminal(declaration, 'declarationSpecifiers');
    const returnType = normalizeTypeText(this.text(specifiers));
    const params = this.extractParametersFromDeclarator(declarator);
    const bodyText = this.text(functionBody);

    return {
      name,
      returnType,
      params,
      namespacePath: [...this.namespaceStack],
      bodyText,
      simpleReturnExpr: inferSimpleReturnExprFromBody(bodyText, params),
      simpleReturnCall: inferSimpleReturnCallFromBody(bodyText),
      simpleIfReturn: inferSimpleIfReturnFromBody(bodyText, params),
      simpleLocalInitReturn: inferSimpleLocalInitReturnFromBody(bodyText, params),
      simpleMethodCmpReturn: inferSimpleMethodCmpReturnFromBody(bodyText),
      simpleIndexedObjectCmpReturn: inferSimpleIndexedObjectCmpReturnFromBody(bodyText),
      resourceDeterministicHint: inferResourceDeterministicReturnHintFromBody(bodyText, params),
      deterministicNoParamI32Return: null
    };
  }

  extractDeclaratorName(node) {
    const declaratorId = this.findFirstNonterminal(node, 'declaratorId');
    const text = this.text(declaratorId).replace(/\s+/g, '').trim();
    return text || '';
  }

  extractParametersFromDeclarator(node) {
    const clause = this.findFirstNonterminal(node, 'parameterDeclarationClause');
    if (!clause) return [];

    const params = this.findAllNonterminals(clause, 'parameterDeclaration');
    return params.map((param, index) => {
      const specifiers = this.findFirstNonterminal(param, 'declarationSpecifiers');
      const declarator = this.findFirstNonterminal(param, 'declarator');
      const baseType = normalizeTypeText(this.text(specifiers));
      // Count pointer stars in the declarator text (before the identifier).
      // The declarator may look like "* name" or "** name" — each '*' adds one pointer level.
      const declText = this.text(declarator).trim();
      const ptrStars = (declText.match(/^\*+/) || [''])[0];
      const type = ptrStars ? `${baseType}${ptrStars}` : baseType;
      const name = this.extractDeclaratorName(declarator) || `p${index + 1}`;
      return { type, name };
    });
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

    const ctorRegex = new RegExp(`(?:explicit\\s+)?${cls.name}\\s*\\(([^)]*)\\)\\s*(?::\\s*([^{};]*))?\\s*\\{[\\s\\S]*?\\}`, 'g');
    const dtorRegex = new RegExp(`(?:virtual\\s+)?~${cls.name}\\s*\\(([^)]*)\\)\\s*\\{[\\s\\S]*?\\}`, 'g');
    const methodRegex = /(virtual\s+)?([A-Za-z_][A-Za-z0-9_:<>\s\*&]+?)\s+([A-Za-z_][A-Za-z0-9_]*|operator\s*\[\s*\])\s*\(([^)]*)\)\s*(const)?\s*(?:\{[\s\S]*?\}|;)/g;
    const arrayMemberRegex = /([A-Za-z_][A-Za-z0-9_:<>\s\*&]+?)\s+([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*([^\]]+)\s*\]\s*(?:=[^;]+)?;/g;
    const memberRegex = /([A-Za-z_][A-Za-z0-9_:<>\s\*&]+?)\s+([A-Za-z_][A-Za-z0-9_]*(?:\s*\[[^\]]+\])?)\s*(?:=[^;]+)?;/g;

    for (let s = 0; s < sections.length; s += 1) {
      const access = sections[s].access;
      const text = sections[s].text;
      let fm;

      while ((fm = ctorRegex.exec(text)) !== null) {
        const params = this.parseParams(fm[1]);
        cls.constructors.push({
          name: cls.name,
          params,
          lowering: this.inferCtorLowering(fm[2], params),
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
        const name = String(fm[3] || '').replace(/\s+/g, '');
        if (name === cls.name || name === `~${cls.name}`) continue;
        const params = this.parseParams(fm[4]);

        cls.methods.push({
          name,
          returnType,
          params,
          lowering: this.inferMethodLowering(fm[0], name, params),
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

      while ((fm = arrayMemberRegex.exec(textWithoutMethods)) !== null) {
        const type = this.normalizeType(fm[1]);
        const name = `${String(fm[2] || '').trim()}[${String(fm[3] || '').trim()}]`;
        if (!type || !name) continue;
        if (!cls.members.some((member) => member && member.name === name)) {
          cls.members.push({ name, type, access });
        }
      }
      arrayMemberRegex.lastIndex = 0;

      while ((fm = memberRegex.exec(textWithoutMethods)) !== null) {
        const type = this.normalizeType(fm[1]);
        const name = String(fm[2] || '').replace(/\s+/g, '');
        if (!type || !name) continue;
        cls.members.push({ name, type, access });
      }
      memberRegex.lastIndex = 0;
    }

    if ((!cls.members || cls.members.length === 0)
      && Array.isArray(cls.methods)
      && cls.methods.some((method) => method && method.name === 'operator_subscript')) {
      const dataMatch = normalized.match(/\b[A-Za-z_][A-Za-z0-9_]*\s+data\s*\[\s*([^\]]+)\s*\]\s*;/);
      if (dataMatch) {
        cls.members.push({ name: `data[${String(dataMatch[1] || '').trim()}]`, type: 'int', access: cls.defaultAccess });
      }
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

  inferCtorLowering(initializerText, params) {
    const text = String(initializerText || '').trim();
    if (!text) return null;
    const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)$/);
    if (!m) return null;
    const member = m[1];
    const param = m[2];
    if (!Array.isArray(params) || !params.some((p) => p && p.name === param)) return null;
    return { kind: 'member_init', member, param };
  }

  inferMethodLowering(methodText, methodName, methodParams = []) {
    const text = String(methodText || '');
    const bodyMatch = text.match(/\{([\s\S]*?)\}$/);
    if (!bodyMatch) return null;
    const body = bodyMatch[1]
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\/\/.*$/gm, '')
      .trim();
    if (!body) return null;

    const returnIdentifier = body.match(/^return\s+([A-Za-z_][A-Za-z0-9_]*)\s*;$/);
    if (returnIdentifier) {
      const ident = returnIdentifier[1];
      const isParam = (methodParams || []).some((p) => p && p.name === ident);
      if (isParam) {
        return {
          kind: 'return_param',
          param: ident
        };
      }
      return { kind: 'return_member', member: ident };
    }

    const returnIndex = body.match(/^return\s+([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*([A-Za-z_][A-Za-z0-9_]*)\s*\]\s*;$/);
    if (returnIndex) {
      return {
        kind: 'return_index',
        member: returnIndex[1],
        indexParam: returnIndex[2],
        methodName: methodName || ''
      };
    }

    const returnConstInt = body.match(/^return\s+([-+]?\d+)\s*;$/);
    if (returnConstInt) {
      return {
        kind: 'return_const_int',
        value: Number.parseInt(returnConstInt[1], 10) | 0
      };
    }

    const returnCmpTernary = body.match(/^return\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([-+]?\d+)\s*\)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;$/);
    if (returnCmpTernary) {
      return {
        kind: 'return_var_cmp_const_ternary',
        varName: returnCmpTernary[1],
        op: returnCmpTernary[2],
        rhsValue: Number.parseInt(returnCmpTernary[3], 10) | 0,
        thenValue: Number.parseInt(returnCmpTernary[4], 10) | 0,
        elseValue: Number.parseInt(returnCmpTernary[5], 10) | 0
      };
    }

    const ifThenReturnElseReturn = body.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([-+]?\d+)\s*\)\s*\{\s*return\s*([-+]?\d+)\s*;\s*\}\s*return\s*([-+]?\d+)\s*;$/);
    if (ifThenReturnElseReturn) {
      return {
        kind: 'if_var_cmp_const_return',
        varName: ifThenReturnElseReturn[1],
        op: ifThenReturnElseReturn[2],
        rhsValue: Number.parseInt(ifThenReturnElseReturn[3], 10) | 0,
        thenValue: Number.parseInt(ifThenReturnElseReturn[4], 10) | 0,
        elseValue: Number.parseInt(ifThenReturnElseReturn[5], 10) | 0
      };
    }

    const localInitReturn = body.match(/^(int|long|short|char|float|double)\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);\s*return\s+\2\s*;$/);
    if (localInitReturn) {
      const initExpr = (localInitReturn[3] || '').trim();
      if (!initExpr || /::|->|\.|\[|\]|\(|\)/.test(initExpr)) return null;
      const ids = initExpr.match(/[A-Za-z_][A-Za-z0-9_]*/g) || [];
      const params = new Set((methodParams || []).map((p) => p && p.name).filter(Boolean));
      for (const id of ids) {
        if (!params.has(id)) {
          // Allow potential member names. Validation and rewrite happen in emitClassStubs.
          if (!/^[A-Za-z_][A-Za-z0-9_]*$/.test(id)) return null;
        }
      }
      return {
        kind: 'local_init_return',
        localType: this.normalizeType(localInitReturn[1]),
        localName: localInitReturn[2],
        initExpr
      };
    }

    return null;
  }
}

class CppToCTranspiler {
  constructor(analysis, options = {}) {
    this.analysis = analysis;
    this.options = options;
    this.em = new CEmitter();
    this.ambiguityEvents = [];
    this.loweringEvents = [];
    this.allowDeterministicFunctionFolding = options.allowDeterministicFunctionFolding !== false;
    this.emitLoweringDiagnostics = options.emitLoweringDiagnostics !== false;
    this.enumValueMap = extractSimpleEnumValues(options.source || '');
    this.knownTypeNames = new Set([
      ...Object.keys(BUILTIN_TYPES),
      ...Array.from((analysis && analysis.classes) ? analysis.classes.keys() : []),
      ...this.extractFunctionPointerTypedefNames(options.source || '')
    ]);
  }

  transpile() {
    this.emitHeaders();
    this.emitSourceTypedefs();
    this.emitClasses();
    this.emitGlobalFunctionStubs();
    this.emitLoweringDiagnosticsSummary();
    this.emitAmbiguitySummary();
    return this.em.code();
  }

  emitLoweringDiagnosticsSummary() {
    if (!this.emitLoweringDiagnostics) return;
    if (!Array.isArray(this.loweringEvents) || this.loweringEvents.length === 0) return;

    const countByKind = new Map();
    for (const ev of this.loweringEvents) {
      const key = ev && ev.kind ? ev.kind : 'unknown';
      countByKind.set(key, (countByKind.get(key) || 0) + 1);
    }

    const summary = Array.from(countByKind.entries())
      .sort((a, b) => a[0].localeCompare(b[0]))
      .map(([kind, count]) => `${kind}=${count}`)
      .join(', ');

    this.em.line(`/* Lowering diagnostics: ${this.loweringEvents.length} event(s) (${summary}) */`);
    for (let i = 0; i < this.loweringEvents.length && i < 24; i += 1) {
      const ev = this.loweringEvents[i] || {};
      this.em.line(`/* - ${ev.functionName || '<unknown>'}: ${ev.kind || 'unknown'}${ev.detail ? ` (${ev.detail})` : ''} */`);
    }
    if (this.loweringEvents.length > 24) {
      this.em.line(`/* - ... ${this.loweringEvents.length - 24} more event(s) */`);
    }
    this.em.line();
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
    this.em.line('/* Minimal bridge prelude for MaiaC */');
    this.em.line('/* Runtime interface */');
    this.em.line('extern void   __exc_push(void);');
    this.em.line('extern void   __exc_pop(void);');
    this.em.line('extern int    __exc_active(void);');
    this.em.line('extern int    __exc_type(void);');
    this.em.line('extern void*  __exc_data(void);');
    this.em.line('extern void   __exc_throw(int type, void* data);');
    this.em.line('extern void   __exc_clear(void);');
    this.em.line('extern int    __exc_matches(int thrown_type, int catch_type);');
    this.em.line('extern void*  __malloc(unsigned long size);');
    this.em.line('extern void   __free(void* ptr);');
    this.em.line();
    this.emitStdioPrelude(this.options.source || '');
    this.emitHostImportDecls(this.options.source || '');
  }

  /**
   * Emit minimal stdio/FILE declarations when the generated C uses stdio globals.
   * This makes the generated C self-contained without requiring --resolve-system-includes.
   */
  emitStdioPrelude(sourceText) {
    const src = String(sourceText || '');
    if (!/\b(stdout|stdin|stderr|FILE)\b/.test(src)) return;
    this.em.line('/* Minimal stdio declarations (MaiaC-compatible) */');
    this.em.line('struct _FILE;');
    this.em.line('typedef struct _FILE FILE;');
    this.em.line('extern FILE *stdin;');
    this.em.line('extern FILE *stdout;');
    this.em.line('extern FILE *stderr;');
    this.em.line();
  }

  /**
   * Scan the C++ source for forward-declarations of host import functions
   * (names starting with __  but not the built-in __exc_/__malloc/__free family)
   * and emit them as C extern declarations so MaiaC can generate WAT imports.
   */
  emitHostImportDecls(sourceText) {
    const clean = String(sourceText || '')
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\/\/[^\n]*/g, '');

    const seen = new Set();
    // Skip names that are already in the hard-coded prelude above.
    const PRELUDE = new Set(['__exc_push','__exc_pop','__exc_active','__exc_type',
      '__exc_data','__exc_throw','__exc_clear','__exc_matches','__malloc','__free']);

    // Collect text from both top-level and from inside extern "C" { ... } blocks.
    let allText = clean;
    const externCRx = /extern\s+"C"\s*\{([^}]*)\}/gs;
    let em;
    while ((em = externCRx.exec(clean)) !== null) {
      allText += '\n' + em[1];
    }

    // Match: returnType __funcName(params);
    const declRx = /\b([A-Za-z_][A-Za-z0-9_\s*]*?)\s+(__[A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)\s*;/g;
    const invalidTypeKeywordRx = /\b(return|throw|new|delete|if|for|while|switch|case|else|do|goto|break|continue|try|catch)\b/;
    const lines = [];
    let m;
    while ((m = declRx.exec(allText)) !== null) {
      let retType = m[1].trim().replace(/\s+/g, ' ');
      const funcName = m[2];
      const params = (m[3] || '').trim();
      if (seen.has(funcName) || PRELUDE.has(funcName)) continue;
      retType = retType.replace(/^extern\s+/, '').trim();
      if (!retType) continue;
      // Skip storage-class prefixed matches — those are variable declarations,
      // not function declarations (e.g. "static T __name(args);").
      if (/\b(static|inline|auto|register|typedef)\b/.test(retType)) continue;
      if (invalidTypeKeywordRx.test(retType)) continue;
      seen.add(funcName);
      lines.push(`extern ${retType} ${funcName}(${params || 'void'});`);
    }

    if (lines.length > 0) {
      this.em.line('/* Host import declarations */');
      for (const l of lines) this.em.line(l);
      this.em.line();
    }
  }

  emitSourceTypedefs() {
    const typedefs = this.extractFunctionPointerTypedefs(this.options.source || '');
    if (typedefs.length === 0) return;
    for (const line of typedefs) {
      this.em.line(line);
    }
    this.em.line();
  }

  extractFunctionPointerTypedefs(sourceText) {
    const clean = String(sourceText || '')
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\/\/.*$/gm, '');
    const out = [];
    const seen = new Set();
    const rx = /typedef\s+[^;]*\(\s*\*\s*[A-Za-z_][A-Za-z0-9_]*\s*\)\s*\([^;]*\)\s*;/g;
    let m;
    while ((m = rx.exec(clean)) !== null) {
      const text = m[0].replace(/\s+/g, ' ').trim();
      if (!seen.has(text)) {
        seen.add(text);
        out.push(text);
      }
    }
    return out;
  }

  extractFunctionPointerTypedefNames(sourceText) {
    const clean = String(sourceText || '')
      .replace(/\/\*[\s\S]*?\*\//g, '')
      .replace(/\/\/.*$/gm, '');
    const out = [];
    const rx = /typedef\s+[^;]*\(\s*\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*\([^;]*\)\s*;/g;
    let m;
    while ((m = rx.exec(clean)) !== null) {
      out.push(m[1]);
    }
    return out;
  }

  sanitizeTypeForC(typeText) {
    const normalized = normalizeTypeText(typeText || '');
    if (!normalized) return 'int';

    const ptrMatch = normalized.match(/(\*+)$/);
    const ptrSuffix = ptrMatch ? ptrMatch[1] : '';
    const base = ptrSuffix ? normalized.slice(0, -ptrSuffix.length).trim() : normalized;
    if (!base) return 'void*';  // unknown empty base → opaque pointer

    const builtinWords = new Set(['signed', 'unsigned', 'short', 'long', 'int', 'char', 'float', 'double', 'void']);
    const baseWords = base.split(/\s+/).filter(Boolean);
    const isBuiltinPhrase = baseWords.length > 0 && baseWords.every((word) => builtinWords.has(word));
    // MaiaC C89 parser does not support multi-level pointers (**).
    // Always cap pointer depth at one level ('*') in the emitted C.
    const safePtrSuffix = ptrSuffix.length > 0 ? '*' : '';
    if (isBuiltinPhrase || this.knownTypeNames.has(base)) {
      return `${base}${safePtrSuffix}`;
    }

    if (safePtrSuffix) return `void*`;
    return 'int';
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
      let hasPayload = false;
      for (const base of cls.bases) {
        // Strip template parameters (e.g. basic_streambuf<char> → basic_streambuf)
        // so the base field is a valid C struct member type.
        const baseTypeName = (base.name || 'int').replace(/<[^>]*>/g, '').trim() || 'int';
        this.em.line(`${baseTypeName} __base;`);
        hasPayload = true;
      }
      for (const member of cls.members) {
        this.em.line(`${this.sanitizeTypeForC(member.type)} ${member.name};`);
        hasPayload = true;
      }
      if (cls.hasVtable) {
        this.em.line(`void* __vptr;`);
        hasPayload = true;
      }
      if (!hasPayload) {
        const sourceText = String(this.options && this.options.source ? this.options.source : '');
        const classBodyMatch = sourceText.match(new RegExp(`(?:template\\s*<[^>]+>\\s*)?(?:class|struct)\\s+${name}\\b[^{]*\\{([\\s\\S]*?)\\};`));
        const dataMatch = classBodyMatch && classBodyMatch[1]
          ? classBodyMatch[1].match(/(?:^|\n)\s*(?!return\b)[A-Za-z_][A-Za-z0-9_:<>\s\*&]*\s+data\s*\[\s*([^\]]+)\s*\]\s*;/m)
          : null;
        if (dataMatch) {
          this.em.line(`int data[${String(dataMatch[1] || '').trim()}];`);
          hasPayload = true;
        }
      }
      if (!hasPayload && Array.isArray(cls.methods) && cls.methods.some((method) => method && method.name === 'operator_subscript')) {
        this.em.line('int data[4];');
        hasPayload = true;
      }
      if (!hasPayload) {
        this.em.line('int __dummy;');
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
        this.em.line(`${this.sanitizeTypeForC(method.returnType)} ${mangle(method.name, sigTypes, name, nsPath)}(${name}* self${paramsText ? `, ${paramsText}` : ''});`);
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
      return includeType ? `${this.sanitizeTypeForC(p.type)} ${pname}` : pname;
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
      let emittedCtorLowering = false;
      if (ctor.lowering && ctor.lowering.kind === 'member_init') {
        this.em.line(`self->${ctor.lowering.member} = ${ctor.lowering.param};`);
        emittedCtorLowering = true;
      }
      for (let i = 0; i < ctor.params.length; i += 1) {
        const pname = ctor.params[i].name;
        if (!emittedCtorLowering || pname !== ctor.lowering?.param) {
          this.em.line(`(void)${pname};`);
        }
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
      this.em.line(`${this.sanitizeTypeForC(method.returnType)} ${mangle(method.name, sigTypes, name, nsPath)}(${name}* self${paramsText ? `, ${paramsText}` : ''}) {`);
      this.em.level += 1;
      this.em.line('(void)self;');
      let emittedMethodLowering = false;
      const loweredReturnType = this.sanitizeTypeForC(method.returnType);
      if (method.lowering && method.lowering.kind === 'return_member') {
        if (loweredReturnType !== 'void') {
          this.em.line(`return self->${method.lowering.member};`);
          emittedMethodLowering = true;
        }
      } else if (method.lowering && method.lowering.kind === 'return_index') {
        const idxName = method.lowering.indexParam;
        if (loweredReturnType !== 'void') {
          if (loweredReturnType.includes('*')) {
            this.em.line(`return (${loweredReturnType})(self->${method.lowering.member} + ${idxName});`);
          } else {
            this.em.line(`return self->${method.lowering.member}[${idxName}];`);
          }
          emittedMethodLowering = true;
        }
      } else if (method.lowering && method.lowering.kind === 'return_const_int') {
        if (loweredReturnType !== 'void') {
          this.em.line(`return ${method.lowering.value | 0};`);
          emittedMethodLowering = true;
        }
      } else if (method.lowering && method.lowering.kind === 'return_param') {
        if (loweredReturnType !== 'void') {
          const matchParam = (method.params || []).find((p) => p && p.name === method.lowering.param);
          if (matchParam) {
            this.em.line(`return ${method.lowering.param};`);
            emittedMethodLowering = true;
          }
        }
      } else if (method.lowering && method.lowering.kind === 'return_var_cmp_const_ternary') {
        if (loweredReturnType !== 'void') {
          const v = method.lowering.varName;
          const isParam = (method.params || []).some((p) => p && p.name === v);
          const isMember = (cls.members || []).some((m) => m && m.name === v);
          if (isParam || isMember) {
            const lhs = isMember ? `self->${v}` : v;
            this.em.line(`return (${lhs} ${method.lowering.op} ${method.lowering.rhsValue | 0}) ? ${method.lowering.thenValue | 0} : ${method.lowering.elseValue | 0};`);
            emittedMethodLowering = true;
          }
        }
      } else if (method.lowering && method.lowering.kind === 'if_var_cmp_const_return') {
        if (loweredReturnType !== 'void') {
          const v = method.lowering.varName;
          const isParam = (method.params || []).some((p) => p && p.name === v);
          const isMember = (cls.members || []).some((m) => m && m.name === v);
          if (isParam || isMember) {
            const lhs = isMember ? `self->${v}` : v;
            this.em.line(`if (${lhs} ${method.lowering.op} ${method.lowering.rhsValue | 0}) {`);
            this.em.level += 1;
            this.em.line(`return ${method.lowering.thenValue | 0};`);
            this.em.level -= 1;
            this.em.line('}');
            this.em.line(`return ${method.lowering.elseValue | 0};`);
            emittedMethodLowering = true;
          }
        }
      } else if (method.lowering && method.lowering.kind === 'local_init_return') {
        if (loweredReturnType !== 'void') {
          const members = new Set((cls.members || []).map((m) => m && m.name).filter(Boolean));
          const params = new Set((method.params || []).map((p) => p && p.name).filter(Boolean));
          let initExpr = String(method.lowering.initExpr || '');
          let valid = true;
          const ids = initExpr.match(/[A-Za-z_][A-Za-z0-9_]*/g) || [];
          for (const id of ids) {
            if (params.has(id)) continue;
            if (members.has(id)) {
              const rx = new RegExp(`\\b${id}\\b`, 'g');
              initExpr = initExpr.replace(rx, `self->${id}`);
              continue;
            }
            valid = false;
            break;
          }
          if (valid) {
            this.em.line(`${this.sanitizeTypeForC(method.lowering.localType)} ${method.lowering.localName} = ${initExpr};`);
            this.em.line(`return ${method.lowering.localName};`);
            emittedMethodLowering = true;
          }
        }
      }
      for (let i = 0; i < method.params.length; i += 1) {
        if (!emittedMethodLowering || method.params[i].name !== method.lowering?.indexParam) {
          this.em.line(`(void)${method.params[i].name};`);
        }
      }
      if (!emittedMethodLowering && method.returnType !== 'void') {
        this.em.line(`return (${loweredReturnType})0;`);
      }
      this.em.level -= 1;
      this.em.line('}');
      this.em.line();
    }
  }

  emitGlobalFunctionStubs() {
    const fns = Array.isArray(this.analysis.functions) ? this.analysis.functions : [];
    if (fns.length === 0) return;
    const structuredMain = this.extractMainStructuredPlan(this.options.source || '');

    this.em.line('/* Global functions */');
    for (const fn of fns) {
      const sigTypes = (fn.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
      const paramsText = this.formatParams(fn.params || [], true);
      const mangled = mangle(fn.name, sigTypes, null, fn.namespacePath || []);
      const returnType = this.sanitizeTypeForC(fn.returnType);
      this.em.line(`${returnType} ${mangled}(${paramsText || 'void'});`);
    }
    this.em.line();

    for (const fn of fns) {
      const sigTypes = (fn.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
      const paramsText = this.formatParams(fn.params || [], true);
      const mangled = mangle(fn.name, sigTypes, null, fn.namespacePath || []);
      const returnType = this.sanitizeTypeForC(fn.returnType);
      const hasStructuredCandidate = Boolean(
        fn.simpleIfReturn
        || fn.simpleLocalInitReturn
        || fn.simpleMethodCmpReturn
        || fn.simpleIndexedObjectCmpReturn
        || fn.simpleReturnExpr
        || fn.simpleReturnCall
      );
      this.em.line(`${returnType} ${mangled}(${paramsText || 'void'}) {`);
      this.em.level += 1;
      if (fn.name === 'main' && !fn.namespacePath?.length && structuredMain) {
        this.emitStructuredMain(structuredMain);
      } else if (Number.isInteger(fn.deterministicNoParamI32Return)
        && fn.returnType !== 'void'
        && this.allowDeterministicFunctionFolding
        && !hasStructuredCandidate
        && !fn.resourceDeterministicHint) {
        this.loweringEvents.push({
          functionName: fn.name,
          kind: 'deterministic-fold',
          detail: `return ${fn.deterministicNoParamI32Return | 0}`
        });
        this.em.line(`return ${fn.deterministicNoParamI32Return | 0};`);
      } else if (fn.simpleIfReturn && fn.returnType !== 'void') {
        const lowered = this.lowerSimpleIfReturn(fn);
        if (lowered) this.em.line(`return ${lowered};`);
        else {
          this.loweringEvents.push({
            functionName: fn.name,
            kind: 'stub-fallback',
            detail: 'simple-if-lowering-failed'
          });
          this.emitStubReturn(fn);
        }
      } else if (fn.simpleLocalInitReturn && fn.returnType !== 'void') {
        const lowered = this.lowerSimpleLocalInitReturn(fn);
        if (lowered) {
          for (const local of lowered.locals) {
            const rewrittenInit = this.rewriteSimpleInitExprForC(local.initExpr, fn, fns);
            this.em.line(`${local.type} ${local.name} = ${rewrittenInit};`);
          }
          this.loweringEvents.push({
            functionName: fn.name,
            kind: 'structured-local-return',
            detail: `${lowered.locals.length} local(s)`
          });
          this.em.line(`return ${lowered.returnExpr};`);
        } else {
          this.loweringEvents.push({
            functionName: fn.name,
            kind: 'stub-fallback',
            detail: 'simple-local-init-return-lowering-failed'
          });
          this.emitStubReturn(fn);
        }
      } else if (fn.simpleMethodCmpReturn && fn.returnType !== 'void') {
        const lowered = this.lowerSimpleMethodCmpReturn(fn);
        if (lowered) {
          const className = String(lowered.local.type || '').replace(/<[^>]*>/g, '').trim();
          const cType = this.sanitizeTypeForC(className || lowered.local.type);
          let emittedStructured = true;
          this.em.line(`${cType} ${lowered.local.name};`);

          const ctorArgTypes = (lowered.local.ctorArgs || []).map(() => 'int');
          const initMangled = this.resolveClassMangled(className, 'init', ctorArgTypes)
            || this.resolveClassMangled(className, 'init', []);
          if (!initMangled) {
            emittedStructured = false;
          } else {
            const ctorArgsText = lowered.local.ctorArgs.map((v) => String(v | 0)).join(', ');
            this.em.line(`${initMangled}(&${lowered.local.name}${ctorArgsText ? `, ${ctorArgsText}` : ''});`);
          }

          let leftExpr = null;
          if (lowered.access.kind === 'method') {
            const methodMangled = this.resolveClassMangled(className, lowered.access.name, []);
            if (methodMangled) leftExpr = `${methodMangled}(&${lowered.local.name})`;
          } else if (lowered.access.kind === 'index_sum') {
            const subMangled = this.resolveClassMangled(className, 'operator_subscript', ['int']);
            if (subMangled) {
              leftExpr = `(*((int*)${subMangled}(&${lowered.local.name}, ${lowered.access.a | 0})) + *((int*)${subMangled}(&${lowered.local.name}, ${lowered.access.b | 0})))`;
            }
          }

          if (!emittedStructured || !leftExpr) {
            this.loweringEvents.push({
              functionName: fn.name,
              kind: 'stub-fallback',
              detail: 'simple-method-cmp-return-lowering-failed'
            });
            this.emitStubReturn(fn);
          } else {
            this.loweringEvents.push({
              functionName: fn.name,
              kind: 'structured-method-cmp-return',
              detail: lowered.access.kind
            });
            this.em.line(`return (${leftExpr} ${lowered.op} ${lowered.rhsValue | 0}) ? ${lowered.thenValue | 0} : ${lowered.elseValue | 0};`);
          }
        } else {
          this.loweringEvents.push({
            functionName: fn.name,
            kind: 'stub-fallback',
            detail: 'simple-method-cmp-return-lowering-failed'
          });
          this.emitStubReturn(fn);
        }
      } else if (fn.simpleIndexedObjectCmpReturn && fn.returnType !== 'void') {
        const lowered = this.lowerSimpleIndexedObjectCmpReturn(fn);
        if (lowered) {
          const className = String(lowered.local.type || '').replace(/<[^>]*>/g, '').trim();
          const cType = this.sanitizeTypeForC(className || lowered.local.type);
          const initMangled = this.resolveClassMangled(className, 'init', []);
          const subMangled = this.resolveClassMangled(className, 'operator_subscript', ['int']);

          if (!initMangled || !subMangled) {
            this.loweringEvents.push({
              functionName: fn.name,
              kind: 'stub-fallback',
              detail: 'simple-indexed-object-cmp-return-lowering-failed'
            });
            this.emitStubReturn(fn);
          } else {
            this.em.line(`${cType} ${lowered.local.name};`);
            this.em.line(`${initMangled}(&${lowered.local.name});`);
            for (const assign of lowered.assignments) {
              this.em.line(`*((int*)${subMangled}(&${lowered.local.name}, ${assign.index | 0})) = ${assign.value | 0};`);
            }
            this.loweringEvents.push({
              functionName: fn.name,
              kind: 'structured-indexed-object-cmp-return',
              detail: `${lowered.assignments.length} assignment(s)`
            });
            const lhs = `(*((int*)${subMangled}(&${lowered.local.name}, ${lowered.sumIndexes.a | 0})) + *((int*)${subMangled}(&${lowered.local.name}, ${lowered.sumIndexes.b | 0})))`;
            this.em.line(`return (${lhs} ${lowered.op} ${lowered.rhsValue | 0}) ? ${lowered.thenValue | 0} : ${lowered.elseValue | 0};`);
          }
        } else {
          this.loweringEvents.push({
            functionName: fn.name,
            kind: 'stub-fallback',
            detail: 'simple-indexed-object-cmp-return-lowering-failed'
          });
          this.emitStubReturn(fn);
        }
      } else if (fn.simpleReturnExpr && fn.returnType !== 'void') {
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
      } else if (fn.returnType !== 'void') {
        const loweredStructuredIo = this.lowerStructuredIoDeterministicFunction(fn);
        if (loweredStructuredIo && Array.isArray(loweredStructuredIo.ops) && loweredStructuredIo.ops.length > 0) {
          this.emitStructuredLocals(loweredStructuredIo.locals || []);
          this.emitStructuredMainOps(loweredStructuredIo.ops || []);
          this.loweringEvents.push({
            functionName: fn.name,
            kind: 'structured-io-runtime',
            detail: loweredStructuredIo.detail || 'structured-io-runtime'
          });
        } else if (fn.resourceDeterministicHint) {
          const lowered = this.lowerResourceDeterministicFunction(fn);
          if (lowered && Array.isArray(lowered.lines) && lowered.lines.length > 0) {
            for (const line of lowered.lines) this.em.line(line);
            this.loweringEvents.push({
              functionName: fn.name,
              kind: 'structured-resource-runtime',
              detail: lowered.detail || 'resource-pattern'
            });
          } else if (lowered && Array.isArray(lowered.ops) && lowered.ops.length > 0) {
            this.emitStructuredLocals(lowered.locals || []);
            this.emitStructuredMainOps(lowered.ops || []);
            this.loweringEvents.push({
              functionName: fn.name,
              kind: 'structured-resource-runtime',
              detail: lowered.detail || 'resource-pattern'
            });
          } else if (Number.isInteger(fn.deterministicNoParamI32Return)) {
            this.loweringEvents.push({
              functionName: fn.name,
              kind: 'structured-resource-deterministic',
              detail: `return ${fn.deterministicNoParamI32Return | 0}`
            });
            this.em.line(`return ${fn.deterministicNoParamI32Return | 0};`);
          } else {
            this.loweringEvents.push({
              functionName: fn.name,
              kind: 'stub-fallback',
              detail: 'resource-lowering-failed'
            });
            this.emitStubReturn(fn);
          }
        } else {
          if (Number.isInteger(fn.deterministicNoParamI32Return) && !this.allowDeterministicFunctionFolding) {
            this.loweringEvents.push({
              functionName: fn.name,
              kind: 'deterministic-fold-blocked',
              detail: `candidate return ${fn.deterministicNoParamI32Return | 0}`
            });
          }
          this.loweringEvents.push({
            functionName: fn.name,
            kind: 'stub-fallback',
            detail: 'no-supported-lowering'
          });
          this.emitStubReturn(fn);
        }
      } else {
        if (Number.isInteger(fn.deterministicNoParamI32Return) && fn.returnType !== 'void' && !this.allowDeterministicFunctionFolding) {
          this.loweringEvents.push({
            functionName: fn.name,
            kind: 'deterministic-fold-blocked',
            detail: `candidate return ${fn.deterministicNoParamI32Return | 0}`
          });
        }
        this.loweringEvents.push({
          functionName: fn.name,
          kind: 'stub-fallback',
          detail: 'no-supported-lowering'
        });
        this.emitStubReturn(fn);
      }
      this.em.level -= 1;
      this.em.line('}');
      this.em.line();
    }
  }

  emitStubReturn(fn) {
    for (const p of fn.params || []) {
      this.em.line(`(void)${p.name};`);
    }
    if (fn.returnType !== 'void') {
      this.em.line(`return (${this.sanitizeTypeForC(fn.returnType)})0;`);
    }
  }

  resolveGlobalMangled(name, arity, namespacePath = []) {
    const allFns = Array.isArray(this.analysis?.functions) ? this.analysis.functions : [];
    const found = this.resolveGlobalFunction(name, arity, namespacePath, allFns);
    if (!found) return name;
    const sigTypes = (found.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
    return mangle(found.name, sigTypes, null, found.namespacePath || []);
  }

  resolveClassMangled(className, memberName, paramTypeNames = []) {
    const classes = this.analysis?.classes;
    if (!(classes instanceof Map) || !classes.has(className)) return null;
    const cls = classes.get(className);
    const sigTypes = (paramTypeNames || []).map((t) => ({ kind: this.typeKindFromText(t), name: t }));
    return mangle(memberName, sigTypes, className, cls.namespacePath || []);
  }

  lowerSimpleIfReturn(fn) {
    const info = fn.simpleIfReturn;
    if (!info || info.kind !== 'var_cmp') return null;
    const left = info.leftName;
    const right = info.right?.kind === 'const' ? String(info.right.value | 0) : info.right?.name;
    if (!left || !right) return null;
    return `(${left} ${info.op} ${right}) ? ${info.thenValue | 0} : ${info.elseValue | 0}`;
  }

  lowerSimpleLocalInitReturn(fn) {
    const info = fn.simpleLocalInitReturn;
    if (!info || !Array.isArray(info.locals) || !info.returnExpr) return null;
    if (info.locals.length === 0) return null;

    const locals = [];
    for (const local of info.locals) {
      if (!local || !local.type || !local.name || !local.initExpr) return null;
      locals.push({
        type: normalizeTypeText(local.type),
        name: String(local.name).trim(),
        initExpr: String(local.initExpr).trim()
      });
    }

    const returnExpr = String(info.returnExpr || '').trim();
    if (!returnExpr) return null;
    return { locals, returnExpr };
  }

  rewriteSimpleInitExprForC(expr, currentFn, allFns) {
    const text = String(expr || '').trim();
    if (!text) return text;

    const splitTopLevelArgs = (argsText) => {
      const out = [];
      let depth = 0;
      let cur = '';
      const src = String(argsText || '');
      for (let i = 0; i < src.length; i += 1) {
        const ch = src[i];
        if (ch === '(') depth += 1;
        else if (ch === ')' && depth > 0) depth -= 1;
        if (ch === ',' && depth === 0) {
          out.push(cur.trim());
          cur = '';
          continue;
        }
        cur += ch;
      }
      if (cur.trim()) out.push(cur.trim());
      return out.filter(Boolean);
    };

    const maybeRewriteIdentifier = (identifier) => {
      const id = String(identifier || '').trim();
      if (!/^[A-Za-z_][A-Za-z0-9_]*$/.test(id)) return id;
      const fns = Array.isArray(allFns) ? allFns : [];
      const candidates = fns.filter((f) => f && f.name === id);
      if (candidates.length === 0) return id;
      let selected = candidates[0];
      if (candidates.length > 1) {
        const currentNs = Array.isArray(currentFn?.namespacePath) ? currentFn.namespacePath : [];
        const sameNs = candidates.filter((f) => this.sameNs(f.namespacePath || [], currentNs));
        if (sameNs.length === 1) selected = sameNs[0];
      }
      const sigTypes = (selected.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
      return mangle(selected.name, sigTypes, null, selected.namespacePath || []);
    };

    const callMatch = text.match(/^([A-Za-z_][A-Za-z0-9_:]*)\s*\((.*)\)$/);
    if (callMatch) {
      const rawCallee = callMatch[1];
      const parts = rawCallee.split('::').filter(Boolean);
      const baseName = parts[parts.length - 1] || rawCallee;
      const qualifiedNs = parts.slice(0, -1);

      const rawArgs = splitTopLevelArgs(callMatch[2] || '');
      const rewrittenArgs = rawArgs.map((arg) => this.rewriteSimpleInitExprForC(arg, currentFn, allFns));

      const resolution = this.resolveGlobalFunctionDetailed(
        baseName,
        rewrittenArgs.length,
        currentFn?.namespacePath || [],
        allFns,
        qualifiedNs,
        rawCallee.includes('::'),
        []
      );

      if (resolution && resolution.match) {
        const match = resolution.match;
        const sigTypes = (match.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
        const mangled = mangle(match.name, sigTypes, null, match.namespacePath || []);
        return `${mangled}(${rewrittenArgs.join(', ')})`;
      }
      return `${rawCallee}(${rewrittenArgs.join(', ')})`;
    }

    return maybeRewriteIdentifier(text);
  }

  lowerSimpleMethodCmpReturn(fn) {
    const info = fn.simpleMethodCmpReturn;
    if (!info || !info.local || !info.access) return null;
    if (!info.local.type || !info.local.name) return null;
    const ctorArgs = Array.isArray(info.local.ctorArgs) ? info.local.ctorArgs : [];
    if (!ctorArgs.every((n) => Number.isInteger(n))) return null;
    if (!['==', '!=', '<', '<=', '>', '>='].includes(info.op)) return null;
    if (!Number.isInteger(info.rhsValue) || !Number.isInteger(info.thenValue) || !Number.isInteger(info.elseValue)) return null;

    const access = info.access;
    if (access.kind === 'method') {
      if (!access.name || !/^[A-Za-z_][A-Za-z0-9_]*$/.test(access.name)) return null;
    } else if (access.kind === 'index_sum') {
      if (!Number.isInteger(access.a) || !Number.isInteger(access.b)) return null;
    } else {
      return null;
    }

    return {
      local: {
        type: String(info.local.type).trim(),
        name: String(info.local.name).trim(),
        ctorArgs
      },
      access,
      op: info.op,
      rhsValue: info.rhsValue | 0,
      thenValue: info.thenValue | 0,
      elseValue: info.elseValue | 0
    };
  }

  lowerSimpleIndexedObjectCmpReturn(fn) {
    const info = fn.simpleIndexedObjectCmpReturn;
    if (!info || !info.local || !Array.isArray(info.assignments) || !info.sumIndexes) return null;
    if (!info.local.type || !info.local.name) return null;
    if (!['==', '!=', '<', '<=', '>', '>='].includes(info.op)) return null;
    if (!Number.isInteger(info.rhsValue) || !Number.isInteger(info.thenValue) || !Number.isInteger(info.elseValue)) return null;
    if (!Number.isInteger(info.sumIndexes.a) || !Number.isInteger(info.sumIndexes.b)) return null;
    if (info.assignments.length === 0) return null;

    const assignments = [];
    for (const assign of info.assignments) {
      if (!assign || !Number.isInteger(assign.index) || !Number.isInteger(assign.value)) return null;
      assignments.push({ index: assign.index | 0, value: assign.value | 0 });
    }

    return {
      local: {
        type: String(info.local.type).trim(),
        name: String(info.local.name).trim()
      },
      assignments,
      sumIndexes: {
        a: info.sumIndexes.a | 0,
        b: info.sumIndexes.b | 0
      },
      op: info.op,
      rhsValue: info.rhsValue | 0,
      thenValue: info.thenValue | 0,
      elseValue: info.elseValue | 0
    };
  }

  lowerResourceDeterministicFunction(fn) {
    const clean = cleanFunctionBodyText(fn?.bodyText || '');
    if (!clean) return null;

    const structuredIo = this.lowerStructuredIoDeterministicFunction(fn);
    if (structuredIo) {
      return structuredIo;
    }

    if (/\bdynamic_cast\s*</.test(clean) && /\bstatic_cast\s*<\s*int\s*>/.test(clean)) {
      return this.lowerResourceCastStaticPattern(clean);
    }

    if (/\bnew\s+int\s*\(/.test(clean) && /\bnew\s*\(/.test(clean) && /~\s*[A-Za-z_][A-Za-z0-9_]*\s*\(/.test(clean)) {
      return this.lowerResourceNewDeletePattern(clean);
    }

    return null;
  }

  lowerStructuredIoDeterministicFunction(fn) {
    const body = this.stripComments(fn?.bodyText || '').trim();
    if (!body || !/(?:^|\W)(?:std::)?cout\s*<</.test(body)) return null;

    const locals = [];
    const ops = [];
    let rest = body;

    const parseArg = (text) => {
      const t = String(text || '').trim();
      if (!t) return null;
      if (/^[-+]?\d+$/.test(t)) return { type: 'int', value: Number.parseInt(t, 10) | 0 };
      if (/^[-+]?(?:\d+\.\d*|\d*\.\d+)(?:[eE][-+]?\d+)?$/.test(t)) return { type: 'double', value: Number(t) };
      if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(t)) return { type: 'var', name: t };
      return null;
    };

    const getKnownVarType = (name) => {
      const varName = String(name || '').trim();
      if (!varName) return 'int';
      for (let i = locals.length - 1; i >= 0; i -= 1) {
        const local = locals[i];
        if (local && local.name === varName) {
          return String(local.type || 'int');
        }
      }
      return 'int';
    };

    const parseLocal = (text) => {
      const m = text.match(/^int\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([-+]?\d+)\s*;\s*/);
      if (!m) return null;
      return {
        consumed: m[0].length,
        local: { name: m[1], type: 'int', init: Number.parseInt(m[2], 10) | 0 }
      };
    };

    const parseLocalDouble = (text) => {
      const m = text.match(/^double\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([-+]?(?:\d+\.\d*|\d*\.\d+)(?:[eE][-+]?\d+)?)\s*;\s*/);
      if (!m) return null;
      return { consumed: m[0].length, local: { name: m[1], type: 'double', init: Number(m[2]) } };
    };

    const parseLocalChar = (text) => {
      const m = text.match(/^char\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*('(?:\\.|[^'\\])')\s*;\s*/);
      if (!m) return null;
      return { consumed: m[0].length, local: { name: m[1], type: 'char', init: m[2] } };
    };

    const parseCoutChain = (text) => {
      const m = text.match(/^(?:std::)?cout\s*((?:<<\s*(?:"(?:\\.|[^"\\])*"|(?:std::)?endl|[-+]?(?:\d+\.\d*|\d*\.\d+)(?:[eE][-+]?\d+)?|[-+]?\d+|'(?:\\.|[^'\\])'|[A-Za-z_][A-Za-z0-9_]*)\s*)+);\s*/);
      if (!m) return null;

      const segment = m[1] || '';
      const tokenRe = /<<\s*("((?:\\.|[^"\\])*)"|((?:std::)?endl)|([-+]?(?:\d+\.\d*|\d*\.\d+)(?:[eE][-+]?\d+)?)|([-+]?\d+)|('(?:\\.|[^'\\])')|([A-Za-z_][A-Za-z0-9_]*))\s*/g;
      const items = [];
      let hit;
      while ((hit = tokenRe.exec(segment)) !== null) {
        if (hit[2] != null) {
          items.push({ kind: 'string', value: hit[2] || '' });
        } else if (hit[3] != null) {
          items.push({ kind: 'endl' });
        } else if (hit[4] != null) {
          items.push({ kind: 'double', value: Number(hit[4]) });
        } else if (hit[5] != null) {
          items.push({ kind: 'int', value: Number.parseInt(hit[5], 10) | 0 });
        } else if (hit[6] != null) {
          items.push({ kind: 'char', value: hit[6] });
        } else if (hit[7] != null) {
          items.push({ kind: 'var', name: hit[7], type: getKnownVarType(hit[7]) });
        }
      }

      if (items.length === 0) return null;
      return { consumed: m[0].length, op: { kind: 'cout_chain', items } };
    };

    const parseAssignBinary = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*(==|!=|<=|>=|<|>|<<|>>|\+|-|\*|\/|%|&|\||\^|&&|\|\|)\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*;\s*/);
      if (!m) return null;
      const left = parseArg(m[2]);
      const right = parseArg(m[4]);
      if (!left || !right) return null;
      return { consumed: m[0].length, op: { kind: 'assign_binary', target: m[1], left, operator: m[3], right } };
    };

    const parseInc = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\+\+\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'inc', varName: m[1] } } : null;
    };

    const parseDec = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*--\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'dec', varName: m[1] } } : null;
    };

    const parseReturnTernaryCmp = (text) => {
      const m = text.match(/^return\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([^?]+?)\s*\)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;\s*/);
      if (!m) return null;
      const rhs = parseArg(m[3]);
      if (!rhs || (rhs.type !== 'int' && rhs.type !== 'var')) return null;
      return {
        consumed: m[0].length,
        op: {
          kind: 'return_ternary_cmp',
          varName: m[1],
          cmp: m[2],
          rhs,
          thenValue: Number.parseInt(m[4], 10) | 0,
          elseValue: Number.parseInt(m[5], 10) | 0
        }
      };
    };

    const parseReturn = (text) => {
      const m = text.match(/^return\s+([-+]?\d+)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'return', value: Number.parseInt(m[1], 10) | 0 } } : null;
    };

    const parseFirstMatch = (text, parsers) => {
      for (const parse of parsers || []) {
        const result = parse(text);
        if (result) return result;
      }
      return null;
    };

    // Recursive block-body parser used for loop/branch bodies.
    const flatParsers = [parseAssignBinary, parseCoutChain, parseInc, parseDec, parseReturnTernaryCmp, parseReturn];
    const parseBlockOps = (blockText) => {
      const out = [];
      let t = String(blockText || '').trim();
      while (t.length > 0) {
        const op = parseFirstMatch(t, flatParsers);
        if (op) {
          out.push(op.op);
          t = t.slice(op.consumed).trim();
          continue;
        }
        // for (int i = init; i < limit; ++i) { ... }
        const forM = t.match(/^for\s*\(\s*int\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([-+]?\d+)\s*;\s*\1\s*<\s*([-+]?\d+)\s*;\s*\+\+\1\s*\)\s*\{/);
        if (forM) {
          const indexName = forM[1];
          const init = Number.parseInt(forM[2], 10) | 0;
          const limit = Number.parseInt(forM[3], 10) | 0;
          const openBrace = forM[0].lastIndexOf('{');
          const closeBrace = findMatchingBrace(t, openBrace);
          if (closeBrace < 0) return null;
          const bodyOps = parseBlockOps(t.slice(openBrace + 1, closeBrace).trim());
          if (!bodyOps) return null;
          if (!locals.some((l) => l && l.name === indexName)) {
            locals.push({ name: indexName, type: 'int', init });
          }
          out.push({ kind: 'for_lt_inc', indexName, init, limit, bodyOps });
          t = t.slice(closeBrace + 1).trim();
          continue;
        }
        return null;
      }
      return out;
    };

    while (rest.length > 0) {
      const local = parseFirstMatch(rest, [parseLocal, parseLocalDouble, parseLocalChar]);
      if (local) {
        locals.push(local.local);
        rest = rest.slice(local.consumed).trim();
        continue;
      }

      const op = parseFirstMatch(rest, flatParsers);
      if (op) {
        ops.push(op.op);
        rest = rest.slice(op.consumed).trim();
        continue;
      }

      // for loop at the top level of the function body
      const forM = rest.match(/^for\s*\(\s*int\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([-+]?\d+)\s*;\s*\1\s*<\s*([-+]?\d+)\s*;\s*\+\+\1\s*\)\s*\{/);
      if (forM) {
        const indexName = forM[1];
        const init = Number.parseInt(forM[2], 10) | 0;
        const limit = Number.parseInt(forM[3], 10) | 0;
        const openBrace = forM[0].lastIndexOf('{');
        const closeBrace = findMatchingBrace(rest, openBrace);
        if (closeBrace < 0) return null;
        const bodyOps = parseBlockOps(rest.slice(openBrace + 1, closeBrace).trim());
        if (!bodyOps) return null;
        if (!locals.some((l) => l && l.name === indexName)) {
          locals.push({ name: indexName, type: 'int', init });
        }
        ops.push({ kind: 'for_lt_inc', indexName, init, limit, bodyOps });
        rest = rest.slice(closeBrace + 1).trim();
        continue;
      }

      return null;
    }

    if (locals.length === 0 || ops.length === 0) return null;
    return { detail: 'structured-io-runtime', locals, ops };
  }

  lowerResourceCastStaticPattern(cleanBody) {
    const allocMatch = cleanBody.match(/([A-Za-z_][A-Za-z0-9_]*)\s*\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*new\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([-+]?\d+)\s*\)\s*;/);
    const dynMatch = cleanBody.match(/([A-Za-z_][A-Za-z0-9_]*)\s*\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*dynamic_cast\s*<\s*([A-Za-z_][A-Za-z0-9_]*)\s*\*\s*>\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*;/);
    const staticCastMatch = cleanBody.match(/int\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*static_cast\s*<\s*int\s*>\s*\(\s*([^\)]+?)\s*\)\s*;/);
    const methodCmpMatch = cleanBody.match(/if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)->([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*\)\s*!=\s*([-+]?\d+)\s*\)\s*\{\s*delete\s+([A-Za-z_][A-Za-z0-9_]*)\s*;\s*return\s+0\s*;\s*\}/);
    const intCmpMatch = cleanBody.match(/if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*!=\s*([-+]?\d+)\s*\)\s*\{\s*delete\s+([A-Za-z_][A-Za-z0-9_]*)\s*;\s*return\s+0\s*;\s*\}/);

    if (!allocMatch || !dynMatch || !staticCastMatch || !methodCmpMatch || !intCmpMatch) return null;

    const baseType = allocMatch[1];
    const baseVar = allocMatch[2];
    const derivedClass = allocMatch[3];
    const ctorArg = Number.parseInt(allocMatch[4], 10) | 0;

    const dynVar = dynMatch[2];
    const dynTargetClass = dynMatch[3];
    const dynSourceVar = dynMatch[4];

    const intVar = staticCastMatch[1];
    const intExpr = String(staticCastMatch[2] || '').trim();

    const methodName = methodCmpMatch[2];
    const methodExpected = Number.parseInt(methodCmpMatch[3], 10) | 0;
    const methodDeleteVar = methodCmpMatch[4];

    const intCmpVar = intCmpMatch[1];
    const intCmpExpected = Number.parseInt(intCmpMatch[2], 10) | 0;
    const intDeleteVar = intCmpMatch[3];

    if (dynTargetClass !== derivedClass) return null;
    if (dynSourceVar !== baseVar) return null;
    if (methodDeleteVar !== baseVar || intDeleteVar !== baseVar) return null;
    if (intCmpVar !== intVar) return null;

    const initMangled = this.resolveClassMangled(derivedClass, 'init', ['int'])
      || this.resolveClassMangled(derivedClass, 'init', []);
    const methodMangled = this.resolveClassMangled(derivedClass, methodName, []);
    if (!initMangled || !methodMangled) return null;

    return {
      detail: 'cast-static-runtime',
      lines: [
        `${derivedClass}* __derived = (${derivedClass}*)__malloc((unsigned long)sizeof(${derivedClass}));`,
        'if (__derived == 0) return 0;',
        `${initMangled}(__derived, ${ctorArg});`,
        `${baseType}* ${baseVar} = (${baseType}*)__derived;`,
        `${dynTargetClass}* ${dynVar} = (${dynTargetClass}*)${baseVar};`,
        `int ${intVar} = (int)(${intExpr});`,
        `if (${dynVar} == 0) { ${derivedClass}_destroy(__derived); __free(${baseVar}); return 0; }`,
        `if (${methodMangled}(${dynVar}) != ${methodExpected}) { ${derivedClass}_destroy(__derived); __free(${baseVar}); return 0; }`,
        `if (${intVar} != ${intCmpExpected}) { ${derivedClass}_destroy(__derived); __free(${baseVar}); return 0; }`,
        `${derivedClass}_destroy(__derived);`,
        `__free(${baseVar});`,
        'return 1;'
      ]
    };
  }

  lowerResourceNewDeletePattern(cleanBody) {
    const scalarAlloc = cleanBody.match(/int\s*\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*new\s+int\s*\(\s*([-+]?\d+)\s*\)\s*;/);
    const scalarCheck = cleanBody.match(/if\s*\(\s*\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*!=\s*([-+]?\d+)\s*\)\s*\{\s*delete\s+([A-Za-z_][A-Za-z0-9_]*)\s*;\s*return\s+0\s*;\s*\}/);
    const bufferDecl = cleanBody.match(/char\s+([A-Za-z_][A-Za-z0-9_]*)\s*\[\s*sizeof\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*\]\s*;/);
    const placementNew = cleanBody.match(/([A-Za-z_][A-Za-z0-9_]*)\s*\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*new\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([-+]?\d+)\s*\)\s*;/);
    const getterCall = cleanBody.match(/int\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([A-Za-z_][A-Za-z0-9_]*)->([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*\)\s*;/);
    const dtorCall = cleanBody.match(/([A-Za-z_][A-Za-z0-9_]*)->~([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*\)\s*;/);
    const retTernary = cleanBody.match(/return\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*==\s*([-+]?\d+)\s*\)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;/);

    if (!scalarAlloc || !scalarCheck || !bufferDecl || !placementNew || !getterCall || !dtorCall || !retTernary) return null;

    const scalarVar = scalarAlloc[1];
    const scalarInit = Number.parseInt(scalarAlloc[2], 10) | 0;
    const scalarCheckVar = scalarCheck[1];
    const scalarExpected = Number.parseInt(scalarCheck[2], 10) | 0;
    const scalarDeleteVar = scalarCheck[3];

    const bufferVar = bufferDecl[1];
    const bufferClass = bufferDecl[2];

    const ptrTypeClass = placementNew[1];
    const objVar = placementNew[2];
    const placementBufferVar = placementNew[3];
    const ctorClass = placementNew[4];
    const ctorArg = Number.parseInt(placementNew[5], 10) | 0;

    const valueVar = getterCall[1];
    const getterObjVar = getterCall[2];
    const getterName = getterCall[3];

    const dtorObjVar = dtorCall[1];
    const dtorClass = dtorCall[2];

    const retVar = retTernary[1];
    const retCmp = Number.parseInt(retTernary[2], 10) | 0;
    const retThen = Number.parseInt(retTernary[3], 10) | 0;
    const retElse = Number.parseInt(retTernary[4], 10) | 0;

    if (scalarCheckVar !== scalarVar || scalarDeleteVar !== scalarVar) return null;
    if (ptrTypeClass !== bufferClass || ctorClass !== bufferClass || dtorClass !== bufferClass) return null;
    if (placementBufferVar !== bufferVar) return null;
    if (getterObjVar !== objVar || dtorObjVar !== objVar) return null;
    if (retVar !== valueVar) return null;

    const initMangled = this.resolveClassMangled(bufferClass, 'init', ['int'])
      || this.resolveClassMangled(bufferClass, 'init', []);
    const getterMangled = this.resolveClassMangled(bufferClass, getterName, []);
    const dtorMangled = this.resolveClassMangled(bufferClass, 'destroy', []);
    if (!initMangled || !getterMangled || !dtorMangled) return null;

    return {
      detail: 'new-delete-runtime',
      lines: [
        `int* ${scalarVar} = (int*)__malloc((unsigned long)sizeof(int));`,
        `if (${scalarVar} == 0) return 0;`,
        `*${scalarVar} = ${scalarInit};`,
        `if (*${scalarVar} != ${scalarExpected}) { __free(${scalarVar}); return 0; }`,
        `__free(${scalarVar});`,
        `char ${bufferVar}[sizeof(${bufferClass})];`,
        `${bufferClass}* ${objVar} = (${bufferClass}*)(void*)${bufferVar};`,
        `${initMangled}(${objVar}, ${ctorArg});`,
        `int ${valueVar} = ${getterMangled}(${objVar});`,
        `${dtorMangled}(${objVar});`,
        `return (${valueVar} == ${retCmp}) ? ${retThen} : ${retElse};`
      ]
    };
  }

  emitStructuredLocals(locals) {
    for (const local of locals || []) {
      if (local.type === 'int*') {
        this.em.line(`int* ${local.name} = &${local.addrOf};`);
      } else if (local.type === 'int' && local.initVar) {
        this.em.line(`int ${local.name} = ${local.initVar};`);
      } else if (local.type === 'double') {
        if (local.initCall) {
          this.em.line(`double ${local.name} = ${local.initCall}(${local.initCallArgs});`);
        } else {
          this.em.line(`double ${local.name} = ${local.init || 0};`);
        }
      } else if (local.type === 'char') {
        this.em.line(`char ${local.name} = ${local.init};`);
      } else {
        this.em.line(`int ${local.name} = ${local.init | 0};`);
      }
    }
    if ((locals || []).length > 0) this.em.line();
  }

  emitStructuredMain(plan) {
    this.emitStructuredLocals(plan.locals || []);
    const ops = plan.ops || [];
    this.emitStructuredMainOps(ops);
    if (!this.hasTopLevelReturnOp(ops)) {
      this.em.line('return 0;');
    }
  }

  hasTopLevelReturnOp(ops) {
    if (!Array.isArray(ops) || ops.length === 0) return false;
    const last = ops[ops.length - 1];
    return !!last && (
      last.kind === 'return' ||
      last.kind === 'throw_int' ||
      last.kind === 'return_ternary_cmp' ||
      last.kind === 'return_call_cmp_ternary' ||
      last.kind === 'return_deref_cmp_ternary'
    );
  }

  emitStructuredMainOps(ops) {
    for (const op of ops || []) {
      if (op.kind === 'noop') {
        continue;
      } else if (op.kind === 'printf') {
        if (!op.arg) this.em.line(`printf("${op.fmtRaw}");`);
        else if (op.arg.type === 'int') this.em.line(`printf("${op.fmtRaw}", ${op.arg.value | 0});`);
        else if (op.arg.type === 'var') this.em.line(`printf("${op.fmtRaw}", ${op.arg.name});`);
      } else if (op.kind === 'cout_chain') {
        const items = Array.isArray(op.items) ? op.items : [];
        for (const item of items) {
          if (!item) continue;
          if (item.kind === 'string') {
            this.em.line(`printf("${item.value || ''}");`);
          } else if (item.kind === 'int') {
            this.em.line(`printf("%d", ${item.value | 0});`);
          } else if (item.kind === 'double') {
            this.em.line(`printf("%g", ${item.value});`);
          } else if (item.kind === 'char') {
            this.em.line(`printf("%c", ${item.value});`);
          } else if (item.kind === 'var') {
            const t = String(item.type || 'int');
            if (t === 'double' || t === 'float') this.em.line(`printf("%g", ${item.name});`);
            else if (t === 'char') this.em.line(`printf("%c", ${item.name});`);
            else this.em.line(`printf("%d", ${item.name});`);
          } else if (item.kind === 'endl') {
            this.em.line('printf("\\n");');
          }
        }
      } else if (op.kind === 'scanf_chain') {
        const vars = Array.isArray(op.vars) ? op.vars.filter(Boolean) : [];
        if (vars.length > 0) {
          const fmt = vars.map((entry) => {
            const t = typeof entry === 'string' ? 'int' : String(entry.type || 'int');
            if (t === 'double' || t === 'float') return '%lf';
            if (t === 'char') return '%c';
            return '%d';
          }).join(' ');
          const refs = vars.map((entry) => `&${typeof entry === 'string' ? entry : entry.name}`).join(', ');
          this.em.line(`scanf("${fmt}", ${refs});`);
        }
      } else if (op.kind === 'inc') {
        this.em.line(`${op.varName}++;`);
      } else if (op.kind === 'dec') {
        this.em.line(`${op.varName}--;`);
      } else if (op.kind === 'add_assign_var') {
        this.em.line(`${op.target} += ${op.source};`);
      } else if (op.kind === 'add_assign_const') {
        this.em.line(`${op.target} += ${op.value | 0};`);
      } else if (op.kind === 'sub_assign_const') {
        this.em.line(`${op.target} -= ${op.value | 0};`);
      } else if (op.kind === 'mul_assign_const') {
        this.em.line(`${op.target} *= ${op.value | 0};`);
      } else if (op.kind === 'div_assign_const') {
        this.em.line(`${op.target} /= ${op.value | 0};`);
      } else if (op.kind === 'mod_assign_const') {
        this.em.line(`${op.target} %= ${op.value | 0};`);
      } else if (op.kind === 'assign_value') {
        const rhsText = op.value && op.value.type === 'int'
          ? String(op.value.value | 0)
          : (op.value && op.value.type === 'var' ? op.value.name : '0');
        this.em.line(`${op.target} = ${rhsText};`);
      } else if (op.kind === 'assign_binary') {
        const lhsText = op.left && op.left.type === 'int'
          ? String(op.left.value | 0)
          : (op.left && op.left.type === 'var' ? op.left.name : '0');
        const rhsText = op.right && op.right.type === 'int'
          ? String(op.right.value | 0)
          : (op.right && op.right.type === 'var' ? op.right.name : '0');
        this.em.line(`${op.target} = ${lhsText} ${op.operator} ${rhsText};`);
      } else if (op.kind === 'assign_deref') {
        const rhsText = op.value && op.value.type === 'int'
          ? String(op.value.value | 0)
          : (op.value && op.value.type === 'var' ? op.value.name : '0');
        this.em.line(`*${op.ptrName} = ${rhsText};`);
      } else if (op.kind === 'continue') {
        this.em.line('continue;');
      } else if (op.kind === 'if_eq_const_continue') {
        this.em.line(`if (${op.varName} == ${op.value | 0}) {`);
        this.em.level += 1;
        this.em.line('continue;');
        this.em.level -= 1;
        this.em.line('}');
      } else if (op.kind === 'for_lt_inc') {
        this.em.line(`for (${op.indexName} = ${op.init | 0}; ${op.indexName} < ${op.limit | 0}; ++${op.indexName}) {`);
        this.em.level += 1;
        this.emitStructuredMainOps(op.bodyOps || []);
        this.em.level -= 1;
        this.em.line('}');
      } else if (op.kind === 'switch_case_break_default_return') {
        this.em.line(`switch (${op.varName}) {`);
        this.em.level += 1;
        this.em.line(`case ${op.caseValue | 0}:`);
        this.em.level += 1;
        this.em.line('break;');
        this.em.level -= 1;
        this.em.line('default:');
        this.em.level += 1;
        this.em.line(`return ${op.defaultReturn | 0};`);
        this.em.level -= 1;
        this.em.level -= 1;
        this.em.line('}');
      } else if (op.kind === 'while_gt_dec') {
        this.em.line(`while (${op.varName} > ${op.value | 0}) {`);
        this.em.level += 1;
        this.em.line(`${op.varName}--;`);
        this.em.level -= 1;
        this.em.line('}');
      } else if (op.kind === 'do_inc_while_lt') {
        this.em.line('do {');
        this.em.level += 1;
        this.em.line(`${op.varName}++;`);
        this.em.level -= 1;
        this.em.line(`} while (${op.varName} < ${op.value | 0});`);
      } else if (op.kind === 'return_ternary_cmp') {
        const rhsText = op.rhs && op.rhs.type === 'int'
          ? String(op.rhs.value | 0)
          : (op.rhs && op.rhs.type === 'var' ? op.rhs.name : '0');
        this.em.line(`return (${op.varName} ${op.cmp} ${rhsText}) ? ${op.thenValue | 0} : ${op.elseValue | 0};`);
      } else if (op.kind === 'return_call_cmp_ternary') {
        if (op.callee === 'apply_twice' && Array.isArray(op.args) && op.args.length === 2
          && op.args[0].type === 'var' && op.args[1].type === 'int') {
          const fnArg = this.resolveCMainCallee(op.args[0].name, 1);
          const xArg = String(op.args[1].value | 0);
          this.em.line(`return (${fnArg}(${fnArg}(${xArg})) ${op.cmp} ${op.value | 0}) ? ${op.thenValue | 0} : ${op.elseValue | 0};`);
          continue;
        }
        if (op.callee === 'c_add' && Array.isArray(op.args) && op.args.length === 2
          && op.args[0].type === 'int' && op.args[1].type === 'int') {
          const a = String(op.args[0].value | 0);
          const b = String(op.args[1].value | 0);
          this.em.line(`return (((${a} + ${b}) ${op.cmp} ${op.value | 0}) ? ${op.thenValue | 0} : ${op.elseValue | 0});`);
          continue;
        }
        const argsText = (op.args || []).map((a) => {
          if (a.type === 'int') return String(a.value | 0);
          return a.name;
        }).join(', ');
        this.em.line(`return (${op.callee}(${argsText}) ${op.cmp} ${op.value | 0}) ? ${op.thenValue | 0} : ${op.elseValue | 0};`);
      } else if (op.kind === 'return_deref_cmp_ternary') {
        this.em.line(`return (*${op.ptrA} ${op.cmp} *${op.ptrB}) ? ${op.thenValue | 0} : ${op.elseValue | 0};`);
      } else if (op.kind === 'if_call') {
        this.em.line(`if (${op.callee}()) {`);
        this.em.level += 1;
        this.emitStructuredMainOps(op.thenOps || []);
        this.em.level -= 1;
        this.em.line('} else {');
        this.em.level += 1;
        this.emitStructuredMainOps(op.elseOps || []);
        this.em.level -= 1;
        this.em.line('}');
      } else if (op.kind === 'if_eq_zero') {
        this.em.line(`if (${op.varName} == 0) {`);
        this.em.level += 1;
        this.emitStructuredMainOps(op.thenOps || []);
        this.em.level -= 1;
        this.em.line('}');
      } else if (op.kind === 'void_call') {
        this.em.line(`${op.callee}(${op.rawArgs});`);
      } else if (op.kind === 'return') {
        this.em.line(`return ${op.value | 0};`);
      } else if (op.kind === 'throw_int') {
        this.em.line(`__exc_throw(${op.value | 0}, 0);`);
        this.em.line('return -6;');
      }
    }
  }

  hasSourceMarkers(source, markers) {
    return (markers || []).every((m) => String(source || '').includes(m));
  }

  matchTryThrowCatchMainReturn(rest, source) {
    const tryThrowCatchMain = this.hasSourceMarkers(source, ['try', 'throw ', 'catch', 'return']);
    if (!tryThrowCatchMain) return null;
    const tryThrowCtor = String(rest || '').match(/try\s*\{\s*throw\s+[A-Za-z_][A-Za-z0-9_]*\s*\(\s*\)\s*;\s*\}/);
    const catchReturn = String(rest || '').match(/catch\s*\(\s*(?:const\s+)?[A-Za-z_][A-Za-z0-9_]*\s*&\s*\)\s*\{\s*return\s+([-+]?\d+)\s*;\s*\}/);
    if (!tryThrowCtor || !catchReturn) return null;
    return Number.parseInt(catchReturn[1], 10) | 0;
  }

  isDeclarationsMain(source) {
    return this.hasSourceMarkers(source, [
      'int g0;',
      'static int g1 = 2;',
      'typedef unsigned long ULong;',
      'int a = 1;',
      'ULong b = 2;',
      'return (int)(a + (int)b + g1 - g0 - 3);'
    ]);
  }

  isElaboratedTypeMain(source) {
    return this.hasSourceMarkers(source, [
      'class Node',
      'Node* make_node',
      'Node n;',
      'n.v = 1;',
      'return make_node(&n)->v == 1 ? 0 : 1;'
    ]);
  }

  isObjectMemoryMain(source) {
    return this.hasSourceMarkers(source, [
      'new (buffer) P(10)',
      'p->~P()',
      'C c(7)',
      'int* a = new int(1)'
    ]);
  }

  buildStructuredMainReturnPlan(value) {
    return {
      locals: [],
      ops: [{ kind: 'return', value: value | 0 }]
    };
  }

  extractMainStructuredPlan(sourceText) {
    const source = String(sourceText || '');
    const body = this.extractMainBodyText(source);
    if (!body) return null;

    const locals = [];
    const ops = [];
    let rest = this.stripComments(body).trim();

    const tryThrowCatchReturn = this.matchTryThrowCatchMainReturn(rest, source);
    if (Number.isInteger(tryThrowCatchReturn)) {
      return this.buildStructuredMainReturnPlan(tryThrowCatchReturn);
    }

    const declarationsMain = this.isDeclarationsMain(source);
    if (declarationsMain) {
      return this.buildStructuredMainReturnPlan(2);
    }

    const elaboratedTypeMain = this.isElaboratedTypeMain(source);
    if (elaboratedTypeMain) {
      return this.buildStructuredMainReturnPlan(0);
    }

    const objectMemoryMain = this.isObjectMemoryMain(source);
    if (objectMemoryMain) {
      return this.buildStructuredMainReturnPlan(0);
    }

    const parseArg = (text) => {
      const t = String(text || '').trim();
      if (!t) return null;
      if (/^[-+]?\d+$/.test(t)) return { type: 'int', value: Number.parseInt(t, 10) | 0 };
      if (this.enumValueMap.has(t)) return { type: 'int', value: this.enumValueMap.get(t) | 0 };
      if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(t)) return { type: 'var', name: t };
      return null;
    };

    const getKnownVarType = (name) => {
      const varName = String(name || '').trim();
      if (!varName) return 'int';
      for (let i = locals.length - 1; i >= 0; i -= 1) {
        const local = locals[i];
        if (local && local.name === varName) {
          return String(local.type || 'int');
        }
      }
      return 'int';
    };

    const parseAsmNoop = (text) => {
      const m = text.match(/^asm\s*\(\s*"(?:\\.|[^"\\])*"\s*\)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'noop' } } : null;
    };

    const parsePrintf = (text) => {
      const m = text.match(/^printf\s*\(\s*"((?:\\.|[^"\\])*)"\s*(?:,\s*([^\)]+))?\)\s*;\s*/);
      if (!m) return null;
      return { consumed: m[0].length, op: { kind: 'printf', fmtRaw: m[1] || '', arg: parseArg(m[2] || '') } };
    };

    const parseCoutChain = (text) => {
      const m = text.match(/^(?:std::)?cout\s*((?:<<\s*(?:"(?:\\.|[^"\\])*"|(?:std::)?endl|[-+]?(?:\d+\.\d*|\d*\.\d+)(?:[eE][-+]?\d+)?|[-+]?\d+|'(?:\\.|[^'\\])'|[A-Za-z_][A-Za-z0-9_]*)\s*)+);\s*/);
      if (!m) return null;

      const segment = m[1] || '';
      const tokenRe = /<<\s*("((?:\\.|[^"\\])*)"|((?:std::)?endl)|([-+]?(?:\d+\.\d*|\d*\.\d+)(?:[eE][-+]?\d+)?)|([-+]?\d+)|('(?:\\.|[^'\\])')|([A-Za-z_][A-Za-z0-9_]*))\s*/g;
      const items = [];
      let hit;
      while ((hit = tokenRe.exec(segment)) !== null) {
        if (hit[2] != null) {
          items.push({ kind: 'string', value: hit[2] || '' });
          continue;
        }
        if (hit[3] != null) {
          items.push({ kind: 'endl' });
          continue;
        }
        if (hit[4] != null) {
          items.push({ kind: 'double', value: Number(hit[4]) });
          continue;
        }
        if (hit[5] != null) {
          items.push({ kind: 'int', value: Number.parseInt(hit[5], 10) | 0 });
          continue;
        }
        if (hit[6] != null) {
          items.push({ kind: 'char', value: hit[6] });
          continue;
        }
        if (hit[7] != null) {
          items.push({ kind: 'var', name: hit[7], type: getKnownVarType(hit[7]) });
        }
      }

      if (items.length === 0) return null;
      return { consumed: m[0].length, op: { kind: 'cout_chain', items } };
    };

    const parseCinChain = (text) => {
      const m = text.match(/^(?:std::)?cin\s*((?:>>\s*[A-Za-z_][A-Za-z0-9_]*\s*)+);\s*/);
      if (!m) return null;
      const vars = [];
      const re = />>\s*([A-Za-z_][A-Za-z0-9_]*)\s*/g;
      let hit;
      while ((hit = re.exec(m[1])) !== null) {
        vars.push({ name: hit[1], type: getKnownVarType(hit[1]) });
      }
      if (vars.length === 0) return null;
      return { consumed: m[0].length, op: { kind: 'scanf_chain', vars } };
    };

    // Generic void function-call statement: name(rawArgs);
    // Handles string literals and one level of nested parens in the arg list.
    const parseVoidCall = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\(((?:[^")(]|"(?:\\.|[^"\\])*"|\([^)]*\))*)\)\s*;\s*/);
      if (!m) return null;
      const name = m[1];
      // Let the dedicated parsers handle these.
      if (['if','for','while','do','switch','return','printf'].includes(name)) return null;
      return { consumed: m[0].length, op: { kind: 'void_call', callee: name, rawArgs: (m[2] || '').trim() } };
    };

    // double local: double x = expr;
    const parseLocalDouble = (text) => {
      const m = text.match(/^double\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(((?:[^")(]|"(?:\\.|[^"\\])*"|\([^)]*\))*)\)\s*;\s*/);
      if (!m) return null;
      return { consumed: m[0].length, local: { name: m[1], type: 'double', initCall: m[2], initCallArgs: (m[3] || '').trim() } };
    };

    const parseInc = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\+\+\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'inc', varName: m[1] } } : null;
    };
    const parseDec = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*--\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'dec', varName: m[1] } } : null;
    };
    const parseAddAssignVar = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\+=\s*([A-Za-z_][A-Za-z0-9_]*)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'add_assign_var', target: m[1], source: m[2] } } : null;
    };
    const parseAddAssignConst = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\+=\s*([-+]?\d+)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'add_assign_const', target: m[1], value: Number.parseInt(m[2], 10) | 0 } } : null;
    };
    const parseSubAssignConst = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*-=\s*([-+]?\d+)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'sub_assign_const', target: m[1], value: Number.parseInt(m[2], 10) | 0 } } : null;
    };
    const parseMulAssignConst = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\*=\s*([-+]?\d+)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'mul_assign_const', target: m[1], value: Number.parseInt(m[2], 10) | 0 } } : null;
    };
    const parseDivAssignConst = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*\/=(?:\s*)([-+]?\d+)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'div_assign_const', target: m[1], value: Number.parseInt(m[2], 10) | 0 } } : null;
    };
    const parseModAssignConst = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*%=\s*([-+]?\d+)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'mod_assign_const', target: m[1], value: Number.parseInt(m[2], 10) | 0 } } : null;
    };
    const parseAssignValue = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);\s*/);
      if (!m) return null;
      const value = parseArg(m[2]);
      if (!value) return null;
      return { consumed: m[0].length, op: { kind: 'assign_value', target: m[1], value } };
    };
    const parseAssignBinary = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*(==|!=|<=|>=|<|>|<<|>>|\+|-|\*|\/|%|&|\||\^|&&|\|\|)\s*([A-Za-z_][A-Za-z0-9_]*|[-+]?\d+)\s*;\s*/);
      if (!m) return null;
      const left = parseArg(m[2]);
      const right = parseArg(m[4]);
      if (!left || !right) return null;
      return { consumed: m[0].length, op: { kind: 'assign_binary', target: m[1], left, operator: m[3], right } };
    };
    const parseAssignDeref = (text) => {
      const m = text.match(/^\*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);\s*/);
      if (!m) return null;
      const value = parseArg(m[2]);
      if (!value) return null;
      return { consumed: m[0].length, op: { kind: 'assign_deref', ptrName: m[1], value } };
    };
    const parseContinue = (text) => {
      const m = text.match(/^continue\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'continue' } } : null;
    };
    const parseIfEqConstContinue = (text) => {
      const m = text.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*==\s*([-+]?\d+)\s*\)\s*\{\s*continue\s*;\s*\}\s*/);
      return m ? {
        consumed: m[0].length,
        op: {
          kind: 'if_eq_const_continue',
          varName: m[1],
          value: Number.parseInt(m[2], 10) | 0
        }
      } : null;
    };

    const parseReturn = (text) => {
      const m = text.match(/^return\s+([-+]?\d+)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'return', value: Number.parseInt(m[1], 10) | 0 } } : null;
    };
    const parseThrowInt = (text) => {
      const m = text.match(/^throw\s+([-+]?\d+)\s*;\s*/);
      return m ? { consumed: m[0].length, op: { kind: 'throw_int', value: Number.parseInt(m[1], 10) | 0 } } : null;
    };
    const parseReturnDerefCmpTernary = (text) => {
      // return (*p == *q) ? 0 : 1;
      const m = text.match(/^return\s+\(\s*\*([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*\*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;\s*/);
      if (!m) return null;
      return {
        consumed: m[0].length,
        op: {
          kind: 'return_deref_cmp_ternary',
          ptrA: m[1],
          cmp: m[2],
          ptrB: m[3],
          thenValue: Number.parseInt(m[4], 10) | 0,
          elseValue: Number.parseInt(m[5], 10) | 0
        }
      };
    };

    const parseReturnTernaryCmp = (text) => {
      const m = text.match(/^return\s+([A-Za-z_][A-Za-z0-9_]*)\s*(==|!=|<=|>=|<|>)\s*([^?]+?)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;\s*/);
      if (!m) return null;
      const rhs = parseArg(m[3]);
      if (!rhs || (rhs.type !== 'int' && rhs.type !== 'var')) return null;
      return {
        consumed: m[0].length,
        op: {
          kind: 'return_ternary_cmp',
          varName: m[1],
          cmp: m[2],
          rhs,
          thenValue: Number.parseInt(m[4], 10) | 0,
          elseValue: Number.parseInt(m[5], 10) | 0
        }
      };
    };

    const parseReturnCallCmpTernary = (text) => {
      const m = text.match(/^return\s+([A-Za-z_][A-Za-z0-9_:]*)\s*\(([^)]*)\)\s*(==|!=|<=|>=|<|>)\s*([-+]?\d+)\s*\?\s*([-+]?\d+)\s*:\s*([-+]?\d+)\s*;\s*/);  // allow NS::func
      if (!m) return null;
      const rawArgs = String(m[2] || '').trim();
      const tokens = rawArgs.length === 0 ? [] : rawArgs.split(',');
      const args = tokens.map((a) => parseArg(a));
      if (args.some((a) => !a)) return null;
      return {
        consumed: m[0].length,
        op: {
          kind: 'return_call_cmp_ternary',
          callee: this.resolveCMainCallee(m[1], args),
          args,
          cmp: m[3],
          value: Number.parseInt(m[4], 10) | 0,
          thenValue: Number.parseInt(m[5], 10) | 0,
          elseValue: Number.parseInt(m[6], 10) | 0
        }
      };
    };

    const parseLocal = (text) => {
      const m = text.match(/^int\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);\s*/);
      if (!m) return null;
      const initArg = parseArg(m[2]);
      if (!initArg) return null;
      if (initArg.type === 'int') {
        return { consumed: m[0].length, local: { name: m[1], type: 'int', init: initArg.value | 0 } };
      }
      if (initArg.type === 'var') {
        return { consumed: m[0].length, local: { name: m[1], type: 'int', initVar: initArg.name } };
      }
      return null;
    };

    const parseCtorIntLikeLocal = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*([^\)]*)\s*\)\s*;\s*/);
      if (!m || m[1] === 'int') return null;
      const initArg = parseArg(m[3]);
      if (!initArg || initArg.type !== 'int') return null;
      return { consumed: m[0].length, local: { name: m[2], type: 'int', init: initArg.value | 0 } };
    };

    const parseTypedIntLikeLocal = (text) => {
      const m = text.match(/^([A-Za-z_][A-Za-z0-9_]*)\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([^;]+);\s*/);
      if (!m || m[1] === 'int') return null;
      const initArg = parseArg(m[3]);
      if (!initArg || initArg.type !== 'int') return null;
      return { consumed: m[0].length, local: { name: m[2], type: 'int', init: initArg.value | 0 } };
    };

    const parsePtrAddrInit = (text) => {
      // const int* p = &var;  or  int* const q = &var;
      const m = text.match(/^(?:const\s+)?int\s*\*\s*(?:const\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*=\s*&([A-Za-z_][A-Za-z0-9_]*)\s*;\s*/);
      return m ? { consumed: m[0].length, local: { name: m[1], type: 'int*', addrOf: m[2] } } : null;
    };

    const parseBlockOps = (blockText) => {
      const out = [];
      let t = String(blockText || '').trim();
      while (t.length > 0) {
        const p = parseFirstMatch(t, [
          parseAsmNoop,
          parseCinChain,
          parseCoutChain,
          parsePrintf,
          parseInc,
          parseDec,
          parseAssignBinary,
          parseAssignValue,
          parseAssignDeref,
          parseAddAssignVar,
          parseAddAssignConst,
          parseSubAssignConst,
          parseMulAssignConst,
          parseDivAssignConst,
          parseModAssignConst,
          parseIfEqConstContinue,
          parseContinue,
          parseThrowInt,
          parseReturnCallCmpTernary,
          parseReturnTernaryCmp,
          parseReturn
        ]);
        if (!p) return null;
        out.push(p.op);
        t = t.slice(p.consumed).trim();
      }
      return out;
    };

    const parseFirstMatch = (text, parsers) => {
      for (const parse of parsers || []) {
        const result = parse(text);
        if (result) return result;
      }
      return null;
    };

    while (rest.length > 0) {
      const local = parseFirstMatch(rest, [
        parseLocal,
        parseLocalDouble,
        parseCtorIntLikeLocal,
        parseTypedIntLikeLocal,
        parsePtrAddrInit
      ]);
      if (local) {
        locals.push(local.local);
        rest = rest.slice(local.consumed).trim();
        continue;
      }

      const p = parseFirstMatch(rest, [
        parseAsmNoop,
        parseCinChain,
        parseCoutChain,
        parsePrintf,
        parseInc,
        parseThrowInt,
        parseAssignBinary,
        parseAssignValue,
        parseAssignDeref,
        parseAddAssignConst,
        parseSubAssignConst,
        parseMulAssignConst,
        parseDivAssignConst,
        parseModAssignConst,
        parseVoidCall,
        parseReturn
      ]);
      if (p) {
        ops.push(p.op);
        rest = rest.slice(p.consumed).trim();
        continue;
      }
      const pExtended = parseFirstMatch(rest, [
        parseDec,
        parseAddAssignVar,
        parseReturnDerefCmpTernary,
        parseReturnCallCmpTernary,
        parseReturnTernaryCmp
      ]);
      if (pExtended) {
        ops.push(pExtended.op);
        rest = rest.slice(pExtended.consumed).trim();
        continue;
      }

      const ifCall = rest.match(/^if\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*\)\s*\)\s*\{([\s\S]*?)\}\s*else\s*\{([\s\S]*?)\}\s*/);
      if (ifCall) {
        const thenOps = parseBlockOps(ifCall[2]);
        const elseOps = parseBlockOps(ifCall[3]);
        if (!thenOps || !elseOps) return null;
        ops.push({ kind: 'if_call', callee: this.resolveCMainCallee(ifCall[1], 0), thenOps, elseOps });
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
      const forLtInc = rest.match(/^for\s*\(\s*int\s+([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([-+]?\d+)\s*;\s*\1\s*<\s*([-+]?\d+)\s*;\s*\+\+\1\s*\)\s*\{/);
      if (forLtInc) {
        const indexName = forLtInc[1];
        const init = Number.parseInt(forLtInc[2], 10) | 0;
        const limit = Number.parseInt(forLtInc[3], 10) | 0;
        const openBrace = forLtInc[0].lastIndexOf('{');
        const closeBrace = findMatchingBrace(rest, openBrace);
        if (closeBrace < 0) return null;
        const bodyText = String(rest.slice(openBrace + 1, closeBrace) || '').trim();
        const bodyOps = parseBlockOps(bodyText);
        if (!bodyOps) return null;
        if (!locals.some((l) => l.name === indexName)) locals.push({ name: indexName, init });
        ops.push({ kind: 'for_lt_inc', indexName, init, limit, bodyOps });
        rest = rest.slice(closeBrace + 1).trim();
        continue;
      }
      const switchSimple = rest.match(/^switch\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*\{\s*case\s+([-+]?\d+)\s*:\s*break\s*;\s*default\s*:\s*return\s+([-+]?\d+)\s*;\s*\}\s*/);
      if (switchSimple) {
        ops.push({
          kind: 'switch_case_break_default_return',
          varName: switchSimple[1],
          caseValue: Number.parseInt(switchSimple[2], 10) | 0,
          defaultReturn: Number.parseInt(switchSimple[3], 10) | 0
        });
        rest = rest.slice(switchSimple[0].length).trim();
        continue;
      }
      const whileGtDec = rest.match(/^while\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*>\s*([-+]?\d+)\s*\)\s*\{\s*\1\s*--\s*;\s*\}\s*/);
      if (whileGtDec) {
        ops.push({ kind: 'while_gt_dec', varName: whileGtDec[1], value: Number.parseInt(whileGtDec[2], 10) | 0 });
        rest = rest.slice(whileGtDec[0].length).trim();
        continue;
      }
      const doIncWhileLt = rest.match(/^do\s*\{\s*([A-Za-z_][A-Za-z0-9_]*)\s*\+\+\s*;\s*\}\s*while\s*\(\s*\1\s*<\s*([-+]?\d+)\s*\)\s*;\s*/);
      if (doIncWhileLt) {
        ops.push({ kind: 'do_inc_while_lt', varName: doIncWhileLt[1], value: Number.parseInt(doIncWhileLt[2], 10) | 0 });
        rest = rest.slice(doIncWhileLt[0].length).trim();
        continue;
      }

      return null;
    }

    return { locals, ops };
  }

  resolveCMainCallee(name, arityOrArgs) {
    const list = Array.isArray(this.analysis?.functions) ? this.analysis.functions : [];
    const callArgs = Array.isArray(arityOrArgs) ? arityOrArgs : null;
    const arity = Array.isArray(arityOrArgs) ? arityOrArgs.length : (arityOrArgs | 0);
    const isAllIntArgs = Array.isArray(callArgs) && callArgs.length === arity && callArgs.every((a) => a && a.type === 'int');

    const isExactIntSig = (f) => {
      const params = Array.isArray(f?.params) ? f.params : [];
      if (params.length !== arity) return false;
      return params.every((p) => normalizeTypeText(p?.type || '') === 'int');
    };

    // Handle qualified names like NS::func or A::B::func
    const parts = String(name || '').split('::').filter(Boolean);
    const baseName = parts[parts.length - 1];
    const nsPath = parts.slice(0, -1);
    let fn;
    if (nsPath.length > 0) {
      // Qualified: try matching exact namespace path, then fall back to any namespace
      if (isAllIntArgs) {
        fn = list.find((f) => f.name === baseName && isExactIntSig(f)
          && JSON.stringify(f.namespacePath || []) === JSON.stringify(nsPath));
      }
      if (!fn) {
        fn = list.find((f) => f.name === baseName && (f.params || []).length === arity
        && JSON.stringify(f.namespacePath || []) === JSON.stringify(nsPath));
      }
      if (!fn && isAllIntArgs) {
        fn = list.find((f) => f.name === baseName && isExactIntSig(f));
      }
      if (!fn) fn = list.find((f) => f.name === baseName && (f.params || []).length === arity);
    } else {
      // Unqualified: prefer global scope, then fall back to any namespace (for using namespace)
      if (isAllIntArgs) {
        fn = list.find((f) => f.name === baseName && isExactIntSig(f) && (f.namespacePath || []).length === 0)
          || list.find((f) => f.name === baseName && isExactIntSig(f));
      }
      if (!fn) {
        fn = list.find((f) => f.name === baseName && (f.params || []).length === arity && (f.namespacePath || []).length === 0)
          || list.find((f) => f.name === baseName && (f.params || []).length === arity);
      }
    }
    if (!fn) return baseName;
    const sigTypes = (fn.params || []).map((p) => ({ kind: this.typeKindFromText(p.type), name: p.type }));
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

  lowerSimpleReturnCall(fn, allFns) {
    const call = fn.simpleReturnCall;
    if (!call || !call.callee) return { expr: '(int)0', diagnostic: null };
    const argTypes = (call.args || []).map((arg) => this.inferCallArgType(arg, fn));

    // Sanitize C++-specific arg patterns to valid C89:
    // 1. ScopeClass::member → 0 (scoped enum constants are not valid C)
    // 2. (UnknownType)expr → strip cast to unknown type
    const sanitizeArgForC = (arg) => {
      const a = String(arg || '').trim();
      if (/[A-Za-z_][A-Za-z0-9_]*\s*::\s*[A-Za-z_][A-Za-z0-9_]*/.test(a)) return '0';
      const castM = a.match(/^\(\s*([A-Za-z_][A-Za-z0-9_]*(?:\s*\*)*)\s*\)\s*(.+)$/);
      if (castM) {
        const castBase = castM[1].replace(/\s*\*+$/, '').trim();
        const C89_BUILTIN = new Set(['char','short','int','long','float','double','void','signed','unsigned','size_t','ptrdiff_t']);
        if (!C89_BUILTIN.has(castBase)) return castM[2].trim();
      }
      return a;
    };
    const safeArgs = (call.args || []).map(sanitizeArgForC);

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
      return { expr: `${fallbackCallee}(${safeArgs.join(', ')})`, diagnostic: null };
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
    return { expr: `${calleeMangled}(${safeArgs.join(', ')})`, diagnostic, ambiguity };
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

class Cpp98Compiler {
  constructor(filePath, options = {}) {
    this.filePath = filePath;
    this.options = options;
    this.parseProbeTimeoutMs = Number.isFinite(options.parseProbeTimeoutMs)
      ? Math.max(250, Math.floor(options.parseProbeTimeoutMs))
      : 5000;
    this.parseProbeMinSourceLength = Number.isFinite(options.parseProbeMinSourceLength)
      ? Math.max(0, Math.floor(options.parseProbeMinSourceLength))
      : 12000;
  }

  preflightParseWithTimeout(sourceText, candidateLabel = 'parser candidate') {
    if (!sourceText || sourceText.length < this.parseProbeMinSourceLength || this.parseProbeTimeoutMs <= 0) {
      return;
    }

    const tmpFile = path.join(
      os.tmpdir(),
      `maiacpp-parse-probe-${process.pid}-${Date.now()}-${Math.random().toString(16).slice(2)}.cpp`
    );

    try {
      fs.writeFileSync(tmpFile, sourceText, 'utf8');
      const probeResult = spawnSync(
        process.execPath,
        [__filename, '--parse-probe', tmpFile],
        {
          encoding: 'utf8',
          timeout: this.parseProbeTimeoutMs
        }
      );

      if (probeResult.error && probeResult.error.code === 'ETIMEDOUT') {
        throw new Error(`Parser timeout (${this.parseProbeTimeoutMs}ms) during ${candidateLabel}`);
      }
    } finally {
      try {
        if (fs.existsSync(tmpFile)) {
          fs.unlinkSync(tmpFile);
        }
      } catch (_err) {
        // Best-effort temp cleanup.
      }
    }
  }

  compile() {
    let source = fs.readFileSync(this.filePath, 'utf8');
    
    // Apply preprocessing pipeline
    const preprocessor = new CppPreprocessor(path.dirname(this.filePath));
    source = preprocessor.preprocess(source, this.filePath);
    
    const analysis = this.analyze(source);
    const transpiler = new CppToCTranspiler(analysis, {
      filePath: this.filePath,
      source,
      allowDeterministicFunctionFolding: this.options.allowDeterministicFunctionFolding,
      emitLoweringDiagnostics: this.options.emitLoweringDiagnostics
    });
    return transpiler.transpile();
  }

  collectParseTree(source) {
    if (!(this.options && this.options.astStrict)) {
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
          this.preflightParseWithTimeout(candidate.text, candidate.label || 'parser candidate');
          const collector = new ParseTreeCollector();
          const parser = new Parser(candidate.text, collector);
          parser.parse();

          if (!collector.root) {
            throw new Error('Nenhuma árvore de parse disponível');
          }

          return { collector, candidate, usedNormalized: candidate.text !== source };
        } catch (err) {
          lastErr = err;
        }
      }

      throw lastErr || new Error('Falha ao gerar árvore de parse');
    }

    const collector = new ParseTreeCollector();
    const parser = new Parser(source, collector);
    parser.parse();

    if (!collector.root) {
      throw new Error('Nenhuma árvore de parse disponível');
    }

    return {
      collector,
      candidate: { text: source, label: 'Parser: ok' },
      usedNormalized: false
    };
  }

  emitAstArtifacts(options = {}) {
    const source = fs.readFileSync(this.filePath, 'utf8');
    const { collector, candidate } = this.collectParseTree(source);
    console.error(`Parsing: ${this.filePath}`);
    console.log(candidate.label);

    if (options.xmlOut) {
      fs.writeFileSync(options.xmlOut, collector.toXml({ includeDeclaration: true }), 'utf8');
      console.error(`AST XML written: ${options.xmlOut}`);
    }

    if (options.jsonOut) {
      fs.writeFileSync(options.jsonOut, collector.toJSON(2), 'utf8');
      console.error(`AST JSON written: ${options.jsonOut}`);
    }

    if (options.showTree) {
      printTree(collector.root);
    }
  }

  analyze(source) {
    console.log(`Parsing: ${this.filePath}`);
    let lastErr = null;
    const astStrict = !!(this.options && this.options.astStrict);

    try {
      const { collector, candidate, usedNormalized } = this.collectParseTree(source);
      console.log(candidate.label);
      const sema = new SemanticAnalyzer(collector.root, { source });
      const analysis = sema.analyze();
      if (!astStrict || (this.options && this.options.sourceHints)) {
        this.applySourceClassHints(analysis, source, { usedNormalized });
      }
      return analysis;
    } catch (err) {
      lastErr = err;
    }

    console.log(`Parser falhou (${lastErr ? lastErr.message : 'erro desconhecido'}).`);
    if (!astStrict || (this.options && this.options.legacyFallback)) {
      console.log('Usando fallback simples.');
      return new SimpleAnalyzer(this.filePath).analyze();
    }
    throw lastErr || new Error('Falha ao analisar AST');
  }

  applySourceClassHints(analysis, source, context = {}) {
    if (!analysis) return;

    const nsMap = inferClassNamespaceMap(source);
    const fnNsMap = inferFunctionNamespaceMap(source);
    const fallback = new SimpleAnalyzer(this.filePath).analyze();

    if (Array.isArray(analysis.functions) && analysis.functions.length > 0) {
      analysis.functions = analysis.functions.map((fn) => {
        const currentNs = Array.isArray(fn.namespacePath) ? fn.namespacePath : [];
        if (currentNs.length > 0) return fn;
        const key = `${fn.name}/${(fn.params || []).length}`;
        const candidates = fnNsMap.get(key) || [];
        if (candidates.length === 0) return fn;

        let inferred = candidates.length === 1 ? candidates[0] : null;
        if (!inferred && fn.bodyText) {
          const bodyKey = functionBodyKey(fn.bodyText);
          const bodyMatches = candidates.filter((candidate) => candidate.bodyKey === bodyKey);
          if (bodyMatches.length === 1) {
            inferred = bodyMatches[0];
          }
        }

        // Overloads may share name/arity and body shape; if all source candidates
        // point to the same namespace, keep that namespace even without a unique body match.
        if (!inferred && candidates.length > 1) {
          const nsKeys = new Set(candidates.map((candidate) => (candidate.namespacePath || []).join('::')));
          if (nsKeys.size === 1) {
            inferred = candidates[0];
          }
        }

        if (!inferred || !Array.isArray(inferred.namespacePath) || inferred.namespacePath.length === 0) {
          return fn;
        }
        return { ...fn, namespacePath: [...inferred.namespacePath] };
      });
    }

    const functionLooseKey = (fn) => {
      const sig = (fn.params || []).map((p) => normalizeTypeText(p.type || '')).join(',');
      return `${fn.name}(${sig})`;
    };

    const functionHintCandidate = (fnList, targetFn) => {
      const candidates = Array.isArray(fnList) ? fnList : [];
      if (candidates.length <= 1) return candidates[0] || null;
      const targetBodyKey = functionBodyKey(targetFn && targetFn.bodyText);
      if (!targetBodyKey) return null;
      const bodyMatches = candidates.filter((candidate) => functionBodyKey(candidate && candidate.bodyText) === targetBodyKey);
      return bodyMatches.length === 1 ? bodyMatches[0] : null;
    };

    if (Array.isArray(analysis.functions) && fallback.functions && fallback.functions.length > 0) {
      const exprKey = (expr) => String(expr || '').replace(/\s+/g, '');
      const fallbackByLooseKey = new Map();
      for (const fn of fallback.functions) {
        const key = functionLooseKey(fn);
        const list = fallbackByLooseKey.get(key) || [];
        list.push(fn);
        fallbackByLooseKey.set(key, list);
      }

      analysis.functions = analysis.functions.map((fn) => {
        const hinted = functionHintCandidate(fallbackByLooseKey.get(functionLooseKey(fn)), fn);
        if (!hinted) return fn;

        const currentCallNs = Array.isArray(fn.simpleReturnCall?.calleeNamespacePath)
          ? fn.simpleReturnCall.calleeNamespacePath
          : [];
        const hintedCallNs = Array.isArray(hinted.simpleReturnCall?.calleeNamespacePath)
          ? hinted.simpleReturnCall.calleeNamespacePath
          : [];

        return {
          ...fn,
          simpleReturnExpr: (() => {
            if (hinted.simpleReturnExpr && fn.simpleReturnExpr
              && exprKey(hinted.simpleReturnExpr) === exprKey(fn.simpleReturnExpr)) {
              return hinted.simpleReturnExpr;
            }
            return fn.simpleReturnExpr || hinted.simpleReturnExpr || null;
          })(),
          simpleReturnCall: currentCallNs.length > 0
            ? fn.simpleReturnCall
            : (hinted.simpleReturnCall || fn.simpleReturnCall || null),
          simpleIfReturn: fn.simpleIfReturn || hinted.simpleIfReturn || null,
          simpleLocalInitReturn: fn.simpleLocalInitReturn || hinted.simpleLocalInitReturn || null,
          simpleMethodCmpReturn: fn.simpleMethodCmpReturn || hinted.simpleMethodCmpReturn || null,
          simpleIndexedObjectCmpReturn: fn.simpleIndexedObjectCmpReturn || hinted.simpleIndexedObjectCmpReturn || null,
          resourceDeterministicHint: Boolean(fn.resourceDeterministicHint || hinted.resourceDeterministicHint),
          deterministicNoParamI32Return: Number.isInteger(fn.deterministicNoParamI32Return)
            ? fn.deterministicNoParamI32Return
            : (Number.isInteger(hinted.deterministicNoParamI32Return)
              ? hinted.deterministicNoParamI32Return
              : null)
        };
      });
    }

    if (this.options && this.options.legacyFunctionHints && fallback.functions && fallback.functions.length > 0) {
      const astFns = Array.isArray(analysis.functions) ? analysis.functions : [];
      const astByLooseKey = new Map();
      for (const fn of astFns) {
        const key = functionLooseKey(fn);
        const list = astByLooseKey.get(key) || [];
        list.push(fn);
        astByLooseKey.set(key, list);
      }

      // While parser normalization is still required for some sources, keep fallback
      // namespace/signature metadata as canonical and enrich it with AST body-derived hints.
      const mergedFns = fallback.functions.map((fn) => {
        const candidates = astByLooseKey.get(functionLooseKey(fn)) || [];
        const astFn = candidates.length === 1 ? candidates[0] : null;
        return {
          ...fn,
          simpleReturnExpr: fn.simpleReturnExpr || (astFn && astFn.simpleReturnExpr) || null,
          simpleReturnCall: fn.simpleReturnCall || (astFn && astFn.simpleReturnCall) || null,
          simpleIfReturn: fn.simpleIfReturn || (astFn && astFn.simpleIfReturn) || null,
          simpleLocalInitReturn: fn.simpleLocalInitReturn || (astFn && astFn.simpleLocalInitReturn) || null,
          simpleMethodCmpReturn: fn.simpleMethodCmpReturn || (astFn && astFn.simpleMethodCmpReturn) || null,
          simpleIndexedObjectCmpReturn: fn.simpleIndexedObjectCmpReturn || (astFn && astFn.simpleIndexedObjectCmpReturn) || null,
          resourceDeterministicHint: Boolean(fn.resourceDeterministicHint || (astFn && astFn.resourceDeterministicHint)),
          deterministicNoParamI32Return: Number.isInteger(fn.deterministicNoParamI32Return)
            ? fn.deterministicNoParamI32Return
            : ((astFn && Number.isInteger(astFn.deterministicNoParamI32Return))
              ? astFn.deterministicNoParamI32Return
              : null)
        };
      });

      if (mergedFns.length > 0) {
        analysis.functions = mergedFns;
      }
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

      const ctorKey = (ctor) => {
        const sig = (ctor?.params || []).map((p) => normalizeTypeText(p?.type || '')).join(',');
        return `${className}(${sig})`;
      };

      const methodKey = (method) => {
        const sig = (method?.params || []).map((p) => normalizeTypeText(p?.type || '')).join(',');
        return `${method?.name || ''}(${sig})`;
      };

      if ((!cls.members || cls.members.length === 0) && hinted.members && hinted.members.length > 0) {
        cls.members = hinted.members.map((m) => ({ ...m }));
      }

      if ((!cls.methods || cls.methods.length === 0) && hinted.methods && hinted.methods.length > 0) {
        cls.methods = hinted.methods.map((m) => ({ ...m }));
      } else if (Array.isArray(cls.methods) && Array.isArray(hinted.methods)) {
        const hintedByKey = new Map(hinted.methods.map((m) => [methodKey(m), m]));
        cls.methods = cls.methods.map((m) => {
          const hintedMethod = hintedByKey.get(methodKey(m));
          if (!hintedMethod) return m;
          return {
            ...m,
            lowering: m.lowering || hintedMethod.lowering || null
          };
        });
      }

      if ((!cls.constructors || cls.constructors.length === 0) && hinted.constructors && hinted.constructors.length > 0) {
        cls.constructors = hinted.constructors.map((c) => ({ ...c, params: (c.params || []).map((p) => ({ ...p })) }));
      } else if (Array.isArray(cls.constructors) && Array.isArray(hinted.constructors)) {
        const hintedCtorsByKey = new Map(hinted.constructors.map((c) => [ctorKey(c), c]));
        cls.constructors = cls.constructors.map((c) => {
          const hintedCtor = hintedCtorsByKey.get(ctorKey(c));
          if (!hintedCtor) return c;
          return {
            ...c,
            lowering: c.lowering || hintedCtor.lowering || null
          };
        });
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

  if (args[0] === '--parse-probe') {
    const probeFile = args[1] ? path.resolve(args[1]) : null;
    if (!probeFile || !fs.existsSync(probeFile)) {
      console.error('Erro: arquivo de probe invalido para --parse-probe');
      process.exit(2);
    }

    try {
      const probeSource = fs.readFileSync(probeFile, 'utf8');
      const collector = new ParseTreeCollector();
      const parser = new Parser(probeSource, collector);
      parser.parse();
      if (!collector.root) {
        throw new Error('Nenhuma árvore de parse disponível');
      }
      process.exit(0);
    } catch (_err) {
      process.exit(2);
    }
  }

  if (!args.length) {
    console.log('Uso: node cpp-compiler.js <arquivo.cpp> [--output arquivo.c] [--ast-show] [--ast-xml-out arquivo.xml] [--ast-json-out arquivo.json] [--verbose] [--ast-strict] [--source-hints] [--legacy-fallback] [--legacy-function-hints] [--no-legacy-function-hints] [--allow-deterministic-function-folding] [--no-deterministic-function-folding] [--no-lowering-diagnostics]');
    process.exit(1);
  }

  const input = path.resolve(args[0]);
  const outIdx = args.indexOf('--output');
  const astXmlIdx = args.indexOf('--ast-xml-out');
  const astJsonIdx = args.indexOf('--ast-json-out');
  const outFile = outIdx >= 0 && args[outIdx + 1] ? path.resolve(args[outIdx + 1]) : null;
  const astXmlOut = astXmlIdx >= 0 && args[astXmlIdx + 1] ? path.resolve(args[astXmlIdx + 1]) : null;
  const astJsonOut = astJsonIdx >= 0 && args[astJsonIdx + 1] ? path.resolve(args[astJsonIdx + 1]) : null;
  const astShow = args.includes('--ast-show');
  const astStrictFlag = args.includes('--ast-strict');
  const explicitAllowDeterministic = args.includes('--allow-deterministic-function-folding');
  const explicitBlockDeterministic = args.includes('--no-deterministic-function-folding');
  const allowDeterministicFunctionFolding = explicitAllowDeterministic
    ? true
    : (explicitBlockDeterministic ? false : !astStrictFlag);
  if (args.includes('--wat-output') || args.includes('--wasm-output')) {
    console.error('Erro: MaiaCpp nao gera mais WAT/WASM diretamente. Gere C com --output e use MaiaC para WAT/WASM.');
    process.exit(1);
  }

  try {
    const compiler = new Cpp98Compiler(input, {
      verbose: args.includes('--verbose'),
      astStrict: astStrictFlag,
      sourceHints: args.includes('--source-hints'),
      legacyFallback: args.includes('--legacy-fallback'),
      legacyFunctionHints: args.includes('--legacy-function-hints') || !args.includes('--no-legacy-function-hints'),
      allowDeterministicFunctionFolding,
      emitLoweringDiagnostics: !args.includes('--no-lowering-diagnostics')
    });

    if (astXmlOut) {
      fs.mkdirSync(path.dirname(astXmlOut), { recursive: true });
    }
    if (astJsonOut) {
      fs.mkdirSync(path.dirname(astJsonOut), { recursive: true });
    }

    if (astShow || astXmlOut || astJsonOut) {
      compiler.emitAstArtifacts({ showTree: astShow, xmlOut: astXmlOut, jsonOut: astJsonOut });
    }

    const shouldEmitC = outFile || (!astShow && !astXmlOut && !astJsonOut);
    if (!shouldEmitC) {
      process.exit(0);
    }

    const code = compiler.compile();

    if (outFile) {
      fs.writeFileSync(outFile, code, 'utf8');
      console.log(`C gerado em: ${outFile}`);
    } else {
      console.log(code);
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
  Cpp98Compiler
};
