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
  int series;
} Fibonacci;

void Fibonacci_init(Fibonacci* self);
void Fibonacci_destroy(Fibonacci* self);
int Fibonacci_nFibonacci__i(Fibonacci* self, int n);
char* Fibonacci_createSeries__i(Fibonacci* self, int n);

void Fibonacci_init(Fibonacci* self) {
  (void)self;
}

void Fibonacci_destroy(Fibonacci* self) {
  (void)self;
}

int Fibonacci_nFibonacci__i(Fibonacci* self, int n) {
  (void)self;
  if (n <= 1) return n;
  return Fibonacci_nFibonacci__i(self, n - 1) + Fibonacci_nFibonacci__i(self, n - 2);
  (void)n;
}

char* Fibonacci_createSeries__i(Fibonacci* self, int n) {
  (void)self;
  {
    static char __fib_buf[2048];
    int __off = 0;
    int i;
    __fib_buf[0] = 0;
    for (i = 1; i <= n; ++i) {
      __off += sprintf(__fib_buf + __off, " %d", Fibonacci_nFibonacci__i(self, i));
    }
    return __fib_buf;
  }
  (void)n;
}

/* Global functions */
int main(void);

int main(void) {
  Fibonacci fib;
  int n = 0;

  printf("How many terms would you like to display? ");
  scanf("%d", &n);
  printf("\n");
  printf("%s", Fibonacci_createSeries__i(&fib, n));
  printf("\n");
  return 0;
}
