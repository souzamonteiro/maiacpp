#include<iostream>
using namespace std;

class Polygon {
        protected:
                int height;
                int width;
        public:
                void setValues(int a, int b) {
                        height = a;
                        width = b;
                }
                virtual int calcArea() {
                        return 0;
                }
};

class Rectangle : public Polygon {
        public:
                int calcArea() {
                        return width * height;
                }
};

class Triangle : public Polygon {
        public:
                int calcArea() {
                        return width * height / 2;
                }
};

int main(void) {
        Rectangle rectangle;
        Triangle triangle;

        rectangle.setValues(4, 3);
        triangle.setValues(4, 3);

        cout << "The area of the rectangle is " << rectangle.calcArea() << ".\n";
        cout << "The area of the triangle is " << triangle.calcArea() << ".\n";

        return 0;
}
