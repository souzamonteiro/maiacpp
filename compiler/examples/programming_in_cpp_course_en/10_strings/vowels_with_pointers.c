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
int count_vowels_with_repeat__pv(char* name);
int count_vowels_no_repeat__pv(char* name);
int main(void);

int count_vowels_with_repeat__pv(char* name) {
  char *p;
  int n = 0;
  p = name;
  while (*p) {
    switch (tolower(*p)) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                n++;
                break;
            default:
                break;
        }
        p++;
  }
  return n;
}

int count_vowels_no_repeat__pv(char* name) {
  char *p;
  int na = 0;
  int ne = 0;
  int ni = 0;
  int no = 0;
  int nu = 0;
  p = name;
  while (*p) {
    switch (tolower(*p)) {
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
        p++;
  }
  return na + ne + ni + no + nu;
}

int main(void) {
  char name[50];

  printf("Enter your name: ");
  scanf("%s", name);
  printf("Your name has ");
  printf("%d", count_vowels_with_repeat__pv(name));
  printf(" vowels (with repetition).\n");
  printf("Your name has ");
  printf("%d", count_vowels_no_repeat__pv(name));
  printf(" vowels (without repetition).\n");
  return 0;
}

/* Lowering diagnostics: 2 event(s) (structured-cstyle-body=2) */
/* - count_vowels_with_repeat: structured-cstyle-body (5 stmt(s)) */
/* - count_vowels_no_repeat: structured-cstyle-body (9 stmt(s)) */
