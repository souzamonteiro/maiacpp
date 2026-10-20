#!/usr/bin/env node
'use strict';

const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { execFileSync } = require('node:child_process');
const { Cpp98Compiler } = require('../cpp-compiler');

const root = path.resolve(__dirname, '../..');
const cc = path.join(root, 'maiacc');
const source = path.join(cc, 'parser-generator/cpp98');
const { compileSource } = require(path.join(root, 'maiac/compiler/c-compiler'));
const GrammarParser = require(path.join(cc, 'parser-generator/grammar-parser'));
const CodeGenerator = require(path.join(cc, 'parser-generator/code-generator'));
const temporary = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-selfhost-wasm-'));
const keep = process.env.MAIACPP_SELFHOST_KEEP_TEMP === '1';
const run = (command, args) => execFileSync(command, args, {
  cwd: root, encoding: 'utf8', timeout: 240000, maxBuffer: 64 * 1024 * 1024,
});
let memoryGrowths = 0;

function equalOutput(actual, expected, label) {
  if (actual !== expected) {
    const name = label.replace(/[^a-zA-Z0-9_-]/g, '_');
    fs.writeFileSync(path.join(temporary, name + '-actual.txt'), actual || '');
    fs.writeFileSync(path.join(temporary, name + '-expected.txt'), expected);
    let offset = 0;
    while (offset < actual.length && offset < expected.length && actual[offset] === expected[offset]) ++offset;
    assert.fail(`${label}: output differs at character ${offset} (actual ${actual.length}, expected ${expected.length}); see ${temporary}`);
  }
}

function instantiate(bytes) {
  const wasmModule = new WebAssembly.Module(bytes);
  let instance;
  const env = {
    __builtin_wasm_memory_size: (index) => index === 0 ? instance.exports.memory.buffer.byteLength / 65536 : -1,
    __builtin_wasm_memory_grow: (index, pages) => {
      if (index !== 0) return -1;
      try { const previous = instance.exports.memory.grow(pages); ++memoryGrowths; return previous; } catch { return -1; }
    },
    __malloc: (size) => instance.exports.malloc(size),
    __free: (pointer) => instance.exports.free(pointer),
  };
  for (const imported of WebAssembly.Module.imports(wasmModule)) {
    assert.ok(imported.module === 'env' && imported.name in env,
      `Unexpected host dependency: ${imported.module}.${imported.name}`);
  }
  instance = new WebAssembly.Instance(wasmModule, { env });
  return instance;
}

function execute(instance, text, generator, cpp = true) {
  const input = Buffer.from(text, 'utf8');
  const api = instance.exports;
  const pointer = api.malloc(input.length + 1);
  assert.ok(pointer, 'Guest input allocation');
  new Uint8Array(api.memory.buffer).set(input, pointer);
  new Uint8Array(api.memory.buffer)[pointer + input.length] = 0;
  const stack = api.__stack_ptr.value;
  const result = generator ? api.maia_cc_generate(pointer, input.length, cpp ? 1 : 0)
    : api.maia_rex_parse(pointer, input.length);
  const status = generator ? api.maia_cc_status() : api.maia_rex_status();
  assert.equal(api.__stack_ptr.value, stack, 'Guest call must restore its frame');
  let output = null;
  if (result) {
    const memory = new Uint8Array(api.memory.buffer);
    const length = generator ? api.maia_cc_length() : memory.indexOf(0, result) - result;
    assert.ok(length >= 0 && result + length < memory.length, 'Guest output bounds');
    assert.equal(memory[result + length], 0, 'Guest output terminator');
    output = Buffer.from(memory.slice(result, result + length)).toString('utf8');
    api.free(result);
  }
  api.free(pointer);
  assert.equal(api.__stack_ptr.value, stack, 'Guest cleanup must restore its frame');
  return { output, status };
}

function build(directory, frontend) {
  const name = frontend ? 'frontend' : 'generator';
  const driver = path.join(directory, name + '-driver.cpp');
  fs.writeFileSync(driver, frontend ? `#include "REx.cpp"
static int lastStatus;
extern "C" char* maia_rex_parse(const char* input, unsigned int length) {
  return maiaCCRExToXml(input, length, &lastStatus);
}
extern "C" int maia_rex_status(void) { return lastStatus; }
int main(void) { return 0; }
` : `#include "generator.cpp"
static int lastStatus;
static unsigned int lastLength;
extern "C" char* maia_cc_generate(const char* input, unsigned int length, int cpp) {
  return maiaCCGenerateXml(input, length, cpp, &lastLength, &lastStatus);
}
extern "C" int maia_cc_status(void) { return lastStatus; }
extern "C" unsigned int maia_cc_length(void) { return lastLength; }
int main(void) { return 0; }
`);
  console.log(`Building ${path.basename(directory)}/${name}: C++98 -> C89 -> WASM`);
  const emitted = path.join(directory, name + '.c');
  const code = new Cpp98Compiler(driver, { astStrict: true, legacyFunctionHints: false }).compile();
  assert.doesNotMatch(code, /stub-fallback|deterministic-fold/, 'No unimplemented body may stand in for the generator');
  fs.writeFileSync(emitted, code);
  // C89's minimum required string length is 509. This implementation supports
  // the longer template data; suppress only that translation-limit diagnostic.
  run(process.env.CC || 'clang', ['-std=c89', '-pedantic-errors', '-Wno-overlength-strings',
    '-ffreestanding', '-Werror=implicit-function-declaration', '-fsyntax-only', emitted]);
  const linked = path.join(directory, name + '-linked.c');
  fs.writeFileSync(linked, `#define MAIACPP_USE_C_ALLOCATOR
#include "${path.join(root, 'maiac/src/stdlib.c')}"
#include "${path.join(root, 'maiac/src/string.c')}"
#include "${path.join(root, 'maiaregex/maia_regex.c')}"
#include "${emitted}"
`);
  const compiled = compileSource(fs.readFileSync(linked, 'utf8'), {
    sourcePath: linked, resolveSystemIncludes: true, validate: true,
  });
  assert.ok(compiled.wasm, compiled.validationError && compiled.validationError.message);
  const bytes = Buffer.from(compiled.wasm);
  fs.writeFileSync(path.join(directory, name + '.wasm'), bytes);
  console.log(`PASS ${path.basename(directory)}/${name} WASM validation (${bytes.length} bytes)`);
  return instantiate(bytes);
}

async function main() {
  assert.ok(fs.existsSync(path.join(source, 'generator.cpp')), 'Synchronize the published complete MaiaCC port first');
  const rex = path.join(cc, 'grammar/REx.ebnf');
  const xmlGrammar = path.join(source, 'xml.ebnf');
  const fixtures = [rex, xmlGrammar, path.join(cc, 'parser-generator/examples/sample-grammar.ebnf'),
    path.join(cc, 'parser-generator/examples/WAT.ebnf'), path.join(cc, 'parser-generator/tests/fixtures/shell.ebnf')];
  const references = new Map();
  for (const file of fixtures) {
    const xml = run(process.execPath, [path.join(cc, 'parser-generator/REx.js'), file]);
    const model = await new GrammarParser(xml).parse();
    references.set(file, { xml, cpp: new CodeGenerator(model, { language: 'cpp98' }).generate(),
      javascript: new CodeGenerator(model).generate() });
  }
  let previous;
  for (let generation = 0; generation <= 2; ++generation) {
    const directory = path.join(temporary, `generation-${generation}`); fs.mkdirSync(directory);
    for (const filename of fs.readdirSync(source).filter((name) => name.endsWith('.hpp') || name.endsWith('.cpp'))) {
      if (!filename.endsWith('-main.cpp')) fs.copyFileSync(path.join(source, filename), path.join(directory, filename));
    }
    fs.copyFileSync(path.join(root, 'maiaregex/maia_regex.h'), path.join(directory, 'maia_regex.h'));
    for (const [file, name] of [[rex, 'rex-parser.cpp'], [xmlGrammar, 'xml-parser.cpp']]) {
      let code;
      if (!previous) code = references.get(file).cpp; // JS seeds generation zero only.
      else {
        console.log(`Generating ${generation}/${name} using the previous WASM pair`);
        const parsed = execute(previous.frontend, fs.readFileSync(file, 'utf8'), false);
        assert.equal(parsed.status, 0); equalOutput(parsed.output, references.get(file).xml, `generation-${generation}-${name}-xml`);
        const emitted = execute(previous.generator, parsed.output, true);
        assert.equal(emitted.status, 0); code = emitted.output;
        equalOutput(code, references.get(file).cpp, `generation-${generation}-${name}`);
      }
      fs.writeFileSync(path.join(directory, name), code);
    }
    previous = { frontend: build(directory, true), generator: build(directory, false) };
    console.log(`PASS complete WASM bootstrap generation ${generation}`);
  }
  for (const file of fixtures) {
    console.log(`Checking final guest EBNF/XML/generation parity: ${path.basename(file)}`);
    const parsed = execute(previous.frontend, fs.readFileSync(file, 'utf8'), false);
    assert.equal(parsed.status, 0); equalOutput(parsed.output, references.get(file).xml, path.basename(file) + '-xml');
    for (const [cpp, key] of [[true, 'cpp'], [false, 'javascript']]) {
      const generated = execute(previous.generator, parsed.output, true, cpp);
      assert.equal(generated.status, 0); equalOutput(generated.output, references.get(file)[key], path.basename(file) + '-' + key);
      console.log(`PASS ${path.basename(file)} ${key} byte parity`);
    }
  }
  for (const invalid of ['Start ::= (', '@']) {
    const rejected = execute(previous.frontend, invalid, false);
    assert.notEqual(rejected.status, 0); assert.equal(rejected.output, null);
  }
  for (const invalid of ['', '<Grammar></Other>', '<Grammar>&missing;</Grammar>']) {
    const rejected = execute(previous.generator, invalid, true);
    assert.notEqual(rejected.status, 0); assert.equal(rejected.output, null);
  }
  assert.ok(memoryGrowths > 0, 'The complete guest bootstrap must exercise real memory growth');
  console.log(`PASS complete MaiaCC C++98 -> C89 -> WASM self-hosting: three stable generations, both backends, ${memoryGrowths} memory growths`);
}

main().catch((error) => { console.error(error); process.exitCode = 1; }).finally(() => {
  if (keep) console.log(`Preserved self-host WASM artifacts at ${temporary}`);
  else fs.rmSync(temporary, { recursive: true, force: true });
});
