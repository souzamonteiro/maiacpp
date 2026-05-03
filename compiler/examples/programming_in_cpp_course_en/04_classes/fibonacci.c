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

#define EXC_Fibonacci 1

typedef struct Fibonacci {
  int __dummy;
} Fibonacci;

void Fibonacci_init(Fibonacci* self);
void Fibonacci_destroy(Fibonacci* self);
int Fibonacci_nFibonacci__i(Fibonacci* self, int n);
void Fibonacci_createSeries__i(Fibonacci* self, int n);

void Fibonacci_init(Fibonacci* self) {
  (void)self;
}

void Fibonacci_destroy(Fibonacci* self) {
  (void)self;
}

int Fibonacci_nFibonacci__i(Fibonacci* self, int n) {
  (void)self;
  return fib__i(n);
  (void)n;
}

void Fibonacci_createSeries__i(Fibonacci* self, int n) {
  (void)self;
  int i;
  for (i = 1; i <= n; i++) {
    printf(" %d", fib__i(i));
  }
  printf("\n");
  (void)n;
}

/* Global functions */
int fib__i(int n);
int main(void);

int fib__i(int n) {
  if (n <= 1) {
    return n;
  }
  return fib__i(n - 1) + fib__i(n - 2);
}

int main(void) {
  Fibonacci fib_obj;
  int n = 0;

  printf("How many terms would you like to display? ");
  scanf("%d", &n);
  printf("\n");
  Fibonacci_createSeries__i(&fib_obj, n);
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-cstyle-body=1) */
/* - fib: structured-cstyle-body (2 stmt(s)) */
