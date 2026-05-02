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
  int age = 0;
  float height = 0;
  char name[255];

  printf("Enter your age: ");
  scanf("%d", &age);
  printf("You are ");
  printf("%d", age);
  printf(" years old.");
  printf("\n");
  printf("Enter your height in meters: ");
  scanf("%f", &height);
  printf("You are ");
  printf("%g", height);
  printf(" meters tall.");
  printf("\n");
  printf("Enter your name: ");
  scanf("%s", name);
  printf("Hello ");
  printf("%s", name);
  printf("!");
  return 0;
}
