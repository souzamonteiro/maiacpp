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
  if (strcmp(1("macro-hello"), ""macro-hello"") == 0) {
    printf("PASS stringification_raw\n");
  } else {
    printf("FAIL stringification_raw\n");
  }
  if (strcmp("macro-hello", "macro-hello") == 0) {
    printf("PASS object_like_string\n");
  } else {
    printf("FAIL object_like_string\n");
  }
  printf("ALL PASS\n");
  return 0;
}
