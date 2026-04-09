class C {
public:
    explicit C(int x) : value(x) {}
    int get() const { return value; }
private:
    int value;
};

template <typename T>
class Box {
public:
    T& operator[](int i) { return data[i]; }
private:
    T data[4];
};

typedef int (*BinaryOp)(int, int);

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }
int execute(int a, int b, BinaryOp fn) { return fn(a, b); }

int run_class_tests() {
    C c(42);
    return (c.get() == 42) ? 1 : 0;
}

int run_template_tests() {
    Box<int> box;
    box[0] = 10;
    box[1] = 20;
    return (box[0] + box[1] == 30) ? 1 : 0;
}

int run_function_pointer_tests() {
    int s = execute(7, 3, add);
    int m = execute(7, 3, multiply);
    return (s == 10 && m == 21) ? 1 : 0;
}
