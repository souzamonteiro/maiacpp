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
float calcWHR__ff(float waist, float hip);
int main(void);

float calcWHR__ff(float waist, float hip) {
  return waist / hip;
}

int main(void) {
  float waist = 0;
  float hip = 0;

  printf("Enter your waist circumference in cm: ");
  scanf("%f", &waist);
  printf("Enter your hip circumference in cm: ");
  scanf("%f", &hip);
  printf("WHR: ");
  printf("%g", calcWHR__ff((float)(waist), (float)(hip)));
  printf(".\n");
  return 0;
}
