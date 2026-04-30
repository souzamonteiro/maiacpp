#include<iostream>
using namespace std;

class Vector {
        public:
                int x;
                int y;
        Vector (void) {
        }
        Vector (int a, int b) {
                x = a;
                y = b;
        }
        Vector operator + (Vector param) {
                Vector temp;
                temp.x = x + param.x;
                temp.y = y + param.y;
                return temp;
        }
};

int main(void) {
        Vector a(3, 1);
        Vector b(1, 2);
        Vector c;

        c = a + b;

        cout << "c = " << c.x << ", " << c.y << ".\n";

        return 0;
}
