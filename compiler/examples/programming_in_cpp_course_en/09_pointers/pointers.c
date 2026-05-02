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
static int __cpp_toupper(int c) { return (c >= 97 && c <= 122) ? c - 32 : c; }
#define toupper(c) __cpp_toupper(c)

/* Global functions */
int length__pv(char* txt);
void to_uppercase__pvpv(char* source, char* dest);
int main(void);

int length__pv(char* txt) {
  int n;
  n = 0;
  while (txt[n]) {
    n++;
  }
  return n;
}

void to_uppercase__pvpv(char* source, char* dest) {
  char *p;
  char *q;
  p = source;
  q = dest;
  while (*p) {
    *q = toupper(*p);
        p++;
        q++;
  }
  *q = '\0';
}

int main(void) {
  char name[20];
  char name_upper[20];
  char* p = 0;
  int i = 0;

  printf("Enter your name: ");
  scanf("%s", name);
  printf("Hello ");
  printf("%s", name);
  printf("!\n");
  printf(">>");
  for (i = 0; i < 20; i++) {
    printf("%c", name[i]);
  }
  printf("<<\n");
  printf(">>");
  i = 0;
  while (name[i] != '\0') {
    printf("%c", name[i]);
    i++;
  }
  printf("<<\n");
  printf(">>");
  i = 0;
  while (name[i]) {
    printf("%c", name[i]);
    i++;
  }
  printf("<<\n");
  printf("Your name has ");
  printf("%d", length__pv(name));
  printf(" characters.\n");
  printf("Your name has ");
  printf("%d", strlen(name));
  printf(" characters.\n");
  p = name;
  printf(">>");
  while (*p) {
    printf("%c", *p);
    p++;
  }
  printf("<<\n");
  to_uppercase__pvpv(name, name_upper);
  printf("Your name in uppercase is ");
  printf("%s", name_upper);
  printf(".\n");
  return 0;
}

/* Lowering diagnostics: 2 event(s) (structured-cstyle-body=2) */
/* - length: structured-cstyle-body (4 stmt(s)) */
/* - to_uppercase: structured-cstyle-body (6 stmt(s)) */
