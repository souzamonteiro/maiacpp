'use strict';
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { execFileSync } = require('node:child_process');
const { Cpp98Compiler } = require('../cpp-compiler');

const temporary = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-generator-contracts-'));
try {
  const compiler = new Cpp98Compiler(path.join(__dirname, 'equivalence', '445_adjacent_string_literals.cpp'), { astStrict: true, legacyFunctionHints: false });
  const protectedText = [
    'const char* scalar = "this.failureMemo = new Set();";',
    'const char* array = "items = new Item[count];";',
    'const char* escaped = "\\\" = new Set(); \\\\";',
    "int quote = '\"';",
    '// scalar = new Set();',
    '/* array = new Item[count]; */'
  ].join('\n');
  const normalized = compiler.normalizeGeneratedC89(protectedText + '\nscalar = new Item();\narray = new Item[count];');
  assert.ok(normalized.includes(protectedText), 'Allocation lowering must preserve literals and comments');
  assert.match(normalized, /scalar = \(Item\*\)__malloc\(\(unsigned long\)sizeof\(Item\)\);/);
  assert.match(normalized, /array = \(Item\*\)__malloc\(\(unsigned long\)\(sizeof\(Item\) \* \(count\)\)\);/);
  console.log('PASS allocation lowering preserves source template literals and comments');
  for (const name of ['445_adjacent_string_literals', '446_recursive_structs', '447_generator_declarations', '448_source_template_literals']) {
    const input = path.join(__dirname, 'equivalence', name + '.cpp');
    const code = new Cpp98Compiler(input, { astStrict: true, legacyFunctionHints: false }).compile();
    assert.doesNotMatch(code, /stub-fallback|structured-main|deterministic-fold/, 'The AST lowering must implement each body');
    const output = path.join(temporary, name + '.c'); fs.writeFileSync(output, code);
    const native = path.join(temporary, name + '-reference');
    const lowered = path.join(temporary, name + '-lowered');
    execFileSync(process.env.CXX || 'clang++', ['-std=c++98', '-pedantic-errors', input, '-o', native]);
    execFileSync(process.env.CC || 'clang', ['-std=c89', '-pedantic-errors', '-Werror=implicit-function-declaration', output, '-o', lowered]);
    execFileSync(native); execFileSync(lowered);
    console.log(`PASS strict AST C++98/C89 executable parity: ${name}`);
  }
} finally { fs.rmSync(temporary, { recursive: true, force: true }); }
