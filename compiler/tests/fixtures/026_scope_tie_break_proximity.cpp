namespace B {
long add(long a, long b) {
    return a - b;
}
}

namespace A {
long add(long a, long b) {
    return a + b;
}

namespace C {
long sum2(int x, int y) {
    return add(x, y);
}
}
}

int main() {
    return (int)A::C::sum2(1, 2);
}
