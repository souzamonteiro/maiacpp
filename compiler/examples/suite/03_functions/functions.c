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
int square__d(double x);
int swap_ref__pvpv(int* a, int* b);
int sum_cref__ii(int x, int y);
int clamp__iii(int x, int lo, int hi);
int double_val__i(int x);
int negate__i(int x);
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

int square__d(double x) {
  return x * x;
}

int swap_ref__pvpv(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
  return (int)0;
}

int sum_cref__ii(int x, int y) {
  return x + y;
}

int clamp__iii(int x, int lo, int hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

int double_val__i(int x) {
  return x * 2;
}

int negate__i(int x) {
  return -x;
}

int apply__N5IntOpi(IntOp f, int x) {
  return f(x);
}

int main(void) {
  int p = 3;
  int q = 8;
  IntOp arr[3] = {double_val__i, negate__i, square__i};

  if (factorial__i(0) == 1) {
    printf("PASS fact_0\n");
  }
  if (factorial__i(1) == 1) {
    printf("PASS fact_1\n");
  }
  if (factorial__i(5) == 120) {
    printf("PASS fact_5\n");
  }
  if (factorial__i(7) == 5040) {
    printf("PASS fact_7\n");
  }
  if (fib__i(0) == 0) {
    printf("PASS fib_0\n");
  }
  if (fib__i(1) == 1) {
    printf("PASS fib_1\n");
  }
  if (fib__i(7) == 13) {
    printf("PASS fib_7\n");
  }
  if (fib__i(10) == 55) {
    printf("PASS fib_10\n");
  }
  if (square__i(7) == 49) {
    printf("PASS sq_int\n");
  }
  if (square__d(5.0) == 25.0) {
    printf("PASS sq_double\n");
  }
  swap_ref__pvpv(&p, &q);
  if (p == 8 && q == 3) {
    printf("PASS swap_ref\n");
  }
  if (sum_cref__ii(10, 10) == 20) {
    printf("PASS sum_cref_10\n");
  }
  if (sum_cref__ii(50, 50) == 100) {
    printf("PASS sum_cref_100\n");
  }
  if (clamp__iii(50, 0, 100) == 50) {
    printf("PASS clamp_mid\n");
  }
  if (clamp__iii(-5, 0, 100) == 0) {
    printf("PASS clamp_lo\n");
  }
  if (clamp__iii(150, 0, 100) == 100) {
    printf("PASS clamp_hi\n");
  }
  if (apply__N5IntOpi(double_val__i, 7) == 14) {
    printf("PASS fptr_double\n");
  }
  if (apply__N5IntOpi(negate__i, 5) == -5) {
    printf("PASS fptr_negate\n");
  }
  if (apply__N5IntOpi(square__i, 8) == 64) {
    printf("PASS fptr_square\n");
  }
  if (arr[0](3) == 6) {
    printf("PASS fptr_arr_0\n");
  }
  if (arr[1](7) == -7) {
    printf("PASS fptr_arr_1\n");
  }
  if (arr[2](9) == 81) {
    printf("PASS fptr_arr_2\n");
  }
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 4 event(s) (structured-cstyle-body=4) */
/* - factorial: structured-cstyle-body (2 stmt(s)) */
/* - fib: structured-cstyle-body (2 stmt(s)) */
/* - swap_ref: structured-cstyle-body (3 stmt(s)) */
/* - clamp: structured-cstyle-body (3 stmt(s)) */
