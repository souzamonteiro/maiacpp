#!/usr/bin/env node
const fs = require('fs');
const path = require('path');

function parseArgs(argv) {
  const out = {
    generatedDir: './out/reports/cpp-vs-c',
    sources: []
  };

  for (let i = 0; i < argv.length; i += 1) {
    const a = argv[i];
    if (a === '--generated-dir' && i + 1 < argv.length) {
      out.generatedDir = argv[++i];
    } else if (a === '--source' && i + 1 < argv.length) {
      out.sources.push(argv[++i]);
    }
  }

  return out;
}

function main() {
  const args = parseArgs(process.argv.slice(2));
  const repoRoot = process.cwd();
  const generatedDir = path.resolve(repoRoot, args.generatedDir);

  if (args.sources.length === 0) {
    console.log('[fail] no sources provided (use --source <file.cpp>)');
    process.exit(2);
  }

  if (!fs.existsSync(generatedDir)) {
    console.log(`[fail] generated dir not found: ${path.relative(repoRoot, generatedDir)}`);
    process.exit(2);
  }

  const disallowed = [
    'no-supported-lowering',
    'stub-fallback'
  ];

  const findings = [];
  const missing = [];

  for (const source of args.sources) {
    const sourcePath = path.resolve(repoRoot, source);
    const stem = path.parse(sourcePath).name;
    const generatedPath = path.join(generatedDir, `${stem}.generated.c`);

    if (!fs.existsSync(generatedPath)) {
      missing.push(path.relative(repoRoot, generatedPath));
      continue;
    }

    const text = fs.readFileSync(generatedPath, 'utf8');
    const hit = disallowed.filter((token) => text.includes(token));
    if (hit.length > 0) {
      findings.push({
        generated: path.relative(repoRoot, generatedPath),
        tokens: hit
      });
    }
  }

  if (missing.length > 0) {
    console.log('[fail] missing generated C artifacts:');
    for (const item of missing) console.log(`- ${item}`);
    process.exit(1);
  }

  if (findings.length > 0) {
    console.log('[fail] disallowed lowering markers found:');
    for (const item of findings) {
      console.log(`- ${item.generated}: ${item.tokens.join(', ')}`);
    }
    process.exit(1);
  }

  console.log(`[ok] no disallowed lowering markers in ${args.sources.length} generated file(s)`);
  process.exit(0);
}

main();
