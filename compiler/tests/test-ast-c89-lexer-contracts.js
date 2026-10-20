'use strict';

const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { execFileSync } = require('node:child_process');
const { Cpp98Compiler } = require('../cpp-compiler.js');
const { CppPreprocessor } = require('../cpp-preprocessor.js');

const source = path.join(__dirname, 'equivalence/444_generated_lexer_contracts.cpp');
const temporary = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-lexer-contracts-'));
try {
  const root = path.resolve(__dirname, '../..');
  const preprocessor = new CppPreprocessor(temporary);
  assert.equal(preprocessor.resolveIncludePath('stdlib.h', temporary, true),
    path.join(root, 'maiac/include/stdlib.h'), 'Default headers must come from the repository-local dependency');
  assert.ok(preprocessor.includeDirs.every((directory) => directory.startsWith(`${root}${path.sep}`)),
    'Default includes must not search sibling principal repositories');
  const constants = preprocessor.preprocess('#include <limits.h>\nunsigned int bound(void) { return UINT_MAX; }\n');
  assert.match(constants, /return 4294967295U;/, 'C compatibility headers must supply target-width macros');
  assert.doesNotMatch(constants, /\bUINT_MAX\b/);
  assert.match(new CppPreprocessor(temporary).preprocess(
    '#ifdef __cplusplus\nlong cpp98_mode = __cplusplus;\n#else\n#error C++98 macro missing\n#endif\n'), /long cpp98_mode = 199711L;/);
  const emitted = new Cpp98Compiler(source, { astStrict: true, legacyFunctionHints: false }).compile();
  assert.match(emitted, /int \(\*accept\)\(void\*, unsigned int\);/);
  assert.match(emitted, /events\.accept = accept__/);
  assert.match(emitted, /events \. accept \(/);
  assert.match(emitted, /eventPointer -> accept \(/);
  assert.match(emitted, /extern unsigned int declaredOnly\(const Pattern\*\);/);
  assert.match(emitted, /extern int variadicOnly\(const char\*, \.\.\.\);/);
  const target = path.join(temporary, 'contracts.c');
  fs.writeFileSync(target, emitted);
  const native = path.join(temporary, 'reference');
  const result = path.join(temporary, 'lowered');
  execFileSync(process.env.CXX || 'clang++', ['-std=c++98', '-pedantic-errors', source, '-o', native]);
  execFileSync(process.env.CC || 'clang', ['-std=c89', '-pedantic-errors', '-Werror=implicit-function-declaration', target, '-o', result]);
  execFileSync(native);
  execFileSync(result);
  console.log('AST C89 lexer contracts: native C++98 and emitted C89 agree');
} finally {
  fs.rmSync(temporary, { recursive: true, force: true });
}
