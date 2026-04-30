#include<iostream>
using namespace std;

namespace numeric {
        float golden = 1.1680;
        float pi = 3.1416;
}

int main(void) {
        cout << "The golden ratio is " << numeric::golden << ".\n";
        cout << "The number pi is " << numeric::pi << ".\n";

        return 0;
}
