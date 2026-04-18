#!/usr/bin/env node
const fs = require('fs');
const path = require('path');
const os = require('os');
const { spawnSync } = require('child_process');

function parseArgs(argv) {
  const out = { file: '', functions: '' };
  for (let i = 0; i < argv.length; i += 1) {
    const a = argv[i];
    if (a === '--file' && i + 1 < argv.length) out.file = argv[++i];
    else if (a === '--functions' && i + 1 < argv.length) out.functions = argv[++i];
  }
  return out;
}

function extractFunctionBodies(cText) {
  const lines = cText.split(/\r?\n/);
  const out = new Map();
  let i = 0;

  while (i < lines.length) {
    const line = lines[i];
    const m = line.match(/^\s*[A-Za-z_][\w\s\*]*\s+([A-Za-z_][A-Za-z0-9_]*)\s*\([^;]*\)\s*\{\s*$/);
    if (!m) {
      i += 1;
      continue;
    }

    const name = m[1];
    let braceDepth = 1;
    const bodyLines = [];
    i += 1;

    while (i < lines.length && braceDepth > 0) {
      const cur = lines[i];
      braceDepth += (cur.match(/\{/g) || []).length;
      braceDepth -= (cur.match(/\}/g) || []).length;
      if (braceDepth > 0) bodyLines.push(cur);
      i += 1;
    }

    out.set(name, bodyLines.join('\n'));
  }

  return out;
}

function main() {
  const args = parseArgs(process.argv.slice(2));
  const repoRoot = process.cwd();
  const inputCpp = path.resolve(repoRoot, args.file || '');
  const compilerJs = path.join(repoRoot, 'compiler', 'cpp-compiler.js');

  if (!args.file || !fs.existsSync(inputCpp)) {
    console.log(`[fail] input not found: ${inputCpp}`);
    process.exit(2);
  }
  if (!fs.existsSync(compilerJs)) {
    console.log(`[fail] compiler not found: ${compilerJs}`);
    process.exit(2);
  }

  const targetFunctions = String(args.functions || '').split(',').map((s) => s.trim()).filter(Boolean);
  if (targetFunctions.length === 0) {
    console.log('[fail] no target functions provided');
    process.exit(2);
  }

  const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp_stub_guard_'));
  try {
    const cOut = path.join(tempDir, 'out.c');
    const proc = spawnSync('node', [compilerJs, inputCpp, '--output', cOut], { cwd: repoRoot, encoding: 'utf8' });
    if (proc.status !== 0) {
      console.log('[fail] could not generate C output');
      if (proc.stdout) process.stdout.write(proc.stdout);
      if (proc.stderr) process.stdout.write(proc.stderr);
      process.exit(1);
    }

    const cText = fs.readFileSync(cOut, 'utf8');
    const bodies = extractFunctionBodies(cText);
    const failures = [];

    for (const fn of targetFunctions) {
      const body = bodies.get(fn);
      if (body == null) {
        failures.push(`function not found in emitted C: ${fn}`);
        continue;
      }
      if (body.includes('return (int)0;')) {
        failures.push(`stub return found in function: ${fn}`);
      }
    }

    if (failures.length > 0) {
      console.log('[fail] stub guard violations');
      for (const f of failures) console.log(`- ${f}`);
      process.exit(1);
    }

    console.log('[ok] selected emitted C functions have no stub return');
    process.exit(0);
  } finally {
    fs.rmSync(tempDir, { recursive: true, force: true });
  }
}

main();
