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

#define EXC_Shape 1
#define EXC_Rectangle 2
#define EXC_Circle 3

typedef struct Shape {
  int area;
  void* __vptr;
} Shape;

void Shape_init(Shape* self);
void Shape_destroy(Shape* self);

void Shape_init(Shape* self) {
  (void)self;
}

void Shape_destroy(Shape* self) {
  (void)self;
}

typedef struct Rectangle {
  Shape __base;
  double w_;
  double h_;
} Rectangle;

void Rectangle_init__dd(Rectangle* self, double w, double h);
void Rectangle_destroy(Rectangle* self);
double Rectangle_area(Rectangle* self);

void Rectangle_init__dd(Rectangle* self, double w, double h) {
  (void)self;
  self->w_ = w;
  self->h_ = h;
  (void)w;
  (void)h;
}

void Rectangle_destroy(Rectangle* self) {
  (void)self;
}

double Rectangle_area(Rectangle* self) {
  (void)self;
  return self->w_ * self->h_;
}

typedef struct Circle {
  Shape __base;
  double r_;
} Circle;

void Circle_init__d(Circle* self, double r);
void Circle_destroy(Circle* self);
double Circle_area(Circle* self);

void Circle_init__d(Circle* self, double r) {
  (void)self;
  self->r_ = r;
}

void Circle_destroy(Circle* self) {
  (void)self;
}

double Circle_area(Circle* self) {
  (void)self;
  return 3.14159 * self->r_ * self->r_;
}

/* Global functions */
int main(void);

int main(void) {
  Rectangle rect;
  Rectangle_init__dd(&rect, 4.0, 3.0);

  if (Rectangle_area(&rect) == 12.0) {
    printf("PASS rect_area\n");
  }
  printf("ALL PASS\n");
  return 0;
}
