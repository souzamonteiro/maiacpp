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
  float position = 0;
  float initial_position = 0;
  float velocity = 0;
  float time = 0;

  printf("Enter the initial position of the object in m: ");
  scanf("%f", &initial_position);
  printf("Enter the velocity of the object in m/s: ");
  scanf("%f", &velocity);
  printf("Enter the elapsed time in s: ");
  scanf("%f", &time);
  position = initial_position + velocity * time;
  printf("Current position of the object: %.2f m.\n", position);
  return 0;
}
