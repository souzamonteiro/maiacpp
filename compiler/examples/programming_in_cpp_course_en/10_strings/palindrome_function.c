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
int is_palindrome__pv(char* word);
int main(void);

int is_palindrome__pv(char* word) {
  int i, j;
  int result = 1;
  j = strlen(word) - 1;
  for (i = 0; i < strlen(word); i++) {
    if (word[i] != word[j]) {
            result = 0;
        }
        j--;
  }
  return result;
}

int main(void) {
  char word[50];

  printf("Enter a word: ");
  scanf("%s", word);
  if (is_palindrome__pv(word)) {
    printf("The word is a palindrome!");
  } else {
    printf("The word is not a palindrome!");
  }
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-cstyle-body=1) */
/* - is_palindrome: structured-cstyle-body (5 stmt(s)) */
