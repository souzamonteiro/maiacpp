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
  (void)arr;
  (void)n;
  return (int)0;
}

int main(void) {
  return (int)0;
}

/* Lowering diagnostics: 2 event(s) (stub-fallback=2) */
/* - array_sum: stub-fallback (no-supported-lowering) */
/* - main: stub-fallback (no-supported-lowering) */
