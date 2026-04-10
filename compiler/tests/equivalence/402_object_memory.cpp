#include <new>

class C {
public:
    explicit C(int x) : value(x) {}
    int get() const { return value; }

private:
    int value;
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
    C c(7);
    if (c.get() != 7) return 1;

    int* a = new int(1);
    if (*a != 1) {
        delete a;
        return 1;
    }
    delete a;

    char buffer[sizeof(P)];
    P* p = new (buffer) P(10);
    int v = p->get();
    p->~P();

    return v == 10 ? 0 : 1;
}
