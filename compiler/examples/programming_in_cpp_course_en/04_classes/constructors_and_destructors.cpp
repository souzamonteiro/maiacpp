#include<iostream>
using namespace std;

class Rectangle {
        int height;
        int base;

        public:
                Rectangle(int a, int b) {
                        height = a;
                        base = b;
                }
                ~Rectangle() {
                }

                void setValues(int a, int b) {
                        height = a;
                        base = b;
                }

                int calcArea() {
                        return base * height;
                }
};

int main(void) {
        Rectangle rectangle(4, 5);

        cout << "The area of the rectangle is " << rectangle.calcArea() << ".\n";

        return 0;
}
