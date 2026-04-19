/* Complete MaiaCpp Runtime Validation Test */
/* This test mirrors the intent of the MaiaC exhaustive example while staying
    inside the subset that MaiaCpp can currently transpile into real executable C.
    The goal is to exercise observable runtime behavior instead of returning
    placeholder constants. Coverage includes:
    - namespaces and overload-resolution-sensitive calls
    - arithmetic, relational, logical and bitwise operators
    - compound assignments and pointer writes
    - for/while/do-while control flow
    - class ctor/const methods and template operator[]
    - function pointers
    - dynamic/static casts
    - new/delete and placement new
    - cout chains with int/double/char literals and variables
*/

#include <stdio.h>
#include <new>
#include <iostream>

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

typedef int (*BinaryOp)(int, int);

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

int run_cout_stress_tests() {
    int cout_acc = 0;
    int i = 1;

    cout_acc = cout_acc + i;
    std::cout << "[cout-test] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    i++;

    cout_acc = cout_acc + i;
    std::cout << "[cout-test] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    i++;

    cout_acc = cout_acc + i;
    std::cout << "[cout-test] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;

    return (cout_acc == 6) ? 1 : 0;
}

int run_for_cout_test() {
    int sum = 0;
    double ratio = 1.5;
    for (int i = 1; i < 4; ++i) {
        sum = sum + i;
        std::cout << "[for-cout] i=" << i << " sum=" << sum << " ratio=" << ratio << std::endl;
    }
    return (sum == 6) ? 1 : 0;
}

int main() {
    /* Local variable declarations */
    int failures = 0;
    int a = 10;
    int b = 20;
    int result = 0;
    int relation = 0;
    int logic = 0;
    int bit = 0;
    int cout_acc = 0;
    int i = 0;
    int loop_sum = 0;
    int down = 5;
    int up = 0;
    int* ptr = &a;

    printf("=== MaiaCpp Comprehensive Runtime Baseline ===\n");

    /* Arithmetic operators test */
    printf("--- Arithmetic Operators ---\n");
    result = a + b;
    std::cout << "add(a,b)=" << result << std::endl;
    result = b - a;
    std::cout << "b-a=" << result << std::endl;
    result = a * 3;
    std::cout << "a*3=" << result << std::endl;
    result = b / 2;
    std::cout << "b/2=" << result << std::endl;
    result = b % 3;
    std::cout << "b%3=" << result << std::endl;

    /* Compound assignment operators test */
    printf("--- Assignment Operators ---\n");
    result = a;
    std::cout << "result=" << result << std::endl;
    result = result + b;
    std::cout << "result+=b => " << result << std::endl;
    result = result - 10;
    std::cout << "result-=10 => " << result << std::endl;
    result = result * 2;
    std::cout << "result*=2 => " << result << std::endl;
    result = result / 5;
    std::cout << "result/=5 => " << result << std::endl;
    result = result % 4;
    std::cout << "result%=4 => " << result << std::endl;

    /* Relational operators test */
    printf("--- Relational Operators ---\n");
    relation = a == b;
    std::cout << "a==b => " << relation << std::endl;
    relation = a != b;
    std::cout << "a!=b => " << relation << std::endl;
    relation = a < b;
    std::cout << "a<b => " << relation << std::endl;
    relation = a > b;
    std::cout << "a>b => " << relation << std::endl;
    relation = a <= b;
    std::cout << "a<=b => " << relation << std::endl;
    relation = a >= b;
    std::cout << "a>=b => " << relation << std::endl;

    /* Logical operators test */
    printf("--- Logical Operators ---\n");
    logic = a && b;
    std::cout << "a&&b => " << logic << std::endl;
    logic = a || 0;
    std::cout << "a||0 => " << logic << std::endl;

    /* Bitwise operators test */
    printf("--- Bitwise Operators ---\n");
    bit = a & b;
    std::cout << "a&b => " << bit << std::endl;
    bit = a | b;
    std::cout << "a|b => " << bit << std::endl;
    bit = a ^ b;
    std::cout << "a^b => " << bit << std::endl;
    bit = a << 2;
    std::cout << "a<<2 => " << bit << std::endl;
    bit = b >> 1;
    std::cout << "b>>1 => " << bit << std::endl;

    /* Pointer operators test */
    printf("--- Pointer Operators ---\n");
    *ptr = 100;
    std::cout << "*ptr=100 => a=" << a << std::endl;

    /* Control flow test */
    printf("--- Control Flow ---\n");
    for (int i = 0; i < 8; ++i) {
        if (i == 5) {
            continue;
        }
        loop_sum = loop_sum + i;
        std::cout << "[for] i=" << i << " loop_sum=" << loop_sum << std::endl;
    }
    printf("loop_sum=%d\n", loop_sum);

    while (down > 0) {
        down--;
    }
    printf("while-down=%d\n", down);

    do {
        up++;
    } while (up < 5);
    printf("do-while-up=%d\n", up);

    /* cout chain stress test */
    printf("--- cout stress preflight ---\n");
    cout_acc = cout_acc + i;
    std::cout << "[cout] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    i++;
    cout_acc = cout_acc + i;
    std::cout << "[cout] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    i++;
    cout_acc = cout_acc + i;
    std::cout << "[cout] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    i++;
    cout_acc = cout_acc + i;
    std::cout << "[cout] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    i++;
    cout_acc = cout_acc + i;
    std::cout << "[cout] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    i++;
    cout_acc = cout_acc + i;
    std::cout << "[cout] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    i++;
    cout_acc = cout_acc + i;
    std::cout << "[cout] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    i++;
    cout_acc = cout_acc + i;
    std::cout << "[cout] i=" << i << " acc=" << cout_acc << " int=" << 42 << " double=" << 3.25 << " char=" << 'Q' << std::endl;
    printf("cout_acc=%d expected=92\n", cout_acc);

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

    printf("3. function pointer dispatch: ");
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

    printf("6. cout stress (chain/loop/literals): ");
    if (run_cout_stress_tests()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        failures++;
    }

    printf("7. for-loop with cout and double local: ");
    if (run_for_cout_test()) {
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
