namespace A {
namespace B {
int add(int a, int b) {
    return a + b;
}
}
}

int sum2(int x, int y) {
    return A::B::add(x, y);
}

int main() {
    return sum2(1, 2);
}
