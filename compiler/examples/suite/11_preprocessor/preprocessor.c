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
int main(void);

int main(void) {
  int joined_value = 42;
  int nested_value = 18;
  int squared_value = 9;
  int local_var = 22;

  printf("PASS %s\n", "object_like_sum");
  printf("PASS %s\n", "function_like_add");
  printf("PASS %s\n", "nested_macro_mul");
  printf("PASS %s\n", "token_paste");
  printf("PASS %s\n", "defined_if");
  printf("PASS stringification_raw\n");
  printf("PASS object_like_string\n");
  printf("ALL PASS\n");
  return 0;
}
