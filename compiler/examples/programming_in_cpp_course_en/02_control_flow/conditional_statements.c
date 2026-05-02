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
  char gender = 0;

  printf("Enter your age: ");
  scanf("%d", &age);
  if (age > 18) {
    printf("You are older than 18.");
    printf("\n");
  } else {
    if (age < 18) {
      printf("You are younger than 18.");
      printf("\n");
    } else {
      printf("You are 18 years old.");
      printf("\n");
    }
  }
  printf("Enter your gender (M/F): ");
  scanf(" %c", &gender);
  switch (gender) {
    case 'm':
    case 'M':
      printf("You are male.");
      printf("\n");
      break;
    case 'f':
    case 'F':
      printf("You are female.");
      printf("\n");
      break;
    default:
      printf("Gender undefined.");
      printf("\n");
  }
  return 0;
}
