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
int char_count__pvc(char* s, char c);
int main(void);

int char_count__pvc(char* s, char c) {
  int n = 0;
  while (*s) {
    if (*s == c) ++n; ++s;
  }
  return n;
}

int main(void) {
  char s1[64];

  if (strlen("") == 0) {
    printf("PASS strlen_empty\n");
  }
  if (strlen("hello") == 5) {
    printf("PASS strlen_5\n");
  }
  if (strcmp("abc", "abc") == 0) {
    printf("PASS strcmp_eq\n");
  }
  if (strcmp("mississippi", "mississippi") == 0 && 
        char_count__pvc("mississippi", 's') == 4) {
    printf("PASS char_count\n");
  }
  strcpy(s1, "foo");
  strcat(s1, "bar");
  if (strcmp(s1, "foobar") == 0) {
    printf("PASS strcat\n");
  }
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-cstyle-body=1) */
/* - char_count: structured-cstyle-body (3 stmt(s)) */
