#include <stdio.h>

// Minimal class example for onboarding.
class C {
public:
    explicit C(int x) : value(x) {}

    int get() const {
        return value;
    }

private:
    int value;
};

int run_class_plus_one() {
    C c(0);
    return (c.get() + 1 == 1) ? 1 : 0;
}

int main() {
    printf("=== MaiaCpp Minimal Class Example ===\n");
    printf("class +1 check: ");
    if (run_class_plus_one()) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}
