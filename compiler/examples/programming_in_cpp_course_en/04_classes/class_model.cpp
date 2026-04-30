#include<iostream>
using namespace std;

class Rectangle {
        // Members can have public, protected, or private access.
        // Public members are accessible from any class.
        // Protected members are accessible from the class itself or derived classes.
        // Private members are only accessible from the class itself.
        // The default access type is private.
        int height;
        int base;

        public:
                void setValues(int a, int b) {
                        height = a;
                        base = b;
                }
                int calcArea() {
                        return base * height;
                }
};

int main(void) {
        Rectangle rectangle;

        rectangle.setValues(4, 5);

        cout << "The area of the rectangle is " << rectangle.calcArea() << ".\n";

        return 0;
}
