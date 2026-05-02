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
  int j = 0;
  char word[255];
  int is_palindrome = 0;

  printf("Enter a word: ");
  scanf("%s", word);
  printf("\n");
  i = 0;
  j = strlen(word) - 1;
  is_palindrome = 1;
  while (i < strlen(word)) {
    if (word[i] != word[j]) {
      is_palindrome = 0;
      break;
    }
    if (i == j) {
      break;
    }
    i++;
    j--;
  }
  if (is_palindrome) {
    printf("The word is a palindrome!");
  } else {
    printf("The word is not a palindrome!");
  }
  return 0;
}
