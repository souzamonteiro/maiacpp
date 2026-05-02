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
int main(int argc, int p2, char* env);

int main(int argc, int p2, char* env) {
  int i = 0;
  char** p = 0;

  for (i = 0; i < argc; i++) {
    printf("argv[");
    printf("%d", i);
    printf("] = ");
    printf("%d", argv[i]);
    printf("\n");
  }
  p = env;
  while (*p) {
    printf("%c", *p);
    printf("\n");
    p++;
  }
  return 0;
}
