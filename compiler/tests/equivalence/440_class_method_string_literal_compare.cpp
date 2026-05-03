#include <string>
#include <stdlib.h>
using namespace std;

char* make_hello_word() {
    char* p = (char*)malloc(7);
    p[0] = 'H';
    p[1] = 'e';
    p[2] = 'l';
    p[3] = 'l';
    p[4] = 'o';
    p[5] = '!';
    p[6] = '\0';
    return p;
}

char* make_other_word() {
    char* p = (char*)malloc(6);
    p[0] = 'O';
    p[1] = 't';
    p[2] = 'h';
    p[3] = 'e';
    p[4] = 'r';
    p[5] = '\0';
    return p;
}

class StringComparePatterns {
public:
    int isHello(string s) {
        if (s == "Hello!") {
            return 1;
        }
        return 0;
    }
};

int main() {
    StringComparePatterns value;
    if (value.isHello(make_hello_word()) != 1) return 11;
    if (value.isHello(make_other_word()) != 0) return 12;

    return 0;
}
