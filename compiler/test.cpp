/*
 * MaiaCpp comprehensive baseline test.
 *
 * This file is inspired by the large parser test style, but constrained to
 * runtime-friendly features for browser/node (printf output, no iostream/std::cout).
 */

#include <stdio.h>
#include <new>

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

typedef int (*BinaryOp)(int, int);

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int execute(int a, int b, BinaryOp fn) {
    return fn(a, b);
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

int run_function_pointer_tests() {
    int s = execute(7, 3, add);
    int m = execute(7, 3, multiply);
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

    printf("=== MaiaCpp Comprehensive Baseline ===\n");

    printf("1. class/ctor/const: ");
    if (run_class_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("2. template/operator[]: ");
    if (run_template_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("3. function pointer: ");
    if (run_function_pointer_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("4. casts (dynamic/static): ");
    if (run_cast_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("5. new/delete/placement-new: ");
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
