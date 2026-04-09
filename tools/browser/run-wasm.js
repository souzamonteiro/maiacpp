/* eslint-disable no-console */
(function () {
  function readCString(memory, ptr) {
    var mem = new Uint8Array(memory.buffer);
    var offset = ptr >>> 0;
    var end = offset;
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

  var PLACEHOLDER_RE = /%%|%([+\-0 #']*)(\*|\d+)?(?:\.(\*|\d+))?([lh])?([diouxXeEfgGscptu%])/g;

  function sprintf(fmt, args) {
    var cursor = 0;
    return String(fmt).replace(PLACEHOLDER_RE, function (full, flags, widthSpec, precSpec, _length, type) {
      if (full === '%%') return '%';

      var value = args[cursor++];
      var f = String(flags || '');
      var leftAlign = f.indexOf('-') >= 0;
      var forceSign = f.indexOf('+') >= 0;
      var zeroPad = f.indexOf('0') >= 0 && !leftAlign;
      var spaceSign = f.indexOf(' ') >= 0;

      var width = widthSpec != null ? parseInt(widthSpec, 10) : 0;
      var prec = precSpec != null ? parseInt(precSpec, 10) : null;

      var text = '';
      var sign = '';

      switch (type) {
        case 'd':
        case 'i': {
          var ni = value | 0;
          sign = ni < 0 ? '-' : (forceSign ? '+' : (spaceSign ? ' ' : ''));
          text = String(Math.abs(ni));
          if (prec != null) text = text.padStart(prec, '0');
          break;
        }
        case 'u': {
          var nu = value >>> 0;
          text = String(nu);
          if (prec != null) text = text.padStart(prec, '0');
          break;
        }
        case 'x': {
          var nx = value >>> 0;
          text = nx.toString(16);
          if (prec != null) text = text.padStart(prec, '0');
          break;
        }
        case 'X': {
          var nX = value >>> 0;
          text = nX.toString(16).toUpperCase();
          if (prec != null) text = text.padStart(prec, '0');
          break;
        }
        case 'o': {
          var no = value >>> 0;
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
          var nn = Number(value);
          sign = nn < 0 ? '-' : (forceSign ? '+' : (spaceSign ? ' ' : ''));
          nn = Math.abs(nn);
          var p = prec != null ? prec : 6;
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

      var body = sign + text;
      if (width > body.length) {
        var pad = width - body.length;
        if (leftAlign) return body + ' '.repeat(pad);
        if (zeroPad && sign) return sign + text.padStart(width - sign.length, '0');
        if (zeroPad) return body.padStart(width, '0');
        return body.padStart(width);
      }
      return body;
    });
  }

  var TYPES_RE = /%%|%[+\-0 #']*(?:\*|\d+)?(?:\.(?:\*|\d+))?[lh]?([diouxXeEfgGscptu%])/g;
  function extractTypes(fmt) {
    var out = [];
    var m;
    TYPES_RE.lastIndex = 0;
    while ((m = TYPES_RE.exec(fmt)) !== null) {
      if (m[0] !== '%%' && m[1]) out.push(m[1]);
    }
    return out;
  }

  function resolveArgs(memory, fmt, rawArgs) {
    var types = extractTypes(fmt);
    return types.map(function (type, index) {
      var raw = Number(rawArgs[index] != null ? rawArgs[index] : 0);
      if (type === 's') return readCString(memory, Math.trunc(raw) >>> 0);
      if (type === 'c') return Math.trunc(raw) & 0xff;
      if (type === 'd' || type === 'i') return Math.trunc(raw) | 0;
      if (type === 'u' || type === 'x' || type === 'X' || type === 'o' || type === 'p') return Math.trunc(raw) >>> 0;
      return raw;
    });
  }

  var outputEl = document.getElementById('output');
  var runBtn = document.getElementById('run');
  var wasmInput = document.getElementById('wasm-url');
  var statusEl = document.getElementById('status');

  function write(text) {
    outputEl.textContent += String(text);
    outputEl.scrollTop = outputEl.scrollHeight;
  }

  async function run() {
    outputEl.textContent = '';
    statusEl.textContent = 'Running...';

    var wasmUrl = wasmInput.value.trim();
    if (!wasmUrl) {
      statusEl.textContent = 'Missing wasm URL';
      return;
    }

    var memoryRef = null;
    var imports = {
      env: {
        printf: function (fmtPtr, a1, a2, a3, a4, a5, a6, a7) {
          if (!memoryRef) return 0;
          try {
            var rawFmt = readCString(memoryRef, fmtPtr >>> 0);
            var fmt = normalizeCFormat(rawFmt);
            var args = resolveArgs(memoryRef, fmt, [a1, a2, a3, a4, a5, a6, a7]);
            var text = sprintf(fmt, args);
            write(text);
            return text.length | 0;
          } catch (_err) {
            write('[printf-host-error]');
            return 0;
          }
        },
        __malloc: function () { return 0; },
        __free: function () {},
        __exc_push: function () {},
        __exc_pop: function () {},
        __exc_throw: function () {},
        __exc_active: function () { return 0; },
        __exc_clear: function () {}
      }
    };

    try {
      var response = await fetch(wasmUrl);
      if (!response.ok) throw new Error('HTTP ' + response.status);
      var bytes = await response.arrayBuffer();
      var instantiated = await WebAssembly.instantiate(bytes, imports);
      var instance = instantiated.instance;
      memoryRef = instance.exports.memory || null;

      var entry = instance.exports.main || instance.exports._start;
      if (typeof entry !== 'function') {
        throw new Error('Missing entrypoint (main or _start)');
      }

      var result = entry();
      write('\n[maiacpp] program returned: ' + result + '\n');
      statusEl.textContent = 'Done';
    } catch (error) {
      statusEl.textContent = 'Error';
      write('\n[runner-error] ' + error.message + '\n');
    }
  }

  function readQueryWasm() {
    var params = new URLSearchParams(window.location.search);
    var wasm = params.get('wasm');
    if (wasm) wasmInput.value = wasm;
  }

  runBtn.addEventListener('click', run);
  readQueryWasm();
})();
