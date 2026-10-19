#!/usr/bin/env node

const fs = require('fs');
const path = require('path');
const { createC89JsHosts } = require('../../maiac/src/runtime/c89-js-hosts.js');
const { createDefaultHostBuiltins } = require('../../maiac/src/runtime/default-host-builtins.js');

function usage() {
  console.log('Usage: node tools/node/run-wasm-node.js <file.wasm>');
}

function createEnvImports() {
  let heapTop = null;
  let memoryRef = null;
  let instanceRef = null;

  function allocate(size) {
    if (!memoryRef) return 0;
    const requested = Math.max(1, Number(size) >>> 0);
    const alignDown = (value) => Math.floor(value / 8) * 8;
    if (heapTop === null) heapTop = memoryRef.buffer.byteLength;

    const stackGlobal = instanceRef && instanceRef.exports && instanceRef.exports.__stack_ptr;
    const stackFloor = stackGlobal && Number.isFinite(stackGlobal.value)
      ? Math.max(1024, stackGlobal.value)
      : 1024;
    let pointer = alignDown(heapTop - requested);
    if (pointer < stackFloor) {
      const missingBytes = stackFloor - pointer;
      memoryRef.grow(Math.ceil(missingBytes / 65536) + 1);
      heapTop = memoryRef.buffer.byteLength;
      pointer = alignDown(heapTop - requested);
    }

    heapTop = pointer;
    return pointer;
  }

  function getMemory() { return memoryRef; }

  const c89Hosts = createC89JsHosts(getMemory, {
    write: (text) => process.stdout.write(String(text))
  });
  const memoryHosts = createDefaultHostBuiltins(getMemory);

  const env = {
    ...c89Hosts,
    __builtin_wasm_memory_grow: memoryHosts._memory_grow_js,
    __builtin_wasm_memory_size: memoryHosts._memory_size_js,
    // MaiaCpp-specific bridge symbols not exposed by the C89 host bundle.
    printf: (formatPtr, a1, a2, a3, a4, a5, a6, a7) => c89Hosts.fprintf(2, formatPtr, a1, a2, a3, a4, a5, a6, a7),
    __malloc: allocate,
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
    setMemory: (mem, instance) => {
      memoryRef = mem || null;
      instanceRef = instance || null;
      heapTop = memoryRef ? memoryRef.buffer.byteLength : null;
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
  host.setMemory(instance.exports.memory || null, instance);

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
