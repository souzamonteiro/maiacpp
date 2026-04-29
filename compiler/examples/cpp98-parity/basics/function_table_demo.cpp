#include <iostream>

typedef int (*binary_op)(int, int);

int op_add(int left, int right) {
    return left + right;
}

int op_sub(int left, int right) {
    return left - right;
}

int op_mul(int left, int right) {
    return left * right;
}

int main() {
    const char *labels[3];
    binary_op ops[3];
    const int left = 9;
    const int right = 4;

    labels[0] = "add";
    labels[1] = "sub";
    labels[2] = "mul";

    ops[0] = op_add;
    ops[1] = op_sub;
    ops[2] = op_mul;

    for (int i = 0; i < 3; ++i) {
        std::cout << labels[i] << '(' << left << ',' << right << ")=" << ops[i](left, right) << std::endl;
    }

    return 0;
}