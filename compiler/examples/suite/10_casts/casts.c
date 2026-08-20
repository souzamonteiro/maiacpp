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
extern int    sprintf(char* dest, const char* format, ...);

#define EXC_Base 1
#define EXC_Derived 2

typedef struct Base {
  int tag;
  void* __vptr;
} Base;

void Base_init__i(Base* self, int t);
void Base_destroy(Base* self);

void Base_init__i(Base* self, int t) {
  (void)self;
  self->tag = t;
}

void Base_destroy(Base* self) {
  (void)self;
}

typedef struct Derived {
  Base __base;
  int extra;
} Derived;

void Derived_init__ii(Derived* self, int t, int e);
void Derived_destroy(Derived* self);

void Derived_init__ii(Derived* self, int t, int e) {
  (void)self;
  Base_init__i((Base*)self, t);
  self->extra = e;
  (void)t;
  (void)e;
}

void Derived_destroy(Derived* self) {
  (void)self;
}

/* Global functions */
int main(void);

int main(void) {
  double d = 3.7;
  int di = (int)(d);
  if (di == 3) {
    (void)printf("PASS sc_double_to_int\n");
  }
  int j = 65;
  char ch = (char)(j);
  if (ch == 'A') {
    (void)printf("PASS sc_int_to_char\n");
  }
  Derived deriv;
  (void)Derived_init__ii(&deriv, 10, 99);
  Base* bp = (Base*)(&deriv);
  if (bp->tag == 10) {
    (void)printf("PASS sc_upcast\n");
  }
  Derived* dp = (Derived*)(bp);
  if (dp->extra == 99) {
    (void)printf("PASS sc_downcast\n");
  }
  int mutable_val = 55;
  const int* cptr = &mutable_val;
  int* mptr = (int*)(cptr);
  *mptr = 77;
  if (mutable_val == 77) {
    (void)printf("PASS cc_write\n");
  }
  double pi = 3.14159;
  int pi_i = (int)pi;
  if (pi_i == 3) {
    (void)printf("PASS cstyle_trunc\n");
  }
  (void)printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-resource-runtime=1) */
/* - main: structured-resource-runtime (22 stmt(s)) */
