'use strict';

const PLACEHOLDER_RE = /%%|%([+\-0 #']*)(\*|\d+)?(?:\.(\*|\d+))?([lh])?([diouxXeEfgGscptu%])/g;

function sprintf(fmt, args) {
  let cursor = 0;
  return String(fmt).replace(PLACEHOLDER_RE, (full, flags, widthSpec, precSpec, _length, type) => {
    if (full === '%%') return '%';

    const value = args[cursor++];
    const f = String(flags || '');
    const leftAlign = f.includes('-');
    const forceSign = f.includes('+');
    const zeroPad = f.includes('0') && !leftAlign;
    const spaceSign = f.includes(' ');

    const width = widthSpec != null ? parseInt(widthSpec, 10) : 0;
    const prec = precSpec != null ? parseInt(precSpec, 10) : null;

    let text = '';
    let sign = '';

    switch (type) {
      case 'd':
      case 'i': {
        const n = value | 0;
        sign = n < 0 ? '-' : (forceSign ? '+' : (spaceSign ? ' ' : ''));
        text = String(Math.abs(n));
        if (prec != null) text = text.padStart(prec, '0');
        break;
      }
      case 'u': {
        const n = value >>> 0;
        text = String(n);
        if (prec != null) text = text.padStart(prec, '0');
        break;
      }
      case 'o': {
        const n = value >>> 0;
        text = n.toString(8);
        if (prec != null) text = text.padStart(prec, '0');
        break;
      }
      case 'x': {
        const n = value >>> 0;
        text = n.toString(16);
        if (prec != null) text = text.padStart(prec, '0');
        break;
      }
      case 'X': {
        const n = value >>> 0;
        text = n.toString(16).toUpperCase();
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
        let n = typeof value === 'number' ? value : Number(value);
        sign = n < 0 ? '-' : (forceSign ? '+' : (spaceSign ? ' ' : ''));
        n = Math.abs(n);
        const p = prec != null ? prec : 6;
        if (type === 'e' || type === 'E') {
          text = n.toExponential(p);
          if (type === 'E') text = text.toUpperCase();
        } else if (type === 'g' || type === 'G') {
          text = n.toPrecision(p === 0 ? 1 : p);
          if (type === 'G') text = text.toUpperCase();
        } else {
          text = n.toFixed(p);
        }
        break;
      }
      case 'c': {
        text = String.fromCharCode((value | 0) & 0xff);
        break;
      }
      case 's': {
        text = value == null ? '(null)' : String(value);
        if (prec != null) text = text.slice(0, prec);
        break;
      }
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

module.exports = { sprintf };
