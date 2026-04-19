class MethodFlowPatterns {
public:
    MethodFlowPatterns(int seed) : value(seed) {}

    int classify() {
        if (value == 3) {
            return 1;
        }
        return 0;
    }

    int offset(int x) {
        int y = x + value;
        return y;
    }

private:
    int value;
};

int main() {
    MethodFlowPatterns a(3);
    MethodFlowPatterns b(5);

    if (a.classify() != 1) return 1;
    if (b.classify() != 0) return 1;
    if (a.offset(7) != 10) return 1;
    if (b.offset(2) != 7) return 1;

    return 0;
}
