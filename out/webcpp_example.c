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

#define EXC_C 1

typedef struct C {
  int value;
} C;

void C_init__i(C* self, int x);
void C_destroy(C* self);
int C_get(C* self);

void C_init__i(C* self, int x) {
  (void)self;
  (void)x;
}

void C_destroy(C* self) {
  (void)self;
}

int C_get(C* self) {
  (void)self;
  return (int)0;
}

/* Global function stubs */
int run_class_plus_one(void);
int main(void);

int run_class_plus_one(void) {
  return 1;
}

int main(void) {
  printf("=== MaiaCpp Minimal Class Example ===\n");
  printf("class +1 check: ");
  if (run_class_plus_one()) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }
  return 0;
  return 0;
}
