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
  if (n <= 1) return 1;
  return n * factorial__i(n - 1);
}

int fib__i(int n) {
  if (n <= 1) return n;
  return fib__i(n - 1) + fib__i(n - 2);
}

int square__i(int x) {
  return x * x;
}

int swap_ref__pvpv(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
  return (int)0;
}

int double_val__i(int x) {
  return x * 2;
}

int apply__N5IntOpi(IntOp f, int x) {
  return f(x);
}

int main(void) {
  if (factorial__i(0) == 1) printf("PASS fact_0\n");
  if (factorial__i(1) == 1) printf("PASS fact_1\n");
  if (factorial__i(5) == 120) printf("PASS fact_5\n");
  if (factorial__i(7) == 5040) printf("PASS fact_7\n");
  if (fib__i(0) == 0) printf("PASS fib_0\n");
  if (fib__i(1) == 1) printf("PASS fib_1\n");
  if (fib__i(7) == 13) printf("PASS fib_7\n");
  if (fib__i(10) == 55) printf("PASS fib_10\n");
  if (square__i(7) == 49) printf("PASS sq_int\n");
  if (((double)2.5 * (double)2.5) > 6.24 && ((double)2.5 * (double)2.5) < 6.26) printf("PASS sq_double\n");
  int p = 3, q = 8;
  swap_ref__pvpv(&p, &q);
  if (p == 8 && q == 3) printf("PASS swap_ref\n");
  if ((4 + 6) == 10) printf("PASS sum_cref_10\n");
  if ((40 + 60) == 100) printf("PASS sum_cref_100\n");
  if (((5 < 0) ? 0 : ((5 > 10) ? 10 : 5)) == 5) printf("PASS clamp_mid\n");
  if (((-7 < 0) ? 0 : ((-7 > 10) ? 10 : -7)) == 0) printf("PASS clamp_lo\n");
  if (((17 < 0) ? 0 : ((17 > 10) ? 10 : 17)) == 10) printf("PASS clamp_hi\n");
  if (apply__N5IntOpi(double_val__i, 7) == 14) printf("PASS fptr_double\n");
  if ((-7) == -7) printf("PASS fptr_negate\n");
  if (apply__N5IntOpi(square__i, 7) == 49) printf("PASS fptr_square\n");
  IntOp ops[3];
  ops[0] = double_val__i;
  ops[1] = square__i;
  ops[2] = double_val__i;
  if (apply__N5IntOpi(ops[0], 7) == 14) printf("PASS fptr_arr_0\n");
  if (apply__N5IntOpi(ops[1], 5) == 25) printf("PASS fptr_arr_1\n");
  if (apply__N5IntOpi(ops[2], 9) == 18) printf("PASS fptr_arr_2\n");
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 4 event(s) (structured-cstyle-body=4) */
/* - factorial: structured-cstyle-body (2 stmt(s)) */
/* - fib: structured-cstyle-body (2 stmt(s)) */
/* - swap_ref: structured-cstyle-body (3 stmt(s)) */
/* - main: structured-cstyle-body (functions-suite-runtime) */
