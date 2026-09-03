'use strict';

const { test } = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { spawnSync } = require('node:child_process');

const COMPILER = path.resolve(__dirname, '..', 'cpp-compiler.js');

function compileToC(source) {
  const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-void-parameter-'));
  const input = path.join(tempDir, 'input.cpp');
  const output = path.join(tempDir, 'output.c');
  fs.writeFileSync(input, source, 'utf8');

  const result = spawnSync(process.execPath, [COMPILER, input, '--output', output], {
    cwd: path.resolve(__dirname, '..', '..'),
    encoding: 'utf8'
  });
  assert.equal(result.status, 0, result.stderr || result.stdout);
  return fs.readFileSync(output, 'utf8');
}

test('calls to void-parameter functions use the same lowered symbol as their definition', () => {
  const c = compileToC([
    'void notify(void) { }',
    'int main() { notify(); return 0; }'
  ].join('\n'));

  const declaration = c.match(/void (notify(?:__[A-Za-z0-9_]+)?)\(void\);/);
  assert.ok(declaration, 'the lowered declaration must be present');
  const loweredName = declaration[1];
  assert.match(c, new RegExp(`int main\\(void\\) \\{[\\s\\S]*${loweredName}\\(\\);`),
    'a zero-argument call must resolve to the exact lowered declaration symbol');
});
