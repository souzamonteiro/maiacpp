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

/* Inline ctype helpers (MaiaC-compatible, no external import needed) */
static int __cpp_tolower(int c) { return (c >= 65 && c <= 90) ? c + 32 : c; }
#define tolower(c) __cpp_tolower(c)

/* Global functions */
int count_unique_vowels__pv(char* name);
int main(void);

int count_unique_vowels__pv(char* name) {
  int i;
  int na = 0;
  int ne = 0;
  int ni = 0;
  int no = 0;
  int nu = 0;
  for (i = 0; i < strlen(name); i++) {
    switch (tolower(name[i])) {
            case 'a':
                if (na == 0) {
                    na++;
                }
                break;
            case 'e':
                if (ne == 0) {
                    ne++;
                }
                break;
            case 'i':
                if (ni == 0) {
                    ni++;
                }
                break;
            case 'o':
                if (no == 0) {
                    no++;
                }
                break;
            case 'u':
                if (nu == 0) {
                    nu++;
                }
                break;
            default:
                break;
        }
  }
  return na + ne + ni + no + nu;
}

int main(void) {
  char name[50];

  printf("Enter your name: ");
  scanf("%s", name);
  printf("Your name has ");
  printf("%d", count_unique_vowels__pv(name));
  printf(" unique vowels.\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-cstyle-body=1) */
/* - count_unique_vowels: structured-cstyle-body (8 stmt(s)) */
