int apply_twice(int (*fn)(int), int x) {
    return fn(fn(x));
}

int inc(int x) {
    return x + 1;
}

int main() {
    return apply_twice(inc, 1) == 3 ? 0 : 1;
}
