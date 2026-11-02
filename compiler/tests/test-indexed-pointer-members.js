'use strict';
const assert=require('node:assert/strict'),fs=require('node:fs'),os=require('node:os'),path=require('node:path');
const {execFileSync}=require('node:child_process'),{Cpp98Compiler}=require('../cpp-compiler'),{compileSource}=require('../../maiac/compiler/c-compiler');
const temporary=fs.mkdtempSync(path.join(os.tmpdir(),'maiacpp-pointer-members-'));
try{
 for(const type of ['char','int','Element']){
  const source=`struct Element {int a;int b;}; struct Token {int guard;${type}* value;int end;}; static Token tokens[3];static ${type} data[8];extern "C" int sample(){tokens[1].value=data+5;return tokens[1].value-data==5 && data-tokens[1].value==-5 && (tokens[1].value+2)-data==7 && (tokens[1].value-3)-data==2;}`;
  const file=path.join(temporary,'probe.cpp'),emitted=path.join(temporary,'probe.c'),native=path.join(temporary,'native');fs.writeFileSync(file,source+'\nint main(){return sample()!=1;}\n');
  execFileSync('clang++',['-std=c++98','-pedantic-errors','-O1','-fsanitize=address,undefined',file,'-o',native]);execFileSync(native);
  fs.writeFileSync(file,source);const code=new Cpp98Compiler(file,{astStrict:true,legacyFunctionHints:false}).compile();assert.doesNotMatch(code,/stub-fallback|deterministic-fold/);fs.writeFileSync(emitted,code);
  execFileSync('clang',['--target=wasm32-unknown-unknown','-std=c89','-pedantic-errors','-ffreestanding','-fsyntax-only',emitted]);
  const linked=path.join(temporary,'linked.c');
  fs.writeFileSync(linked,[path.resolve(__dirname,'../../maiac/src/stdlib.c'),path.resolve(__dirname,'../../maiac/src/string.c'),emitted].map(p=>`#include "${p}"\n`).join(''));
  const r=compileSource(fs.readFileSync(linked,'utf8'),{sourcePath:linked,resolveSystemIncludes:true,validate:true,serializeParseTrees:false});assert.ok(r.wasm,r.validationError?.message);
  const module=new WebAssembly.Module(r.wasm);let instance;
  const env={__builtin_wasm_memory_size:i=>i?-1:instance.exports.memory.buffer.byteLength/65536,__builtin_wasm_memory_grow:(i,n)=>{if(i)return-1;try{return instance.exports.memory.grow(n);}catch{return-1;}},__malloc:n=>instance.exports.malloc(n),__free:p=>instance.exports.free(p)};
  for(const i of WebAssembly.Module.imports(module))assert.ok(i.module==='env'&&i.name in env);instance=new WebAssembly.Instance(module,{env});assert.equal(instance.exports.sample(),1,type);console.log(`PASS C++98 native/ASan/UBSan -> strict C89 -> WASM pointer member ${type}`);
 }
}finally{fs.rmSync(temporary,{recursive:true,force:true});}
