'use strict';

const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const {execFileSync} = require('node:child_process');
const {Cpp98Compiler} = require('../cpp-compiler');
const {compileSource} = require('../../maiac/compiler/c-compiler');

const cases = [
  ['collector-like nested globals', `
struct Inner {int active;};
struct Collector {unsigned int count; int overflow; Inner inner;};
static Collector tree;
extern "C" int sample(void) {
  Collector* alias=&tree;
  tree.count=0; tree.overflow=0; tree.inner.active=-1;
  do {tree.count+=static_cast<unsigned int>(1);} while(tree.count<3);
  alias->overflow=7;
  int* member=&tree.inner.active; *member=9;
  return tree.count==3 && tree.overflow==7 && tree.inner.active==9;
}`],
  ['initialized global and unsigned update', `
struct S {unsigned int value; int guard;};
static S g={4294967295U,77};
extern "C" int sample(void) {
  g.value/=10; g.value%=10; g.value>>=1;
  return g.value==4 && g.guard==77;
}`],
  ['static local lifetime', `
struct S {int value; int guard;};
int tick(void) {static S g={19,77}; ++g.value; return g.value+g.guard;}
extern "C" int sample(void) {
  int a=tick(); int b=tick(); return a==97 && b==98;
}`],
  ['global copy and callback', `
int add(int x) {return x+1;}
struct S {int value; int (*fn)(int);};
static S g; static S copy; static S* view;
extern "C" int sample(void) {
  g.value=7; g.fn=add; copy=g; view=&copy;
  return view->value==7 && copy.fn(6)==7;
}`],
];

const temporary=fs.mkdtempSync(path.join(os.tmpdir(),'maiacpp-static-members-'));
try {
  for (const [name, source] of cases) {
    const input=path.join(temporary,'test.cpp');
    const output=path.join(temporary,'test.c');
    const native=path.join(temporary,'native');
    fs.writeFileSync(input,source+'\nint main(void){return sample()!=1;}\n');
    execFileSync(process.env.CXX||'clang++',['-std=c++98','-pedantic-errors','-O1',
      '-fsanitize=address,undefined',input,'-o',native]);
    execFileSync(native);
    const code=new Cpp98Compiler(input,{astStrict:true,legacyFunctionHints:false}).compile();
    fs.writeFileSync(output,code);
    assert.doesNotMatch(code,/structured-cstyle-body|structured-main|deterministic-fold/);
    execFileSync(process.env.CC||'clang',['--target=wasm32-unknown-unknown','-std=c89',
      '-pedantic-errors','-ffreestanding','-Werror=implicit-function-declaration','-fsyntax-only',output]);
    const compiled=compileSource(code,{sourcePath:output,resolveSystemIncludes:true,validate:true});
    assert.ok(compiled.wasm,compiled.validationError?.message);
    assert.ok(WebAssembly.validate(compiled.wasm));
    const module=new WebAssembly.Module(compiled.wasm);
    assert.equal(new WebAssembly.Instance(module).exports.main(),0,name);
    assert.equal(new WebAssembly.Instance(module).exports.sample(),1,name);
    console.log(`PASS native C++98/ASan/UBSan -> strict C89 -> WASM: ${name}`);
  }
} finally {fs.rmSync(temporary,{recursive:true,force:true});}
