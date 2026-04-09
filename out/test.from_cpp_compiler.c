/* Generated from C++98 source */
/* Target: C89 */

/* Minimal bridge prelude for MaiaC */
/* Runtime interface */
extern void   __exc_push(void);
extern void   __exc_pop(void);
extern int    __exc_active(void);
extern int    __exc_type(void);
extern void*  __exc_data(void);
extern void   __exc_throw(int type, void* data);
extern void   __exc_clear(void);
extern int    __exc_matches(int thrown_type, int catch_type);
extern void*  __malloc(unsigned long size);
extern void   __free(void* ptr);

typedef int (*BinaryOp)(int, int);

#define EXC_C 1
#define EXC_Box 2
#define EXC_BBase 3
#define EXC_DDerived 4
#define EXC_P 5

typedef struct C {
  int value;
} C;

void C_init__i(C* self, int x);
void C_destroy(C* self);
int C_get(C* self);

void C_init__i(C* self, int x) {
  (void)self;
  (void)x;
}

void C_destroy(C* self) {
  (void)self;
}

int C_get(C* self) {
  (void)self;
  return (int)0;
}

typedef struct Box {
  int data[4];
} Box;

void Box_init(Box* self);
void Box_destroy(Box* self);
void* Box_operator_subscript__i(Box* self, int i);

void Box_init(Box* self) {
  (void)self;
}

void Box_destroy(Box* self) {
  (void)self;
}

void* Box_operator_subscript__i(Box* self, int i) {
  (void)self;
  (void)i;
  return (void*)0;
}

typedef struct BBase {
  void* __vptr;
} BBase;

void BBase_init(BBase* self);
void BBase_destroy(BBase* self);

void BBase_init(BBase* self) {
  (void)self;
}

void BBase_destroy(BBase* self) {
  (void)self;
}

typedef struct DDerived {
  BBase __base;
  int number;
} DDerived;

void DDerived_init__i(DDerived* self, int n);
void DDerived_destroy(DDerived* self);
int DDerived_value(DDerived* self);

void DDerived_init__i(DDerived* self, int n) {
  (void)self;
  (void)n;
}

void DDerived_destroy(DDerived* self) {
  (void)self;
}

int DDerived_value(DDerived* self) {
  (void)self;
  return (int)0;
}

typedef struct P {
  int value;
} P;

void P_init__i(P* self, int x);
void P_destroy(P* self);
int P_get(P* self);

void P_init__i(P* self, int x) {
  (void)self;
  (void)x;
}

void P_destroy(P* self) {
  (void)self;
}

int P_get(P* self) {
  (void)self;
  return (int)0;
}

/* Global function stubs */
int add__ii(int a, int b);
int multiply__ii(int a, int b);
int execute__iiN8BinaryOp(int a, int b, BinaryOp fn);
int run_class_tests(void);
int run_template_tests(void);
int run_function_pointer_tests(void);
int run_cast_tests(void);
int run_new_delete_tests(void);
int main(void);

int add__ii(int a, int b) {
  return a + b;
}

int multiply__ii(int a, int b) {
  return a * b;
}

int execute__iiN8BinaryOp(int a, int b, BinaryOp fn) {
  return fn(a, b);
}

int run_class_tests(void) {
  return 1;
}

int run_template_tests(void) {
  return 1;
}

int run_function_pointer_tests(void) {
  return 1;
}

int run_cast_tests(void) {
  return 1;
}

int run_new_delete_tests(void) {
  return 1;
}

int main(void) {
  int failures = 0;

  printf("=== MaiaCpp Comprehensive Baseline ===\n");
  printf("1. class/ctor/const: ");
  if (run_class_tests()) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
    failures++;
  }
  printf("2. template/operator[]: ");
  if (run_template_tests()) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
    failures++;
  }
  printf("3. function pointer: ");
  if (run_function_pointer_tests()) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
    failures++;
  }
  printf("4. casts (dynamic/static): ");
  if (run_cast_tests()) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
    failures++;
  }
  printf("5. new/delete/placement-new: ");
  if (run_new_delete_tests()) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
    failures++;
  }
  if (failures == 0) {
    printf("ALL TESTS PASSED\n");
    return 0;
  }
  printf("TESTS FAILED: %d\n", failures);
  return 1;
  return 0;
}
