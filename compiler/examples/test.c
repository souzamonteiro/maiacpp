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
int run_cout_stress_tests(void);
int run_for_cout_test(void);
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
  Box_init(&box);
  *((int*)Box_operator_subscript__i(&box, 0)) = 10;
  *((int*)Box_operator_subscript__i(&box, 1)) = 20;
  return ((*((int*)Box_operator_subscript__i(&box, 0)) + *((int*)Box_operator_subscript__i(&box, 1))) == 30) ? 1 : 0;
}

int run_function_pointer_tests(void) {
  int s = execute__iiN8BinaryOp(7, 3, add__ii);
  int m = execute__iiN8BinaryOp(7, 3, multiply__ii);
  return (s == 10 && m == 21) ? 1 : 0;
}

int run_cast_tests(void) {
  DDerived* __derived = (DDerived*)__malloc((unsigned long)sizeof(DDerived));
  if (__derived == 0) return 0;
  DDerived_init__i(__derived, 15);
  BBase* b = (BBase*)__derived;
  DDerived* d = (DDerived*)b;
  int n = (int)(3.2);
  if (d == 0) { DDerived_destroy(__derived); __free(b); return 0; }
  if (DDerived_value(d) != 15) { DDerived_destroy(__derived); __free(b); return 0; }
  if (n != 3) { DDerived_destroy(__derived); __free(b); return 0; }
  DDerived_destroy(__derived);
  __free(b);
  return 1;
}

int run_new_delete_tests(void) {
  int* a = (int*)__malloc((unsigned long)sizeof(int));
  if (a == 0) return 0;
  *a = 1;
  if (*a != 1) { __free(a); return 0; }
  __free(a);
  char buffer[sizeof(P)];
  P* p = (P*)(void*)buffer;
  P_init__i(p, 10);
  int v = P_get(p);
  P_destroy(p);
  return (v == 10) ? 1 : 0;
}

int run_cout_stress_tests(void) {
  int cout_acc = 0;
  int i = 1;

  cout_acc = cout_acc + i;
  printf("[cout-test] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  i++;
  cout_acc = cout_acc + i;
  printf("[cout-test] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  i++;
  cout_acc = cout_acc + i;
  printf("[cout-test] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  return (cout_acc == 6) ? 1 : 0;
}

int run_for_cout_test(void) {
  int sum = 0;
  double ratio = 1.5;
  int i = 1;

  for (i = 1; i < 4; ++i) {
    sum = sum + i;
    printf("[for-cout] i=");
    printf("%d", i);
    printf(" sum=");
    printf("%d", sum);
    printf(" ratio=");
    printf("%g", ratio);
    printf("\n");
  }
  return (sum == 6) ? 1 : 0;
}

int main(void) {
  int failures = 0;
  int a = 10;
  int b = 20;
  int result = 0;
  int relation = 0;
  int logic = 0;
  int bit = 0;
  int cout_acc = 0;
  int i = 0;
  int loop_sum = 0;
  int down = 5;
  int up = 0;
  int* ptr = &a;

  printf("=== MaiaCpp Comprehensive Runtime Baseline ===\n");
  printf("--- Arithmetic Operators ---\n");
  result = a + b;
  printf("add(a,b)=");
  printf("%d", result);
  printf("\n");
  result = b - a;
  printf("b-a=");
  printf("%d", result);
  printf("\n");
  result = a * 3;
  printf("a*3=");
  printf("%d", result);
  printf("\n");
  result = b / 2;
  printf("b/2=");
  printf("%d", result);
  printf("\n");
  result = b % 3;
  printf("b%3=");
  printf("%d", result);
  printf("\n");
  printf("--- Assignment Operators ---\n");
  result = a;
  printf("result=");
  printf("%d", result);
  printf("\n");
  result = result + b;
  printf("result+=b => ");
  printf("%d", result);
  printf("\n");
  result = result - 10;
  printf("result-=10 => ");
  printf("%d", result);
  printf("\n");
  result = result * 2;
  printf("result*=2 => ");
  printf("%d", result);
  printf("\n");
  result = result / 5;
  printf("result/=5 => ");
  printf("%d", result);
  printf("\n");
  result = result % 4;
  printf("result%=4 => ");
  printf("%d", result);
  printf("\n");
  printf("--- Relational Operators ---\n");
  relation = a == b;
  printf("a==b => ");
  printf("%d", relation);
  printf("\n");
  relation = a != b;
  printf("a!=b => ");
  printf("%d", relation);
  printf("\n");
  relation = a < b;
  printf("a<b => ");
  printf("%d", relation);
  printf("\n");
  relation = a > b;
  printf("a>b => ");
  printf("%d", relation);
  printf("\n");
  relation = a <= b;
  printf("a<=b => ");
  printf("%d", relation);
  printf("\n");
  relation = a >= b;
  printf("a>=b => ");
  printf("%d", relation);
  printf("\n");
  printf("--- Logical Operators ---\n");
  logic = a && b;
  printf("a&&b => ");
  printf("%d", logic);
  printf("\n");
  logic = a || 0;
  printf("a||0 => ");
  printf("%d", logic);
  printf("\n");
  printf("--- Bitwise Operators ---\n");
  bit = a & b;
  printf("a&b => ");
  printf("%d", bit);
  printf("\n");
  bit = a | b;
  printf("a|b => ");
  printf("%d", bit);
  printf("\n");
  bit = a ^ b;
  printf("a^b => ");
  printf("%d", bit);
  printf("\n");
  bit = a << 2;
  printf("a<<2 => ");
  printf("%d", bit);
  printf("\n");
  bit = b >> 1;
  printf("b>>1 => ");
  printf("%d", bit);
  printf("\n");
  printf("--- Pointer Operators ---\n");
  *ptr = 100;
  printf("*ptr=100 => a=");
  printf("%d", a);
  printf("\n");
  printf("--- Control Flow ---\n");
  for (i = 0; i < 8; ++i) {
    if (i == 5) {
      continue;
    }
    loop_sum = loop_sum + i;
    printf("[for] i=");
    printf("%d", i);
    printf(" loop_sum=");
    printf("%d", loop_sum);
    printf("\n");
  }
  printf("loop_sum=%d\n", loop_sum);
  while (down > 0) {
    down--;
  }
  printf("while-down=%d\n", down);
  do {
    up++;
  } while (up < 5);
  printf("do-while-up=%d\n", up);
  printf("--- cout stress preflight ---\n");
  cout_acc = cout_acc + i;
  printf("[cout] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  i++;
  cout_acc = cout_acc + i;
  printf("[cout] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  i++;
  cout_acc = cout_acc + i;
  printf("[cout] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  i++;
  cout_acc = cout_acc + i;
  printf("[cout] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  i++;
  cout_acc = cout_acc + i;
  printf("[cout] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  i++;
  cout_acc = cout_acc + i;
  printf("[cout] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  i++;
  cout_acc = cout_acc + i;
  printf("[cout] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  i++;
  cout_acc = cout_acc + i;
  printf("[cout] i=");
  printf("%d", i);
  printf(" acc=");
  printf("%d", cout_acc);
  printf(" int=");
  printf("%d", 42);
  printf(" double=");
  printf("%g", 3.25);
  printf(" char=");
  printf("%c", 'Q');
  printf("\n");
  printf("cout_acc=%d expected=92\n", cout_acc);
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
  printf("3. function pointer dispatch: ");
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
  printf("6. cout stress (chain/loop/literals): ");
  if (run_cout_stress_tests()) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
    failures++;
  }
  printf("7. for-loop with cout and double local: ");
  if (run_for_cout_test()) {
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
}

/* Lowering diagnostics: 7 event(s) (structured-indexed-object-cmp-return=1, structured-io-runtime=2, structured-local-return=1, structured-method-cmp-return=1, structured-resource-runtime=2) */
/* - run_class_tests: structured-method-cmp-return (method) */
/* - run_template_tests: structured-indexed-object-cmp-return (2 assignment(s)) */
/* - run_function_pointer_tests: structured-local-return (2 local(s)) */
/* - run_cast_tests: structured-resource-runtime (cast-static-runtime) */
/* - run_new_delete_tests: structured-resource-runtime (new-delete-runtime) */
/* - run_cout_stress_tests: structured-io-runtime (structured-io-runtime) */
/* - run_for_cout_test: structured-io-runtime (structured-io-runtime) */

