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
int Stack_push__pv(Stack* self, void* v);
int Stack_pop__pv(Stack* self, void* v);
int Stack_size(Stack* self);

void Stack_init(Stack* self) {
  (void)self;
}

void Stack_destroy(Stack* self) {
  (void)self;
}

int Stack_push__pv(Stack* self, void* v) {
  (void)self;
  if (self->top_ >= (int)(sizeof(self->data) / sizeof(self->data[0]))) return 0;
  self->data[self->top_++] = v;
  return 1;
  (void)v;
}

int Stack_pop__pv(Stack* self, void* v) {
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
  int ip = 10;
  int iq = 20;

  if (tmax(3, 7) == 7) {
    printf("PASS tmax_int_r\n");
  }
  if (tmax(9, 2) == 9) {
    printf("PASS tmax_int_l\n");
  }
  if (tmax(1.5, 2.7) == 2.7) {
    printf("PASS tmax_double\n");
  }
  tswap__pvpv(ip, iq);
  if (ip == 20 && iq == 10) {
    printf("PASS tswap_int\n");
  }
  if (si.size() == 3) {
    printf("PASS stack_size_3\n");
  }
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-template-swap=1) */
/* - tswap: structured-template-swap (swap-by-pointer) */
