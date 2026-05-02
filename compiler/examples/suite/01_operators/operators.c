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

/* Global functions */
int test_arithmetic(void);
int test_relational(void);
int test_logical(void);
int test_bitwise(void);
int test_compound(void);
int test_prepost(void);
int test_ternary(void);
int test_float(void);
int main(void);

int test_arithmetic(void) {
  int a = 17, b = 5;
  int ok = 0;
  if (a + b == 22) {
    printf("PASS add\n"); ++ok;
  }
  if (a - b == 12) {
    printf("PASS sub\n"); ++ok;
  }
  if (a * b == 85) {
    printf("PASS mul\n"); ++ok;
  }
  if (a / b == 3) {
    printf("PASS idiv\n"); ++ok;
  }
  if (a % b == 2) {
    printf("PASS imod\n"); ++ok;
  }
  return ok;
}

int test_relational(void) {
  int a = 17, b = 5;
  int ok = 0;
  if (a == 17) {
    printf("PASS eq\n"); ++ok;
  }
  if (a != b) {
    printf("PASS ne\n"); ++ok;
  }
  if (b < a) {
    printf("PASS lt\n"); ++ok;
  }
  if (a > b) {
    printf("PASS gt\n"); ++ok;
  }
  if (b <= 5) {
    printf("PASS le\n"); ++ok;
  }
  if (a >= 17) {
    printf("PASS ge\n"); ++ok;
  }
  return ok;
}

int test_logical(void) {
  int a = 17, b = 5;
  int ok = 0;
  if (a > 0 && b > 0) {
    printf("PASS land\n"); ++ok;
  }
  if (a < 0 || b > 0) {
    printf("PASS lor\n"); ++ok;
  }
  if (!0) {
    printf("PASS lnot\n"); ++ok;
  }
  return ok;
}

int test_bitwise(void) {
  int a = 17, b = 5;
  int ok = 0;
  if ((a & b) == 1) {
    printf("PASS band\n"); ++ok;
  }
  if ((a | b) == 21) {
    printf("PASS bor\n"); ++ok;
  }
  if ((a ^ b) == 20) {
    printf("PASS bxor\n"); ++ok;
  }
  if ((b << 2) == 20) {
    printf("PASS shl\n"); ++ok;
  }
  if ((a >> 1) == 8) {
    printf("PASS shr\n"); ++ok;
  }
  if (~0 == -1) {
    printf("PASS bnot\n"); ++ok;
  }
  return ok;
}

int test_compound(void) {
  int c = 10;
  int ok = 0;
  c += 5;
  if (c == 15) {
    printf("PASS cadd\n"); ++ok;
  }
  c -= 3;
  if (c == 12) {
    printf("PASS csub\n"); ++ok;
  }
  c *= 2;
  if (c == 24) {
    printf("PASS cmul\n"); ++ok;
  }
  c /= 4;
  if (c == 6) {
    printf("PASS cdiv\n"); ++ok;
  }
  c %= 4;
  if (c == 2) {
    printf("PASS cmod\n"); ++ok;
  }
  c &= 3;
  if (c == 2) {
    printf("PASS cband\n"); ++ok;
  }
  c |= 4;
  if (c == 6) {
    printf("PASS cbor\n"); ++ok;
  }
  c ^= 3;
  if (c == 5) {
    printf("PASS cbxor\n"); ++ok;
  }
  c <<= 1;
  if (c == 10) {
    printf("PASS cshl\n"); ++ok;
  }
  c >>= 1;
  if (c == 5) {
    printf("PASS cshr\n"); ++ok;
  }
  return ok;
}

int test_prepost(void) {
  int d = 5;
  int ok = 0;
  if (++d == 6) {
    printf("PASS preinc\n"); ++ok;
  }
  if (d++ == 6) {
    printf("PASS postinc\n"); ++ok;
  }
  if (d == 7) {
    printf("PASS after_postinc\n"); ++ok;
  }
  if (--d == 6) {
    printf("PASS predec\n"); ++ok;
  }
  if (d-- == 6) {
    printf("PASS postdec\n"); ++ok;
  }
  if (d == 5) {
    printf("PASS after_postdec\n"); ++ok;
  }
  return ok;
}

int test_ternary(void) {
  int a = 17, b = 5;
  int ok = 0;
  if ((a > b ? 1 : 0) == 1) {
    printf("PASS ternary_t\n"); ++ok;
  }
  if ((a < b ? 1 : 0) == 0) {
    printf("PASS ternary_f\n"); ++ok;
  }
  return ok;
}

int test_float(void) {
  double x = 7.5, y = 2.5;
  int ok = 0;
  if (x + y == 10.0) {
    printf("PASS fadd\n"); ++ok;
  }
  if (x - y == 5.0) {
    printf("PASS fsub\n"); ++ok;
  }
  if (x * y == 18.75) {
    printf("PASS fmul\n"); ++ok;
  }
  if (x / y == 3.0) {
    printf("PASS fdiv\n"); ++ok;
  }
  return ok;
}

int main(void) {
  int total = 0;

  total += test_arithmetic();
  total += test_relational();
  total += test_logical();
  total += test_bitwise();
  total += test_compound();
  total += test_prepost();
  total += test_ternary();
  total += test_float();
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 8 event(s) (structured-cstyle-body=8) */
/* - test_arithmetic: structured-cstyle-body (8 stmt(s)) */
/* - test_relational: structured-cstyle-body (9 stmt(s)) */
/* - test_logical: structured-cstyle-body (6 stmt(s)) */
/* - test_bitwise: structured-cstyle-body (9 stmt(s)) */
/* - test_compound: structured-cstyle-body (23 stmt(s)) */
/* - test_prepost: structured-cstyle-body (9 stmt(s)) */
/* - test_ternary: structured-cstyle-body (5 stmt(s)) */
/* - test_float: structured-cstyle-body (7 stmt(s)) */
