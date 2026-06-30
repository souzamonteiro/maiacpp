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

test('normalizeGeneratedC89 rewrites local struct arguments when emitted C signature expects pointers', () => {
  const compiler = new Cpp98Compiler(__filename);
  const normalized = compiler.normalizeGeneratedC89(`
typedef struct Vec2 {
  double x;
  double y;
} Vec2;

double Vec2_dot__pv(Vec2* self, Vec2* other);

int main(void) {
  Vec2 a;
  Vec2 unit;
  return Vec2_dot__pv(&a, unit) == 3.0;
}
`);

  assert.match(
    normalized,
    /Vec2_dot__pv\(&a, &unit\)/,
    'frame-backed struct locals must be passed by address when the lowered signature expects pointers'
  );
});
