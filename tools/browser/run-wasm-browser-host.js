#!/usr/bin/env node

const fs = require('fs');
const path = require('path');

function readCString(memory, ptr) {
  const mem = new Uint8Array(memory.buffer);
  const offset = ptr >>> 0;
  let end = offset;
  while (end < mem.length && mem[end] !== 0) {
    end += 1;
  }
  return new TextDecoder('utf-8').decode(mem.subarray(offset, end));
}

function normalizeCFormat(fmt) {
  return String(fmt)
    .replace(/%([+\-0 #']*)(\d+)?(?:\.\d+)?[lh]([diouxXeEfgG])/g, '%$1$2$3')
    .replace(/%p/g, '%x');
}

const PLACEHOLDER_RE = /%%|%([+\-0 #']*)(\*|\d+)?(?:\.(\*|\d+))?([lh])?([diouxXeEfgGscptu%])/g;

function sprintf(fmt, args) {
  let cursor = 0;
  return String(fmt).replace(PLACEHOLDER_RE, (full, flags, widthSpec, precSpec, _length, type) => {
    if (full === '%%') return '%';

    const value = args[cursor++];
    const f = String(flags || '');
    const leftAlign = f.indexOf('-') >= 0;
    const forceSign = f.indexOf('+') >= 0;
    const zeroPad = f.indexOf('0') >= 0 && !leftAlign;
    const spaceSign = f.indexOf(' ') >= 0;

    const width = widthSpec != null ? parseInt(widthSpec, 10) : 0;
    const prec = precSpec != null ? parseInt(precSpec, 10) : null;

    let text = '';
    let sign = '';

    switch (type) {
      case 'd':
      case 'i': {
        const ni = value | 0;
        sign = ni < 0 ? '-' : (forceSign ? '+' : (spaceSign ? ' ' : ''));
        text = String(Math.abs(ni));
        if (prec != null) text = text.padStart(prec, '0');
        break;
      }
      case 'u': {
        const nu = value >>> 0;
        text = String(nu);
        if (prec != null) text = text.padStart(prec, '0');
        break;
      }
      case 'x': {
        const nx = value >>> 0;
        text = nx.toString(16);
        if (prec != null) text = text.padStart(prec, '0');
        break;
      }
      case 'X': {
        const nX = value >>> 0;
        text = nX.toString(16).toUpperCase();
        if (prec != null) text = text.padStart(prec, '0');
        break;
      }
      case 'o': {
        const no = value >>> 0;
        text = no.toString(8);
        if (prec != null) text = text.padStart(prec, '0');
        break;
      }
      case 'p': {
        text = '0x' + (value >>> 0).toString(16);
        break;
      }
      case 'f':
      case 'e':
      case 'E':
      case 'g':
      case 'G': {
        let nn = Number(value);
        sign = nn < 0 ? '-' : (forceSign ? '+' : (spaceSign ? ' ' : ''));
        nn = Math.abs(nn);
        const p = prec != null ? prec : 6;
        if (type === 'e' || type === 'E') {
          text = nn.toExponential(p);
          if (type === 'E') text = text.toUpperCase();
        } else if (type === 'g' || type === 'G') {
          text = nn.toPrecision(p === 0 ? 1 : p);
          if (type === 'G') text = text.toUpperCase();
        } else {
          text = nn.toFixed(p);
        }
        break;
      }
      case 'c':
        text = String.fromCharCode((value | 0) & 0xff);
        break;
      case 's':
        text = value == null ? '(null)' : String(value);
        if (prec != null) text = text.slice(0, prec);
        break;
      default:
        return full;
    }

    const body = sign + text;
    if (width > body.length) {
      const pad = width - body.length;
      if (leftAlign) return body + ' '.repeat(pad);
      if (zeroPad && sign) return sign + text.padStart(width - sign.length, '0');
      if (zeroPad) return body.padStart(width, '0');
      return body.padStart(width);
    }
    return body;
  });
}

const TYPES_RE = /%%|%[+\-0 #']*(?:\*|\d+)?(?:\.(?:\*|\d+))?[lh]?([diouxXeEfgGscptu%])/g;

function extractTypes(fmt) {
  const out = [];
  let m;
  TYPES_RE.lastIndex = 0;
  while ((m = TYPES_RE.exec(fmt)) !== null) {
    if (m[0] !== '%%' && m[1]) out.push(m[1]);
  }
  return out;
}

function resolveArgs(memory, fmt, rawArgs) {
  const types = extractTypes(fmt);
  return types.map((type, index) => {
    const raw = Number(rawArgs[index] != null ? rawArgs[index] : 0);
    if (type === 's') return readCString(memory, Math.trunc(raw) >>> 0);
    if (type === 'c') return Math.trunc(raw) & 0xff;
    if (type === 'd' || type === 'i') return Math.trunc(raw) | 0;
    if (type === 'u' || type === 'x' || type === 'X' || type === 'o' || type === 'p') return Math.trunc(raw) >>> 0;
    return raw;
  });
}

function usage() {
  console.log('Usage: node tools/browser/run-wasm-browser-host.js <file.wasm>');
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

  let memoryRef = null;
  const imports = {
    env: {
      printf: (fmtPtr, a1, a2, a3, a4, a5, a6, a7) => {
        if (!memoryRef) return 0;
        try {
          const rawFmt = readCString(memoryRef, fmtPtr >>> 0);
          const fmt = normalizeCFormat(rawFmt);
          const args = resolveArgs(memoryRef, fmt, [a1, a2, a3, a4, a5, a6, a7]);
          const text = sprintf(fmt, args);
          process.stdout.write(String(text));
          return text.length | 0;
        } catch (_err) {
          process.stdout.write('[printf-host-error]');
          return 0;
        }
      },
      __malloc: () => 0,
      __free: () => {},
      __exc_push: () => {},
      __exc_pop: () => {},
      __exc_throw: () => {},
      __exc_active: () => 0,
      __exc_type: () => 0,
      __exc_data: () => {},
      __exc_matches: () => 0,
      __exc_clear: () => {}
    }
  };

  const bytes = fs.readFileSync(wasmPath);
  const { instance } = await WebAssembly.instantiate(bytes, imports);
  memoryRef = instance.exports.memory || null;

  const entry = instance.exports.main || instance.exports._start;
  if (typeof entry !== 'function') {
    console.error('No export found: expected main or _start');
    process.exit(3);
  }

  const rc = entry();
  console.log(`[browser-host] main() => ${rc}`);
}

main().catch((err) => {
  console.error(`Browser-host WASM runner failed: ${err.message}`);
  process.exit(10);
});
