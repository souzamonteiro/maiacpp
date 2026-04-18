#!/usr/bin/env node
const fs = require('fs');
const path = require('path');
const os = require('os');
const { spawnSync } = require('child_process');

function parseArgs(argv) {
  const out = {
    file: './compiler/test_cpp98_extended.cpp',
    outDir: './out/reports/cpp-vs-c',
    runtime: 'node',
    pipelineOnly: false,
    expectWasmRc: 0,
    webcppExtra: [],
    keepTemp: false
  };

  for (let i = 0; i < argv.length; i += 1) {
    const a = argv[i];
    if (a === '--file' && i + 1 < argv.length) out.file = argv[++i];
    else if (a === '--out-dir' && i + 1 < argv.length) out.outDir = argv[++i];
    else if (a === '--runtime' && i + 1 < argv.length) out.runtime = argv[++i];
    else if (a === '--pipeline-only') out.pipelineOnly = true;
    else if (a === '--expect-wasm-rc' && i + 1 < argv.length) out.expectWasmRc = Number(argv[++i]);
    else if (a === '--webcpp-extra' && i + 1 < argv.length) out.webcppExtra.push(argv[++i]);
    else if (a === '--keep-temp') out.keepTemp = true;
  }
  return out;
}

function chooseCxx() {
  for (const candidate of ['clang++', 'g++']) {
    const probe = spawnSync('sh', ['-lc', `command -v ${candidate}`], { encoding: 'utf8' });
    if (probe.status === 0) return candidate;
  }
  return null;
}

function runCmd(cmd, cwd) {
  return spawnSync(cmd[0], cmd.slice(1), { cwd, encoding: 'utf8' });
}

function processReturnCode(proc) {
  if (typeof proc.status === 'number') return proc.status;
  if (!proc.signal) return 1;

  const sigMap = os.constants && os.constants.signals ? os.constants.signals : {};
  const sigNum = sigMap[proc.signal];
  if (typeof sigNum === 'number') return -sigNum;
  return 1;
}

function parseNodeMainRc(stdoutText) {
  const m = String(stdoutText || '').match(/\[node\]\s+main\(\)\s+=>\s+(-?\d+)\s*$/m);
  return m ? Number(m[1]) : null;
}

function parseBrowserMainRc(stdoutText) {
  const m = String(stdoutText || '').match(/\[browser-host\]\s+main\(\)\s+=>\s+(-?\d+)\s*$/m);
  return m ? Number(m[1]) : null;
}

function normalizeNodeProgramOutput(stdoutText) {
  const lines = String(stdoutText || '').split(/\r?\n/);
  const out = [];
  for (const ln of lines) {
    if (ln.startsWith('Parsing: ')) continue;
    if (ln.startsWith('Parser: ')) continue;
    if (ln.startsWith('C gerado em: ')) continue;
    if (ln.startsWith('[node] wasm: ')) continue;
    if (ln.startsWith('[node] main() => ')) continue;
    if (ln.startsWith('[webc] ')) continue;
    out.push(ln);
  }
  return out.join('\n').trim();
}

function normalizeBrowserProgramOutput(stdoutText) {
  const lines = String(stdoutText || '').split(/\r?\n/);
  const out = [];
  for (const ln of lines) {
    if (ln.startsWith('Parsing: ')) continue;
    if (ln.startsWith('Parser: ')) continue;
    if (ln.startsWith('C gerado em: ')) continue;
    if (ln.startsWith('[browser-host] main() => ')) continue;
    if (ln.startsWith('[webc] ')) continue;
    out.push(ln);
  }
  return out.join('\n').trim();
}

function normalizeNativeOutput(stdoutText) {
  return String(stdoutText || '').split(/\r?\n/).join('\n').trim();
}

function unifiedDiff(aText, bText, fromFile, toFile) {
  const a = aText.split('\n');
  const b = bText.split('\n');
  const out = [`--- ${fromFile}`, `+++ ${toFile}`];

  let i = 0;
  let j = 0;
  while (i < a.length || j < b.length) {
    if (i < a.length && j < b.length && a[i] === b[j]) {
      i += 1;
      j += 1;
      continue;
    }

    const startI = i;
    const startJ = j;
    const remA = [];
    const remB = [];

    while (i < a.length && (j >= b.length || a[i] !== b[j])) {
      remA.push(a[i]);
      i += 1;
    }
    while (j < b.length && (startI >= a.length || b[j] !== a[startI])) {
      remB.push(b[j]);
      j += 1;
      if (i < a.length && j < b.length && a[i] === b[j]) break;
    }

    out.push(`@@ -${startI + 1},${Math.max(remA.length, 1)} +${startJ + 1},${Math.max(remB.length, 1)} @@`);
    for (const ln of remA) out.push(`-${ln}`);
    for (const ln of remB) out.push(`+${ln}`);
  }

  return out.join('\n');
}

function main() {
  const args = parseArgs(process.argv.slice(2));
  const repoRoot = process.cwd();
  const inputCpp = path.resolve(repoRoot, args.file);
  const outDir = path.resolve(repoRoot, args.outDir);

  if (!fs.existsSync(inputCpp)) {
    console.log(`[fail] input not found: ${inputCpp}`);
    process.exit(2);
  }

  let cxx = null;
  if (!args.pipelineOnly) {
    cxx = chooseCxx();
    if (!cxx) {
      console.log('[fail] no C++ compiler found (clang++/g++)');
      process.exit(2);
    }
  }

  const cppCompiler = path.join(repoRoot, 'compiler', 'cpp-compiler.js');
  const webcpp = path.join(repoRoot, 'bin', 'webcpp.sh');
  const nodeRunner = path.join(repoRoot, 'tools', 'node', 'run-wasm-node.js');
  const browserRunner = path.join(repoRoot, 'tools', 'browser', 'run-wasm-browser-host.js');

  const requiredTools = [cppCompiler, webcpp, args.runtime === 'node' ? nodeRunner : browserRunner];
  for (const required of requiredTools) {
    if (!fs.existsSync(required)) {
      console.log(`[fail] required tool missing: ${required}`);
      process.exit(2);
    }
  }

  fs.mkdirSync(outDir, { recursive: true });
  const stem = path.parse(inputCpp).name;
  const generatedC = path.join(outDir, `${stem}.generated.c`);

  const genProc = runCmd(['node', cppCompiler, inputCpp, '--output', generatedC], repoRoot);
  if (genProc.status !== 0) {
    console.log('[fail] failed to generate C from C++');
    process.stdout.write(genProc.stdout || '');
    process.stdout.write(genProc.stderr || '');
    process.exit(1);
  }

  const temp = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp_compare_'));
  try {
    const nativeBin = path.join(temp, `${stem}.native`);
    const wasmFile = path.join(temp, `${stem}.wasm`);

    let nativeRun = null;
    if (!args.pipelineOnly) {
      const nativeBuild = runCmd([cxx, '-std=c++98', '-O0', inputCpp, '-o', nativeBin], repoRoot);
      if (nativeBuild.status !== 0) {
        console.log('[fail] native C++ compilation failed');
        process.stdout.write(nativeBuild.stdout || '');
        process.stdout.write(nativeBuild.stderr || '');
        process.exit(1);
      }
      nativeRun = runCmd([nativeBin], repoRoot);
    }

    const wasmCmd = ['bash', webcpp, '--file', inputCpp, '--wasm-out', wasmFile, ...args.webcppExtra];
    const wasmBuild = runCmd(wasmCmd, repoRoot);
    if (wasmBuild.status !== 0) {
      console.log('[fail] MaiaCpp->MaiaC pipeline failed');
      process.stdout.write(wasmBuild.stdout || '');
      process.stdout.write(wasmBuild.stderr || '');
      process.exit(1);
    }

    const runtimeRunner = args.runtime === 'node' ? nodeRunner : browserRunner;
    const wasmRun = runCmd(['node', runtimeRunner, wasmFile], repoRoot);
    if (wasmRun.status !== 0) {
      console.log(`[fail] ${args.runtime} WASM runner failed`);
      process.stdout.write(wasmRun.stdout || '');
      process.stdout.write(wasmRun.stderr || '');
      process.exit(1);
    }

    const runnerOut = `${wasmBuild.stdout || ''}${wasmRun.stdout ? `\n${wasmRun.stdout}` : ''}`;
    const wasmOut = args.runtime === 'node'
      ? normalizeNodeProgramOutput(runnerOut)
      : normalizeBrowserProgramOutput(runnerOut);
    const wasmRc = args.runtime === 'node'
      ? parseNodeMainRc(wasmRun.stdout)
      : parseBrowserMainRc(wasmRun.stdout);

    let ok = true;
    let nativeOut = '';
    let nativeRc = 0;

    if (wasmRc == null) {
      console.log(`[fail] could not parse [${args.runtime}] main() return code`);
      ok = false;
    } else if (args.pipelineOnly) {
      if (wasmRc !== args.expectWasmRc) {
        console.log(`[fail] return code mismatch (pipeline-only): expected=${args.expectWasmRc}, wasm=${wasmRc}`);
        ok = false;
      }
    } else {
      nativeOut = normalizeNativeOutput(nativeRun.stdout);
      nativeRc = processReturnCode(nativeRun);
      if (wasmRc !== nativeRc) {
        console.log(`[fail] return code mismatch: native=${nativeRc}, wasm=${wasmRc}`);
        ok = false;
      }
      if (nativeOut !== wasmOut) {
        console.log('[fail] stdout mismatch between native C++ and pipeline output');
        console.log(unifiedDiff(nativeOut, wasmOut, 'native-cpp', 'maiacpp-pipeline'));
        ok = false;
      }
    }

    console.log('C++ vs Pipeline Comparison');
    console.log(`- Input: ${path.relative(repoRoot, inputCpp)}`);
    console.log(`- Generated C: ${path.relative(repoRoot, generatedC)}`);
    console.log(`- Native compiler: ${cxx || 'skipped (pipeline-only)'}`);
    console.log(`- Runtime host: ${args.runtime}`);
    if (args.webcppExtra.length > 0) console.log(`- webcpp extra: ${args.webcppExtra.join(' ')}`);
    if (wasmRc != null) {
      if (args.pipelineOnly) console.log(`- Return code: expected=${args.expectWasmRc}, wasm=${wasmRc}`);
      else console.log(`- Return code: native=${nativeRc}, wasm=${wasmRc}`);
    }
    console.log(`- Stdout equal: ${args.pipelineOnly ? 'n/a (pipeline-only)' : (nativeOut === wasmOut ? 'yes' : 'no')}`);

    if (!ok) process.exit(1);
    console.log(args.pipelineOnly
      ? '[ok] MaiaCpp pipeline-only WASM run matches expected return code'
      : '[ok] native C++ output matches MaiaCpp pipeline output');
    process.exit(0);
  } finally {
    if (!args.keepTemp) fs.rmSync(temp, { recursive: true, force: true });
  }
}

main();
