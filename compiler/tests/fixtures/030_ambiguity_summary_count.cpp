namespace A {
namespace B {
long add(long a, long b) {
    return a + b;
}
}

namespace D {
long add(long a, long b) {
    return a - b;
}
}

namespace C {
long sum2(int x, int y) {
    return add(x, y);
}
}
}

int main() {
    return (int)A::C::sum2(3, 1);
}
