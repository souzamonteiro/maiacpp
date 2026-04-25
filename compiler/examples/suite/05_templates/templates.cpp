// 05_templates — Exercises C++98 template features:
//   function templates, class templates with non-type parameters
#include <stdio.h>

template<typename T>
T tmax(const T& a, const T& b) {
    return a > b ? a : b;
}

template<typename T>
void tswap(T& a, T& b) {
    T tmp = a; a = b; b = tmp;
}

template<typename T, int N>
class Stack {
    T   data[N];
    int top_;
public:
    Stack() : top_(0) {}
    bool push(const T& v) {
        if (top_ >= N) return false;
        data[top_++] = v;
        return true;
    }
    bool pop(T& v) {
        if (top_ <= 0) return false;
        v = data[--top_];
        return true;
    }
    int  size() const { return top_; }
};

int main() {
    if (tmax(3, 7) == 7) printf("PASS tmax_int_r\n");
    if (tmax(9, 2) == 9) printf("PASS tmax_int_l\n");
    if (tmax(1.5, 2.7) == 2.7) printf("PASS tmax_double\n");
    
    int  ip = 10, iq = 20;
    tswap(ip, iq);
    if (ip == 20 && iq == 10) printf("PASS tswap_int\n");
    
    Stack<int, 4> si;
    si.push(10);
    si.push(20);
    si.push(30);
    if (si.size() == 3) printf("PASS stack_size_3\n");
    
    printf("ALL PASS\n");
    return 0;
}
