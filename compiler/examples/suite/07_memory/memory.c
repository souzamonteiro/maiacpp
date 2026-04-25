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

#define EXC_Widget 1
#define EXC_IntBuf 2

typedef struct Widget {
  int id;
} Widget;

void Widget_init(Widget* self);
void Widget_init__i(Widget* self, int n);
void Widget_destroy(Widget* self);

void Widget_init(Widget* self) {
  (void)self;
}

void Widget_init__i(Widget* self, int n) {
  (void)self;
  self->id = n;
}

void Widget_destroy(Widget* self) {
  (void)self;
}

typedef struct IntBuf {
  int data_;
  int n_;
} IntBuf;

void IntBuf_init__i(IntBuf* self, int n);
void IntBuf_destroy(IntBuf* self);
int IntBuf_operator_subscript__i(IntBuf* self, int i);

void IntBuf_init__i(IntBuf* self, int n) {
  (void)self;
  self->n_ = n;
}

void IntBuf_destroy(IntBuf* self) {
  (void)self;
}

int IntBuf_operator_subscript__i(IntBuf* self, int i) {
  (void)self;
  return self->data_[i];
}

/* Global functions */
int main(void);

int main(void) {
  return (int)0;
}

/* Lowering diagnostics: 1 event(s) (stub-fallback=1) */
/* - main: stub-fallback (resource-lowering-failed) */
