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

#define EXC_Rectangle 1

typedef struct Rectangle {
  int height;
  int base;
} Rectangle;

void Rectangle_init__ii(Rectangle* self, int a, int b);
void Rectangle_init(Rectangle* self);
void Rectangle_destroy(Rectangle* self);
void Rectangle_setValues__ii(Rectangle* self, int a, int b);
int Rectangle_calcArea(Rectangle* self);

void Rectangle_init__ii(Rectangle* self, int a, int b) {
  (void)self;
  self->height = a;
  self->base = b;
  (void)a;
  (void)b;
}

void Rectangle_init(Rectangle* self) {
  (void)self;
}

void Rectangle_destroy(Rectangle* self) {
  (void)self;
}

void Rectangle_setValues__ii(Rectangle* self, int a, int b) {
  (void)self;
  self->height = a;
  self->base = b;
  (void)a;
  (void)b;
}

int Rectangle_calcArea(Rectangle* self) {
  (void)self;
  return self->base * self->height;
}

/* Global functions */
int main(void);

int main(void) {
  Rectangle rectangle;
  Rectangle_init__ii(&rectangle, 4, 5);

  printf("The area of the rectangle is ");
  printf("%d", Rectangle_calcArea(&rectangle));
  printf(".\n");
  return 0;
}
