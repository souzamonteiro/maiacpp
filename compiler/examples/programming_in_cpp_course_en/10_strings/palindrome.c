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
  char word[50];
  int i = 0;
  int j = 0;
  int is_palindrome = 1;

  printf("Enter a word: ");
  scanf("%s", word);
  j = strlen(word) - 1;
  for (i = 0; i < strlen(word); i++) {
    if (word[i] != word[j]) {
      is_palindrome = 0;
    }
    j--;
  }
  if (is_palindrome) {
    printf("The word is a palindrome!");
  } else {
    printf("The word is not a palindrome!");
  }
  return 0;
}
