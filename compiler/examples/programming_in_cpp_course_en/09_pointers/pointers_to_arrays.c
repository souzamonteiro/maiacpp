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
  int a = 1;
  int b = 255;
  int c[3];
  int d[3] = {4, 5, 6};
  int* p = 0;
  int i = 0;

  c[0] = 1;
  c[1] = 2;
  c[2] = 3;
  p = &a;
  printf("The memory address pointed to by p is %ld.\n", p);
  p = &b;
  printf("The memory address pointed to by p is %ld.\n", p);
  p = d;
  printf("The memory address pointed to by p is %ld.\n", p);
  p++;
  printf("The memory address pointed to by p is %ld.\n", p);
  *p = 7;
  for (i = 0; i < (sizeof(d) / sizeof(int)); i++) {
    printf("d[");
    printf("%d", i);
    printf("] = ");
    printf("%d", d[i]);
    printf(".\n");
  }
  return 0;
}
