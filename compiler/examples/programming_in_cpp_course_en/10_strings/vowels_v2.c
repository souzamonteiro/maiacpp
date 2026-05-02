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
  int i = 0;
  char name[255];
  int count = 0;

  printf("Enter your name: ");
  scanf("%s", name);
  printf("\n");
  count = 0;
  for (i = 0; i < strlen(name); i++) {
    if ((name[i] == 'a') || (name[i] == 'e') || (name[i] == 'i') || (name[i] == 'o') || (name[i] == 'u')) {
      count++;
    }
  }
  printf("Your name contains ");
  printf("%d", count);
  printf(" vowels.\n");
  return 0;
}
