long mix(int a, long b) {
    return a + b;
}

long mix(long a, int b) {
    return a - b;
}

long use_mix(int x, int y) {
    return mix(x, y);
}

int main() {
    return (int)use_mix(3, 1);
}
