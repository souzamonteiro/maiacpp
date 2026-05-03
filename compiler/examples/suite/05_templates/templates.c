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
int tmax__N1TN1T(int a, int b);
int main(void);

int tmax__N1TN1T(int a, int b) {
  return a > b ? a : b;
}

int main(void) {
  if (tmax__N1TN1T(3, 7) == 7) {
    printf("PASS tmax_int_r\n");
  }
  if (tmax__N1TN1T(9, 2) == 9) {
    printf("PASS tmax_int_l\n");
  }
  printf("ALL PASS\n");
  return 0;
}
