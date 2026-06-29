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

#define EXC_Vector 1

typedef struct Vector {
  int x;
  int y;
} Vector;

void Vector_init(Vector* self);
void Vector_init__ii(Vector* self, int a, int b);
void Vector_destroy(Vector* self);
Vector Vector_operator_add__N6Vector(Vector* self, Vector a);
Vector Vector_operator_sub__N6Vector(Vector* self, Vector a);
int Vector_getX(Vector* self);
int Vector_getY(Vector* self);

void Vector_init(Vector* self) {
  (void)self;
}

void Vector_init__ii(Vector* self, int a, int b) {
  (void)self;
  self->x = a;
  self->y = b;
  (void)a;
  (void)b;
}

void Vector_destroy(Vector* self) {
  (void)self;
}

Vector Vector_operator_add__N6Vector(Vector* self, Vector a) {
  (void)self;
  Vector temp;
  temp.x = self->x + a.x;
  temp.y = self->y + a.y;
  return temp;
  (void)a;
}

Vector Vector_operator_sub__N6Vector(Vector* self, Vector a) {
  (void)self;
  Vector temp;
  temp.x = self->x - a.x;
  temp.y = self->y - a.y;
  return temp;
  (void)a;
}

int Vector_getX(Vector* self) {
  (void)self;
  return self->x;
}

int Vector_getY(Vector* self) {
  (void)self;
  return self->y;
}

/* Global functions */
int main(void);

int main(void) {
  Vector a;
  Vector_init__ii(&a, 1, 2);
  Vector b;
  Vector_init__ii(&b, 3, 4);
  Vector c;
  Vector d;

  c.x = a.x + b.x;
  c.y = a.y + b.y;
  d.x = a.x - b.x;
  d.y = a.y - b.y;
  printf("c(");
  printf("%d", Vector_getX(&c));
  printf(",");
  printf("%d", Vector_getY(&c));
  printf(")");
  printf("\n");
  printf("d(");
  printf("%d", Vector_getX(&d));
  printf(",");
  printf("%d", Vector_getY(&d));
  printf(")");
  printf("\n");
  return 0;
}
