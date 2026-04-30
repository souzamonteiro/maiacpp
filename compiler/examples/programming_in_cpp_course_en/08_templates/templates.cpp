#include<iostream>
using namespace std;

template <class T>
T tmax(T a, T b) {
        return a > b ? a : b;
}

int main(void) {
        cout << "The greater value between " << 1 << " and " << 2 << " is " << tmax<int>(1, 2) << ".\n";
        cout << "The greater value between " << 3.2 << " and " << 3.7 << " is " << tmax<float>(3.2, 3.7) << ".\n";

        return 0;
}
