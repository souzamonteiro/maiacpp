namespace A {
    const int V = 1;
}

namespace B {
    using namespace A;
    int f() { return V; }
}
