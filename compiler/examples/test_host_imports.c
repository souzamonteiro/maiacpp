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

/* Host import declarations */
extern void __console__log(const char *msg);
extern void __console__error(const char *msg);
extern double __Math__sqrt(double x);
extern double __Math__floor(double x);
extern double __Math__pow(double base, double exp);

/* Global functions */
int main(void);

int main(void) {
  double root = __Math__sqrt(144.0);
  double flr = __Math__floor(3.7);
  double pw = __Math__pow(2.0, 10.0);

  __console__log("Hello from MaiaCpp!");
  __console__error("This is an error message");
  printf("sqrt(144) = %.0f\n", root);
  printf("floor(3.7) = %.0f\n", flr);
  printf("2^10 = %.0f\n", pw);
  return 0;
}
