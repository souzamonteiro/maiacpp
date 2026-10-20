#!/usr/bin/env node
'use strict';

const assert = require('node:assert/strict');
const { execFileSync } = require('node:child_process');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');

const root = path.resolve(__dirname, '../..');
const trex = path.join(root, 'maiacc/bin/tREx.sh');
const webcpp = path.join(root, 'bin/webcpp.sh');
const wasmRunner = path.join(root, 'tools/node/run-wasm-node.js');
const arithmeticGrammar = `Expression ::= Number | '+' Number

<?TOKENS?>
Number ::= [0-9]+
Whitespace ::= [#x20#x9#xA#xD]+ /* ws: definition */
`;

function run(command, args, options = {}) {
  return execFileSync(command, args, {
    cwd: root,
    encoding: 'utf8',
    maxBuffer: 16 * 1024 * 1024,
    ...options,
  });
}

function main() {
  for (const file of [trex, webcpp, wasmRunner]) {
    if (!fs.existsSync(file)) throw new Error(`Required integration tool missing: ${file}`);
  }

  const tempRoot = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-maiacc-parser-'));
  const grammarPath = path.join(tempRoot, 'arithmetic.ebnf');
  const grammarXmlPath = path.join(tempRoot, 'arithmetic.xml');
  const generatedParserPath = path.join(tempRoot, 'generated-parser.cpp');
  const driverPath = path.join(tempRoot, 'parser-driver.cpp');
  const cPath = path.join(tempRoot, 'parser-driver.c');
  const wasmPath = path.join(tempRoot, 'parser-driver.wasm');
  const linkedPath = path.join(tempRoot, 'linked-driver.c');
  const keepTemp = process.env.MAIACPP_PARSER_TEST_KEEP_TEMP === '1';

  try {
    fs.copyFileSync(path.join(root, 'maiaregex/maia_regex.h'), path.join(tempRoot, 'maia_regex.h'));
    fs.writeFileSync(grammarPath, arithmeticGrammar);
    run('bash', [
      trex,
      '--ebnf',
      '--language',
      'cpp98',
      '--to-xml',
      grammarXmlPath,
      grammarPath,
      generatedParserPath,
    ]);

    fs.writeFileSync(driverPath, `#include "generated-parser.cpp"

struct Trace {
  unsigned int count;
  unsigned int kinds[16];
  unsigned int types[16];
};
static unsigned int traceCheckpoint(void* context) {
  Trace* trace = (Trace*)context;
  return trace->count;
}
static void traceRestore(void* context, unsigned int mark) {
  Trace* trace = (Trace*)context;
  trace->count = mark;
}
static void traceStart(void* context, const char*, unsigned int) {
  Trace* trace = (Trace*)context;
  if (trace->count >= 16) return;
  trace->kinds[trace->count] = 1;
  ++trace->count;
}
static void traceTerminal(void* context, unsigned int type, const char*, unsigned int, unsigned int) {
  Trace* trace = (Trace*)context;
  if (trace->count >= 16) return;
  trace->kinds[trace->count] = 2;
  trace->types[trace->count] = type;
  ++trace->count;
}
static void traceEnd(void* context, const char*, unsigned int) {
  Trace* trace = (Trace*)context;
  if (trace->count >= 16) return;
  trace->kinds[trace->count] = 3;
  ++trace->count;
}
static void traceAbort(void*, const char*, unsigned int) {}

int main() {
  size_t bound = (size_t)-1;
  if (bound / 4 != 1073741823) return 3;
  MaiaCCParserToken validToken;
  validToken.type = MAIACC_TOKEN_Number;
  validToken.value = "7";
  validToken.length = 1;
  validToken.start = 0;
  validToken.end = 1;
  MaiaCCParserState validState;
  maiaCCParserStateInit(&validState, &validToken, 1, 0);
  if (!maiaCC_parse(&validState)) return 1;

  MaiaCCParserToken invalidToken;
  invalidToken.type = MAIACC_TOKEN_TOKEN__2B_;
  invalidToken.value = "+";
  invalidToken.length = 1;
  invalidToken.start = 0;
  invalidToken.end = 1;
  MaiaCCParserState invalidState;
  maiaCCParserStateInit(&invalidState, &invalidToken, 1, 0);
  if (maiaCC_parse(&invalidState)) return 2;

  MaiaCCParserToken scannedTokens[8];
  MaiaCCLexerState lexer;
  maiaCCLexerStateInit(&lexer, " + 27 ", 6);
  if (!maiaCCLexerTokenize(&lexer, scannedTokens, 8)) return 40 + lexer.failed;
  if (lexer.tokenCount != 3) return 5;
  if (scannedTokens[0].type != MAIACC_TOKEN_TOKEN__2B_) return 6;
  if (scannedTokens[1].type != MAIACC_TOKEN_Number) return 7;
  if (scannedTokens[1].start != 3 || scannedTokens[1].end != 5) return 8;
  if (scannedTokens[1].length != 2) return 9;
  MaiaCCParserState scannedState;
  Trace trace;
  trace.count = 0;
  MaiaCCParseEvents events;
  events.context = &trace;
  events.checkpoint = traceCheckpoint;
  events.restore = traceRestore;
  events.startNonterminal = traceStart;
  events.terminal = traceTerminal;
  events.endNonterminal = traceEnd;
  events.abortNonterminal = traceAbort;
  events.matchesContextual = 0;
  maiaCCParserStateInit(&scannedState, scannedTokens, lexer.tokenCount, &events);
  if (!maiaCC_parse(&scannedState)) return 10;
  if (trace.count != 4) return 11;
  if (trace.kinds[0] != 1 || trace.kinds[1] != 2 || trace.kinds[2] != 2 || trace.kinds[3] != 3) return 12;
  if (trace.types[1] != MAIACC_TOKEN_TOKEN__2B_ || trace.types[2] != MAIACC_TOKEN_Number) return 13;
  maiaCCLexerStateInit(&lexer, "@", 1);
  if (maiaCCLexerTokenize(&lexer, scannedTokens, 8) || lexer.failed != 1) return 14;
  maiaCCLexerStateInit(&lexer, "27", 2);
  if (maiaCCLexerTokenize(&lexer, scannedTokens, 1) || lexer.failed != 3) return 15;
  return 0;
}
`);

    run('bash', [
      webcpp,
      '--file',
      driverPath,
      '--c-out',
      cPath,
      '--ast-strict',
      '--no-legacy-function-hints',
      '--no-system-includes',
    ]);
    const cSource = fs.readFileSync(cPath, 'utf8');
    assert.doesNotMatch(cSource, /MAIACC_TOKEN_/, 'MaiaCpp must expand generated token-ID macros');
    assert.match(cSource, /static const MaiaRegexInstruction maiaCCLexCode0\[\] = \{/);
    assert.match(cSource, /program\.entry = pattern -> entry;/);
    fs.writeFileSync(linkedPath, `#define MAIACPP_USE_C_ALLOCATOR\n#include "${path.join(root, 'maiac/src/stdlib.c')}"\n#include "${path.join(root, 'maiaregex/maia_regex.c')}"\n#include "${cPath}"\n`);
    const { compileSource } = require(path.join(root, 'maiac/compiler/c-compiler.js'));
    const result = compileSource(fs.readFileSync(linkedPath, 'utf8'), {
      sourcePath: linkedPath, resolveSystemIncludes: true, validate: true,
    });
    assert.ok(result.wasm, result.validationError && result.validationError.message);
    fs.writeFileSync(wasmPath, Buffer.from(result.wasm));
    assert.ok(fs.existsSync(wasmPath), 'MaiaCpp/MaiaC did not produce WASM');
    assert.ok(fs.statSync(wasmPath).size > 0, 'Generated WASM is empty');

    const output = run(process.execPath, [wasmRunner, wasmPath]);
    assert.match(output, /\[node\] main\(\) => 0/);
    console.log('MaiaCC C++98 raw-text lexer/parser/callbacks -> MaiaCpp -> MaiaC -> WASM: PASS');
  } finally {
    if (keepTemp) {
      console.log(`Preserved integration artifacts at ${tempRoot}`);
    } else {
      fs.rmSync(tempRoot, { recursive: true, force: true });
    }
  }
}

try {
  main();
} catch (error) {
  console.error(error.stack || error.message);
  process.exitCode = 1;
}
