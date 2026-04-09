namespace A { const int V = 1; }
namespace B { using namespace A; int f() { return V; } }

int add(int a, int b) { return a + b; }

int main() {
    return add(1, 2) + B::f();
}
