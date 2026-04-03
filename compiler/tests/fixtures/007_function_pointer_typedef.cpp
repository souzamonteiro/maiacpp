typedef int (*Fn)(int, int);

int add(int a, int b) { return a + b; }

int run(int x, int y, Fn fn) {
    return fn(x, y);
}
