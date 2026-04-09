double add(double a, double b) {
    return a + b;
}

long add(long a, long b) {
    return a + b;
}

long sum2(int x, int y) {
    return add(x, y);
}

int main() {
    return (int)sum2(1, 2);
}
