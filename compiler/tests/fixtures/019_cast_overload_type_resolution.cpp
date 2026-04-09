int add(int a, int b) {
    return a + b;
}

long add(long a, long b) {
    return a + b;
}

long sum2(int x, int y) {
    return add((long)x, (long)y);
}

int main() {
    return (int)sum2(1, 2);
}
