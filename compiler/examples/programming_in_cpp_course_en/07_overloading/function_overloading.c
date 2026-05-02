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
float FahrenheitToCelsius__f(float t);
int FahrenheitToCelsius__i(int t);
int main(void);

float FahrenheitToCelsius__f(float t) {
  return (t - 32.0) * 5.0 / 9.0;
}

int FahrenheitToCelsius__i(int t) {
  return (t - 32.0) * 5.0 / 9.0;
}

int main(void) {
  int t1 = 0;
  float t2 = 0;

  printf("Enter the temperature in Fahrenheit as an integer: ");
  scanf("%d", &t1);
  printf("The temperature in Celsius is ");
  printf("%g", FahrenheitToCelsius__f((float)(t1)));
  printf(".\n");
  printf("Enter the temperature in Fahrenheit as a real number: ");
  scanf("%f", &t2);
  printf("The temperature in Celsius is ");
  printf("%g", FahrenheitToCelsius__f((float)(t2)));
  printf(".\n");
  return 0;
}
