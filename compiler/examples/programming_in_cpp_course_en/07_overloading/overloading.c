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
int square__i(int x);
float square__f(float x);
int main(void);

int square__i(int x) {
  return x * x;
}

float square__f(float x) {
  return x * x;
}

int main(void) {
  int x1 = 0;
  float x2 = 0;

  printf("Enter an integer: ");
  scanf("%d", &x1);
  printf("Enter a real number: ");
  scanf("%f", &x2);
  printf("The square of ");
  printf("%d", x1);
  printf(" is: ");
  printf("%d", square__i(x1));
  printf("\n");
  printf("The square of ");
  printf("%g", x2);
  printf(" is: ");
  printf("%g", square__f((float)(x2)));
  printf("\n");
  return 0;
}
