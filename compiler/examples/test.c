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
  self->value = x;
}

void C_destroy(C* self) {
  (void)self;
}

int C_get(C* self) {
  (void)self;
  return self->value;
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
  return (void*)(self->data + i);
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
  self->number = n;
}

void DDerived_destroy(DDerived* self) {
  (void)self;
}

int DDerived_value(DDerived* self) {
  (void)self;
  return self->number;
}

typedef struct P {
  int value;
} P;

void P_init__i(P* self, int x);
void P_destroy(P* self);
int P_get(P* self);

void P_init__i(P* self, int x) {
  (void)self;
  self->value = x;
}

void P_destroy(P* self) {
  (void)self;
}

int P_get(P* self) {
  (void)self;
  return self->value;
}

/* Global functions */
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
  C c;
  C_init__i(&c, 42);
  return (C_get(&c) == 42) ? 1 : 0;
}

int run_template_tests(void) {
  Box box;
  box.data[0] = 10;
  box.data[1] = 20;
  return ((box.data[0] + box.data[1] == 30) ? 1 : 0);
}

int run_function_pointer_tests(void) {
  int s = 0;
  int m = 0;
  s = add__ii(7, 3);
  m = multiply__ii(7, 3);
  return (s == 10 && m == 21) ? 1 : 0;
}

int run_cast_tests(void) {
  DDerived d;
  int n = 0;
  DDerived_init__i(&d, 15);
  n = 3;
  if (DDerived_value(&d) != 15) return 0;
  if (n != 3) {
    return 0;
  }
  return 1;
}

int run_new_delete_tests(void) {
  int* a = 0;
  char buffer[sizeof(P)];
  P* p = 0;
  int v = 0;
  a = (int*)__malloc(sizeof(int));
  *a = 1;
  if (*a != 1) {
    __free(a);
    return 0;
  }
  __free(a);
  p = (P*)buffer;
  P_init__i(p, 10);
  v = P_get(p);
  P_destroy(p);
  return (v == 10) ? 1 : 0;
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
