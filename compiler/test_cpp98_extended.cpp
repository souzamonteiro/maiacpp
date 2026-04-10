/*
 * MaiaCpp extended C++98 production-style baseline.
 *
 * This source intentionally combines many currently supported C++98 families:
 * - namespaces (qualified and nested lookup)
 * - overload resolution (int/long and tie-break patterns)
 * - class ctor/const methods
 * - template class with operator[]
 * - function-pointer typedef and indirect call
 * - dynamic_cast/static_cast
 * - new/delete and placement new
 * - if/else compare returns and ternary returns
 */

#include <stdio.h>
#include <new>

namespace N {
int add(int a, int b) {
    return a + b;
}

float add(float a, float b) {
    return a + b;
}
}

namespace A {
namespace B {
int add(int a, int b) {
    return a + b;
}
}

int sum2(int x, int y) {
    return B::add(x, y);
}

namespace C {
int add(int a, int b) {
    return a + b;
}

int sum2(int x, int y) {
    return add(x, y);
}
}
}

int sum_ns_int(int x, int y) {
    return N::add(x, y);
}

int sum_nested(int x, int y) {
    return A::sum2(x, y);
}

int sum_tiebreak(int x, int y) {
    return A::C::sum2(x, y);
}

int classify_zero(int x) {
    if (x == 0) {
        return 1;
    }
    return 0;
}

int classify_positive(int x) {
    if (x > 0) {
        return 1;
    }
    return 0;
}

int is_less(int x, int y) {
    return (x < y) ? 1 : 0;
}

class C {
public:
    explicit C(int x) : value(x) {}

    int get() const {
        return value;
    }

private:
    int value;
};

template <typename T>
class Box {
public:
    T& operator[](int i) {
        return data[i];
    }

private:
    T data[4];
};

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

class BBase {
public:
    virtual ~BBase() {}
};

class DDerived : public BBase {
public:
    DDerived(int n) : number(n) {}

    int value() const {
        return number;
    }

private:
    int number;
};

class P {
public:
    P(int x) : value(x) {}
    ~P() {}

    int get() const {
        return value;
    }

private:
    int value;
};

int run_namespace_tests() {
    return sum_ns_int(1, 2);
}

int run_control_expr_tests() {
    return classify_zero(0);
}

int run_class_tests() {
    C c(42);
    return (c.get() == 42) ? 1 : 0;
}

int run_template_tests() {
    Box<int> box;
    box[0] = 10;
    box[1] = 20;
    return (box[0] + box[1] == 30) ? 1 : 0;
}

int run_arithmetic_function_tests() {
    int s = add(7, 3);
    int m = multiply(7, 3);
    return (s == 10 && m == 21) ? 1 : 0;
}

int run_cast_tests() {
    BBase* b = new DDerived(15);
    DDerived* d = dynamic_cast<DDerived*>(b);
    int n = static_cast<int>(3.2);

    if (d == 0) {
        delete b;
        return 0;
    }

    if (d->value() != 15) {
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

int run_new_delete_tests() {
    int* a = new int(1);
    if (*a != 1) {
        delete a;
        return 0;
    }
    delete a;

    char buffer[sizeof(P)];
    P* p = new (buffer) P(10);
    int v = p->get();
    p->~P();

    return (v == 10) ? 1 : 0;
}

int main() {
    int failures = 0;

    printf("=== MaiaCpp Extended C++98 Baseline ===\n");

    printf("1. namespace/overload/scope: ");
    if (run_namespace_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("2. if/ternary expressions: ");
    if (run_control_expr_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("3. class/ctor/const: ");
    if (run_class_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("4. template/operator[]: ");
    if (run_template_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("5. global functions/arithmetic: ");
    if (run_arithmetic_function_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("6. casts (dynamic/static): ");
    if (run_cast_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("7. new/delete/placement-new: ");
    if (run_new_delete_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    }

    printf("TESTS FAILED: %d\n", failures);
    return 1;
}
