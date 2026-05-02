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
  int x = 0;
  float y = 0;
  double z = 0;
  char a = 0;
  char b[] = "Hello World";

  x = 1;
  y = 2;
  z = 3;
  a = 'A';
  printf("%d", x);
  printf("\n");
  printf("%g", y);
  printf("\n");
  printf("%g", z);
  printf("\n");
  printf("%c", a);
  printf("\n");
  printf("%s", b);
  printf("\n");
  return 0;
}
