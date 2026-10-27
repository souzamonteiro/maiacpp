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
  ['static scalar constant expression', `
int tick(void) {static int n=(2+3)*4-1; return ++n;}
extern "C" int sample(void) {int a=tick(); int b=tick(); return a==20 && b==21;}
`],
  ['static array partial initialization', `
int tick(void) {static int a[3]={19,77}; ++a[0]; return a[0]+a[1]+a[2];}
extern "C" int sample(void) {int a=tick(); int b=tick(); return a==97 && b==98;}
`],
  ['static zero storage and aliases', `
struct S {int value; int guard;};
int tick(void) {static S g; int* p=&g.value; ++*p; return g.value+g.guard;}
extern "C" int sample(void) {int a=tick(); int b=tick(); return a==1 && b==2;}
`],
  ['nested block static storage', `
int tick(void) {int result=0; {static int n=4; result=++n;} return result;}
extern "C" int sample(void) {int a=tick(); int b=tick(); return a==5 && b==6;}
`],
  ['static narrow floating and cast constants', `
struct S {unsigned char value; double weight;};
int tick(void) {static S g={static_cast<unsigned char>(19),2.0}; ++g.value; return g.value+static_cast<int>(g.weight);}
extern "C" int sample(void) {int a=tick(); int b=tick(); return a==22 && b==23;}
`],
  ['static const reads', `
int tick(void) {static const int n=19; return n;}
extern "C" int sample(void) {return tick()==19 && tick()==19;}
`],
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
const failures=[];
try {
  for (const [name, source] of cases) {
    try {
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
      // Independently execute the emitted C89 with the native allocation bridge.
      const nativeC=path.join(temporary,'emitted-native.c');
      const nativeCBinary=path.join(temporary,'emitted-native');
      fs.writeFileSync(nativeC,code+'\n#include <stdlib.h>\nvoid* __malloc(unsigned long size){return malloc(size);}\nvoid __free(void* p){free(p); }\n');
      execFileSync(process.env.CC||'clang',['-std=c89','-pedantic-errors','-O1',
        '-fsanitize=address,undefined',nativeC,'-o',nativeCBinary]);
      execFileSync(nativeCBinary);
      // Link the real guest allocator used by the generated global-object runtime.
      const linked=path.join(temporary,'linked.c');
      fs.writeFileSync(linked,`#define MAIACPP_USE_C_ALLOCATOR\n#include "${path.resolve(__dirname,'../../maiac/src/stdlib.c')}"\n#include "${output}"\n`);
      const compiled=compileSource(fs.readFileSync(linked,'utf8'),{sourcePath:linked,resolveSystemIncludes:true,validate:true});
      assert.ok(compiled.wasm,compiled.validationError?.message);
      assert.ok(WebAssembly.validate(compiled.wasm));
      const module=new WebAssembly.Module(compiled.wasm);
      function instantiate() {
        let instance;
        const env={
          __malloc:size=>instance.exports.malloc(size),
          __free:pointer=>instance.exports.free(pointer),
          __builtin_wasm_memory_size:()=>instance.exports.memory.buffer.byteLength/65536,
          __builtin_wasm_memory_grow:(_index,pages)=>instance.exports.memory.grow(pages),
        };
        for(const imported of WebAssembly.Module.imports(module))
          assert.ok(imported.module==='env' && imported.name in env,JSON.stringify(imported));
        instance=new WebAssembly.Instance(module,{env});
        return instance;
      }
      assert.equal(instantiate().exports.main(),0,name);
      assert.equal(instantiate().exports.sample(),1,name);
      console.log(`PASS native C++98/ASan/UBSan -> strict C89 -> WASM: ${name}`);
    } catch(error) {
      failures.push(name+': '+error.message);
      console.error('FAIL '+name+': '+error.message);
    }
  }
  const dynamic=path.join(temporary,'dynamic.cpp');
  fs.writeFileSync(dynamic,'int init(void){return 19;} int tick(void){static int n=init(); return ++n;} int main(void){return tick()!=20;}');
  const dynamicNative=path.join(temporary,'dynamic-native');
  execFileSync(process.env.CXX||'clang++',['-std=c++98','-pedantic-errors',dynamic,'-o',dynamicNative]);
  execFileSync(dynamicNative);
  assert.throws(()=>new Cpp98Compiler(dynamic,{astStrict:true,legacyFunctionHints:false}).compile(),
    /Unsupported static local initialization.*dynamic initialization is not implemented/);
  console.log('PASS supported C++ dynamic initialization receives explicit unsupported diagnostic');
  assert.equal(failures.length,0,failures.join('\n'));
} finally {fs.rmSync(temporary,{recursive:true,force:true});}
