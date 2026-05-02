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
void* Vec2_operator_add__N9constVec2(Vec2* self, void* o);
void* Vec2_operator_eq__N9constVec2(Vec2* self, void* o);
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

void* Vec2_operator_add__N9constVec2(Vec2* self, void* o) {
  (void)self;
  (void)o;
  return (void*)0;
}

void* Vec2_operator_eq__N9constVec2(Vec2* self, void* o) {
  (void)self;
  (void)o;
  return (void*)0;
}

double Vec2_dot__pv(Vec2* self, Vec2* o) {
  (void)self;
  (void)o;
  return (double)0;
}

double Vec2_lengthSq(Vec2* self) {
  (void)self;
  return (double)0;
}

/* Global functions */
int main(void);

int main(void) {
  Vec2 a;
  Vec2_init__dd(&a, 3.0, 4.0);
  Vec2 b;
  Vec2_init__N6object(&b, a);
  Vec2 c;
  Vec2_init__dd(&c, 0.0, 0.0);
  Vec2 d;
  Vec2_init__dd(&d, 3.0, 4.0);

  if (a.x == 3.0) {
    printf("PASS ctor_x\n");
  }
  if (a.y == 4.0) {
    printf("PASS ctor_y\n");
  }
  if (b.x == 3.0 && b.y == 4.0) {
    printf("PASS copy_ctor\n");
  }
  c = a;
  if (c.x == 3.0 && c.y == 4.0) {
    printf("PASS assign_op\n");
  }
  if (sum.x == 4.0 && sum.y == 6.0) {
    printf("PASS op_add\n");
  }
  if (a == d) {
    printf("PASS op_eq_true\n");
  }
  if (a.dot(Vec2(1.0, 0.0)) == 3.0) {
    printf("PASS dot_x_axis\n");
  }
  if (a.lengthSq() == 25.0) {
    printf("PASS length_sq\n");
  }
  printf("ALL PASS\n");
  return 0;
}
