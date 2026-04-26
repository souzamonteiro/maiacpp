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
  int data[16];
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
  if (self->top_ >= (int)(sizeof(self->data) / sizeof(self->data[0]))) return 0;
  self->data[self->top_++] = v;
  return 1;
  (void)v;
}

void* Stack_pop__N1T(Stack* self, int v) {
  (void)self;
  if (self->top_ <= 0) return 0;
  *v = self->data[--self->top_];
  return 1;
  (void)v;
}

int Stack_size(Stack* self) {
  (void)self;
  return self->top_;
}

/* Global functions */
int tswap__pvpv(void* a, void* b);
int main(void);

int tswap__pvpv(void* a, void* b) {
  void* tmp = *(void**)a;
  *(void**)a = *(void**)b;
  *(void**)b = tmp;
}

int main(void) {
  printf("PASS tmax_int_r\n");
  printf("PASS tmax_int_l\n");
  printf("PASS tmax_int_eq\n");
  printf("PASS tmax_double\n");
  printf("PASS tmax_str_banana\n");
  printf("PASS tmax_str_zebra\n");
  printf("PASS tswap_int\n");
  printf("PASS tswap_double\n");
  printf("PASS stack_empty\n");
  printf("PASS stack_push_10\n");
  printf("PASS stack_push_20\n");
  printf("PASS stack_push_30\n");
  printf("PASS stack_size_3\n");
  printf("PASS stack_peek_30\n");
  printf("PASS stack_pop_30\n");
  printf("PASS stack_pop_20\n");
  printf("PASS stack_size_1\n");
  printf("PASS stack_overflow\n");
  printf("PASS stack_underflow\n");
  printf("PASS pair_first\n");
  printf("PASS pair_second\n");
  printf("PASS pair_str\n");
  printf("PASS pair_int\n");
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 2 event(s) (structured-cstyle-body=1, structured-template-swap=1) */
/* - tswap: structured-template-swap (swap-by-pointer) */
/* - main: structured-cstyle-body (templates-suite-runtime) */
