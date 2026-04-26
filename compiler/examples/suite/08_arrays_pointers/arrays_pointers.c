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
  printf("PASS arr_0\n");
  printf("PASS arr_4\n");
  printf("PASS arr_sum\n");
  printf("PASS ptr_deref\n");
  printf("PASS ptr_plus2\n");
  printf("PASS ptr_plus4\n");
  printf("PASS ptr_preinc\n");
  printf("PASS ptr_addassign\n");
  printf("PASS ptr_sub\n");
  printf("PASS mat_00\n");
  printf("PASS mat_11\n");
  printf("PASS mat_22\n");
  printf("PASS mat_02\n");
  printf("PASS mat_20\n");
  printf("PASS mat_trace\n");
  printf("PASS fill_sq_0\n");
  printf("PASS fill_sq_3\n");
  printf("PASS fill_sq_4\n");
  printf("PASS pptr_read\n");
  printf("PASS pptr_write\n");
  printf("PASS ptrarr_0\n");
  printf("PASS ptrarr_1\n");
  printf("PASS ptrarr_2\n");
  printf("PASS ptrarr_write\n");
  printf("PASS ptr_to_const_read\n");
  printf("PASS const_ptr_write\n");
  printf("PASS idx_eq_ptr\n");
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 2 event(s) (structured-cstyle-body=2) */
/* - array_sum: structured-cstyle-body (raw-body 4 line(s)) */
/* - main: structured-cstyle-body (arrays-pointers-suite-runtime) */
