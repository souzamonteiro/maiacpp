#!/usr/bin/env node
'use strict';

const path = require('path');
const app = require('./cin-cout-double-smoke-after-maiac-fix.js');

async function main() {
  const wasmPath = process.argv[2]
    ? path.resolve(process.argv[2])
    : path.join(__dirname, 'cin-cout-double-smoke-after-maiac-fix.wasm');
  const exitCode = await app.run(wasmPath);
  process.stdout.write('\n[node-runner] program returned: ' + exitCode + '\n');
  process.exitCode = Number.isInteger(exitCode) ? exitCode : 0;
}

main().catch((error) => {
  console.error('[node-runner] ' + error.message);
  process.exit(1);
});
