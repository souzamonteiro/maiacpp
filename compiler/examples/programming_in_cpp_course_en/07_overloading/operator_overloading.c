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
Vector Vector_operator_add__N6Vector(Vector* self, Vector param);

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

Vector Vector_operator_add__N6Vector(Vector* self, Vector param) {
  (void)self;
  Vector temp;
  temp.x = self->x + param.x;
  temp.y = self->y + param.y;
  return temp;
  (void)param;
}

/* Global functions */
int main(void);

int main(void) {
  Vector a;
  Vector_init__ii(&a, 3, 1);
  Vector b;
  Vector_init__ii(&b, 1, 2);
  Vector c;

  c.x = a.x + b.x;
  c.y = a.y + b.y;
  printf("c = ");
  printf("%d", c.x);
  printf(", ");
  printf("%d", c.y);
  printf(".\n");
  return 0;
}
