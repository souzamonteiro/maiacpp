#!/usr/bin/env node
const http = require('http');
const fs = require('fs');
const path = require('path');

function parseArgs(argv) {
  const out = { root: process.cwd(), port: 8080 };
  for (let i = 0; i < argv.length; i += 1) {
    const a = argv[i];
    if (a === '--root' && i + 1 < argv.length) out.root = argv[++i];
    else if (a === '--port' && i + 1 < argv.length) out.port = Number(argv[++i]);
  }
  return out;
}

function contentType(filePath) {
  const ext = path.extname(filePath).toLowerCase();
  if (ext === '.html') return 'text/html; charset=utf-8';
  if (ext === '.js') return 'application/javascript; charset=utf-8';
  if (ext === '.css') return 'text/css; charset=utf-8';
  if (ext === '.json') return 'application/json; charset=utf-8';
  if (ext === '.wasm') return 'application/wasm';
  if (ext === '.svg') return 'image/svg+xml';
  if (ext === '.png') return 'image/png';
  if (ext === '.jpg' || ext === '.jpeg') return 'image/jpeg';
  if (ext === '.txt') return 'text/plain; charset=utf-8';
  return 'application/octet-stream';
}

function send(res, code, headers, body) {
  res.writeHead(code, headers);
  res.end(body);
}

function main() {
  const args = parseArgs(process.argv.slice(2));
  const root = path.resolve(args.root || process.cwd());
  const port = Number(args.port) || 8080;

  if (!fs.existsSync(root) || !fs.statSync(root).isDirectory()) {
    console.error(`[static-server] invalid root: ${root}`);
    process.exit(2);
  }

  const server = http.createServer((req, res) => {
    const urlPath = decodeURIComponent((req.url || '/').split('?')[0]);
    const rel = urlPath === '/' ? '/index.html' : urlPath;
    const unsafePath = path.join(root, rel);
    const filePath = path.resolve(unsafePath);

    if (!filePath.startsWith(root)) {
      send(res, 403, { 'Content-Type': 'text/plain; charset=utf-8' }, 'Forbidden');
      return;
    }

    let stat;
    try {
      stat = fs.statSync(filePath);
    } catch {
      send(res, 404, { 'Content-Type': 'text/plain; charset=utf-8' }, 'Not Found');
      return;
    }

    let finalPath = filePath;
    if (stat.isDirectory()) {
      finalPath = path.join(filePath, 'index.html');
      try {
        stat = fs.statSync(finalPath);
      } catch {
        send(res, 404, { 'Content-Type': 'text/plain; charset=utf-8' }, 'Not Found');
        return;
      }
    }

    fs.readFile(finalPath, (err, data) => {
      if (err) {
        send(res, 500, { 'Content-Type': 'text/plain; charset=utf-8' }, 'Internal Server Error');
        return;
      }
      send(res, 200, {
        'Content-Type': contentType(finalPath),
        'Cache-Control': 'no-cache'
      }, data);
    });
  });

  server.listen(port, '127.0.0.1', () => {
    console.log(`[static-server] serving ${root}`);
    console.log(`[static-server] http://127.0.0.1:${port}/`);
  });
}

main();
