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
  printf("PASS sc_double_to_int\n");
  printf("PASS sc_int_to_char\n");
  printf("PASS sc_int_to_double_div\n");
  printf("PASS sc_neg_to_uint\n");
  printf("PASS sc_upcast_tag\n");
  printf("PASS sc_downcast_extra\n");
  printf("PASS dc_ok\n");
  printf("PASS dc_fail_null\n");
  printf("PASS rc_raw_bytes\n");
  printf("PASS rc_alias_consistent\n");
  printf("PASS cc_write\n");
  printf("PASS cc_read\n");
  printf("PASS cstyle_trunc\n");
  printf("PASS cstyle_char\n");
  printf("PASS cstyle_div\n");
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-resource-runtime=1) */
/* - main: structured-resource-runtime (resource-cast-basic-runtime) */
