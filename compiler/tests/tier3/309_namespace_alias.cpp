namespace A {
namespace B {
int f() { return 1; }
}
}

namespace AB = A::B;

int main() {
    return AB::f() == 1 ? 0 : 1;
}
