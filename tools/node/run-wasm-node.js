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

  function getMemory() { return memoryRef; }

  function readCString(ptr) {
    if (!memoryRef) return '';
    const bytes = new Uint8Array(memoryRef.buffer);
    let s = '';
    let i = ptr >>> 0;
    while (bytes[i]) s += String.fromCharCode(bytes[i++]);
    return s;
  }

  const env = {
    printf: createPrintfHost({
      getMemory: () => memoryRef,
      write: (text) => process.stdout.write(String(text))
    }),
    strlen: (ptr) => readCString(ptr).length | 0,
    strcmp: (p1, p2) => {
      const s1 = readCString(p1), s2 = readCString(p2);
      for (let i = 0; ; i++) {
        const a = s1.charCodeAt(i) || 0, b = s2.charCodeAt(i) || 0;
        if (a !== b) return a > b ? 1 : -1;
        if (a === 0) break;
      }
      return 0;
    },
    strncmp: (p1, p2, n) => {
      const s1 = readCString(p1), s2 = readCString(p2), len = n >>> 0;
      for (let i = 0; i < len; i++) {
        const a = s1.charCodeAt(i) || 0, b = s2.charCodeAt(i) || 0;
        if (a !== b) return a > b ? 1 : -1;
        if (a === 0) break;
      }
      return 0;
    },
    strcpy: (dst, src) => {
      if (!memoryRef) return dst | 0;
      const bytes = new Uint8Array(memoryRef.buffer);
      const s = readCString(src);
      for (let i = 0; i <= s.length; i++) bytes[(dst >>> 0) + i] = s.charCodeAt(i) || 0;
      return dst | 0;
    },
    strcat: (dst, src) => {
      if (!memoryRef) return dst | 0;
      const bytes = new Uint8Array(memoryRef.buffer);
      const d = dst >>> 0;
      let end = d; while (bytes[end]) end++;
      const s = readCString(src);
      for (let i = 0; i <= s.length; i++) bytes[end + i] = s.charCodeAt(i) || 0;
      return dst | 0;
    },
    strstr: (haystack, needle) => {
      const h = readCString(haystack), n = readCString(needle);
      if (!n.length) return haystack | 0;
      const idx = h.indexOf(n);
      return idx < 0 ? 0 : (haystack + idx) | 0;
    },
    strchr: (s, c) => {
      const str = readCString(s), ch = String.fromCharCode(c & 0xff);
      const idx = str.indexOf(ch);
      if (idx < 0) return (c & 0xff) === 0 ? (s + str.length) | 0 : 0;
      return (s + idx) | 0;
    },
    memcmp: (p1, p2, n) => {
      if (!memoryRef) return 0;
      const bytes = new Uint8Array(memoryRef.buffer);
      const len = n >>> 0;
      for (let i = 0; i < len; i++) {
        const a = bytes[(p1 >>> 0) + i], b = bytes[(p2 >>> 0) + i];
        if (a !== b) return a > b ? 1 : -1;
      }
      return 0;
    },
    memcpy: (dst, src, n) => {
      if (!memoryRef) return dst | 0;
      const bytes = new Uint8Array(memoryRef.buffer);
      const len = n >>> 0, d = dst >>> 0, s = src >>> 0;
      for (let i = 0; i < len; i++) bytes[d + i] = bytes[s + i];
      return dst | 0;
    },
    memmove: (dst, src, n) => {
      if (!memoryRef) return dst | 0;
      const bytes = new Uint8Array(memoryRef.buffer);
      const len = n >>> 0, d = dst >>> 0, s = src >>> 0;
      const tmp = bytes.slice(s, s + len);
      for (let i = 0; i < len; i++) bytes[d + i] = tmp[i];
      return dst | 0;
    },
    memset: (ptr, c, n) => {
      if (!memoryRef) return ptr | 0;
      const bytes = new Uint8Array(memoryRef.buffer);
      const len = n >>> 0, p = ptr >>> 0, val = c & 0xff;
      for (let i = 0; i < len; i++) bytes[p + i] = val;
      return ptr | 0;
    },
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
