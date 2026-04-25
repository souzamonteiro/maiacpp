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

#define EXC_Stack 1

typedef struct Stack {
  int data[N];
  int top_;
} Stack;

void Stack_init(Stack* self);
void Stack_destroy(Stack* self);
void* Stack_push__N6constT(Stack* self, int v);
void* Stack_pop__N1T(Stack* self, int v);
int Stack_size(Stack* self);

void Stack_init(Stack* self) {
  (void)self;
}

void Stack_destroy(Stack* self) {
  (void)self;
}

void* Stack_push__N6constT(Stack* self, int v) {
  (void)self;
  (void)v;
  return (void*)0;
}

void* Stack_pop__N1T(Stack* self, int v) {
  (void)self;
  (void)v;
  return (void*)0;
}

int Stack_size(Stack* self) {
  (void)self;
  return self->top_;
}

/* Global functions */
int tswap__pvpv(void* a, void* b);
int main(void);

int tswap__pvpv(void* a, void* b) {
  (void)a;
  (void)b;
  return (int)0;
}

int main(void) {
  return (int)0;
}

/* Lowering diagnostics: 2 event(s) (stub-fallback=2) */
/* - tswap: stub-fallback (no-supported-lowering) */
/* - main: stub-fallback (no-supported-lowering) */
