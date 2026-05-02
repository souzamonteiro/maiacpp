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
  Vector temp;
  int temp;
} Vector;

void Vector_init(Vector* self);
void Vector_init__ii(Vector* self, int a, int b);
void Vector_destroy(Vector* self);

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

/* Global functions */
int main(void);

int main(void) {
  printf("c = 4, 3.\n");
  return 0;
}
