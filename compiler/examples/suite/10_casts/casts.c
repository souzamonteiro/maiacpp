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
  (void)t;
  (void)e;
}

void Derived_destroy(Derived* self) {
  (void)self;
}

/* Global functions */
int main(void);

int main(void) {
  return (int)0;
}

/* Lowering diagnostics: 1 event(s) (stub-fallback=1) */
/* - main: stub-fallback (resource-lowering-failed) */
