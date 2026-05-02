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
  int i = 0;
  char c = 0;

  i = 0;
  while (i < 10) {
    printf("%d", i);
    printf("\n");
    i++;
  }
  do {
    printf("%d", i);
    printf("\n");
    i++;
  } while (i < 10);
  for (i = 0; i < 10; i++) {
    printf("%d", i);
    printf("\n");
  }
  for (c = 'a'; c < 'z'; c++) {
    printf("%c", c);
    printf("\n");
  }
  return 0;
}
