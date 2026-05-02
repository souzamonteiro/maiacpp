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
void display_message__pv(char* text);
int square__i(int x);
int main(void);

void display_message__pv(char* text) {
  printf("%s", text);
  printf("\n");
}

int square__i(int x) {
  return x * x;
}

int main(void) {
  char msg[] = "Hello World!";

  display_message__pv(msg);
  printf("The square of 5 is ");
  printf("%d", square__i(5));
  printf(".\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-io-runtime=1) */
/* - display_message: structured-io-runtime (structured-io-runtime) */
