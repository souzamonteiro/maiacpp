(module
  ;; C++98 Exception and Memory Runtime
  ;; 
  ;; Provides:
  ;;   - Exception state management
  ;;   - Basic malloc/free
  ;;   - Exception type hierarchy matching

  ;; ========================================================================
  ;; EXCEPTION STATE GLOBALS
  ;; ========================================================================

  ;; Is an exception currently being thrown?
  (global $exc_active (mut i32) (i32.const 0))

  ;; Type ID of the current exception
  (global $exc_type (mut i32) (i32.const 0))

  ;; Pointer to exception data object (in memory)
  (global $exc_data (mut i32) (i32.const 0))

  ;; Current depth in try stack
  (global $exc_depth (mut i32) (i32.const 0))

  ;; ========================================================================
  ;; MEMORY & HEAP
  ;; ========================================================================

  (memory (export "memory") 1)  ;; 64KB

  ;; Heap pointer starts after data segment (safe space)
  (global $heap_ptr (mut i32) (i32.const 4096))

  ;; ========================================================================
  ;; EXCEPTION PUSH/POP (Try Block Management)
  ;; ========================================================================

  (func $__exc_push (export "__exc_push")
    ;; Enter a try block
    (global.set $exc_depth
      (i32.add (global.get $exc_depth) (i32.const 1)))
  )

  (func $__exc_pop (export "__exc_pop")
    ;; Exit a try block
    (global.set $exc_depth
      (i32.sub (global.get $exc_depth) (i32.const 1)))
  )

  ;; ========================================================================
  ;; EXCEPTION STATE QUERIES
  ;; ========================================================================

  (func $__exc_active (export "__exc_active") (result i32)
    ;; Returns: 1 if exception is active, 0 otherwise
    (global.get $exc_active)
  )

  (func $__exc_type (export "__exc_type") (result i32)
    ;; Returns: type ID of current exception (or 0 if none)
    (global.get $exc_type)
  )

  (func $__exc_data (export "__exc_data") (result i32)
    ;; Returns: pointer to exception data object
    (global.get $exc_data)
  )

  ;; ========================================================================
  ;; THROW EXCEPTION
  ;; ========================================================================

  (func $__exc_throw (export "__exc_throw")
    (param $type i32)
    (param $data i32)
    ;; Throw an exception with given type and data pointer
    ;; Note: This function never returns to caller
    (global.set $exc_type (local.get $type))
    (global.set $exc_data (local.get $data))
    (global.set $exc_active (i32.const 1))
    ;; WASM has no longjmp, so control returns to caller,
    ;; who must check __exc_active() after every call inside try blocks
    (return)
  )

  ;; ========================================================================
  ;; CLEAR EXCEPTION
  ;; ========================================================================

  (func $__exc_clear (export "__exc_clear")
    ;; Clear exception state after catch handler completes
    (global.set $exc_active (i32.const 0))
    (global.set $exc_type (i32.const 0))
    (global.set $exc_data (i32.const 0))
  )

  ;; ========================================================================
  ;; EXCEPTION TYPE MATCHING
  ;; ========================================================================

  ;; Simpler version: just check exact match
  ;; (More complex inheritance checking requires symbol table from C code)
  (func $__exc_matches (export "__exc_matches")
    (param $thrown_type i32)
    (param $catch_type i32)
    (result i32)
    ;; Returns: 1 if thrown_type matches catch_type (or is derived), 0 otherwise
    ;; 
    ;; For now: just exact match or check against __exc_hierarchy table
    ;; (The hierarchy table is built by the C++ compiler)
    
    ;; TODO: Implement catch-type hierarchy lookup
    ;; For now, just return 1 if types match
    (i32.eq (local.get $thrown_type) (local.get $catch_type))
  )

  ;; ========================================================================
  ;; MEMORY ALLOCATION (VERY BASIC)
  ;; ========================================================================

  (func $__malloc (export "__malloc")
    (param $size i32)
    (result i32)
    ;; Allocate $size bytes from heap
    ;; Returns: pointer to allocated block
    (local $ptr i32)
    
    ;; Get current heap pointer
    (local.set $ptr (global.get $heap_ptr))
    
    ;; Advance heap by requested size
    (global.set $heap_ptr
      (i32.add (global.get $heap_ptr) (local.get $size)))
    
    ;; Return old pointer
    (local.get $ptr)
  )

  (func $__free (export "__free")
    (param $ptr i32)
    ;; Stub: actual free is complex in a bump allocator
    ;; This is acceptable for many C++ programs that don't delete much
    ;; TODO: Implement real free list if needed
  )

  ;; ========================================================================
  ;; MEMORY COPY (for pass-by-value)
  ;; ========================================================================

  (func $__memcpy (export "__memcpy")
    (param $dest i32)
    (param $src i32)
    (param $size i32)
    (local $i i32)
    ;; Copy $size bytes from src to dest
    (local.set $i (i32.const 0))
    (block $break
      (loop $loop
        ;; Exit if i >= size
        (br_if $break (i32.ge_u (local.get $i) (local.get $size)))
        
        ;; Copy one byte
        (i32.store8
          (i32.add (local.get $dest) (local.get $i))
          (i32.load8_u
            (i32.add (local.get $src) (local.get $i))))
        
        ;; i++
        (local.set $i (i32.add (local.get $i) (i32.const 1)))
        (br $loop)
      )
    )
  )

  ;; ========================================================================
  ;; ZERO-INITIALIZE (for constructors)
  ;; ========================================================================

  (func $__memzero (export "__memzero")
    (param $ptr i32)
    (param $size i32)
    (local $i i32)
    ;; Zero $size bytes at ptr
    (local.set $i (i32.const 0))
    (block $break
      (loop $loop
        (br_if $break (i32.ge_u (local.get $i) (local.get $size)))
        (i32.store8
          (i32.add (local.get $ptr) (local.get $i))
          (i32.const 0))
        (local.set $i (i32.add (local.get $i) (i32.const 1)))
        (br $loop)
      )
    )
  )
)
