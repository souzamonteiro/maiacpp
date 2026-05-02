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
float celsius__f(float f);
int main(void);

float celsius__f(float f) {
  return (f - 32) / 1.8;
}

int main(void) {
  float f = 0;

  printf("Enter the temperature in Fahrenheit: ");
  scanf("%f", &f);
  printf("Temperature in Celsius: ");
  printf("%g", celsius__f((float)(f)));
  printf(".\n");
  return 0;
}
