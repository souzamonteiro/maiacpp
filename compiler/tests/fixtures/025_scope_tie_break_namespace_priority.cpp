namespace N {
long add(long a, long b) {
    return a + b;
}

long sum2(int x, int y) {
    return add(x, y);
}
}

long add(long a, long b) {
    return a - b;
}

int main() {
    return (int)N::sum2(1, 2);
}
