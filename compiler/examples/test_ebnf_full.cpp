/*
 * EBNF coverage target for MaiaCpp.
 *
 * Purpose:
 * - Stress the parser and semantic/codegen layers with broad C++98 syntax.
 * - Serve as target conformance input for AST-first compiler migration.
 *
 * IMPORTANT:
 * - This file is a coverage target and may include sections that are
 *   parse-only until full semantic lowering is implemented.
 */

#include <stdio.h>
#include <new>

/* storage classes / typedef / enum */
typedef int (*Fn2)(int, int);
static int g_static = 7;
extern int g_extern;
enum Mode { ModeA = 1, ModeB = 2 };

/* namespace + using + alias */
namespace Alpha {
  int sum(int a, int b) { return a + b; }

  namespace Beta {
    int diff(int a, int b) { return a - b; }
  }
}

namespace AB = Alpha::Beta;
using Alpha::sum;
using namespace Alpha;

/* class + inheritance + virtual + ctor initializer list + operator[] */
class Base {
public:
  virtual ~Base() {}
  virtual int value() const { return 0; }
};

class Derived : public Base {
public:
  explicit Derived(int n) : m(n) {}
  virtual ~Derived() {}

  virtual int value() const { return m; }

private:
  int m;
};

template <typename T>
class Box {
public:
  T& operator[](int i) { return data[i]; }

private:
  T data[4];
};

/* free functions + overload */
int op(int a, int b) { return a + b; }
long op(long a, long b) { return a + b; }

int apply(int a, int b, Fn2 fn) {
  return fn(a, b);
}

/* casts, new/delete, placement new, try/catch/throw */
int run_runtime_subset() {
  int ok = 1;

  Box<int> box;
  box[0] = 10;
  box[1] = 20;

  int s = apply(7, 3, op);
  if (s != 10) ok = 0;

  Base* b = new Derived(15);
  Derived* d = dynamic_cast<Derived*>(b);
  if (d == 0) ok = 0;
  if (d && d->value() != 15) ok = 0;
  delete b;

  char mem[sizeof(Derived)];
  Derived* pd = new (mem) Derived(30);
  if (pd->value() != 30) ok = 0;
  pd->~Derived();

  int n = static_cast<int>(3.9);
  if (n != 3) ok = 0;

  try {
    if (box[0] + box[1] != 30) {
      throw 123;
    }
  } catch (int e) {
    if (e != 123) ok = 0;
  }

  return ok;
}

/* linkage specification + asm definition (parse/conformance target) */
extern "C" {
  int c_linked_add(int a, int b) { return a + b; }
}

asm("# maia-cpp-parse-target");

/* explicit instantiation/specialization syntax target */
template class Box<int>;
template <> class Box<long> {
public:
  long& operator[](int i) { return data[i]; }
private:
  long data[2];
};

int main() {
  int failures = 0;

  printf("=== MaiaCpp EBNF Coverage Target ===\n");

  if (sum(1, 2) != 3) failures++;
  if (AB::diff(7, 4) != 3) failures++;

  if (run_runtime_subset() == 0) failures++;

  if (c_linked_add(2, 5) != 7) failures++;

  if (failures == 0) {
    printf("EBNF TARGET PASS\n");
    return 0;
  }

  printf("EBNF TARGET FAIL: %d\n", failures);
  return 1;
}
