#!/usr/bin/env node
const fs = require('fs');
const path = require('path');
const { spawnSync } = require('child_process');

function parseArgs(argv) {
  const out = {
    fixturesDir: 'tests/fixtures',
    parser: 'cpp-compiler.js',
    mode: 'compiler',
    parserExtraArg: [],
    cases: ''
  };

  for (let i = 0; i < argv.length; i += 1) {
    const a = argv[i];
    if (a === '--fixtures-dir' && i + 1 < argv.length) out.fixturesDir = argv[++i];
    else if (a === '--parser' && i + 1 < argv.length) out.parser = argv[++i];
    else if (a === '--mode' && i + 1 < argv.length) out.mode = argv[++i];
    else if (a === '--parser-extra-arg' && i + 1 < argv.length) out.parserExtraArg.push(argv[++i]);
    else if (a === '--cases' && i + 1 < argv.length) out.cases = argv[++i];
  }
  return out;
}

function listExpectFiles(fixturesDir) {
  return fs.readdirSync(fixturesDir)
    .filter((n) => n.endsWith('.expect.json'))
    .sort();
}

function loadFixtureSpecs(fixturesDir) {
  const specs = [];
  for (const expectName of listExpectFiles(fixturesDir)) {
    const caseStem = expectName.slice(0, -'.expect.json'.length);
    const inputFile = path.join(fixturesDir, `${caseStem}.cpp`);
    if (!fs.existsSync(inputFile)) {
      throw new Error(`Missing input for fixture '${caseStem}': ${inputFile}`);
    }
    const expectFile = path.join(fixturesDir, expectName);
    const spec = JSON.parse(fs.readFileSync(expectFile, 'utf8'));
    specs.push([caseStem, inputFile, spec]);
  }
  return specs;
}

function countTag(xmlText, tag) {
  return xmlText.split(`<${tag}`).length - 1;
}

function runFixture(parserPath, inputFile, spec, mode, parserExtraArgs) {
  const shouldParse = Boolean(spec.shouldParse !== false);
  const cmd = ['node', parserPath, inputFile, ...(parserExtraArgs || [])];
  const proc = spawnSync(cmd[0], cmd.slice(1), { encoding: 'utf8' });

  const stdout = proc.stdout || '';
  const stderr = proc.stderr || '';
  const errors = [];

  if (mode === 'xml-parser') {
    if (shouldParse) {
      if (proc.status !== 0) {
        errors.push(`expected parse success, got exit code ${proc.status}`);
        const firstErr = stderr.trim().split(/\r?\n/)[0];
        if (firstErr) errors.push(`stderr: ${firstErr}`);
        return errors;
      }

      const xmlText = stdout;
      if (!xmlText.trim()) {
        errors.push('parser returned empty XML');
        return errors;
      }

      if (!xmlText.includes('<translationUnit')) {
        errors.push('unexpected root tag: translationUnit not found');
      }
      if (!xmlText.includes('<EOF')) {
        errors.push('missing EOF marker');
      }

      for (const snippet of spec.mustContain || []) {
        if (!xmlText.includes(snippet)) {
          errors.push(`missing required snippet in XML: ${snippet}`);
        }
      }

      const minCounts = spec.minTagCount || {};
      for (const [tag, minCount] of Object.entries(minCounts)) {
        const actual = countTag(xmlText, String(tag));
        if (actual < Number(minCount)) {
          errors.push(`expected at least ${minCount} occurrences of <${tag}>, got ${actual}`);
        }
      }
    } else {
      if (proc.status === 0) {
        errors.push('expected parse failure, got exit code 0');
      }
      for (const snippet of spec.stderrContains || []) {
        if (!stderr.includes(snippet)) {
          errors.push(`expected stderr to contain: ${snippet}`);
        }
      }
    }
    return errors;
  }

  if (shouldParse) {
    if (proc.status !== 0) {
      errors.push(`expected compile success, got exit code ${proc.status}`);
    }
    if (!stdout.includes('Parser: ok')) {
      errors.push("expected parser success marker: 'Parser: ok'");
    }
    if (stdout.includes('Parser falhou')) {
      errors.push('unexpected parser failure marker in output');
    }
  } else if (!stdout.includes('Parser falhou')) {
    errors.push("expected parser failure marker: 'Parser falhou'");
  }

  for (const snippet of spec.mustContainCompiler || []) {
    if (!stdout.includes(snippet)) {
      errors.push(`missing required snippet in output: ${snippet}`);
    }
  }

  for (const snippet of spec.mustNotContainCompiler || []) {
    if (stdout.includes(snippet)) {
      errors.push(`unexpected snippet in output: ${snippet}`);
    }
  }

  for (const snippet of spec.stderrContains || []) {
    if (!stderr.includes(snippet)) {
      errors.push(`expected stderr to contain: ${snippet}`);
    }
  }

  return errors;
}

function main() {
  try {
    const args = parseArgs(process.argv.slice(2));
    const root = process.cwd();
    const fixturesDir = path.resolve(root, args.fixturesDir);
    const parserPath = path.resolve(root, args.parser);

    if (!fs.existsSync(parserPath)) {
      console.log(`[fail] parser not found: ${parserPath}`);
      process.exit(2);
    }

    if (!['compiler', 'xml-parser'].includes(args.mode)) {
      console.log(`[fail] invalid mode: ${args.mode}`);
      process.exit(2);
    }

    let specs = loadFixtureSpecs(fixturesDir);
    const requestedCases = String(args.cases || '').split(',').map((s) => s.trim()).filter(Boolean);
    if (requestedCases.length > 0) {
      const reqSet = new Set(requestedCases);
      specs = specs.filter((item) => reqSet.has(item[0]));
      const found = new Set(specs.map((item) => item[0]));
      const missing = requestedCases.filter((c) => !found.has(c));
      if (missing.length > 0) {
        console.log(`[fail] requested fixture(s) not found: ${missing.join(', ')}`);
        process.exit(2);
      }
    }

    if (specs.length === 0) {
      console.log(`[fail] no fixtures found in ${fixturesDir}`);
      process.exit(2);
    }

    let failures = 0;
    for (const [caseStem, inputFile, spec] of specs) {
      const errors = runFixture(parserPath, inputFile, spec, args.mode, args.parserExtraArg);
      if (errors.length > 0) {
        failures += 1;
        console.log(`[fail] ${caseStem}`);
        for (const err of errors) console.log(`  - ${err}`);
      } else {
        console.log(`[ok] ${caseStem}`);
      }
    }

    console.log('');
    if (failures > 0) {
      console.log(`Fixture failures: ${failures}/${specs.length}`);
      process.exit(1);
    }

    console.log(`All fixtures passed: ${specs.length}`);
    process.exit(0);
  } catch (err) {
    console.error(`[fail] ${err && err.message ? err.message : String(err)}`);
    process.exit(2);
  }
}

main();
