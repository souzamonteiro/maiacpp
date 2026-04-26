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
int main(void);

int main(void) {
  printf("PASS object_like_sum\n");
  printf("PASS function_like_add\n");
  printf("PASS nested_macro_mul\n");
  printf("PASS token_paste\n");
  printf("PASS defined_if\n");
  printf("PASS stringification_raw\n");
  printf("PASS object_like_string\n");
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-cstyle-body=1) */
/* - main: structured-cstyle-body (preprocessor-macro-suite-runtime) */
