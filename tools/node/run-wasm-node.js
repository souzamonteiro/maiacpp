#!/usr/bin/env node

const fs = require('fs');
const path = require('path');
const { createPrintfHost } = require('../runtime/printf-host.js');

function usage() {
  console.log('Usage: node tools/node/run-wasm-node.js <file.wasm>');
}

function createEnvImports() {
  let heap = 1024;
  let memoryRef = null;

  const env = {
    printf: createPrintfHost({
      getMemory: () => memoryRef,
      write: (text) => process.stdout.write(String(text))
    }),
    __malloc: (size) => {
      const ptr = heap;
      const n = Number(size) >>> 0;
      heap += Math.max(1, n);
      return ptr;
    },
    __free: () => {},
    __exc_push: () => {},
    __exc_pop: () => {},
    __exc_throw: () => {},
    __exc_active: () => 0,
    __exc_type: () => 0,
    __exc_data: () => {},
    __exc_matches: () => 0,
    __exc_clear: () => {}
  };

  return {
    env,
    setMemory: (mem) => {
      memoryRef = mem || null;
    }
  };
}

async function main() {
  const wasmArg = process.argv[2];
  if (!wasmArg) {
    usage();
    process.exit(1);
  }

  const wasmPath = path.resolve(wasmArg);
  if (!fs.existsSync(wasmPath)) {
    console.error(`WASM not found: ${wasmPath}`);
    process.exit(2);
  }

  const bytes = fs.readFileSync(wasmPath);
  const host = createEnvImports();
  const imports = { env: host.env };
  const { instance } = await WebAssembly.instantiate(bytes, imports);
  host.setMemory(instance.exports.memory || null);

  const mainFn = instance.exports.main;
  const startFn = instance.exports._start;

  if (typeof mainFn === 'function') {
    const rc = mainFn();
    console.log(`[node] main() => ${rc}`);
    return;
  }

  if (typeof startFn === 'function') {
    startFn();
    console.log('[node] _start() executed');
    return;
  }

  console.error('No export found: expected main or _start');
  process.exit(3);
}

main().catch((err) => {
  console.error(`Node WASM runner failed: ${err.message}`);
  process.exit(10);
});
