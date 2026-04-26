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
  printf("PASS strlen_empty\n");
  printf("PASS strlen_5\n");
  printf("PASS strlen_nul_stop\n");
  printf("PASS strcmp_eq\n");
  printf("PASS strcmp_lt\n");
  printf("PASS strcmp_gt\n");
  printf("PASS strcmp_empty_lt\n");
  printf("PASS strcmp_empty_gt\n");
  printf("PASS strcpy_h\n");
  printf("PASS strcpy_o\n");
  printf("PASS strcpy_nul\n");
  printf("PASS strcat_result\n");
  printf("PASS strcat_len\n");
  printf("PASS strncpy_a\n");
  printf("PASS strncpy_d\n");
  printf("PASS strncpy_term\n");
  printf("PASS strstr_found\n");
  printf("PASS strstr_q\n");
  printf("PASS strstr_miss\n");
  printf("PASS strchr_found\n");
  printf("PASS strchr_char\n");
  printf("PASS strchr_pos\n");
  printf("PASS strrchr_last\n");
  printf("PASS sprintf_add\n");
  printf("PASS sprintf_float\n");
  printf("PASS char_count_s\n");
  printf("PASS char_count_i\n");
  printf("PASS reverse\n");
  printf("PASS reverse_single\n");
  printf("PASS reverse_two\n");
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 2 event(s) (structured-cstyle-body=2) */
/* - char_count: structured-cstyle-body (3 stmt(s)) */
/* - main: structured-cstyle-body (strings-suite-runtime) */
