'use strict';
const assert=require('node:assert/strict');const fs=require('node:fs');const os=require('node:os');const path=require('node:path');const {execFileSync}=require('node:child_process');
const {Cpp98Compiler}=require('../cpp-compiler.js');const {compileSource}=require('../../maiac/compiler/c-compiler.js');
const temp=fs.mkdtempSync(path.join(os.tmpdir(),'maiacpp-do-casts-'));
try {
 const cases=[
  ['digits',`char digits[10];unsigned int n=0,value=7;do {digits[n++]=static_cast<char>('0'+value%10);value/=10;}while(value);return digits[0]=='7' && n==1 ? 7 : -1;`,7],
  ['once',`int n=0;do {n+=static_cast<int>(3.9);}while(static_cast<int>(0));return n;`,3],
  ['nested',`int n=0,j=0,total=0,checks=0;do {++n;if(n==2)continue;j=0;do {++j;if(j==2)break;total+=static_cast<int>(n);}while(static_cast<int>(j)<3);}while(static_cast<int>(++checks)<3);return n*100+checks*10+total;`,334],
  ['literals',`const char* text="static_cast<char>(x)";int n=0;do {++n;}while(static_cast<int>(n)<2);return text[0]=='s'?n:0;`,2],
  ['nested-cast',`int n=0;char c=0;do {c=static_cast<char>('0'+static_cast<int>(++n));}while(static_cast<int>(c)<51);return c;`,51]
 ];
 for(const [name,body,expected] of cases){
  const source=path.join(temp,name+'.cpp'),output=path.join(temp,name+'.c'),native=path.join(temp,name+'-native');
  fs.writeFileSync(source,`extern "C" int sample(void){${body}}\nint main(void){return sample()!=${expected};}\n`);
  execFileSync(process.env.CXX||'clang++',['-std=c++98','-pedantic-errors',source,'-o',native],{stdio:'pipe'});execFileSync(native);
  const code=new Cpp98Compiler(source,{astStrict:true,legacyFunctionHints:false}).compile();fs.writeFileSync(output,code);
  assert.doesNotMatch(code,/structured-cstyle-body|structured-main|deterministic-fold/,'Do-while must use the AST expression/statement lowering');
  execFileSync(process.env.CC||'clang',['--target=wasm32-unknown-unknown','-std=c89','-pedantic-errors','-ffreestanding','-Werror=implicit-function-declaration','-fsyntax-only',output],{stdio:'pipe'});
  const compiled=compileSource(code,{sourcePath:output,resolveSystemIncludes:true,validate:true});assert.ok(compiled.wasm,compiled.validationError?.message);
  const instance=new WebAssembly.Instance(new WebAssembly.Module(compiled.wasm));assert.equal(instance.exports.sample(),expected);assert.equal(instance.exports.main(),0);
  console.log('PASS native C++98/strict C89/actual WASM do-while '+name);
 }
}finally{fs.rmSync(temp,{recursive:true,force:true});}
