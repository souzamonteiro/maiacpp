'use strict';
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const {execFileSync} = require('node:child_process');
const {Cpp98Compiler,CppToCTranspiler} = require('../cpp-compiler.js');
const {compileSource} = require('../../maiac/compiler/c-compiler.js');
const temporary = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-legacy-c89-'));
try {
  for (const name of ['03_functions/functions', '04_classes/classes', '06_inheritance/inheritance', '10_casts/casts']) {
    const input = path.resolve(__dirname, '../examples/suite', name + '.cpp');
    const output = path.join(temporary, path.basename(name) + '.c');
    // Match the documented CLI mode used to generate the versioned legacy examples.
    const code = new Cpp98Compiler(input,{legacyFunctionHints:true}).compile();
    fs.writeFileSync(output, code);
    execFileSync(process.env.CC || 'clang', ['--target=wasm32-unknown-unknown', '-std=c89', '-pedantic-errors', '-ffreestanding', '-Werror=implicit-function-declaration', '-fsyntax-only', output], {stdio:'pipe'});
    console.log(`PASS strict C89 generated legacy example: ${name}`);
    const reference=path.join(temporary,path.basename(name)+'-cpp');
    const nativeC=path.join(temporary,path.basename(name)+'-c');
    execFileSync(process.env.CXX||'clang++',['-std=c++98','-pedantic-errors',input,'-o',reference],{stdio:'pipe'});
    execFileSync(process.env.CC||'clang',['-std=c89','-pedantic-errors','-Werror=implicit-function-declaration',output,'-o',nativeC],{stdio:'pipe'});
    const expected=execFileSync(reference,{encoding:'utf8'});
    assert.equal(fs.readFileSync(path.join(path.dirname(input),'expected_output.txt'),'utf8'),expected,'Reference fixture must match original native C++98');
    assert.equal(execFileSync(nativeC,{encoding:'utf8'}),expected,'Generated C89 must preserve native source output');
    const compiled=compileSource(code,{sourcePath:output,resolveSystemIncludes:true});
    assert.ok(compiled.wasm,compiled.validationError?.message);
    let guest,printed='';
    const env=Object.fromEntries(['__exc_push','__exc_pop','__exc_active','__exc_type','__exc_data','__exc_throw','__exc_clear','__exc_matches','__malloc','__free'].map(name=>[name,()=>{throw new Error('Unexpected unused runtime bridge: '+name);} ]));
    env.printf=pointer=>{
      const bytes=new Uint8Array(guest.exports.memory.buffer);let text='';
      for(let p=Number(pointer);bytes[p];++p)text+=String.fromCharCode(bytes[p]);
      assert.ok(!text.includes('%'),'This fixture prints constant diagnostic lines');
      printed+=text;return text.length;
    };
    guest=new WebAssembly.Instance(new WebAssembly.Module(compiled.wasm),{env});
    assert.equal(guest.exports.main(),0);assert.equal(printed,expected);
    console.log(`PASS native C++98/C89/WASM output and source-derived fixture: ${name}`);

  }
  const input = path.join(temporary, 'static-void.cpp');
  fs.writeFileSync(input, '#include <stdio.h>\nstatic void set_value(int *value) {*value=7;}\nint main(){int value=0;set_value(&value);printf("%d\\n",value);return value!=7;}\n');
  const output = path.join(temporary, 'static-void.c');
  const code = new Cpp98Compiler(input).compile();fs.writeFileSync(output, code);
  const native = path.join(temporary, 'reference'), lowered = path.join(temporary, 'lowered');
  execFileSync(process.env.CXX || 'clang++', ['-std=c++98','-pedantic-errors',input,'-o',native], {stdio:'pipe'});
  execFileSync(process.env.CC || 'clang', ['-std=c89','-pedantic-errors','-Werror=implicit-function-declaration',output,'-o',lowered], {stdio:'pipe'});
  const expected = execFileSync(native, {encoding:'utf8'});assert.equal(expected,'7\n');
  assert.equal(execFileSync(lowered, {encoding:'utf8'}),expected);
  const compiled = compileSource(code, {sourcePath:output,resolveSystemIncludes:true});
  assert.ok(compiled.wasm,compiled.validationError?.message);
  const module = new WebAssembly.Module(compiled.wasm);
  const unused=['__exc_push','__exc_pop','__exc_active','__exc_type','__exc_data','__exc_throw','__exc_clear','__exc_matches','__malloc','__free'];
  assert.deepEqual(WebAssembly.Module.imports(module).map(i=>`${i.module}.${i.name}`),unused.concat('printf').map(name=>'env.'+name));
  // The legacy prelude declares these bridges even though this fixture does
  // not use them. Fail immediately if a compiler regression invokes one.
  const env=Object.fromEntries(unused.map(name=>[name,()=>{throw new Error('Unexpected legacy bridge call: '+name);} ]));
  let instance;const printed=[];
  env.printf=(format,value)=>{
    const bytes=new Uint8Array(instance.exports.memory.buffer);let text='';
    for(let p=Number(format);bytes[p];++p)text+=String.fromCharCode(bytes[p]);
    assert.equal(text,'%d\n');printed.push(Number(value));return 2;
  };
  instance=new WebAssembly.Instance(module,{env});
  assert.equal(instance.exports.main(),0);assert.deepEqual(printed,[7]);
  console.log('PASS static void/native C++98/C89/WASM parity and printf call ABI');

  // Isolate declaration scheduling from semantic analysis: constructor effects
  // must stay between scalar initializers, with earlier objects still visible.
  const emitter = new CppToCTranspiler({}, {});
  emitter.em.line('typedef struct Record {int value;} Record;');
  emitter.em.line('static void record_init(Record *r,int *trace,int tag){*trace=*trace*10+tag;r->value=*trace;}');
  emitter.em.line('int main(void) {');emitter.em.level++;
  const scopes=emitter.emitStructuredLocals([
    {type:'int',name:'trace',init:0},
    {type:'object',name:'first',className:'Record',initCallee:'record_init',initArgs:'&trace,1'},
    {type:'int',name:'afterFirst',initRaw:'trace'},
    {type:'object',name:'second',className:'Record',initCallee:'record_init',initArgs:'&trace,2'},
    {type:'int',name:'afterSecond',initRaw:'trace'}
  ]);
  emitter.em.line('if(trace!=12||afterFirst!=1||afterSecond!=12||first.value!=1||second.value!=12)return 1;');
  emitter.em.line('return 0;');emitter.closeStructuredLocalScopes(scopes);
  emitter.em.level--;emitter.em.line('}');
  const scheduled=emitter.em.code();
  const scheduledFile=path.join(temporary,'constructors.c'),scheduledExe=path.join(temporary,'constructors');
  fs.writeFileSync(scheduledFile,scheduled);
  execFileSync(process.env.CC||'clang',['-std=c89','-pedantic-errors',scheduledFile,'-o',scheduledExe],{stdio:'pipe'});
  execFileSync(scheduledExe);
  const scheduledModule=compileSource(scheduled,{sourcePath:scheduledFile});
  assert.ok(scheduledModule.wasm,scheduledModule.validationError?.message);
  assert.equal(new WebAssembly.Instance(new WebAssembly.Module(scheduledModule.wasm)).exports.main(),0);
  console.log('PASS C89 constructor/scalar initializer order, visibility and WASM execution');

  const shadowCpp=path.join(temporary,'late-shadow.cpp');
  fs.writeFileSync(shadowCpp,'int value=9;int main(){if(value!=9)return 1;int value=7;return value!=7;}\n');
  const shadowCode=new Cpp98Compiler(shadowCpp).compile();
  const shadowC=path.join(temporary,'late-shadow.c'),shadowExe=path.join(temporary,'late-shadow');
  fs.writeFileSync(shadowC,shadowCode);
  execFileSync(process.env.CC||'clang',['-std=c89','-pedantic-errors',shadowC,'-o',shadowExe],{stdio:'pipe'});
  execFileSync(shadowExe);
  const shadowModule=compileSource(shadowCode,{sourcePath:shadowC});
  assert.ok(shadowModule.wasm,shadowModule.validationError?.message);
  assert.equal(new WebAssembly.Instance(new WebAssembly.Module(shadowModule.wasm),{env}).exports.main(),0);
  console.log('PASS late declaration preserves earlier global binding in native C89 and WASM');


} finally {fs.rmSync(temporary,{recursive:true,force:true});}
