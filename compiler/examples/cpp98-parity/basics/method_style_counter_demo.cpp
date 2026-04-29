#include <iostream>

class Counter {
public:
    explicit Counter(int initial_value) : value_(initial_value) {
    }

    int change(int delta) {
        value_ += delta;
        return value_;
    }

private:
    int value_;
};

int main() {
    Counter counter(10);

    std::cout << "v1=" << counter.change(5) << std::endl;
    std::cout << "v2=" << counter.change(-3) << std::endl;
    return 0;
}