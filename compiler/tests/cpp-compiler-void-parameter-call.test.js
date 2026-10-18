'use strict';

const { test } = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { spawnSync } = require('node:child_process');

const WEBCPP = path.resolve(__dirname, '..', '..', 'bin', 'webcpp.sh');

function compileToC(source, compilerArgs = []) {
  const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-void-parameter-'));
  const input = path.join(tempDir, 'input.cpp');
  const output = path.join(tempDir, 'output.c');
  fs.writeFileSync(input, source, 'utf8');

  const result = spawnSync('bash', [WEBCPP, '--file', input, '--c-out', output, ...compilerArgs], {
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

test('AST-strict C89 lowering preserves an external return call', () => {
  const c = compileToC([
    'extern "C" {',
    '__attribute__((import_module("maia.gfx"), import_name("probe")))',
    'int maia_window_create(int value);',
    '}',
    'int main() { return maia_window_create(7); }'
  ].join('\n'), ['--ast-strict', '--no-legacy-function-hints']);

  assert.match(c, /\/\* Target: C89 \*\//);
  assert.match(c, /extern int __maia__gfx__probe\s*\(int value\);/);
  assert.match(c, /#define maia_window_create __maia__gfx__probe/);
  assert.match(c, /return maia_window_create\s*\(\s*7\s*\);/);
  assert.doesNotMatch(c, /__attribute__/);
  assert.doesNotMatch(c, /main: stub-fallback/);
});

test('AST-strict lowering preserves C linkage export names', () => {
  const c = compileToC([
    'extern "C" {',
    '__attribute__((visibility("default")))',
    'void maiaos_start(void) { }',
    '}',
    'extern "C" __attribute__((visibility("default"))) void maiaos_step(void) { }'
  ].join('\n'), ['--ast-strict', '--no-legacy-function-hints']);

  assert.match(c, /void maiaos_start\(void\);/);
  assert.match(c, /void maiaos_step\(void\);/);
  assert.doesNotMatch(c, /maiaos_(?:start|step)__v/);
});

test('AST-strict call lowering maps empty calls to void-parameter C++98 symbols', () => {
  const c = compileToC([
    'void notify(void) { }',
    'int main() { notify(); return 0; }'
  ].join('\n'), ['--ast-strict', '--no-legacy-function-hints']);

  assert.match(c, /void notify__v\(void\);/);
  assert.match(c, /notify__v\s*\(\s*\);/);
  assert.doesNotMatch(c, /notify\s*\(\s*\);/);
});

test('AST-strict C89 return lowering converts static_cast nodes', () => {
  const c = compileToC(
    'unsigned int read(unsigned char* bytes) { return static_cast<unsigned int>(bytes[0]); }',
    ['--ast-strict', '--no-legacy-function-hints']
  );

  assert.match(c, /return \(unsigned int\)\(bytes\s*\[\s*0\s*\]\);/);
  assert.doesNotMatch(c, /static_cast/);
  assert.doesNotMatch(c, /stub-fallback/);
});

test('AST-strict global function extraction preserves pointer return types', () => {
  const c = compileToC([
    'int* identity(int* value) { return value; }',
    'int main() { return 0; }'
  ].join('\n'), ['--ast-strict', '--no-legacy-function-hints']);

  const loweredName = c.match(/int\* (identity__[A-Za-z0-9_]+)\(int\* value\);/)?.[1];
  assert.ok(loweredName, 'the pointer-returning C89 prototype must preserve its pointer type');
  assert.match(c, new RegExp(`int\\* ${loweredName}\\(int\\* value\\) \\{\\s*return value;`));
  assert.doesNotMatch(c, new RegExp(`${loweredName}\\(int\\* value\\) \\{\\s*return \\(int\\)0;`));
});

test('AST-strict fixed-width WASM types lower to C89 long and int types', () => {
  const c = compileToC([
    'extern "C" {',
    '__attribute__((import_module("maia.time"), import_name("monotonic_ms")))',
    'int64_t maia_monotonic_ms(void);',
    '}',
    'int64_t read_clock() { return maia_monotonic_ms(); }',
    'int32_t pointer_width(uintptr_t address) { return (int32_t)address; }'
  ].join('\n'), ['--ast-strict', '--no-legacy-function-hints']);

  assert.match(c, /extern long __maia__time__monotonic_ms\(void\);/);
  assert.match(c, /long read_clock\(void\);/);
  assert.match(c, /int pointer_width(?:__[A-Za-z0-9_]+)?\(unsigned int address\);/);
  assert.match(c, /return \(int\)\(address\);/);
  assert.doesNotMatch(c, /\b(?:u?int(?:8|16|32|64)_t|u?intptr_t)\b/);
});

test('AST-strict C89 globals preserve strings, arrays, scalar state, and 64-bit initializers', () => {
  const c = compileToC([
    'static const char title[] = "MaiaOS";',
    'static uint8_t event_packet[24];',
    'static int32_t window_handle = -1;',
    'static uint64_t last_seconds = static_cast<uint64_t>(-1);',
    'int main() { return 0; }'
  ].join('\n'), ['--ast-strict', '--no-legacy-function-hints']);

  assert.match(c, /static const char title\[\] = "MaiaOS";/);
  assert.match(c, /static unsigned char event_packet\[24\];/);
  assert.match(c, /static int window_handle = -\s*1;/);
  assert.match(c, /static unsigned long last_seconds = \(unsigned long\)\(/);
  assert.doesNotMatch(c, /\b(?:u?int(?:8|16|32|64)_t|static_cast)\b/);
});

test('AST-strict statement lowering emits local init, host calls, if, and return', () => {
  const c = compileToC([
    'extern "C" {',
    '__attribute__((import_module("maia.gfx"), import_name("probe")))',
    'int maia_window_create(int value);',
    '}',
    'int main() {',
    '  int handle = maia_window_create(4);',
    '  if (handle < 0) return 1;',
    '  int status = 3;',
    '  return handle + status;',
    '}'
  ].join('\n'), ['--ast-strict', '--no-legacy-function-hints']);

  assert.match(c, /int handle;/);
  assert.match(c, /int status;/);
  assert.match(c, /handle = maia_window_create\s*\(\s*4\s*\);/);
  assert.match(c, /if \(handle < 0\) \{/);
  assert.match(c, /if \(handle < 0\)[\s\S]*status = 3;/);
  assert.match(c, /return handle \+ status;/);
  assert.doesNotMatch(c, /main: stub-fallback/);
});

test('AST-strict C89 lowering hoists for-init declarations and preserves loop behavior', () => {
  const c = compileToC([
    'int sum(void) {',
    '  int total = 0;',
    '  for (int index = 0; index < 3; index++) total += index;',
    '  return total;',
    '}',
    'int main() { return sum(); }'
  ].join('\n'), ['--ast-strict', '--no-legacy-function-hints']);

  assert.match(c, /int index;/);
  assert.match(c, /index = 0;/);
  assert.match(c, /for \(; index < 3; index \+\+\)/);
  assert.match(c, /total \+= index;/);
  assert.doesNotMatch(c, /stub-fallback/);
});
