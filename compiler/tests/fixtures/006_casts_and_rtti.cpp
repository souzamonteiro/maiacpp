class B { public: virtual ~B() {} };
class D : public B {};

void f() {
    B* b = new D();
    D* d = dynamic_cast<D*>(b);
    int n = static_cast<int>(3.2);
    (void)n;
    (void)d;
    delete b;
}
