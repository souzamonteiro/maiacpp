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

typedef int (*IntOp)(int);

/* Global functions */
int factorial__i(int n);
int fib__i(int n);
int square__i(int x);
int swap_ref__pvpv(int* a, int* b);
int double_val__i(int x);
int apply__N5IntOpi(IntOp f, int x);
int main(void);

int factorial__i(int n) {
  (void)n;
  return (int)0;
}

int fib__i(int n) {
  (void)n;
  return (int)0;
}

int square__i(int x) {
  return x * x;
}

int swap_ref__pvpv(int* a, int* b) {
  (void)a;
  (void)b;
  return (int)0;
}

int double_val__i(int x) {
  return x * 2;
}

int apply__N5IntOpi(IntOp f, int x) {
  return f(x);
}

int main(void) {
  return (int)0;
}

/* Lowering diagnostics: 4 event(s) (stub-fallback=4) */
/* - factorial: stub-fallback (no-supported-lowering) */
/* - fib: stub-fallback (no-supported-lowering) */
/* - swap_ref: stub-fallback (no-supported-lowering) */
/* - main: stub-fallback (no-supported-lowering) */
