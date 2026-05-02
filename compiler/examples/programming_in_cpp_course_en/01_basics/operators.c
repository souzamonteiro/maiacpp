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
  int b = 2;
  int c = 0;

  c = a + b;
  printf("a + b = ");
  printf("%d", c);
  printf("\n");
  c = a - b;
  printf("a - b = ");
  printf("%d", c);
  printf("\n");
  c = a * b;
  printf("a * b = ");
  printf("%d", c);
  printf("\n");
  c = a / b;
  printf("a / b = ");
  printf("%d", c);
  printf("\n");
  c = a % b;
  printf("a %% b = ");
  printf("%d", c);
  printf("\n");
  c = a < b;
  printf("a < b = ");
  printf("%d", c);
  printf("\n");
  c = a <= b;
  printf("a <= b = ");
  printf("%d", c);
  printf("\n");
  c = a > b;
  printf("a > b = ");
  printf("%d", c);
  printf("\n");
  c = a >= b;
  printf("a >= b = ");
  printf("%d", c);
  printf("\n");
  c = a == b;
  printf("a == b = ");
  printf("%d", c);
  printf("\n");
  c = a != b;
  printf("a != b = ");
  printf("%d", c);
  printf("\n");
  b = 0;
  c = a && b;
  printf("a && b = ");
  printf("%d", c);
  printf("\n");
  c = a || b;
  printf("a || b = ");
  printf("%d", c);
  printf("\n");
  a++;
  printf("a = ");
  printf("%d", a);
  printf("\n");
  b--;
  printf("b = ");
  printf("%d", b);
  printf("\n");
  return 0;
}
