/*
 * MaiaCpp minimum baseline test.
 *
 * This file intentionally avoids iostream/std::cout and heavy std library usage.
 * Output uses printf so the generated wasm can bind to browser/node runtime
 * functions similar to MaiaC integration.
 */

#include <stdio.h>

class B {
public:
    virtual ~B() {}
};

class D : public B {
public:
    D() : value(15) {}
    int get() const { return value; }
private:
    int value;
};

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int execute(int a, int b, int (*fn)(int, int)) {
    return fn(a, b);
}

int runCasts() {
    B* b = new D();
    D* d = dynamic_cast<D*>(b);
    int n = static_cast<int>(3.2);

    if (d == 0) {
        delete b;
        return 0;
    }

    if (d->get() != 15) {
        delete b;
        return 0;
    }

    if (n != 3) {
        delete b;
        return 0;
    }

    delete b;
    return 1;
}

int main() {
    int failures = 0;

    printf("=== MaiaCpp test.cpp baseline ===\n");

    printf("execute(add): %d\n", execute(7, 3, add));
    printf("execute(multiply): %d\n", execute(7, 3, multiply));

    if (runCasts()) {
        printf("runCasts: OK\n");
    } else {
        printf("runCasts: FAIL\n");
        failures++;
    }

    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    }

    printf("TESTS FAILED: %d\n", failures);
    return 1;
}
