#include<iostream>
using namespace std;
class Vetor {
    int x;
    int y;
    
    public:
        Vetor() {
        }
        Vetor(int a, int b) {
            x = a;
            y = b;
        }
        Vetor operator + (Vetor a) {
            Vetor temp;
            temp.x = x + a.x;
            temp.y = y + a.y;
            return temp;
        }
        Vetor operator - (Vetor a) {
            Vetor temp;
            temp.x = x - a.x;
            temp.y = y - a.y;
            return temp;
        }
        int getX() {
            return x;
        }
        int getY() {
            return y;
        }
};

int main(void) {
    Vetor a(1, 2);
    Vetor b(3, 4);
    Vetor c;
    Vetor d;

    c = a + b;
    d = a - b;
    
    cout << "c(" << c.getX() << "," << c.getY() << ")" << "\n";
    cout << "d(" << d.getX() << "," << d.getY() << ")" << "\n";
    
    return 0;
}