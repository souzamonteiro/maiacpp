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
  int j = 65;
  Derived deriv;
  Derived_init__ii(&deriv, 10, 99);
  int mutable_val = 55;
  int* cptr = &mutable_val;
  double pi = 3.14159;

  if (di == 3) {
    printf("PASS sc_double_to_int\n");
  }
  if (ch == 'A') {
    printf("PASS sc_int_to_char\n");
  }
  if (bp->tag == 10) {
    printf("PASS sc_upcast\n");
  }
  if (dp->extra == 99) {
    printf("PASS sc_downcast\n");
  }
  *mptr = 77;
  if (mutable_val == 77) {
    printf("PASS cc_write\n");
  }
  if (pi_i == 3) {
    printf("PASS cstyle_trunc\n");
  }
  printf("ALL PASS\n");
  return 0;
}
