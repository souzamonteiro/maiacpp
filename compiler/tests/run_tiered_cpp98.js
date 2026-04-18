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

function runCase(testCase, repoRoot, tmpDir) {
  const cmd = (testCase.command || []).map((arg) => String(arg).replace('{tmpDir}', tmpDir));
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

  return {
    id: testCase.id,
    family: testCase.family || 'unclassified',
    matrixFamily: testCase.matrixFamily,
    command: cmd,
    exitCode: proc.status,
    ok: errors.length === 0,
    errors,
    stdoutPreview: stdoutLines.slice(Math.max(0, stdoutLines.length - 8)).join('\n'),
    stderrPreview: stderrLines.slice(Math.max(0, stderrLines.length - 8)).join('\n')
  };
}

function runTiers(plan, repoRoot) {
  const results = { tier1: [], tier2: [], tier3: [] };
  const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp_tiers_'));
  try {
    for (const tierName of ['tier1', 'tier2', 'tier3']) {
      for (const testCase of ((plan.tiers || {})[tierName] || [])) {
        results[tierName].push(runCase(testCase, repoRoot, tempDir));
      }
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
  const tierResults = runTiers(plan, repoRoot);

  const summary = {
    tier1: summarizeTier(tierResults.tier1),
    tier2: summarizeTier(tierResults.tier2),
    tier3: summarizeTier(tierResults.tier3)
  };
  const matrixTracking = summarizeMatrixTracking(matrix, tierResults);

  const report = {
    generatedAt: new Date().toISOString(),
    planPath: path.relative(repoRoot, planPath),
    matrixPath: path.relative(repoRoot, matrixPath),
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

  fs.mkdirSync(path.dirname(outPath), { recursive: true });
  fs.writeFileSync(outPath, `${JSON.stringify(report, null, 2)}\n`, 'utf8');

  console.log('Tiered C++98 Report');
  console.log(`- Matrix weighted coverage: ${matrix.weightedPct}% (${matrix.done} done, ${matrix.partial} partial, ${matrix.missing} missing)`);
  console.log(`- Tier 1: ${summary.tier1.ok}/${summary.tier1.total} passed`);
  console.log(`- Tier 2: ${summary.tier2.ok}/${summary.tier2.total} passed`);
  console.log(`- Tier 3: ${summary.tier3.ok}/${summary.tier3.total} passed`);
  console.log(`- Matrix families tracked by tier cases: ${matrixTracking.trackedFamilies}/${matrixTracking.totalFamilies} (${matrixTracking.trackedPct}%)`);
  console.log(`- Report JSON: ${path.relative(repoRoot, outPath)}`);

  if (summary.tier1.failed > 0 || summary.tier2.failed > 0) process.exit(1);
  process.exit(0);
}

main();
