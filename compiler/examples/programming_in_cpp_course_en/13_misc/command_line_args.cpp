#include <iostream>
using namespace std;

int main(int argc, char *argv[], char **env) {
    int i;
    char **p;

    for (i = 0; i < argc; i++) {
        cout << "argv[" << i << "] = " << argv[i] << "\n";
    }

    p = env;
    while (*p) {
        cout << *p << "\n";
        p++;
    }
    
    return 0;
}
