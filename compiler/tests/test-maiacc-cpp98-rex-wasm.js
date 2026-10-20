#!/usr/bin/env node
'use strict';

const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { execFileSync } = require('node:child_process');

const root = path.resolve(__dirname, '../..');
const cc = path.join(root, 'maiacc');
const frontend = path.join(cc, 'parser-generator/cpp98');
const temporary = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-rex-wasm-'));
const keep = process.env.MAIACPP_REX_TEST_KEEP_TEMP === '1';
const run = (command, args) => execFileSync(command, args, {
  cwd: root, encoding: 'utf8', timeout: 240000, maxBuffer: 32 * 1024 * 1024,
});

try {
  for (const filename of ['REx.cpp', 'parse-tree-collector.hpp']) {
    const source = path.join(frontend, filename);
    assert.ok(fs.existsSync(source), `Synchronize the published MaiaCC frontend first: ${source}`);
    fs.copyFileSync(source, path.join(temporary, filename));
  }
  fs.copyFileSync(path.join(root, 'maiaregex/maia_regex.h'), path.join(temporary, 'maia_regex.h'));
  const grammar = path.join(cc, 'grammar/REx.ebnf');
  run('bash', [path.join(cc, 'bin/tREx.sh'), '--ebnf', '--language', 'cpp98',
    '--to-xml', path.join(temporary, 'rex.xml'), grammar, path.join(temporary, 'rex-parser.cpp')]);
  const driver = path.join(temporary, 'driver.cpp');
  fs.writeFileSync(driver, `#include "REx.cpp"
static int lastStatus;
extern "C" char* maia_rex_parse(const char* input, unsigned int length) {
  return maiaCCRExToXml(input, length, &lastStatus);
}
extern "C" int maia_rex_status(void) {return lastStatus;}
int main(void) {return 0;}
`);
  const emitted = path.join(temporary, 'driver.c');
  run('bash', [path.join(root, 'bin/webcpp.sh'), '--file', driver, '--c-out', emitted,
    '--ast-strict', '--no-legacy-function-hints', '--no-system-includes']);
  run(process.env.CC || 'clang', ['-std=c89', '-pedantic-errors', '-ffreestanding',
    '-Werror=implicit-function-declaration', '-fsyntax-only', emitted]);
  console.log('PASS full REx emitted strict C89 syntax and declarations');
  const linked = path.join(temporary, 'linked.c');
  fs.writeFileSync(linked, `#define MAIACPP_USE_C_ALLOCATOR
#include "${path.join(root, 'maiac/src/stdlib.c')}"
#include "${path.join(root, 'maiac/src/string.c')}"
#include "${path.join(root, 'maiaregex/maia_regex.c')}"
#include "${emitted}"
`);
  const { compileSource } = require(path.join(root, 'maiac/compiler/c-compiler.js'));
  const compiled = compileSource(fs.readFileSync(linked, 'utf8'), {
    sourcePath: linked, resolveSystemIncludes: true, validate: true,
  });
  assert.ok(compiled.wasm, compiled.validationError && compiled.validationError.message);
  fs.writeFileSync(path.join(temporary, 'frontend.wasm'), Buffer.from(compiled.wasm));
  const module = new WebAssembly.Module(Buffer.from(compiled.wasm));
  let instance;
  const env = {
    __builtin_wasm_memory_size: (index) => index === 0 ? instance.exports.memory.buffer.byteLength / 65536 : -1,
    __builtin_wasm_memory_grow: (index, pages) => {
      if (index !== 0) return -1;
      try { return instance.exports.memory.grow(pages); } catch { return -1; }
    },
    __malloc: (size) => instance.exports.malloc(size),
    __free: (pointer) => instance.exports.free(pointer),
  };
  for (const imported of WebAssembly.Module.imports(module)) {
    assert.ok(imported.module === 'env' && imported.name in env,
      `Unexpected host dependency: ${imported.module}.${imported.name}`);
  }
  instance = new WebAssembly.Instance(module, { env });
  const execute = (text) => {
    const input = Buffer.from(text, 'utf8');
    const pointer = instance.exports.malloc(input.length + 1);
    assert.ok(pointer, 'Guest input allocation failed');
    new Uint8Array(instance.exports.memory.buffer).set(input, pointer);
    const stackBefore = instance.exports.__stack_ptr.value;
    const result = instance.exports.maia_rex_parse(pointer, input.length);
    const status = instance.exports.maia_rex_status();
    assert.equal(instance.exports.__stack_ptr.value, stackBefore, 'Guest frontend must restore its stack');
    let xml = null;
    if (result) {
      const memory = new Uint8Array(instance.exports.memory.buffer);
      const end = memory.indexOf(0, result);
      assert.ok(end >= result, 'Guest XML is not terminated');
      xml = Buffer.from(memory.slice(result, end)).toString('utf8');
      instance.exports.free(result);
    }
    instance.exports.free(pointer);
    assert.equal(instance.exports.__stack_ptr.value, stackBefore, 'Guest cleanup must restore its stack');
    return { xml, status };
  };
  for (const file of [grammar, path.join(cc, 'parser-generator/examples/sample-grammar.ebnf'),
    path.join(cc, 'parser-generator/tests/fixtures/shell.ebnf')]) {
    console.log(`Checking guest REx XML parity: ${path.basename(file)}`);
    const expected = run(process.execPath, [path.join(cc, 'parser-generator/REx.js'), file]);
    const actual = execute(fs.readFileSync(file, 'utf8'));
    assert.equal(actual.status, 0, file);
    assert.equal(actual.xml, expected, `Guest REx/XML parity: ${file}`);
    console.log(`PASS guest C++98 REx XML parity: ${path.basename(file)}`);
  }
  const invalid = execute('Start ::= (');
  assert.notEqual(invalid.status, 0);
  assert.equal(invalid.xml, null);
  console.log('C++98 REx frontend -> MaiaCpp -> C89 -> MaiaC -> WASM: PASS');
} finally {
  if (keep) console.log(`Preserved REx WASM artifacts at ${temporary}`);
  else fs.rmSync(temporary, { recursive: true, force: true });
}
