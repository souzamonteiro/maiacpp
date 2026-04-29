#include <iostream>

class IntStack {
public:
    enum { STACK_CAPACITY = 8 };

    IntStack() : top_(0) {
    }

    bool push(int value) {
        if (top_ >= STACK_CAPACITY) {
            return false;
        }
        data_[top_++] = value;
        return true;
    }

    bool pop(int &value) {
        if (top_ <= 0) {
            return false;
        }
        --top_;
        value = data_[top_];
        return true;
    }

private:
    int data_[STACK_CAPACITY];
    int top_;
};

int main() {
    IntStack stack;
    int value = 0;

    stack.push(10);
    stack.push(20);
    stack.push(30);

    while (stack.pop(value)) {
        std::cout << "pop=" << value << std::endl;
    }

    return 0;
}