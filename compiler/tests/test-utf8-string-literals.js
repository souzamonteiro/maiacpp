'use strict';

const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const { execFileSync } = require('node:child_process');
const { compileSource } = require('../../maiac/compiler/c-compiler');
const { Cpp98Compiler } = require('../cpp-compiler');
const Parser = require('../cpp-parser');

// Numeric expectations are independent of the compiler's literal decoder.
const cases = [
  ['ASCII', '"abc"', [97, 98, 99]],
  ['Portuguese', '"ação ã"', [97,195,167,195,163,111,32,195,163]],
  ['decomposed', '"e\u0301"', [101,204,129]],
  ['CJK', '"中"', [228,184,173]],
  ['supplementary', '"😀"', [240,159,152,128]],
  ['scalar boundaries', '"\\177\u0080\u07ff\u0800\ud7ff\ue000\uffff\u{10000}\u{10ffff}"',
    [127,194,128,223,191,224,160,128,237,159,191,238,128,128,239,191,191,240,144,128,128,244,143,191,191]],
  ['raw accent', '"é"', [195,169]],
  ['hex byte', String.raw`"\xE9"`, [233]],
  ['octal byte', String.raw`"\351"`, [233]],
  ['mixed bytes', String.raw`"ç\377\xFF\0ã"`, [195,167,255,255,0,195,163]],
  ['adjacent', String.raw`"ç" "\x1" "F" "\1" "7" "😀"`, [195,167,1,70,1,55,240,159,152,128]],
  ['escaped punctuation', String.raw`"ã\"\\\n"`, [195,163,34,92,10]],
];
const definitions = cases.map(([, literal, bytes], index) => {
  const expected = [...bytes, 0];
  const check = (name) => expected.map((value, i) => `(unsigned char)${name}[${i}]!=${value}`).join('||');
  return `
struct Box${index} {char text[${expected.length}]; int guard;};
static struct Box${index} box${index}={${literal},77};
static char global${index}[]=${literal};
static const char *pointer${index}=${literal};
int check${index}(void) {
  char local[]=${literal};
  char exact[${expected.length}]=${literal};
  static char persistent[]=${literal};
  if(sizeof(local)!=${expected.length} || sizeof(global${index})!=${expected.length}
    || sizeof(${literal})!=${expected.length} || sizeof(persistent)!=${expected.length}) return 1;
  if(${check('local')} || ${check('global'+index)} || ${check('pointer'+index)}
    || ${check('persistent')} || ${check('box'+index+'.text')}) return 2;
  if(${bytes.map((value,i)=>`(unsigned char)exact[${i}]!=${value}`).join('||')}) return 3;
  if(box${index}.guard!=77) return 4;
  return 0;
}`;
}).join('\n');
const source = definitions + '\nextern "C" int test_entry(void){' + cases.map((_,i)=>`if(check${i}())return ${i+1};`).join('') + 'return 0;}';
const temporary = fs.mkdtempSync(path.join(os.tmpdir(), 'maiacpp-utf8-literals-'));
try {
  const file = path.join(temporary, 'native.cpp'), binary = path.join(temporary, 'native');
  fs.writeFileSync(file, source + '\nint main(void){return test_entry();}\n');
  execFileSync(process.env.CXX || 'clang++', ['-std=c++98','-pedantic-errors','-O1','-fsanitize=address,undefined',file,'-o',binary]);
  execFileSync(binary);
  for (const [name,literal] of cases) {
    const text = `int f(void){return sizeof(${literal});}`;
    const parser = new Parser(text);
    const tokens = parser.tokens.filter(token=>token.type==='StringLiteral');
    assert.ok(tokens.length, name);
    for (const token of tokens) {
      assert.equal(text.slice(token.start,token.end),token.value,name+' token span');
    }
    parser.parse();
  }
  const emitted = new Cpp98Compiler(file, {astStrict:true,legacyFunctionHints:false}).compile();
  assert.doesNotMatch(emitted,/structured-cstyle-body|structured-main|deterministic-fold/);
  const cFile = path.join(temporary,'emitted.c'), cBinary = path.join(temporary,'emitted');
  fs.writeFileSync(cFile,emitted);
  execFileSync(process.env.CC || 'clang', ['-std=c89','-pedantic-errors','-O1','-fsanitize=address,undefined',cFile,'-o',cBinary]);
  execFileSync(cBinary);
  const result = compileSource(emitted, {sourcePath:cFile,resolveSystemIncludes:true,validate:true});
  assert.ok(result.wasm,result.validationError?.message);
  const module = new WebAssembly.Module(result.wasm);
  // Plain struct helper declarations retain the standard allocation bridge.
  // Literal storage must not invoke it: rejecting callbacks enforce that bound.
  assert.deepEqual(WebAssembly.Module.imports(module), [
    {module:'env',name:'__malloc',kind:'function'},
    {module:'env',name:'__free',kind:'function'},
  ]);
  const unexpectedAllocation = () => { throw new Error('literal storage unexpectedly allocated'); };
  const instance = new WebAssembly.Instance(module,{env:{__malloc:unexpectedAllocation,__free:unexpectedAllocation}});
  // test_entry checks each internal C++ function without relying on name mangling.
  assert.equal(instance.exports.test_entry(),0);
  for (const surrogate of ['\ud800','\udfff','\ud800x','x\udfff']) {
    assert.throws(()=>new Parser(`int f(void){return sizeof("${surrogate}");}`),/Unexpected character/);
  }
  console.log(`PASS ${cases.length} UTF-8/escape cases: six storage forms, byte sizes, canaries, native C++98/emitted C89 ASan/UBSan and WASM; token spans and four malformed surrogates`);
} finally { fs.rmSync(temporary,{recursive:true,force:true}); }
