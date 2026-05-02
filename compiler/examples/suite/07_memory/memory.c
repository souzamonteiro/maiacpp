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
  int* p = (int*)__malloc(sizeof(int));
  int* arr = (int*)__malloc(6 * sizeof(int));
  double* d = (double*)__malloc(sizeof(double));

  if (p != 0) {
    printf("PASS new_not_null\n");
  }
  *p = 42;
  if (*p == 42) {
    printf("PASS new_id\n");
  }
  __free(p);
  if (arr != 0) {
    printf("PASS arr_not_null\n");
  }
  for (int i = 0; i < 6; ++i) {
    arr[i] = (i + 1) * (i + 1);
  }
  if (arr[0] == 1 && arr[5] == 36) {
    printf("PASS int_arr\n");
  }
  __free(arr);
  if (d != 0) {
    printf("PASS double_not_null\n");
  }
  *d = 3.14;
  if (*d > 3.0 && *d < 4.0) {
    printf("PASS double_val\n");
  }
  __free(d);
  printf("ALL PASS\n");
  return 0;
}
