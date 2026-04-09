class Core {
public:
    explicit Core(int v) : value(v) {}
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

typedef int (*Fn)(int, int);

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int run(int x, int y, Fn fn) { return fn(x, y); }

class B { public: virtual ~B() {} };
class D : public B {
public:
    D(int n) : number(n) {}
    int value() const { return number; }
private:
    int number;
};

class P {
public:
    P(int x) : value(x) {}
    ~P() {}
    int get() const { return value; }
private:
    int value;
};

int main() {
    Core c(42);
    Box<int> box;
    box[0] = 10;
    box[1] = 20;

    B* b = new D(15);
    D* d = dynamic_cast<D*>(b);
    int n = static_cast<int>(3.2);

    int* p = new int(1);
    delete p;

    char buffer[sizeof(P)];
    P* obj = new (buffer) P(10);
    int got = obj->get();
    obj->~P();

    int total = c.get() + box[0] + box[1] + run(7, 3, add) + run(7, 3, mul) + n + got;

    if (d == 0) {
        delete b;
        return 1;
    }

    delete b;
    return total > 0 ? 0 : 1;
}
