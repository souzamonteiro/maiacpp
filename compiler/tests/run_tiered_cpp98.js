#!/usr/bin/env node
const fs = require('fs');
const path = require('path');
const os = require('os');
const { spawnSync } = require('child_process');

function parseArgs(argv) {
  const out = {
    plan: 'compiler/tests/ebnf_tiers.json',
    matrix: 'docs/CONFORMANCE_MATRIX.md',
    out: 'out/reports/ebnf-tiered-report.json'
  };

  for (let i = 0; i < argv.length; i += 1) {
    const a = argv[i];
    if (a === '--plan' && i + 1 < argv.length) out.plan = argv[++i];
    else if (a === '--matrix' && i + 1 < argv.length) out.matrix = argv[++i];
    else if (a === '--out' && i + 1 < argv.length) out.out = argv[++i];
  }
  return out;
}

function parseConformanceMatrix(matrixPath) {
  const entries = [];
  const lineRx = /^-\s+(.+):\s+(done|partial|missing)\s*$/;
  const lines = fs.readFileSync(matrixPath, 'utf8').split(/\r?\n/);
  for (const raw of lines) {
    const m = lineRx.exec(raw.trim());
    if (!m) continue;
    entries.push({ name: m[1], status: m[2] });
  }

  const done = entries.filter((e) => e.status === 'done').length;
  const partial = entries.filter((e) => e.status === 'partial').length;
  const missing = entries.filter((e) => e.status === 'missing').length;
  const total = entries.length;
  const weighted = done + (0.5 * partial);
  const weightedPct = total ? Math.round((1000 * weighted / total)) / 10 : 0.0;

  return { total, done, partial, missing, weighted, weightedPct, entries };
}

function formatDurationMs(durationMs) {
  if (!Number.isFinite(durationMs) || durationMs < 0) return '0ms';
  if (durationMs < 1000) return `${durationMs}ms`;
  const seconds = durationMs / 1000;
  if (seconds < 60) return `${seconds.toFixed(seconds >= 10 ? 1 : 2)}s`;
  const minutes = Math.floor(seconds / 60);
  const remSeconds = seconds - (minutes * 60);
  return `${minutes}m${String(remSeconds.toFixed(1)).replace(/\.0$/, '')}s`;
}

function buildReport(planPath, matrixPath, matrix, tierResults) {
  const summary = {
    tier1: summarizeTier(tierResults.tier1),
    tier2: summarizeTier(tierResults.tier2),
    tier3: summarizeTier(tierResults.tier3)
  };
  const matrixTracking = summarizeMatrixTracking(matrix, tierResults);

  return {
    generatedAt: new Date().toISOString(),
    planPath,
    matrixPath,
    matrixCoverage: {
      totalFamilies: matrix.total,
      done: matrix.done,
      partial: matrix.partial,
      missing: matrix.missing,
      weightedImplementedPct: matrix.weightedPct
    },
    tierSummary: summary,
    matrixTracking,
    tierResults
  };
}

function writeReport(outPath, report) {
  fs.mkdirSync(path.dirname(outPath), { recursive: true });
  fs.writeFileSync(outPath, `${JSON.stringify(report, null, 2)}\n`, 'utf8');
}

function collectOutputFlagPaths(cmd) {
  const outputFlags = new Set([
    '--ast-json-out',
    '--ast-xml-out'
  ]);
  const outputs = [];
  for (let i = 0; i < cmd.length - 1; i += 1) {
    if (!outputFlags.has(cmd[i])) continue;
    outputs.push({ flag: cmd[i], path: cmd[i + 1] });
    i += 1;
  }
  return outputs;
}

function buildCacheKey(cmd) {
  if (cmd[0] !== 'node' || cmd[1] !== './compiler/cpp-compiler.js') {
    return null;
  }
  const normalized = [];
  for (let i = 0; i < cmd.length; i += 1) {
    normalized.push(cmd[i]);
    if ((cmd[i] === '--ast-json-out' || cmd[i] === '--ast-xml-out') && i + 1 < cmd.length) {
      normalized.push('<OUT>');
      i += 1;
    }
  }
  if (!normalized.includes('<OUT>')) {
    return null;
  }
  return normalized.join('\u0000');
}

function writeCachedArtifacts(cachedArtifacts, currentOutputs) {
  if (!Array.isArray(cachedArtifacts) || cachedArtifacts.length === 0) return;
  const byFlag = new Map(cachedArtifacts.map((artifact) => [artifact.flag, artifact]));
  for (const output of currentOutputs) {
    const cachedArtifact = byFlag.get(output.flag);
    if (!cachedArtifact) continue;
    fs.mkdirSync(path.dirname(output.path), { recursive: true });
    fs.writeFileSync(output.path, cachedArtifact.contents, 'utf8');
  }
}

function rewriteOutputPaths(text, cachedArtifacts, currentOutputs) {
  let out = String(text || '');
  if (!Array.isArray(cachedArtifacts) || !Array.isArray(currentOutputs)) {
    return out;
  }
  const currentByFlag = new Map(currentOutputs.map((entry) => [entry.flag, entry.path]));
  for (const artifact of cachedArtifacts) {
    const currentPath = currentByFlag.get(artifact.flag);
    if (!currentPath || artifact.path === currentPath) continue;
    out = out.split(artifact.path).join(currentPath);
  }
  return out;
}

function cloneCachedResult(cachedEntry, currentOutputs) {
  writeCachedArtifacts(cachedEntry.artifacts, currentOutputs);
  const stdout = rewriteOutputPaths(cachedEntry.stdout, cachedEntry.artifacts, currentOutputs);
  const stderr = rewriteOutputPaths(cachedEntry.stderr, cachedEntry.artifacts, currentOutputs);
  const stdoutLines = stdout.split(/\r?\n/);
  const stderrLines = stderr.split(/\r?\n/);

  return {
    exitCode: cachedEntry.exitCode,
    durationMs: 0,
    cacheHit: true,
    cachedDurationMs: cachedEntry.durationMs,
    ok: cachedEntry.ok,
    errors: [...cachedEntry.errors],
    stdout,
    stderr,
    stdoutPreview: stdoutLines.slice(Math.max(0, stdoutLines.length - 8)).join('\n'),
    stderrPreview: stderrLines.slice(Math.max(0, stderrLines.length - 8)).join('\n')
  };
}

function runCase(testCase, repoRoot, tmpDir, cache = new Map()) {
  const cmd = (testCase.command || []).map((arg) => String(arg).replace('{tmpDir}', tmpDir));
  const cacheKey = buildCacheKey(cmd);
  const currentOutputs = collectOutputFlagPaths(cmd);
  if (cacheKey && cache.has(cacheKey)) {
    const cached = cloneCachedResult(cache.get(cacheKey), currentOutputs);
    return {
      id: testCase.id,
      family: testCase.family || 'unclassified',
      matrixFamily: testCase.matrixFamily,
      command: cmd,
      ...cached
    };
  }

  const startedAt = Date.now();
  const proc = spawnSync(cmd[0], cmd.slice(1), { cwd: repoRoot, encoding: 'utf8' });
  const stdout = proc.stdout || '';
  const stderr = proc.stderr || '';
  const errors = [];

  const expectedCode = Number(testCase.expectExitCode || 0);
  if ((proc.status ?? 1) !== expectedCode) {
    errors.push(`expected exit code ${expectedCode}, got ${proc.status}`);
  }

  for (const snippet of (testCase.stdoutContains || [])) {
    if (!stdout.includes(snippet)) errors.push(`missing stdout snippet: ${snippet}`);
  }
  for (const snippet of (testCase.stderrContains || [])) {
    if (!stderr.includes(snippet)) errors.push(`missing stderr snippet: ${snippet}`);
  }

  const stdoutLines = stdout.split(/\r?\n/);
  const stderrLines = stderr.split(/\r?\n/);
  const artifacts = currentOutputs
    .filter((output) => fs.existsSync(output.path))
    .map((output) => ({
      flag: output.flag,
      path: output.path,
      contents: fs.readFileSync(output.path, 'utf8')
    }));

  const result = {
    id: testCase.id,
    family: testCase.family || 'unclassified',
    matrixFamily: testCase.matrixFamily,
    command: cmd,
    exitCode: proc.status,
    durationMs: Date.now() - startedAt,
    cacheHit: false,
    cachedDurationMs: null,
    ok: errors.length === 0,
    errors,
    stdout,
    stderr,
    stdoutPreview: stdoutLines.slice(Math.max(0, stdoutLines.length - 8)).join('\n'),
    stderrPreview: stderrLines.slice(Math.max(0, stderrLines.length - 8)).join('\n')
  };

  if (cacheKey && result.ok) {
    cache.set(cacheKey, {
      exitCode: result.exitCode,
      durationMs: result.durationMs,
      ok: result.ok,
      errors: [...result.errors],
      stdout,
      stderr,
      artifacts
    });
  }

  return result;
}

function runTiers(plan, repoRoot, options = {}) {
  const results = { tier1: [], tier2: [], tier3: [] };
  const reportContext = options.reportContext || null;
  const cache = new Map();
  const totalCasesByTier = {
    tier1: ((plan.tiers || {}).tier1 || []).length,
    tier2: ((plan.tiers || {}).tier2 || []).length,
    tier3: ((plan.tiers || {}).tier3 || []).length
  };
  const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp_tiers_'));
  try {
    for (const tierName of ['tier1', 'tier2', 'tier3']) {
      const cases = ((plan.tiers || {})[tierName] || []);
      if (cases.length === 0) {
        console.log(`[tiered] ${tierName}: no cases`);
        continue;
      }
      console.log(`[tiered] ${tierName}: starting ${cases.length} case(s)`);
      for (let index = 0; index < cases.length; index += 1) {
        const testCase = cases[index];
        const label = `${tierName} ${index + 1}/${cases.length}`;
        console.log(`[tiered] ${label}: ${testCase.id}`);
        const result = runCase(testCase, repoRoot, tempDir, cache);
        results[tierName].push(result);
        const status = result.ok ? 'ok' : 'fail';
        const durationText = result.cacheHit
          ? `cached from ${formatDurationMs(result.cachedDurationMs)} baseline`
          : formatDurationMs(result.durationMs);
        console.log(`[tiered] ${label}: ${status} (${durationText})`);
        if (!result.ok) {
          for (const error of result.errors) {
            console.log(`[tiered] ${label}: ${error}`);
          }
        }
        if (reportContext) {
          const report = buildReport(
            reportContext.planPath,
            reportContext.matrixPath,
            reportContext.matrix,
            results
          );
          writeReport(reportContext.outPath, report);
        }
      }
      const tierSummary = summarizeTier(results[tierName]);
      console.log(
        `[tiered] ${tierName}: ${tierSummary.ok}/${totalCasesByTier[tierName]} passed (${tierSummary.passRatePct}%)`
      );
    }
  } finally {
    fs.rmSync(tempDir, { recursive: true, force: true });
  }
  return results;
}

function summarizeTier(items) {
  const total = items.length;
  const ok = items.filter((i) => i.ok).length;
  const failed = total - ok;
  const passRatePct = total ? Math.round((1000 * ok / total)) / 10 : 0.0;
  return { total, ok, failed, passRatePct };
}

function summarizeMatrixTracking(matrix, tierResults) {
  const matrixNames = (matrix.entries || []).map((e) => e.name);
  const matrixSet = new Set(matrixNames);
  const tracked = new Map();

  for (const tierName of ['tier1', 'tier2', 'tier3']) {
    for (const item of (tierResults[tierName] || [])) {
      const fam = item.matrixFamily;
      if (!fam || !matrixSet.has(fam)) continue;
      const bucket = tracked.get(fam) || { family: fam, cases: 0, ok: 0, failed: 0 };
      bucket.cases += 1;
      if (item.ok) bucket.ok += 1;
      else bucket.failed += 1;
      tracked.set(fam, bucket);
    }
  }

  const trackedNames = Array.from(tracked.keys()).sort();
  const untracked = matrixNames.filter((name) => !tracked.has(name));

  return {
    trackedFamilies: trackedNames.length,
    totalFamilies: matrixNames.length,
    trackedPct: matrixNames.length ? Math.round((1000 * trackedNames.length / matrixNames.length)) / 10 : 0.0,
    tracked: trackedNames.map((n) => tracked.get(n)),
    untracked
  };
}

function main() {
  const args = parseArgs(process.argv.slice(2));
  const repoRoot = process.cwd();
  const planPath = path.resolve(repoRoot, args.plan);
  const matrixPath = path.resolve(repoRoot, args.matrix);
  const outPath = path.resolve(repoRoot, args.out);

  if (!fs.existsSync(planPath)) {
    console.log(`[fail] plan not found: ${planPath}`);
    process.exit(2);
  }
  if (!fs.existsSync(matrixPath)) {
    console.log(`[fail] matrix not found: ${matrixPath}`);
    process.exit(2);
  }

  const plan = JSON.parse(fs.readFileSync(planPath, 'utf8'));
  const matrix = parseConformanceMatrix(matrixPath);
  const reportContext = {
    planPath: path.relative(repoRoot, planPath),
    matrixPath: path.relative(repoRoot, matrixPath),
    matrix,
    outPath
  };
  const startedAt = Date.now();
  const tierResults = runTiers(plan, repoRoot, { reportContext });
  const report = buildReport(reportContext.planPath, reportContext.matrixPath, matrix, tierResults);
  writeReport(outPath, report);
  const summary = report.tierSummary;
  const matrixTracking = report.matrixTracking;

  console.log('Tiered C++98 Report');
  console.log(`- Matrix weighted coverage: ${matrix.weightedPct}% (${matrix.done} done, ${matrix.partial} partial, ${matrix.missing} missing)`);
  console.log(`- Tier 1: ${summary.tier1.ok}/${summary.tier1.total} passed`);
  console.log(`- Tier 2: ${summary.tier2.ok}/${summary.tier2.total} passed`);
  console.log(`- Tier 3: ${summary.tier3.ok}/${summary.tier3.total} passed`);
  console.log(`- Matrix families tracked by tier cases: ${matrixTracking.trackedFamilies}/${matrixTracking.totalFamilies} (${matrixTracking.trackedPct}%)`);
  console.log(`- Total duration: ${formatDurationMs(Date.now() - startedAt)}`);
  console.log(`- Report JSON: ${path.relative(repoRoot, outPath)}`);

  if (summary.tier1.failed > 0 || summary.tier2.failed > 0) process.exit(1);
  process.exit(0);
}

main();
