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
void hello_world(void);
void display_message__pv(char* msg);
float square__f(float x);
float power__fi(float x, int y);
int main(void);

void hello_world(void) {
  printf("Hello World!\n");
}

void display_message__pv(char* msg) {
  printf("%s", msg);
  printf("\n");
}

float square__f(float x) {
  return x * x;
}

float power__fi(float x, int y) {
  int i;
  float p;
  p = 1;
  for (i = 0; i < y; i++) {
    p = p * x;
  }
  return p;
}

int main(void) {
  hello_world();
  display_message__pv("Hi there!");
  printf("The square of 5 is ");
  printf("%g", square__f((float)(5)));
  printf(".\n");
  printf("The cube of 2 is ");
  printf("%g", power__fi((float)(2), 3));
  printf(".\n");
  return 0;
}

/* Lowering diagnostics: 3 event(s) (structured-cstyle-body=1, structured-io-runtime=2) */
/* - hello_world: structured-io-runtime (structured-io-runtime) */
/* - display_message: structured-io-runtime (structured-io-runtime) */
/* - power: structured-cstyle-body (5 stmt(s)) */
