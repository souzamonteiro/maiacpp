class MethodPatterns {
public:
    int identity(int x) {
        return x;
    }

    int seven() {
        return 7;
    }

    int is_three(int value) {
        return (value == 3) ? 1 : 0;
    }
};

int main() {
    MethodPatterns m;
    int ok = 1;

    if (m.identity(5) != 5) ok = 0;
    if (m.seven() != 7) ok = 0;
    if (m.is_three(3) != 1) ok = 0;
    if (m.is_three(2) != 0) ok = 0;

    return ok ? 0 : 1;
}
