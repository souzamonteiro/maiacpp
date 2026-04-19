/*
 * MaiaCpp stress source (compile/transpile focus).
 *
 * Goal: keep a larger C++98 surface in one file for parser + transpiler checks.
 * This file is intentionally broader than the runtime-equivalence baseline.
 */

#include <stdio.h>

namespace Stress {
namespace Math {
int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
}

class Counter {
public:
    explicit Counter(int start) : value(start) {}

    void inc() { value = value + 1; }

    int get() const { return value; }

private:
    int value;
};

template <typename T>
class Ring4 {
public:
    T& operator[](int i) { return data[i & 3]; }

private:
    T data[4];
};
}

typedef int (*BinaryOp)(int, int);

int fold_two(BinaryOp op, int a, int b) {
    return op(a, b);
}

int pointer_accumulate(const int* p, int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += p[i];
    }
    return sum;
}

int main() {
    Stress::Counter counter(10);
    counter.inc();

    Stress::Ring4<int> ring;
    ring[0] = 3;
    ring[1] = 5;

    int stack[4];
    stack[0] = counter.get();
    stack[1] = ring[0];
    stack[2] = ring[1];
    stack[3] = fold_two(Stress::Math::add, 7, 9);

    int* dyn = new int[3];
    dyn[0] = Stress::Math::mul(2, 3);
    dyn[1] = Stress::Math::add(4, 8);
    dyn[2] = fold_two(Stress::Math::mul, 3, 4);

    int total = pointer_accumulate(stack, 4) + pointer_accumulate(dyn, 3);

    delete[] dyn;

    printf("stress-total=%d\n", total);
    return (total > 0) ? 0 : 1;
}
