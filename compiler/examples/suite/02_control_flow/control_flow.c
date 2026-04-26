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
  int i;
  int j;
  int x = 10, r = 0;
  if (x > 5) r = 1; else r = 0;
  if (r == 1) printf("PASS if_true\n");
  if (x < 5) r = 1; else r = 0;
  if (r == 0) printf("PASS if_false_else\n");
  int grade = 75, cat = 0;
  if      (grade >= 90) cat = 4;
  else if (grade >= 80) cat = 3;
  else if (grade >= 70) cat = 2;
  else                  cat = 1;
  if (cat == 2) printf("PASS elseif_chain\n");
  int sum = 0;
  for (i = 1; i <= 10; ++i) sum += i;
  if (sum == 55) printf("PASS for_sum\n");
  int prod = 1;
  for (i = 5; i >= 1; --i) prod *= i;
  if (prod == 120) printf("PASS for_backward_prod\n");
  int pairs = 0;
  for (i = 0; i <= 5; ++i)
  for (j = 0; j <= 5; ++j)
  if (i + j == 5) ++pairs;
  if (pairs == 6) printf("PASS nested_for_pairs\n");
  int pw = 1, steps = 0;
  while (pw < 100) { pw *= 2; ++steps; }
  if (pw == 128 && steps == 7) printf("PASS while_pow2\n");
  int val = 256, halvings = 0;
  do { val /= 2; ++halvings; } while (val > 1);
  if (halvings == 8 && val == 1) printf("PASS dowhile_halve\n");
  int day = 3;
  const char* day_name = "";
  switch (day) {
  case 1: day_name = "Mon"; break;
  case 2: day_name = "Tue"; break;
  case 3: day_name = "Wed"; break;
  case 4: day_name = "Thu"; break;
  default: day_name = "?"; break;
  }
  if (day_name[0] == 'W') printf("PASS switch_basic\n");
  int mask = 0;
  switch (2) {
  case 1: mask |= 1;
  case 2: mask |= 2;
  case 3: mask |= 4; break;
  case 4: mask |= 8;
  }
  if (mask == 6) printf("PASS switch_fallthrough\n");
  int found = -1;
  for (i = 0; i < 100; ++i) {
  if (i * i == 49) { found = i; break; }
  }
  if (found == 7) printf("PASS break_loop\n");
  int esum = 0;
  for (i = 0; i <= 10; ++i) {
  if (i % 2 != 0) continue;
  esum += i;
  }
  if (esum == 30) printf("PASS continue_evens\n");
  int fi = -1, fj = -1;
  for (i = 1; i <= 10 && fi == -1; ++i)
  for (j = 1; j <= 10; ++j)
  if (i * j > 20) { fi = i; fj = j; break; }
  if (fi == 3 && fj == 7) printf("PASS nested_break\n");
  printf("ALL PASS\n");
  return 0;
}

/* Lowering diagnostics: 1 event(s) (structured-cstyle-body=1) */
/* - main: structured-cstyle-body (raw-body 66 line(s)) */
