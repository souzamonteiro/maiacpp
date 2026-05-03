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

#define EXC_MultiplicationTable 1

typedef struct MultiplicationTable {
  int __dummy;
} MultiplicationTable;

void MultiplicationTable_init(MultiplicationTable* self);
void MultiplicationTable_destroy(MultiplicationTable* self);
void MultiplicationTable_createTable__i(MultiplicationTable* self, int n);

void MultiplicationTable_init(MultiplicationTable* self) {
  (void)self;
}

void MultiplicationTable_destroy(MultiplicationTable* self) {
  (void)self;
}

void MultiplicationTable_createTable__i(MultiplicationTable* self, int n) {
  (void)self;
  int i;
  for (i = 1; i <= 10; i++) {
    printf("%d x %d = %d\n", n, i, n * i);
  }
  (void)n;
}

/* Global functions */
int main(void);

int main(void) {
  MultiplicationTable multTable;
  int n = 0;

  printf("Which multiplication table would you like to display? ");
  scanf("%d", &n);
  printf("\n");
  MultiplicationTable_createTable__i(&multTable, n);
  return 0;
}
