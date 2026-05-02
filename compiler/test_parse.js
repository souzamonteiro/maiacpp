const {Cpp98Compiler} = require('./cpp-compiler');
const fs = require('fs');
const path = require('path');

const filePath = '/Volumes/External_SSD/Documentos/Projects/maiacpp/compiler/examples/programming_in_cpp_course_en/05_inheritance/dinosaurs.cpp';
console.log('Creating compiler...');
const compiler = new Cpp98Compiler(filePath, {verbose: true});
console.log('Compiler created');
try {
  const code = compiler.compile();
  console.log('Compilation successful');
  fs.writeFileSync('/tmp/test_dinosaurs.c', code);
  console.log('Written to /tmp/test_dinosaurs.c');
} catch (e) {
  console.error('Error:', e.message);
  process.exit(1);
}
