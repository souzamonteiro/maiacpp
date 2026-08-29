#!/usr/bin/env node
'use strict';

const assert = require('assert');
const Parser = require('../cpp-parser');
const { ParseTreeCollector } = require('../parse-tree-collector');

function buildStressSource(statementCount) {
  const statements = [];
  for (let i = 0; i < statementCount; i += 1) {
    statements.push(
      `value = (double)((value + ${i}) * 2) + (flag ? ${i} : ${i + 1});`
    );
  }
  return [
    'int main() {',
    '  double value = 0;',
    '  int flag = 1;',
    ...statements.map((statement) => `  ${statement}`),
    '  return (int)value;',
    '}'
  ].join('\n');
}

const source = buildStressSource(240);
const collector = new ParseTreeCollector();
const startedAt = Date.now();
collector.parse(new Parser(source, collector), 'expression performance fixture');
const elapsedMs = Date.now() - startedAt;

assert.ok(collector.root, 'expected a parse tree for the expression stress fixture');
assert.ok(
  elapsedMs < 5000,
  `expression parser regression: expected under 5000ms, got ${elapsedMs}ms`
);

console.log(`[ok] parser expression performance: ${elapsedMs}ms`);
