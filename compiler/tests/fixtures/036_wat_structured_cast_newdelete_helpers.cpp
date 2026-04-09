class BBase {
public:
    virtual ~BBase() {}
};

class DDerived : public BBase {
public:
    DDerived(int n) : number(n) {}
    int value() const { return number; }
private:
    int number;
};

class P {
public:
    P(int x) : value(x) {}
    ~P() {}
    int get() const { return value; }
private:
    int value;
};

int run_cast_tests() {
    BBase* b = new DDerived(15);
    DDerived* d = dynamic_cast<DDerived*>(b);
    int n = static_cast<int>(3.2);

    if (d == 0) {
        delete b;
        return 0;
    }

    if (d->value() != 15) {
        delete b;
        return 0;
    }

    if (n != 3) {
        delete b;
        return 0;
    }

    delete b;
    return 1;
}

int run_new_delete_tests() {
    int* a = new int(1);
    if (*a != 1) {
        delete a;
        return 0;
    }
    delete a;

    char buffer[sizeof(P)];
    P* p = new (buffer) P(10);
    int v = p->get();
    p->~P();

    return (v == 10) ? 1 : 0;
}
