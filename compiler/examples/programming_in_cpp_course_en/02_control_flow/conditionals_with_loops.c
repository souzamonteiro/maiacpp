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
  int a = 0;
  int b = 0;
  char answer = 0;
  int continue_loop = 1;

  while (continue_loop) {
    printf("Enter the value of the first variable: ");
    scanf("%d", &a);
    printf("Enter the value of the second variable: ");
    scanf("%d", &b);
    if (a < b) {
      printf("The first variable is less than the second!\n");
    } else {
      if (a > b) {
        printf("The first variable is greater than the second!\n");
      } else {
        printf("Both variables are equal!\n");
      }
    }
    printf("Continue (y/n): ");
    scanf(" %c", &answer);
    switch (answer) {
      case 'y':
      case 'Y':
        continue_loop = 1;
        break;
      case 'n':
      case 'N':
        continue_loop = 0;
        break;
      default:
        printf("Invalid option!\n");
    }
  }
  printf("Program finished!\n");
  return 0;
}
