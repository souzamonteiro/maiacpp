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

typedef int (*Fn2)(int, int);

#define EXC_Base 1
#define EXC_Derived 2
#define EXC_Box 3

typedef struct Base {
  void* __vptr;
} Base;

void Base_init(Base* self);
void Base_destroy(Base* self);
int Base_value(Base* self);

void Base_init(Base* self) {
  (void)self;
}

void Base_destroy(Base* self) {
  (void)self;
}

int Base_value(Base* self) {
  (void)self;
  return (int)0;
}

typedef struct Derived {
  Base __base;
  int m;
  void* __vptr;
} Derived;

void Derived_init__i(Derived* self, int n);
void Derived_init(Derived* self);
void Derived_destroy(Derived* self);
int Derived_value(Derived* self);

void Derived_init__i(Derived* self, int n) {
  (void)self;
  self->m = n;
}

void Derived_init(Derived* self) {
  (void)self;
}

void Derived_destroy(Derived* self) {
  (void)self;
}

int Derived_value(Derived* self) {
  (void)self;
  return self->m;
}

typedef struct Box {
  int __dummy;
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

/* Global functions */
int Alpha_Beta_diff__ii(int a, int b);
int Alpha_sum__ii(int a, int b);
int op__ii(int a, int b);
long op__ll(long a, long b);
int apply__iiN3Fn2(int a, int b, Fn2 fn);
int run_runtime_subset(void);
int c_linked_add__ii(int a, int b);
int main(void);

int Alpha_Beta_diff__ii(int a, int b) {
  return a - b;
}

int Alpha_sum__ii(int a, int b) {
  return a + b;
}

int op__ii(int a, int b) {
  return a + b;
}

long op__ll(long a, long b) {
  return a + b;
}

int apply__iiN3Fn2(int a, int b, Fn2 fn) {
  return fn(a, b);
}

int run_runtime_subset(void) {
  return (int)0;
}

int c_linked_add__ii(int a, int b) {
  return a + b;
}

int main(void) {
  return (int)0;
}
