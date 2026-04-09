namespace A {
namespace B {
int add(int a, int b) {
    return a + b;
}
}

int sum2(int x, int y) {
    return B::add(x, y);
}
}

int add(int a, int b) {
    return a - b;
}

int main() {
    return A::sum2(3, 1);
}
