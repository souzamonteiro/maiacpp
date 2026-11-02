'use strict';
const assert=require('node:assert/strict'),fs=require('node:fs'),os=require('node:os'),path=require('node:path');
const {execFileSync}=require('node:child_process'),{Cpp98Compiler}=require('../cpp-compiler');
const root=path.resolve(__dirname,'../..'),{compileSource}=require(path.join(root,'maiac/compiler/c-compiler'));
const temporary=fs.mkdtempSync(path.join(os.tmpdir(),'maiacpp-parser-resources-'));
try{
 fs.copyFileSync(path.join(root,'maiaregex/maia_regex.h'),path.join(temporary,'maia_regex.h'));
 const grammar=path.join(temporary,'input.ebnf'),header=path.join(temporary,'parser.hpp'),source=path.join(temporary,'probe.cpp');
 fs.writeFileSync(grammar,'Input ::= ("a" "b" | "a") EOF\n<?TOKENS?>\nEOF ::= $\n');
 execFileSync('bash',[path.join(root,'maiacc/bin/tREx.sh'),'--ebnf','--language','cpp98','--to-xml',path.join(temporary,'grammar.xml'),grammar,header],{cwd:root});
 fs.copyFileSync(path.join(root,'maiacc/parser-generator/tests/fixtures/cpp98-parser-resources.cpp'),source);
 const emitted=path.join(temporary,'probe.c'),c=new Cpp98Compiler(source,{astStrict:true,legacyFunctionHints:false}).compile();assert.doesNotMatch(c,/stub-fallback|deterministic-fold/);fs.writeFileSync(emitted,c);
 // The wasm32 C89 translation unit supplies standard size_t for sizeof(size_t).
 execFileSync('clang',['--target=wasm32-unknown-unknown','-std=c89','-pedantic-errors','-ffreestanding','-include','stddef.h','-fsyntax-only',emitted]);
 const linked=path.join(temporary,'linked.c');fs.writeFileSync(linked,'#define MAIACPP_USE_C_ALLOCATOR\n'+[path.join(root,'maiac/src/stdlib.c'),path.join(root,'maiac/src/string.c'),path.join(root,'maiaregex/maia_regex.c'),emitted].map(p=>`#include "${p}"\n`).join(''));
 const r=compileSource(fs.readFileSync(linked,'utf8'),{sourcePath:linked,resolveSystemIncludes:true,validate:true,serializeParseTrees:false});assert.ok(r.wasm,r.validationError?.message);
 const module=new WebAssembly.Module(r.wasm);let instance;const env={__builtin_wasm_memory_size:i=>i?-1:instance.exports.memory.buffer.byteLength/65536,__builtin_wasm_memory_grow:(i,n)=>{if(i)return-1;try{return instance.exports.memory.grow(n);}catch{return-1;}},__malloc:n=>instance.exports.malloc(n),__free:p=>instance.exports.free(p)};
 for(const i of WebAssembly.Module.imports(module))assert.ok(i.module==='env'&&i.name in env,`Unexpected import ${i.module}.${i.name}`);instance=new WebAssembly.Instance(module,{env});const e=instance.exports,stack=e.__stack_ptr.value;
 for(let n=0;n<50;n++){assert.equal(e.maiaCCResourceProbe(),0);assert.equal(e.maiaCCResourceChecks(),32*(n+1));assert.equal(e.__stack_ptr.value,stack);}
 console.log('Generated resource API: 32 checks x50 runs / strict C89 / own-chain WASM, including real size_t overflow; guest stack restored');
}finally{fs.rmSync(temporary,{recursive:true,force:true});}
