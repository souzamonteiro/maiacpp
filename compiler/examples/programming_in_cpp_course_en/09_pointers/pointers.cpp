#include <iostream>
#include <string.h>
using namespace std;

int length(char txt[]) {
    int n;

    n = 0;
    while (txt[n]) {
        n++;
    }

    return n;
}

void to_uppercase(char *source, char *dest) {
    char *p;
    char *q;

    p = source;  // p = name
    q = dest;    // q = name_upper

    // source = "ana"
    // dest = ""
    // *p = source -> *p = 'a'
    // *q = dest -> *q = ''
    // *q = toupper(*p) -> *q = 'A'
    // p++ -> *p = 'n'
    // q++ -> *q = ''
    // *q = toupper(*p) -> *q = 'N'
    // p++ -> *p = 'a'
    // q++ -> *q = ''
    // *q = toupper(*p) -> *q = 'A'
    // p++ -> *p = '\0'
    // q++ -> *q = ''
    // *q = '\0'
    while (*p) {
        *q = toupper(*p);
        p++;
        q++;
    }

    *q = '\0';
}

int main(void) {
    char name[20];
    char name_upper[20];
    char *p;

    int i;

    cout << "Enter your name: ";
    cin >> name;

    cout << "Hello " << name << "!\n";
    
    cout << ">>";
    for (i = 0; i < 20; i++) {
        cout << name[i];
    }
    cout << "<<\n";
    
    cout << ">>";
    i = 0;
    while (name[i] != '\0') {
        cout << name[i];
        i++;
    }
    cout << "<<\n";

    cout << ">>";
    i = 0;
    while (name[i]) {
        cout << name[i];
        i++;
    }
    cout << "<<\n";
    
    cout << "Your name has " << length(name) << " characters.\n";
    cout << "Your name has " << strlen(name) << " characters.\n";

    p = name;
    cout << ">>";
    while (*p) {
        cout << *p;
        p++;
    }
    cout << "<<\n";

    to_uppercase(name, name_upper);

    cout << "Your name in uppercase is " << name_upper << ".\n";

    return 0;
}
