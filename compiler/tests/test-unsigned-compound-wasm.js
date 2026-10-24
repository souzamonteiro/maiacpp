'use strict';
const assert=require('node:assert/strict');
const fs=require('node:fs');
const os=require('node:os');
const path=require('node:path');
const {execFileSync}=require('node:child_process');
const {Cpp98Compiler}=require('../cpp-compiler');
const {compileSource}=require('../../maiac/compiler/c-compiler');
const temporary=fs.mkdtempSync(path.join(os.tmpdir(),'maiacpp-unsigned-compound-'));
const cases=[
 ['unsigned remainder','unsigned int x=4294967295U; x%=10; return x==5;'],
 ['unsigned division','unsigned int x=4294967295U; x/=10; return x==429496729U;'],
 ['unsigned shift','unsigned int x=4294967295U; x>>=1; return x==2147483647U;'],
 ['signed remainder','int x=-31; x%=10; return x==-1;'],
 ['mixed RHS','int x=-1; unsigned int y=10; x%=y; return x==5;'],
 ['unsigned pointer','unsigned int x=4294967295U; unsigned int* p=&x; *p%=10; return x==5;'],
 ['unsigned array','unsigned int a[2]; a[0]=4294967295U; a[1]=123; a[0]%=10; return a[0]==5 && a[1]==123;'],
 ['struct member','struct S {unsigned int value;}; S s; s.value=4294967295U; s.value%=10; return s.value==5;'],
 ['assignment value','unsigned int x=4294967295U; unsigned int y=(x%=10); return x==5 && y==5;'],
];
try {
 for(const [name,body] of cases){
  const input=path.join(temporary,'test.cpp'),output=path.join(temporary,'test.c'),native=path.join(temporary,'native');
  fs.writeFileSync(input,`extern "C" int sample(void){${body}}\nint main(void){return sample()!=1;}\n`);
  execFileSync(process.env.CXX||'clang++',['-std=c++98','-pedantic-errors',input,'-o',native]);execFileSync(native);
  const code=new Cpp98Compiler(input,{astStrict:true,legacyFunctionHints:false}).compile();fs.writeFileSync(output,code);
  assert.doesNotMatch(code,/structured-cstyle-body|structured-main|deterministic-fold/);
  execFileSync(process.env.CC||'clang',['--target=wasm32-unknown-unknown','-std=c89','-pedantic-errors','-ffreestanding','-fsyntax-only',output]);
  const linked=path.join(temporary,'linked.c');
  fs.writeFileSync(linked,`#define MAIACPP_USE_C_ALLOCATOR\n#include "${path.resolve(__dirname,'../../maiac/src/stdlib.c')}"\n#include "${output}"\n`);
  const compiled=compileSource(fs.readFileSync(linked,'utf8'),{sourcePath:linked,resolveSystemIncludes:true,validate:true});
  assert.ok(compiled.wasm,compiled.validationError?.message);
  const module=new WebAssembly.Module(Buffer.from(compiled.wasm));let instance;
  const env={__malloc:size=>instance.exports.malloc(size),__free:pointer=>instance.exports.free(pointer),
    __builtin_wasm_memory_size:()=>instance.exports.memory.buffer.byteLength/65536,
    __builtin_wasm_memory_grow:(_index,pages)=>instance.exports.memory.grow(pages)};
  for(const imported of WebAssembly.Module.imports(module))assert.ok(imported.module==='env'&&imported.name in env);
  instance=new WebAssembly.Instance(module,{env});
  assert.equal(instance.exports.sample(),1,name);assert.equal(instance.exports.main(),0,name);
  console.log('PASS native C++98/strict C89/actual WASM '+name);
 }
}finally{fs.rmSync(temporary,{recursive:true,force:true});}
