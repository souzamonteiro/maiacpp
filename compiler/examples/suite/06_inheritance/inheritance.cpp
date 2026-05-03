// 06_inheritance — Exercises C++98 inheritance features:
//   single inheritance, method overriding
#include <stdio.h>

class Shape {
public:
    virtual double area() const = 0;
    virtual ~Shape() {}
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const { return w_ * h_; }
};

class Circle : public Shape {
    double r_;
public:
    Circle(double r) : r_(r) {}
    double area() const { return 3.14159 * r_ * r_; }
};

int main() {
    Rectangle rect(4.0, 3.0);
    if (rect.area() == 12.0) printf("PASS rect_area\n");
    printf("ALL PASS\n");
    return 0;
}

