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

/* Global functions */
int main(void);

int main(void) {
  int a = 0;
  float b = 0;
  char c = 0;
  char d[] = "Hello World!";
  int* pi = 0;
  float* pf = 0;
  char* pc = 0;

  a = 1;
  b = 2;
  c = 'x';
  pi = &a;
  pf = &b;
  pc = &c;
  printf("Address pointed to by pi: %ld, value at the address pointed to by pi: %d\n", pi, *pi);
  printf("Address pointed to by pf: %ld, value at the address pointed to by pf: %f\n", pf, *pf);
  printf("Address pointed to by pc: %ld, value at the address pointed to by pc: %c\n", pc, *pc);
  pc = d;
  printf("Address pointed to by pc: %ld, value at the address pointed to by pc: %c\n", pc, *pc);
  pc++;
  printf("Address pointed to by pc: %ld, value at the address pointed to by pc: %c\n", pc, *pc);
  (*pc)++;
  printf("Address pointed to by pc: %ld, value at the address pointed to by pc: %c\n", pc, *pc);
  printf("%s", d);
  printf("\n");
  return 0;
}
