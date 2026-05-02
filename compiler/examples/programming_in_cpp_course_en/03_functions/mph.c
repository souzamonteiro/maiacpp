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
  float mph = 0;
  float kmh = 0;

  printf("Enter the speed in km/h: ");
  scanf("%f", &kmh);
  mph = kmh / 1.61;
  printf("Speed in mph: ");
  printf("%g", mph);
  printf(".\n");
  return 0;
}
