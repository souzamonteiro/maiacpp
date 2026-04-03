class P { public: P(int) {} ~P() {} };

void f() {
    int* a = new int(1);
    delete a;

    char buffer[sizeof(P)];
    P* p = new (buffer) P(10);
    p->~P();
}
