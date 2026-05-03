/**
 * CppPreprocessor - C++ preprocessing pipeline
 *
 * Handles:
 * - #include directives (local and system includes, with include guard support)
 * - #define macros: object-like and function-like (with multi-line backslash continuation)
 * - #undef
 * - #ifdef / #ifndef / #if defined(X) / #if 0|1 / #else / #endif conditionals (nested)
 * - Stringification (#arg) and token pasting (a##b) in macro bodies
 * - Recursive macro expansion with cycle protection
 */

const fs = require('fs');
const path = require('path');

function dedupeExistingDirs(dirs) {
  const seen = new Set();
  const out = [];
  for (const dir of dirs || []) {
    if (!dir) continue;
    const resolved = path.resolve(String(dir));
    if (seen.has(resolved)) continue;
    if (!fs.existsSync(resolved) || !fs.statSync(resolved).isDirectory()) continue;
    seen.add(resolved);
    out.push(resolved);
  }
  return out;
}

/**
 * Parse a balanced argument list starting at text[start] (which must be '(').
 * Returns { args: string[], end: number } where end is the index after ')'.
 * Handles nested parens and simple string/char literals.
 * Returns null on failure.
 */
function parseArgList(text, start) {
  if (text[start] !== '(') return null;
  let depth = 0;
  const args = [];
  let cur = '';
  let i = start;
  while (i < text.length) {
    const ch = text[i];
    if (ch === '"') {
      let j = i + 1;
      while (j < text.length) {
        if (text[j] === '\\') { j += 2; continue; }
        if (text[j] === '"') { j++; break; }
        j++;
      }
      cur += text.slice(i, j);
      i = j;
      continue;
    }
    if (ch === "'") {
      let j = i + 1;
      while (j < text.length) {
        if (text[j] === '\\') { j += 2; continue; }
        if (text[j] === "'") { j++; break; }
        j++;
      }
      cur += text.slice(i, j);
      i = j;
      continue;
    }
    if (ch === '(') {
      if (depth > 0) cur += ch;
      depth++;
    } else if (ch === ')') {
      depth--;
      if (depth === 0) {
        args.push(cur.trim());
        return { args, end: i + 1 };
      }
      cur += ch;
    } else if (ch === ',' && depth === 1) {
      args.push(cur.trim());
      cur = '';
    } else {
      cur += ch;
    }
    i++;
  }
  return null;
}

class CppPreprocessor {
  constructor(basePath = '.', options = {}) {
    this.basePath = basePath;
    /**
     * Map of macro name → { kind: 'object'|'function', params: string[]|null, body: string }
     */
    this.defines = new Map();
    this.includes = new Map();
    this.includeDirs = this.buildIncludeDirs(options.includeDirs || []);
  }

  buildIncludeDirs(extraDirs = []) {
    const compilerDir = __dirname;
    const repoRoot = path.resolve(compilerDir, '..');
    const projectsRoot = path.resolve(repoRoot, '..');

    // Precedence: source dir (handled in resolveIncludePath for quoted includes),
    // then MaiaCpp include, then sibling MaiaC include, then vendored MaiaC include.
    const defaults = [
      path.resolve(repoRoot, 'include'),
      path.resolve(projectsRoot, 'maiac', 'include'),
      path.resolve(repoRoot, 'maiac', 'include')
    ];

    return dedupeExistingDirs([...extraDirs, ...defaults]);
  }

  resolveIncludePath(includePath, sourceDir, isSystemInclude) {
    const candidates = [];
    if (!isSystemInclude) {
      candidates.push(path.resolve(sourceDir, includePath));
    }
    for (const dir of this.includeDirs) {
      candidates.push(path.resolve(dir, includePath));
    }
    for (const candidate of candidates) {
      if (fs.existsSync(candidate) && fs.statSync(candidate).isFile()) {
        return candidate;
      }
    }
    return null;
  }

  preprocess(source, filePath = 'input.cpp') {
    this.includes.clear();
    this.defines.clear();

    const sourceDir = path.dirname(filePath);
    return this.processLines(source, filePath, sourceDir);
  }

  /**
   * Join physical lines ending with '\' into logical lines for #define parsing.
   */
  joinContinuationLines(lines) {
    const logical = [];
    let buf = '';
    for (const line of lines) {
      if (line.endsWith('\\')) {
        buf += line.slice(0, -1) + ' ';
      } else {
        logical.push(buf + line);
        buf = '';
      }
    }
    if (buf) logical.push(buf);
    return logical;
  }

  processLines(source, filePath, sourceDir, depth = 0) {
    if (depth > 10) {
      throw new Error('Include depth exceeded (circular include?)');
    }

    const rawLines = source.split('\n');
    const lines = this.joinContinuationLines(rawLines);
    const result = [];

    // condStack[i] = boolean: whether the i-th conditional level is currently active.
    // A line is output only when ALL levels are active.
    const condStack = [];
    const isActive = () => condStack.length === 0 || condStack.every(v => v);
    const parentActive = () => condStack.length <= 1 || condStack.slice(0, -1).every(v => v);

    for (let i = 0; i < lines.length; i += 1) {
      const line = lines[i];
      const trimmed = line.trim();

      // ---- Conditional directives (always evaluated, even inside inactive blocks) ----

      if (trimmed.startsWith('#ifdef ')) {
        const macro = trimmed.slice(7).trim();
        condStack.push(isActive() ? this.defines.has(macro) : false);
        continue;
      }

      if (trimmed.startsWith('#ifndef ')) {
        const macro = trimmed.slice(8).trim();
        condStack.push(isActive() ? !this.defines.has(macro) : false);
        continue;
      }

      if (trimmed.startsWith('#if ')) {
        const condition = trimmed.slice(4).trim();
        condStack.push(isActive() ? this.evalIfCondition(condition) : false);
        continue;
      }

      if (trimmed === '#else') {
        if (condStack.length > 0 && parentActive()) {
          condStack[condStack.length - 1] = !condStack[condStack.length - 1];
        }
        continue;
      }

      if (trimmed === '#endif') {
        if (condStack.length > 0) condStack.pop();
        continue;
      }

      // Skip lines inside false conditional blocks
      if (!isActive()) continue;

      // ---- Active directives ----

      if (trimmed.startsWith('#include ')) {
        const includeMatch = trimmed.match(/#include\s*([<"])([^>"]+)[>"]/);
        if (includeMatch) {
          const includePath = includeMatch[2];
          const isSystemInclude = includeMatch[1] === '<';

          try {
            const fullPath = this.resolveIncludePath(includePath, sourceDir, isSystemInclude);
            if (fullPath) {
              if (isSystemInclude) {
                // Keep parser stability for system headers while still validating
                // header lookup against MaiaCpp/MaiaC include roots.
                result.push(`/* System include <${includePath}> resolved */`);
              } else if (!this.includes.has(fullPath)) {
                this.includes.set(fullPath, true);
                const includeSource = fs.readFileSync(fullPath, 'utf8');
                const processed = this.processLines(
                  includeSource, fullPath, path.dirname(fullPath), depth + 1
                );
                result.push(processed);
              }
            } else {
              result.push(`/* Include file not found: ${includePath} */`);
            }
          } catch (err) {
            result.push(`/* Error including ${includePath}: ${err.message} */`);
          }
        }
        continue;
      }

      if (trimmed.startsWith('#define ')) {
        this.parseDefine(trimmed.slice(8).trimStart());
        result.push(`/* Preprocessor: define processed */`);
        continue;
      }

      if (trimmed.startsWith('#undef ')) {
        const undefMatch = trimmed.match(/#undef\s+(\w+)/);
        if (undefMatch) this.defines.delete(undefMatch[1]);
        continue;
      }

      // Regular source line: expand macros
      result.push(this.expandText(line, new Set()));
    }

    return result.join('\n');
  }

  /**
   * Parse a #define directive (everything after '#define ').
   * The distinction between function-like and object-like macros in C/C++ is that
   * function-like macros have '(' IMMEDIATELY after the name (no whitespace).
   */
  parseDefine(rest) {
    const nameMatch = rest.match(/^(\w+)/);
    if (!nameMatch) return;
    const name = nameMatch[1];
    const after = rest.slice(name.length); // everything after the name

    if (after.startsWith('(')) {
      // Function-like: name immediately followed by '(' (no whitespace)
      const bodyMatch = after.match(/^\(([^)]*)\)\s*([\s\S]*)/);
      if (bodyMatch) {
        const paramsStr = bodyMatch[1].trim();
        const body = bodyMatch[2].trim();
        const params = paramsStr.length > 0 ? paramsStr.split(',').map(s => s.trim()) : [];
        this.defines.set(name, { kind: 'function', params, body });
        return;
      }
    }
    // Object-like: name followed by optional whitespace then body
    const body = after.trimStart();
    this.defines.set(name, { kind: 'object', params: null, body: body || '1' });
  }

  /**
   * Evaluate a #if condition.
   * Supports: defined(MACRO), plain identifier (truthy if defined+nonzero), numeric literal.
   */
  evalIfCondition(condition) {
    const m = condition.match(/defined\s*\(\s*(\w+)\s*\)/);
    if (m) return this.defines.has(m[1]);
    const m2 = condition.match(/^(\w+)$/);
    if (m2 && this.defines.has(m2[1])) {
      const def = this.defines.get(m2[1]);
      if (def.kind === 'object') {
        const n = Number(def.body);
        return !isNaN(n) ? n !== 0 : true;
      }
      return true;
    }
    const n = Number(condition.trim());
    if (!isNaN(n)) return n !== 0;
    return false;
  }

  /**
   * Expand all macro invocations in text, skipping strings, char literals, and comments.
   * expanding: Set of macro names currently being expanded (prevents infinite recursion).
   */
  expandText(text, expanding) {
    let result = '';
    let i = 0;
    while (i < text.length) {
      // Skip string literals
      if (text[i] === '"') {
        const end = this.skipString(text, i);
        result += text.slice(i, end);
        i = end;
        continue;
      }
      // Skip char literals
      if (text[i] === "'") {
        const end = this.skipCharLit(text, i);
        result += text.slice(i, end);
        i = end;
        continue;
      }
      // Skip // line comments
      if (text[i] === '/' && text[i + 1] === '/') {
        result += text.slice(i);
        i = text.length;
        continue;
      }
      // Skip /* */ block comments
      if (text[i] === '/' && text[i + 1] === '*') {
        const end = text.indexOf('*/', i + 2);
        if (end === -1) { result += text.slice(i); i = text.length; }
        else { result += text.slice(i, end + 2); i = end + 2; }
        continue;
      }
      // Identifier: potential macro invocation
      if (/[a-zA-Z_]/.test(text[i])) {
        let j = i;
        while (j < text.length && /\w/.test(text[j])) j++;
        const ident = text.slice(i, j);
        const def = this.defines.get(ident);
        if (def && !expanding.has(ident)) {
          if (def.kind === 'object') {
            const newExp = new Set(expanding);
            newExp.add(ident);
            result += this.expandText(def.body, newExp);
            i = j;
          } else {
            // Function-like: require argument list
            let k = j;
            while (k < text.length && text[k] === ' ') k++;
            if (k < text.length && text[k] === '(') {
              const parsed = parseArgList(text, k);
              if (parsed && parsed.args.length === def.params.length) {
                const newExp = new Set(expanding);
                newExp.add(ident);
                const rawArgs = parsed.args;
                // Pre-expand args for normal substitution (not for # or ##)
                const expandArgs = rawArgs.map(a => this.expandText(a, newExp));
                const expanded = this.expandMacroBody(def.body, def.params, rawArgs, expandArgs);
                result += this.expandText(expanded, newExp);
                i = parsed.end;
              } else {
                result += ident;
                i = j;
              }
            } else {
              // Function-like macro without '(' is left unexpanded
              result += ident;
              i = j;
            }
          }
        } else {
          result += ident;
          i = j;
        }
        continue;
      }
      result += text[i];
      i++;
    }
    return result;
  }

  /**
   * Expand a macro body string applying:
   *  1. Stringification:  #param → "raw-arg-text"  (uses raw, unexpanded arg)
   *  2. Token pasting:    left##right → concatenation  (uses raw args for param operands)
   *  3. Parameter substitution with pre-expanded arg values
   */
  expandMacroBody(body, params, rawArgs, expandArgs) {
    // Phase 1: stringification (#param) — raw arg text, not pre-expanded
    // ## sequences are preserved as-is for phase 2 (token pasting)
    let result = '';
    let i = 0;
    while (i < body.length) {
      // ## token paste: preserve for phase 2, skip both characters now
      if (body[i] === '#' && i + 1 < body.length && body[i + 1] === '#') {
        result += '##';
        i += 2;
        continue;
      }
      // Single #: stringification operator
      if (body[i] === '#') {
        let j = i + 1;
        while (j < body.length && body[j] === ' ') j++;
        let k = j;
        while (k < body.length && /\w/.test(body[k])) k++;
        const name = body.slice(j, k);
        const idx = params.indexOf(name);
        if (idx !== -1) {
          const raw = rawArgs[idx];
          result += '"' + raw.replace(/\\/g, '\\\\').replace(/"/g, '\\"') + '"';
          i = k;
          continue;
        }
      }
      result += body[i];
      i++;
    }

    // Phase 2: token pasting (##) — iterative to handle chains
    let pasted = result;
    let prev;
    do {
      prev = pasted;
      pasted = pasted.replace(/(\w+)\s*##\s*(\w+)/g, (match, left, right) => {
        const li = params.indexOf(left);
        const ri = params.indexOf(right);
        const l = li !== -1 ? rawArgs[li] : left;
        const r = ri !== -1 ? rawArgs[ri] : right;
        return l + r;
      });
    } while (pasted !== prev);
    result = pasted;

    // Phase 3: substitute params with pre-expanded arg values
    for (let pi = 0; pi < params.length; pi++) {
      const param = params[pi];
      const val = expandArgs[pi];
      result = result.replace(new RegExp(`\\b${param}\\b`, 'g'), val);
    }

    return result;
  }

  skipString(text, start) {
    let i = start + 1;
    while (i < text.length) {
      if (text[i] === '\\') { i += 2; continue; }
      if (text[i] === '"') return i + 1;
      i++;
    }
    return text.length;
  }

  skipCharLit(text, start) {
    let i = start + 1;
    while (i < text.length) {
      if (text[i] === '\\') { i += 2; continue; }
      if (text[i] === "'") return i + 1;
      i++;
    }
    return text.length;
  }
}

module.exports = { CppPreprocessor };
