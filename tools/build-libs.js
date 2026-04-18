#!/usr/bin/env node
'use strict';

/**
 * tools/build-libs.js
 *
 * Build pipeline: compiles each C++ source in src/ through the two-stage
 * MaiaCpp → MaiaC pipeline and deposits the resulting .wasm artifacts in lib/.
 *
 * Stage 1 – C++ → C   : compiler/cpp-compiler.js  <src/X.cpp>  --output <tmp/X.c>
 * Stage 2 – C  → WASM : ../maiac/tools/webc.js     <tmp/X.c>   -o <lib/X>
 *
 * Usage:
 *   node tools/build-libs.js [options]
 *
 * Options:
 *   --src-dir  DIR   Directory with C++ sources  (default: ./src)
 *   --lib-dir  DIR   Output directory for .wasm  (default: ./lib)
 *   --tmp-dir  DIR   Temp dir for .c files       (default: OS temp)
 *   --wat            Also emit .wat alongside each .wasm
 *   --no-validate    Skip WAT/WASM validation in MaiaC webc
 *   --only NAME,...  Comma-separated list of stems to build (e.g. iostream,string)
 *   --force          Re-build even if the .wasm is already up-to-date
 *   -h, --help       Show this message
 */

const fs      = require('fs');
const path    = require('path');
const os      = require('os');
const { spawnSync } = require('child_process');

// ---------------------------------------------------------------------------
// Paths
// ---------------------------------------------------------------------------

const REPO_ROOT    = path.resolve(__dirname, '..');
const PROJECTS_ROOT = path.resolve(REPO_ROOT, '..');

const COMPILER_JS  = path.join(REPO_ROOT, 'compiler', 'cpp-compiler.js');

function resolveWebcJs() {
  const sibling = path.join(PROJECTS_ROOT, 'maiac', 'tools', 'webc.js');
  if (fs.existsSync(sibling)) return sibling;
  const vendored = path.join(REPO_ROOT, 'maiac', 'tools', 'webc.js');
  if (fs.existsSync(vendored)) return vendored;
  return null;
}

// ---------------------------------------------------------------------------
// CLI parsing
// ---------------------------------------------------------------------------

function usage() {
  console.log(`
Usage:
  node tools/build-libs.js [options]

Options:
  --src-dir  DIR    C++ source directory (default: ./src)
  --lib-dir  DIR    Output WASM directory (default: ./lib)
  --tmp-dir  DIR    Temp directory for .c intermediates (default: OS temp)
  --wat             Also emit .wat alongside each .wasm
  --no-validate     Skip WAT/WASM validation in MaiaC webc
  --only NAME,...   Comma-separated stems to build (e.g. iostream,string)
  --force           Re-build even if .wasm is already up-to-date
  -h, --help        Show this help
`);
}

function parseArgs(argv) {
  const opts = {
    srcDir:     path.join(REPO_ROOT, 'src'),
    libDir:     path.join(REPO_ROOT, 'lib'),
    tmpDir:     null,
    wat:        false,
    validate:   true,
    only:       null,
    force:      false,
  };

  const args = argv.slice(2);
  for (let i = 0; i < args.length; i++) {
    switch (args[i]) {
      case '--src-dir':
        opts.srcDir = path.resolve(args[++i]);
        break;
      case '--lib-dir':
        opts.libDir = path.resolve(args[++i]);
        break;
      case '--tmp-dir':
        opts.tmpDir = path.resolve(args[++i]);
        break;
      case '--wat':
        opts.wat = true;
        break;
      case '--no-validate':
        opts.validate = false;
        break;
      case '--only':
        opts.only = new Set(args[++i].split(',').map(s => s.trim()).filter(Boolean));
        break;
      case '--force':
        opts.force = true;
        break;
      case '-h':
      case '--help':
        usage();
        process.exit(0);
        break;
      default:
        console.error(`Unknown option: ${args[i]}`);
        process.exit(1);
    }
  }
  return opts;
}

// ---------------------------------------------------------------------------
// Build helpers
// ---------------------------------------------------------------------------

function isUpToDate(srcFile, wasmFile) {
  if (!fs.existsSync(wasmFile)) return false;
  try {
    const srcMtime  = fs.statSync(srcFile).mtimeMs;
    const wasmMtime = fs.statSync(wasmFile).mtimeMs;
    return wasmMtime >= srcMtime;
  } catch {
    return false;
  }
}

function runNode(args, label) {
  const result = spawnSync(process.execPath, args, { encoding: 'utf8' });
  if (result.status !== 0) {
    const msg = (result.stderr || result.stdout || '').trim();
    throw new Error(`${label} failed (exit ${result.status}):\n${msg}`);
  }
  return result;
}

// cpp-compiler.js exits 0 even when the full parser fails and the simple
// fallback is used.  The fallback can still produce valid C for many files,
// so we only emit a warning here; stage 2 (webc) acts as the hard gate.
const PARSER_FALLBACK_RE = /parser falhou/i;

function buildOne(cppFile, opts, webcJs, tmpDir) {
  const stem    = path.basename(cppFile, '.cpp');
  const cFile   = path.join(tmpDir, `${stem}.c`);
  const outBase = path.join(opts.libDir, stem);
  const wasmFile = `${outBase}.wasm`;

  // Stage 1: C++ → C
  const stage1 = runNode([COMPILER_JS, cppFile, '--output', cFile], `cpp-compiler (${stem})`);
  const stage1Out = ((stage1.stdout || '') + (stage1.stderr || '')).trim();
  if (PARSER_FALLBACK_RE.test(stage1Out)) {
    const detail = stage1Out.match(/parser falhou[^\n]*/i);
    process.stdout.write(` [warn: parser fallback — ${detail ? detail[0] : 'fallback mode'}]`);
  }

  // Stage 2: C → WASM
  const webcArgs = [webcJs, cFile, '-o', outBase, '--no-system-includes'];
  if (opts.wat)       webcArgs.push('--wat');
  if (!opts.validate) webcArgs.push('--no-validate');

  runNode(webcArgs, `webc (${stem})`);

  return wasmFile;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

function main() {
  const opts  = parseArgs(process.argv);
  const webcJs = resolveWebcJs();
  if (!webcJs) {
    console.error('Error: MaiaC webc.js not found. Expected at ../maiac/tools/webc.js');
    process.exit(1);
  }

  if (!fs.existsSync(COMPILER_JS)) {
    console.error(`Error: MaiaCpp compiler not found: ${COMPILER_JS}`);
    process.exit(1);
  }

  if (!fs.existsSync(opts.srcDir)) {
    console.error(`Error: src dir not found: ${opts.srcDir}`);
    process.exit(1);
  }

  fs.mkdirSync(opts.libDir, { recursive: true });

  // Resolve temp dir (create a fresh subdir so we don't pollute the caller's cwd)
  const tmpRoot = opts.tmpDir || os.tmpdir();
  const tmpDir  = fs.mkdtempSync(path.join(tmpRoot, 'maiacpp-build-libs-'));

  let sources = fs.readdirSync(opts.srcDir)
    .filter(f => f.endsWith('.cpp'))
    .map(f => path.join(opts.srcDir, f));

  if (opts.only) {
    sources = sources.filter(f => opts.only.has(path.basename(f, '.cpp')));
    if (sources.length === 0) {
      console.error(`Error: --only filter matched no files in ${opts.srcDir}`);
      process.exit(1);
    }
  }

  sources.sort();

  const results = { built: [], skipped: [], failed: [] };

  for (const cppFile of sources) {
    const stem     = path.basename(cppFile, '.cpp');
    const wasmFile = path.join(opts.libDir, `${stem}.wasm`);

    if (!opts.force && isUpToDate(cppFile, wasmFile)) {
      console.log(`[build-libs] skip  ${stem}.wasm (up-to-date)`);
      results.skipped.push(stem);
      continue;
    }

    process.stdout.write(`[build-libs] build ${stem} ...`);
    try {
      buildOne(cppFile, opts, webcJs, tmpDir);
      console.log(' ok');
      results.built.push(stem);
    } catch (err) {
      console.log(' FAILED');
      console.error(err.message.split('\n').map(l => `  ${l}`).join('\n'));
      results.failed.push(stem);
    }
  }

  // Clean up temp dir
  try { fs.rmSync(tmpDir, { recursive: true, force: true }); } catch { /* ignore */ }

  // Summary
  console.log('');
  console.log(`[build-libs] done — built: ${results.built.length}, skipped: ${results.skipped.length}, failed: ${results.failed.length}`);
  if (results.built.length)   console.log(`  built:   ${results.built.join(', ')}`);
  if (results.skipped.length) console.log(`  skipped: ${results.skipped.join(', ')}`);
  if (results.failed.length) {
    console.log(`  failed:  ${results.failed.join(', ')}`);
    process.exit(1);
  }
}

main();
