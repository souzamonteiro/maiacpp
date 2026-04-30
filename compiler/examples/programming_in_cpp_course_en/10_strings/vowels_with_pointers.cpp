#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

int count_vowels_with_repeat(char *name) {
    char *p;
    int n = 0;
    
    p = name;
    while (*p) {
        switch (tolower(*p)) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                n++;
                break;
            default:
                break;
        }
        p++;
    }
    
    return n;
}

int count_vowels_no_repeat(char *name) {
    char *p;
    int na = 0;
    int ne = 0;
    int ni = 0;
    int no = 0;
    int nu = 0;
    
    p = name;
    while (*p) {
        switch (tolower(*p)) {
            case 'a':
                if (na == 0) {
                    na++;
                }
                break;
            case 'e':
                if (ne == 0) {
                    ne++;
                }
                break;
            case 'i':
                if (ni == 0) {
                    ni++;
                }
                break;
            case 'o':
                if (no == 0) {
                    no++;
                }
                break;
            case 'u':
                if (nu == 0) {
                    nu++;
                }
                break;
            default:
                break;
        }
        p++;
    }
    
    return na + ne + ni + no + nu;
}

int main(void) {
    char name[50];
    
    cout << "Enter your name: ";
    cin >> name;
    
    cout << "Your name has " << count_vowels_with_repeat(name) << " vowels (with repetition).\n";
    cout << "Your name has " << count_vowels_no_repeat(name) << " vowels (without repetition).\n";
    
        return 0;
}
