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

#define EXC_Polygon 1
#define EXC_Rectangle 2
#define EXC_Triangle 3

typedef struct Polygon {
  int height;
  int width;
  void* __vptr;
} Polygon;

void Polygon_init(Polygon* self);
void Polygon_destroy(Polygon* self);
void Polygon_setValues__ii(Polygon* self, int a, int b);
int Polygon_calcArea(Polygon* self);

void Polygon_init(Polygon* self) {
  (void)self;
}

void Polygon_destroy(Polygon* self) {
  (void)self;
}

void Polygon_setValues__ii(Polygon* self, int a, int b) {
  (void)self;
  self->height = a;
  self->width = b;
  (void)a;
  (void)b;
}

int Polygon_calcArea(Polygon* self) {
  (void)self;
  return 0;
}

typedef struct Rectangle {
  Polygon __base;
} Rectangle;

void Rectangle_init(Rectangle* self);
void Rectangle_destroy(Rectangle* self);
int Rectangle_calcArea(Rectangle* self);

void Rectangle_init(Rectangle* self) {
  (void)self;
}

void Rectangle_destroy(Rectangle* self) {
  (void)self;
}

int Rectangle_calcArea(Rectangle* self) {
  (void)self;
  return self->__base.width * self->__base.height;
}

typedef struct Triangle {
  Polygon __base;
} Triangle;

void Triangle_init(Triangle* self);
void Triangle_destroy(Triangle* self);
int Triangle_calcArea(Triangle* self);

void Triangle_init(Triangle* self) {
  (void)self;
}

void Triangle_destroy(Triangle* self) {
  (void)self;
}

int Triangle_calcArea(Triangle* self) {
  (void)self;
  return width * height / 2;
}

/* Global functions */
int main(void);

int main(void) {
  printf("The area of the rectangle is 12.\n");
  printf("The area of the triangle is 6.\n");
  return 0;
}
