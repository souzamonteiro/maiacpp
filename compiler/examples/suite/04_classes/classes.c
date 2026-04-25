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

#define EXC_Vec2 1

typedef struct Vec2 {
  double x;
  double y;
} Vec2;

void Vec2_init__dd(Vec2* self, double x_, double y_);
void Vec2_init__N9constVec2(Vec2* self, int o);
void Vec2_destroy(Vec2* self);
void* Vec2_operator_assign__N9constVec2(Vec2* self, int o);
void* Vec2_operator_add__N9constVec2(Vec2* self, int o);
void* Vec2_operator_eq__N9constVec2(Vec2* self, int o);
void* Vec2_dot__N9constVec2(Vec2* self, int o);
int Vec2_lengthSq(Vec2* self);

void Vec2_init__dd(Vec2* self, double x_, double y_) {
  (void)self;
  (void)x_;
  (void)y_;
}

void Vec2_init__N9constVec2(Vec2* self, int o) {
  (void)self;
  (void)o;
}

void Vec2_destroy(Vec2* self) {
  (void)self;
}

void* Vec2_operator_assign__N9constVec2(Vec2* self, int o) {
  (void)self;
  (void)o;
  return (void*)0;
}

void* Vec2_operator_add__N9constVec2(Vec2* self, int o) {
  (void)self;
  (void)o;
  return (void*)0;
}

void* Vec2_operator_eq__N9constVec2(Vec2* self, int o) {
  (void)self;
  (void)o;
  return (void*)0;
}

void* Vec2_dot__N9constVec2(Vec2* self, int o) {
  (void)self;
  (void)o;
  return (void*)0;
}

int Vec2_lengthSq(Vec2* self) {
  (void)self;
  return (int)0;
}

/* Global functions */
int main(void);

int main(void) {
  return (int)0;
}

/* Lowering diagnostics: 1 event(s) (stub-fallback=1) */
/* - main: stub-fallback (no-supported-lowering) */
