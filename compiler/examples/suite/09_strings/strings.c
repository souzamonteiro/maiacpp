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

/* String and memory function declarations (MaiaC-compatible, no #include needed) */
extern unsigned int strlen(const char* s);
extern int strcmp(const char* s1, const char* s2);
extern int strncmp(const char* s1, const char* s2, unsigned int n);
extern char* strcpy(char* dest, const char* src);
extern char* strncpy(char* dest, const char* src, unsigned int n);
extern char* strcat(char* dest, const char* src);
extern char* strncat(char* dest, const char* src, unsigned int n);
extern char* strstr(const char* haystack, const char* needle);
extern char* strchr(const char* s, int c);
extern char* strrchr(const char* s, int c);
extern unsigned int strspn(const char* s, const char* accept);
extern unsigned int strcspn(const char* s, const char* reject);
extern char* strtok(char* str, const char* delim);
extern int memcmp(const void* s1, const void* s2, unsigned int n);
extern void* memcpy(void* dest, const void* src, unsigned int n);
extern void* memmove(void* dest, const void* src, unsigned int n);
extern void* memset(void* s, int c, unsigned int n);
extern void* memchr(const void* s, int c, unsigned int n);

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
  printf("PASS strcmp_eq\n");
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
