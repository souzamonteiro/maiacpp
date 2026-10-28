'use strict';
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const {execFileSync} = require('node:child_process');
const {Cpp98Compiler} = require('../cpp-compiler');
const {compileSource} = require('../../maiac/compiler/c-compiler');

// Layout/member expectations come from native C++98 and explicit assertions,
// independently of generated C names or the compiler's semantic type map.
const cases = [
 ['inline local object',1,`extern "C" int sample(){struct Local {unsigned int value;} frame; frame.value=7; return frame.value==7;}`],
 ['separate local declaration',1,`extern "C" int sample(){struct Local {unsigned int value;}; Local frame; frame.value=7; return frame.value==7;}`],
 ['different function identities',2,`int f(){struct Local {int left;}; Local a; a.left=7; return a.left;}
 int g(){struct Local {int right[2];}; Local b; b.right[0]=3; b.right[1]=4;return b.right[0]+b.right[1];}
 extern "C" int sample(){return f()==7 && g()==7;}`],
 ['nested scope restoration',2,`extern "C" int sample(){struct Local {int value;}; Local a; a.value=5; int n=0;
 {struct Local {int other[2];}; Local b; b.other[0]=7;b.other[1]=8;n=b.other[0]+b.other[1];}
 Local c; c.value=11; return a.value==5 && n==15 && c.value==11 && sizeof(Local)==sizeof(a);}`],
 ['sibling block identities',2,`extern "C" int sample(){int result=0;
 {struct Local {int first;}; Local a; a.first=7;result=a.first;}
 {struct Local {int second;int third;}; Local b; b.second=2;b.third=5;result+=b.second+b.third;}
 return result==14;}`],
 ['global shadow and type sizeof',1,`struct Local {int global;}; static Local original={19};
 extern "C" int sample(){struct Local {int local;int guard;}; Local a; a.local=7;a.guard=77;
 return a.local==7 && a.guard==77 && original.global==19 && sizeof(Local)==8 && sizeof(original)==4;}`],
 ['multiple declarators arrays self pointers',1,`extern "C" int sample(){struct Node {int value;Node* next;int pad[2];} a,b;
 Node *p=&a; a.value=7;b.value=11;a.next=&b;a.pad[0]=19;a.pad[1]=77;
 return p->value==7 && p->next->value==11 && a.pad[0]==19 && a.pad[1]==77;}`],
 ['constant aggregate partial fill',1,`extern "C" int sample(){struct Pair {int value;int guard;}; Pair a={19,77}, b={7};
 const Pair* p=&a; return p->value==19 && p->guard==77 && b.value==7 && b.guard==0;}`],
 ['local static aggregate lifetime',1,`int tick(){struct Local{unsigned int value;int guard;};static Local state={19,77};++state.value;return state.value+state.guard;}
 extern "C" int sample(){int a=tick();int b=tick();return b==a+1;}`],
 ['decoder guard shape',1,`extern "C" int sample(){struct Frame{unsigned int before;unsigned int units[96];unsigned int middle;unsigned int offsets[96];unsigned int after;} frame;
 frame.before=1234567;frame.middle=7654321;frame.after=2468135;
 for(unsigned int i=0;i<96;++i){frame.units[i]=i+1;frame.offsets[i]=i+2;}
 return frame.before==1234567 && frame.middle==7654321 && frame.after==2468135 && frame.units[95]==96 && frame.offsets[95]==97;}`],
 ['outer local member type',2,`extern "C" int sample(){struct Item{int value;};{struct Box{Item item;int guard;};Box b;b.item.value=19;b.guard=77;return b.item.value==19&&b.guard==77;}}`],
 ['array objects and pointer sizeof casts',1,`extern "C" int sample(){struct Local{int value;};Local a[2];a[0].value=7;a[1].value=11;void* raw=&a[0];Local* p=static_cast<Local*>(raw);return p->value==7&&a[1].value==11&&sizeof(Local*)==sizeof(void*)&&sizeof(a)==8;}`],
 ['for scope restores outer type',2,`extern "C" int sample(){struct Local{int outer;};int sum=0;for(struct Local{int inner;} item={0};item.inner<3;++item.inner)sum+=item.inner;Local after;after.outer=7;return sum==3&&after.outer==7;}`],
 ['generated identity avoids source names',1,`struct __maia_local_struct_1_Local{int global;};extern "C" int sample(){struct Local{int value;} a;__maia_local_struct_1_Local b;a.value=7;b.global=11;return a.value==7&&b.global==11;}`],
 ['value hides injected type name',1,`extern "C" int sample(){struct Local{int pair[2];};int before=sizeof(Local);int Local=1;return before==8 && sizeof(Local)==4 && Local==1;}`],
 ['source identifiers do not become types',1,`extern "C" int sample(){struct Local{int Local;};Local a;a.Local=7;const char* text="Local";return a.Local==7 && text[0]=='L';}`],
];
const negative = [
 ['local method','struct Local{int value; int get(){return value;}} a;',/Unsupported local class/],
 ['local constructor','struct Local{Local(){} int value;} a;',/Unsupported local class/],
 ['local inheritance','struct Local: Base{int value;} a;',/Unsupported local class/],
 ['anonymous local type','struct {int value;} a;',/Unsupported local class/],
 ['nested local type','struct Outer{struct Inner{int value;};int x;} a;',/Unsupported local class/],
 ['dynamic local static','struct Local{int value;};static Local a={init()};',/Unsupported static local initialization/],
];
const temporary=fs.mkdtempSync(path.join(os.tmpdir(),'maiacpp-local-struct-types-'));
try {
 for(const [name,typeCount,body]of cases){
  const source=body+'\nint main(){return sample()!=1;}\n',input=path.join(temporary,'case.cpp'),output=path.join(temporary,'case.c');
  fs.writeFileSync(input,source);
  const native=path.join(temporary,'native'),cNative=path.join(temporary,'c-native');
  execFileSync(process.env.CXX||'clang++',['-std=c++98','-pedantic-errors','-O1','-fsanitize=address,undefined',input,'-o',native]);execFileSync(native);
  const compiler=new Cpp98Compiler(input,{astStrict:true,legacyFunctionHints:false});
  const analysis=compiler.analyze(source),locals=[...analysis.classes.values()].filter(type=>type.localSourceName);
  assert.equal(locals.length,typeCount,name+' AST-local type count');assert.equal(new Set(locals.map(type=>type.name)).size,typeCount,name+' unique identities');
  for(const fn of analysis.functions){const visit=node=>{if(!node)return;if(node.name==='classSpecifier')assert.ok(analysis.localClassNodes.has(node),name+' generated class node has semantic identity');for(const child of node.children||[])visit(child);};visit(fn.bodyAst);}
  const code=compiler.compile();assert.equal(compiler.compile(),code,name+' deterministic output');
  assert.doesNotMatch(code,/stub-fallback|structured-main|structured-cstyle-body|deterministic-fold/);fs.writeFileSync(output,code);
  const bridge='\n#include <stdlib.h>\nvoid* __malloc(unsigned long n){return malloc(n);}\nvoid __free(void* p){free(p);}\n';
  fs.writeFileSync(path.join(temporary,'native.c'),code+bridge);
  execFileSync(process.env.CC||'clang',['-std=c89','-pedantic-errors','-O1','-fsanitize=address,undefined',path.join(temporary,'native.c'),'-o',cNative]);execFileSync(cNative);
  execFileSync(process.env.CC||'clang',['--target=wasm32-unknown-unknown','-std=c89','-pedantic-errors','-ffreestanding','-Werror=implicit-function-declaration','-fsyntax-only',output]);
  const result=compileSource(code,{sourcePath:output,resolveSystemIncludes:true,validate:true});assert.ok(result.wasm,result.validationError?.message);
  const module=new WebAssembly.Module(result.wasm),unexpected=()=>{throw new Error('plain local struct unexpectedly allocated');};
  for(const item of WebAssembly.Module.imports(module))assert.ok(item.module==='env'&&['__malloc','__free'].includes(item.name),JSON.stringify(item));
  const e=new WebAssembly.Instance(module,{env:{__malloc:unexpected,__free:unexpected}}).exports;
  const stack=e.__stack_ptr.value,memory=e.memory.buffer.byteLength;assert.equal(e.main(),0,name);
  for(let repeat=0;repeat<40;++repeat){assert.equal(e.sample(),1,name);assert.equal(e.__stack_ptr.value,stack);assert.equal(e.memory.buffer.byteLength,memory);}
  console.log('PASS native C++98/emitted C89 ASan/UBSan and WASM: '+name);
 }
 for(const [name,declaration,diagnostic]of negative){const input=path.join(temporary,'negative.cpp');fs.writeFileSync(input,'struct Base{int base;};int init(){return 19;}int main(){'+declaration+'return 0;}');
  // These are valid C++98 inputs, deliberately beyond this bounded plain path.
  execFileSync(process.env.CXX||'clang++',['-std=c++98','-pedantic-errors','-fsyntax-only',input]);
  assert.throws(()=>new Cpp98Compiler(input,{astStrict:true,legacyFunctionHints:false}).compile(),diagnostic,name);console.log('PASS explicit unsupported diagnostic: '+name);
 }
 for(const body of [
  '{struct Local{int value;};} Local a;',
  'for(struct Local{int value;} a={0};a.value<1;++a.value){} Local b;',
 ]){const input=path.join(temporary,'out-of-scope.cpp');fs.writeFileSync(input,'int main(){'+body+'return 0;}');
  assert.throws(()=>new Cpp98Compiler(input,{astStrict:true,legacyFunctionHints:false}).compile(),/out of scope/);
 }
 console.log('PASS two invalid out-of-scope uses are explicitly rejected');
 console.log(`PASS ${cases.length} local plain struct programs and ${negative.length} unsupported diagnostics; AST ownership, deterministic names, lexical scopes/layout, repeated WASM stack/memory and no runtime allocation.`);
}finally{fs.rmSync(temporary,{recursive:true,force:true});}
