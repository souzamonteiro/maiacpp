'use strict';

const { sprintf } = require('./sprintf.js');

function readCString(memory, ptr) {
  const mem = new Uint8Array(memory.buffer);
  const offset = ptr >>> 0;
  let end = offset;
  while (end < mem.length && mem[end] !== 0) end += 1;
  return new TextDecoder('utf-8').decode(mem.subarray(offset, end));
}

function normalizeCFormat(fmt) {
  return String(fmt)
    .replace(/%([+\-0 #']*)(\d+)?(?:\.\d+)?[lh]([diouxXeEfgG])/g, '%$1$2$3')
    .replace(/%p/g, '%x');
}

const PLACEHOLDER_TYPES_RE = /%%|%[+\-0 #']*(?:\*|\d+)?(?:\.(?:\*|\d+))?[lh]?([diouxXeEfgGscptu%])/g;

function extractPlaceholderTypes(fmt) {
  const types = [];
  let match;
  PLACEHOLDER_TYPES_RE.lastIndex = 0;
  while ((match = PLACEHOLDER_TYPES_RE.exec(fmt)) !== null) {
    if (match[0] !== '%%' && match[1]) types.push(match[1]);
  }
  return types;
}

function resolveArgs(memory, normalizedFmt, rawArgs) {
  const types = extractPlaceholderTypes(normalizedFmt);
  return types.map((type, index) => {
    const raw = Number(rawArgs[index] != null ? rawArgs[index] : 0);
    if (type === 's') return readCString(memory, Math.trunc(raw) >>> 0);
    if (type === 'c') return Math.trunc(raw) & 0xff;
    if (['d', 'i'].includes(type)) return Math.trunc(raw) | 0;
    if (['u', 'x', 'X', 'o', 'p'].includes(type)) return Math.trunc(raw) >>> 0;
    return raw;
  });
}

function createPrintfHost({ getMemory, write }) {
  return function printf(fmtPtr, a1, a2, a3, a4, a5, a6, a7) {
    const memory = getMemory();
    if (!memory) return 0;
    try {
      const rawFmt = readCString(memory, fmtPtr >>> 0);
      const fmt = normalizeCFormat(rawFmt);
      const args = resolveArgs(memory, fmt, [a1, a2, a3, a4, a5, a6, a7]);
      const text = sprintf(fmt, args);
      write(text);
      return text.length | 0;
    } catch (_err) {
      write('[printf-host-error]');
      return 0;
    }
  };
}

module.exports = { createPrintfHost, readCString };
