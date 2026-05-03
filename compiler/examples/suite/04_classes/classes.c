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
void Vec2_init__pv(Vec2* self, Vec2* o);
void Vec2_destroy(Vec2* self);
void* Vec2_operator_assign__N9constVec2(Vec2* self, void* o);
double Vec2_dot__pv(Vec2* self, Vec2* o);
double Vec2_lengthSq(Vec2* self);

void Vec2_init__dd(Vec2* self, double x_, double y_) {
  (void)self;
  self->x = x_;
  self->y = y_;
  (void)x_;
  (void)y_;
}

void Vec2_init__pv(Vec2* self, Vec2* o) {
  (void)self;
  (void)o;
}

void Vec2_destroy(Vec2* self) {
  (void)self;
}

void* Vec2_operator_assign__N9constVec2(Vec2* self, void* o) {
  (void)self;
  (void)o;
  return (void*)0;
}

double Vec2_dot__pv(Vec2* self, Vec2* o) {
  (void)self;
  return self->x * o->x + self->y * o->y;
  (void)o;
}

double Vec2_lengthSq(Vec2* self) {
  (void)self;
  return self->x * self->x + self->y * self->y;
}

/* Global functions */
int main(void);

int main(void) {
  Vec2 a;
  Vec2_init__dd(&a, 3.0, 4.0);
  if (a.x == 3.0) printf("PASS ctor_x\n");
  if (a.y == 4.0) printf("PASS ctor_y\n");
  Vec2 b = a;
  if (b.x == 3.0 && b.y == 4.0) printf("PASS copy_ctor\n");
  Vec2 c;
  Vec2_init__dd(&c, 0.0, 0.0);
  c = a;
  if (c.x == 3.0 && c.y == 4.0) printf("PASS assign_op\n");
  Vec2 unit;
  Vec2_init__dd(&unit, 1.0, 0.0);
  if (Vec2_dot__pv(&a, &unit) == 3.0) printf("PASS dot_x_axis\n");
  if (Vec2_lengthSq(&a) == 25.0) printf("PASS length_sq\n");
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-cstyle-body=1) */
/* - main: structured-cstyle-body (16 stmt(s)) */
