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
float calcBMI__ff(float weight, float height);
int main(void);

float calcBMI__ff(float weight, float height) {
  return weight / (height * height);
}

int main(void) {
  float weight = 0;
  float height = 0;

  printf("Enter your weight in kg: ");
  scanf("%f", &weight);
  printf("Enter your height in m: ");
  scanf("%f", &height);
  printf("BMI: ");
  printf("%g", calcBMI__ff((float)(weight), (float)(height)));
  printf(".\n");
  return 0;
}
