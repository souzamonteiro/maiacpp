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

/* Inline ctype helpers (MaiaC-compatible, no external import needed) */
static int __cpp_tolower(int c) { return (c >= 65 && c <= 90) ? c + 32 : c; }
#define tolower(c) __cpp_tolower(c)

/* Global functions */
int count_vowels__pv(char* name);
int main(void);

int count_vowels__pv(char* name) {
  int i;
  int n = 0;
  for (i = 0; i < strlen(name); i++) {
    if (tolower(name[i]) == 'a' || tolower(name[i]) == 'e' || tolower(name[i]) == 'i' || tolower(name[i]) == 'o' || tolower(name[i]) == 'u') {
            n++;
        }
  }
  return n;
}

int main(void) {
  char name[50];

  printf("Enter your name: ");
  scanf("%s", name);
  printf("Your name has ");
  printf("%d", count_vowels__pv(name));
  printf(" vowels.\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-cstyle-body=1) */
/* - count_vowels: structured-cstyle-body (4 stmt(s)) */
