/**
 * C Code Generation Module for C++98 → C Transpiler
 * 
 * Generates human-readable C code with proper indentation,
 * comments, and formatting aligned with ROADMAP specifications.
 */

const { CEmitter, mangle } = require('../cpp-compiler');

class CCodeGenerator {
  constructor(analysis) {
    this.analysis = analysis;
    this.emitter = new CEmitter();
    this.namespace = [];
    this.classId = {};  // Map class names to their type IDs
  }

  generate() {
    this.emitPrelude();
    this.emitExceptionDefinitions();
    this.emitTypeDefinitions();
    this.emitFunctionDeclarations();
    this.emitFunctionDefinitions();
    return this.emitter.code();
  }

  emitPrelude() {
    this.emitter.emit('/* Generated from C++98 source by maiac++ transpiler */');
    this.emitter.emit('/* Target: C89 */');
    this.emitter.emitBlank();

    // Standard headers
    this.emitter.emitComment('Standard headers');
    this.emitter.emit('#include <stddef.h>');
    this.emitter.emit('#include <stdint.h>');
    this.emitter.emit('#include <stdlib.h>');
    this.emitter.emit('#include <string.h>');
    this.emitter.emit('#include <stdio.h>');
    this.emitter.emitBlank();

    // Runtime interface
    this.emitter.emitComment('C++ Runtime Interface (Exception and Memory Management)');
    this.emitter.emit('extern void   __exc_push(void);');
    this.emitter.emit('extern void   __exc_pop(void);');
    this.emitter.emit('extern int    __exc_active(void);');
    this.emitter.emit('extern int    __exc_type(void);');
    this.emitter.emit('extern void*  __exc_data(void);');
    this.emitter.emit('extern void   __exc_throw(int type, void* data);');
    this.emitter.emit('extern void   __exc_clear(void);');
    this.emitter.emit('extern int    __exc_matches(int thrown_type, int catch_type);');
    this.emitter.emit('extern void*  __malloc(size_t size);');
    this.emitter.emit('extern void   __free(void* ptr);');
    this.emitter.emit('extern void   __memcpy(void* dest, const void* src, size_t size);');
    this.emitter.emit('extern void   __memzero(void* ptr, size_t size);');
    this.emitter.emitBlank();
  }

  emitExceptionDefinitions() {
    if (!this.analysis.classes || this.analysis.classes.size === 0) {
      return;
    }

    this.emitter.emitComment('Exception Type Identifiers');
    let typeId = 1;
    for (const [className] of this.analysis.classes) {
      this.classId[className] = typeId;
      this.emitter.emit(`#define EXC_${className} ${typeId}`);
      typeId++;
    }
    this.emitter.emitBlank();

    // Exception hierarchy table (if inheritance is used)
    this.emitter.emitComment('Exception Hierarchy (for catch-type matching)');
    this.emitter.emit('const int __exc_hierarchy[][2] = {');
    this.emitter.indent++;

    // TODO: Add base class relationships
    // For now, just add sentinel
    this.emitter.emit('{ 0, 0 }  /* sentinel */');

    this.emitter.indent--;
    this.emitter.emit('};');
    this.emitter.emitBlank();
  }

  emitTypeDefinitions() {
    if (!this.analysis.classes) {
      return;
    }

    // Forward declarations
    this.emitter.emitComment('Forward declarations of classes');
    for (const [className] of this.analysis.classes) {
      this.emitter.emit(`typedef struct ${className} ${className};`);
    }
    this.emitter.emitBlank();

    // Full definitions
    this.emitter.emitComment('Class Definitions');
    for (const [className, classInfo] of this.analysis.classes) {
      this.emitClassDefinition(className, classInfo);
    }
  }

  emitClassDefinition(className, classInfo) {
    this.emitter.emitComment(`Class ${className} (exception type ID: EXC_${className})`);
    this.emitter.emit(`typedef struct ${className} {`);
    this.emitter.indent++;

    // Base class fields (first, to enable safe upcasting)
    for (const base of classInfo.bases) {
      this.emitter.emit(`${base.name} __base;  /* base class */`);
    }

    // Virtual pointer (if class has virtual methods)
    if (classInfo.hasVtable) {
      this.emitter.emit(`struct ${className}_vtable* __vptr;`);
    }

    // Member variables
    for (const member of classInfo.members) {
      const typeStr = member.type || 'int';
      this.emitter.emit(`${typeStr} ${member.name};  /* ${member.access} */`);
    }

    this.emitter.indent--;
    this.emitter.emit(`} ${className};`);
    this.emitter.emitBlank();

    // Vtable definition (if needed)
    if (classInfo.hasVtable) {
      this.emitVirtualTable(className, classInfo);
    }
  }

  emitVirtualTable(className, classInfo) {
    this.emitter.emitComment(`Virtual table for ${className}`);
    this.emitter.emit(`typedef struct ${className}_vtable {`);
    this.emitter.indent++;

    // Virtual method pointers
    for (const method of classInfo.virtuals) {
      const returnType = method.returnType || 'void';
      const paramList = this.formatParamList(method.params, true);
      this.emitter.emit(`${returnType} (*${method.name})(${paramList});`);
    }

    this.emitter.indent--;
    this.emitter.emit(`} ${className}_vtable;`);
    this.emitter.emitBlank();
  }

  emitFunctionDeclarations() {
    // Constructor declarations
    this.emitter.emitComment('Constructors and Destructors');
    for (const [className, classInfo] of this.analysis.classes) {
      // Constructors
      for (const ctor of classInfo.constructors) {
        const args = this.formatParamList(ctor.params, false);
        const mangledName = mangle('init', ctor.params.map(p => p.type), className);
        this.emitter.emit(`void ${mangledName}(${className}* self${args ? ', ' + args : ''});`);
      }

      // Destructor
      if (classInfo.destructor) {
        const mangledName = mangle('destroy', [], className);
        this.emitter.emit(`void ${mangledName}(${className}* self);`);
      }

      // Regular methods
      for (const method of classInfo.methods) {
        if (method.name === className || method.name.startsWith('~')) {
          continue;  // Constructors/destructors already handled
        }

        const selfParam = `${className}* self`;
        const methodParams = this.formatParamList(method.params, false);
        const args = methodParams ? (selfParam + ', ' + methodParams) : selfParam;
        const mangledName = mangle(method.name, method.params.map(p => p.type), className);
        const returnType = method.returnType || 'void';

        this.emitter.emit(`${returnType} ${mangledName}(${args});`);
      }
    }
    this.emitter.emitBlank();
  }

  emitFunctionDefinitions() {
    // Constructor implementations
    this.emitter.emitComment('Constructor Implementations');
    for (const [className, classInfo] of this.analysis.classes) {
      for (const ctor of classInfo.constructors) {
        this.emitConstructor(className, classInfo, ctor);
      }
    }

    // Destructor implementations
    this.emitter.emitComment('Destructor Implementations');
    for (const [className, classInfo] of this.analysis.classes) {
      if (classInfo.destructor) {
        this.emitDestructor(className, classInfo);
      }
    }

    // Method implementations
    this.emitter.emitComment('Method Implementations');
    for (const [className, classInfo] of this.analysis.classes) {
      for (const method of classInfo.methods) {
        if (method.name !== className && !method.name.startsWith('~')) {
          this.emitMethod(className, classInfo, method);
        }
      }
    }
  }

  emitConstructor(className, classInfo, ctor) {
    const args = this.formatParamList(ctor.params, true);
    const mangledName = mangle('init', ctor.params.map(p => p.type), className);

    this.emitter.emit(`void ${mangledName}(${className}* self${args}) {`);
    this.emitter.indent++;

    // Initialize vtable if has virtual methods
    if (classInfo.hasVtable) {
      this.emitter.emit(`self->__vptr = &__${className}_vtable;`);
    }

    // TODO: Initialize member variables from parameters

    this.emitter.indent--;
    this.emitter.emit('}');
    this.emitter.emitBlank();
  }

  emitDestructor(className, classInfo) {
    const mangledName = mangle('destroy', [], className);
    this.emitter.emit(`void ${mangledName}(${className}* self) {`);
    this.emitter.indent++;

    // TODO: Call destructors for member objects
    // TODO: Support cleanup code from C++ destructor body

    this.emitter.indent--;
    this.emitter.emit('}');
    this.emitter.emitBlank();
  }

  emitMethod(className, classInfo, method) {
    const returnType = method.returnType || 'void';
    const selfParam = `${className}* self`;
    const methodParams = this.formatParamList(method.params, true);
    const args = methodParams ? (selfParam + ', ' + methodParams) : selfParam;
    const mangledName = mangle(method.name, method.params.map(p => p.type), className);

    this.emitter.emit(`${returnType} ${mangledName}(${args}) {`);
    this.emitter.indent++;

    // TODO: Emit method body

    this.emitter.indent--;
    this.emitter.emit('}');
    this.emitter.emitBlank();
  }

  formatParamList(params, includeType = false) {
    if (!params || params.length === 0) {
      return '';
    }

    return params.map(p => {
      return includeType ? `${p.type} ${p.name}` : p.name;
    }).join(', ');
  }
}

module.exports = { CCodeGenerator };
