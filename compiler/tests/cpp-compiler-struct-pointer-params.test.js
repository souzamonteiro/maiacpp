'use strict';

const { test } = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { spawnSync } = require('node:child_process');

const COMPILER = path.resolve(__dirname, '..', 'cpp-compiler.js');

function compileToC(source) {
  const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-struct-pointer-'));
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

test('C++ struct pointer parameters preserve their layout-bearing typedef in lowered C', () => {
  const c = compileToC([
    'struct Frame { int state; };',
    'static void resume(struct Frame* frame) { frame->state = 2; }',
    'int main() { Frame frame; frame.state = 0; resume(&frame); return frame.state; }'
  ].join('\n'));

  assert.match(c, /void resume__pv\(Frame\* frame\);/,
    'a known struct pointer must use the emitted typedef rather than void*');
  assert.match(c, /void resume__pv\(Frame\* frame\) \{[\s\S]*frame->state=2;/,
    'the lowered body must retain a typed receiver for member access');
  assert.doesNotMatch(c, /void resume__pv\(void\* frame\)/,
    'typed struct pointers must not be erased at the MaiaCpp-to-C boundary');
});
