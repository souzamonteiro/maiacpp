#include <iostream>
#include <stdio.h>
using namespace std;

namespace numeric {
    float pi = 3.1415269;
}

int main(void) {
    cout << "PI number: " << numeric::pi << "\n";
    printf("PI number: %0.7f", numeric::pi);
    
    return 0;
}
