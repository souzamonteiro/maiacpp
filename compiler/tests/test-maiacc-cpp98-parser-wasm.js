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
  const keepTemp = process.env.MAIACPP_PARSER_TEST_KEEP_TEMP === '1';

  try {
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

int main() {
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
  return 0;
}
`);

    run('bash', [
      webcpp,
      '--file',
      driverPath,
      '--c-out',
      cPath,
      '--wasm-out',
      wasmPath,
      '--ast-strict',
      '--no-legacy-function-hints',
      '--no-system-includes',
    ]);
    const cSource = fs.readFileSync(cPath, 'utf8');
    assert.doesNotMatch(cSource, /MAIACC_TOKEN_/, 'MaiaCpp must expand generated token-ID macros');
    assert.ok(fs.existsSync(wasmPath), 'MaiaCpp/MaiaC did not produce WASM');
    assert.ok(fs.statSync(wasmPath).size > 0, 'Generated WASM is empty');

    const output = run(process.execPath, [wasmRunner, wasmPath]);
    assert.match(output, /\[node\] main\(\) => 0/);
    console.log('MaiaCC C++98 parser -> MaiaCpp -> MaiaC -> WASM: PASS');
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