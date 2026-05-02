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
int array_sum__pvi(int* arr, int n);
int main(void);

int array_sum__pvi(int* arr, int n) {
  int i;
  int s = 0;
  for (i = 0; i < n; ++i) s += arr[i];
  return s;
}

int main(void) {
  int a[5] = {2, 4, 6, 8, 10};
  int* p = a;
  int mat[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  int sq[5];
  int x = 42;
  int* px = &x;

  if (a[0] == 2 && a[4] == 10) {
    printf("PASS arr_access\n");
  }
  if (array_sum__pvi(a, 5) == 30) {
    printf("PASS arr_sum\n");
  }
  if (*p == 2 && *(p+2) == 6) {
    printf("PASS ptr_arith\n");
  }
  if (mat[1][1] == 5 && mat[2][2] == 9) {
    printf("PASS mat_2d\n");
  }
  for (int i = 0; i < 5; ++i) {
    sq[i] = i * i;
  }
  if (sq[3] == 9 && sq[4] == 16) {
    printf("PASS fill_sq\n");
  }
  if (*px == 42) {
    printf("PASS ptr_deref\n");
  }
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-cstyle-body=1) */
/* - array_sum: structured-cstyle-body (raw-body 4 line(s)) */
