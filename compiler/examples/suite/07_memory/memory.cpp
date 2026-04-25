// 07_memory — Exercises C++98 dynamic-memory features:
//   scalar new/delete, array new[]/delete[], RAII via a buffer class
#include <stdio.h>
#include <new>

static int g_alive = 0;

class Widget {
public:
    int id;
    Widget() : id(0) { ++g_alive; }
    explicit Widget(int n) : id(n) { ++g_alive; }
    ~Widget() { --g_alive; }
};

class IntBuf {
    int* data_;
    int  n_;
public:
    explicit IntBuf(int n) : n_(n) {
        data_ = new int[n];
        for (int i = 0; i < n; ++i) data_[i] = i * i;
    }
    ~IntBuf() { delete[] data_; }
    int  operator[](int i) const { return data_[i]; }
};

int main() {
    Widget* w = new Widget(10);
    if (w != 0 && w->id == 10) printf("PASS new_id\n");
    if (g_alive == 1) printf("PASS alive_1\n");
    delete w;
    if (g_alive == 0) printf("PASS alive_0\n");
    
    int* arr = new int[6];
    for (int i = 0; i < 6; ++i) arr[i] = (i + 1) * (i + 1);
    if (arr[0] == 1 && arr[5] == 36) printf("PASS int_arr\n");
    delete[] arr;
    
    {
        IntBuf buf2(6);
        if (buf2[0] == 0 && buf2[5] == 25) printf("PASS raii\n");
    }
    
    printf("ALL PASS\n");
    return 0;
}
