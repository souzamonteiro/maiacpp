#include<iostream>
using namespace std;
class Vector {
    int x;
    int y;
    
    public:
        Vector() {
        }
        Vector(int a, int b) {
            x = a;
            y = b;
        }
        Vector operator + (Vector a) {
            Vector temp;
            temp.x = x + a.x;
            temp.y = y + a.y;
            return temp;
        }
        Vector operator - (Vector a) {
            Vector temp;
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
    Vector a(1, 2);
    Vector b(3, 4);
    Vector c;
    Vector d;

    c = a + b;
    d = a - b;
    
    cout << "c(" << c.getX() << "," << c.getY() << ")" << "\n";
    cout << "d(" << d.getX() << "," << d.getY() << ")" << "\n";
    
    return 0;
}
