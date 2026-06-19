'use strict';

const { test } = require('node:test');
const assert = require('node:assert/strict');
const path = require('node:path');

const { Cpp98Compiler } = require(path.resolve(__dirname, '..', 'cpp-compiler.js'));

test('normalizeGeneratedC89 rewrites leaked this references in generated prototype helpers', () => {
  const compiler = new Cpp98Compiler(__filename);
  const normalized = compiler.normalizeGeneratedC89(`
int maia_fn_Animal_prototype_accessPrivate(void) {
  return (int)(__maia_runtime_value_get_property((void*)(_animalPrivate(this)), (void*)"privateField"));
}

const char* maia_fn_Animal_prototype_getDescription(void) {
  return (const char*)(this->name + " is a " + this->species);
}
`);

  assert.doesNotMatch(normalized, /\bthis\b/, 'generated prototype helpers must not leave raw this in normalized C89');
  assert.match(
    normalized,
    /_animalPrivate\(0\)/,
    'bare this passed as receiver must be normalized inside generated prototype helpers'
  );
});
