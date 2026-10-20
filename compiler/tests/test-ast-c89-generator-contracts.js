'use strict';
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { execFileSync } = require('node:child_process');
const { Cpp98Compiler } = require('../cpp-compiler');

const temporary = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-generator-contracts-'));
try {
  for (const name of ['445_adjacent_string_literals', '446_recursive_structs', '447_generator_declarations']) {
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
