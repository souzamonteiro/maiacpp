double add(double a, double b) {
    return a + b;
}

long add(long a, long b) {
    return a + b;
}

long sum2(int x, int y) {
    return add(static_cast<long>(x), static_cast<long>(y));
}

int main() {
    return (int)sum2(1, 2);
}
