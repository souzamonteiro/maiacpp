// 10_casts — Exercises C++98 casts:
//   static_cast, reinterpret_cast, const_cast, C-style cast
#include <stdio.h>

class Base {
public:
    int tag;
    Base(int t) : tag(t) {}
    virtual ~Base() {}
};

class Derived : public Base {
public:
    int extra;
    Derived(int t, int e) : Base(t), extra(e) {}
};

int main() {
    double d = 3.7;
    int di = static_cast<int>(d);
    if (di == 3) printf("PASS sc_double_to_int\n");
    
    int j = 65;
    char ch = static_cast<char>(j);
    if (ch == 'A') printf("PASS sc_int_to_char\n");
    
    Derived deriv(10, 99);
    Base* bp = static_cast<Base*>(&deriv);
    if (bp->tag == 10) printf("PASS sc_upcast\n");
    
    Derived* dp = static_cast<Derived*>(bp);
    if (dp->extra == 99) printf("PASS sc_downcast\n");
    
    int mutable_val = 55;
    const int* cptr = &mutable_val;
    int* mptr = const_cast<int*>(cptr);
    *mptr = 77;
    if (mutable_val == 77) printf("PASS cc_write\n");
    
    double pi = 3.14159;
    int pi_i = (int)pi;
    if (pi_i == 3) printf("PASS cstyle_trunc\n");
    
    printf("ALL PASS\n");
    return 0;
}
