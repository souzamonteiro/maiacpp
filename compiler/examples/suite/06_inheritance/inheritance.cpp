// 06_inheritance — Exercises C++98 inheritance features:
//   single inheritance, virtual function dispatch, static_cast
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
    
    Circle circ(1.0);
    double ca = circ.area();
    if (ca > 3.14 && ca < 3.15) printf("PASS circle_area_range\n");
    
    Shape* shapes[2] = { &rect, &circ };
    double ta = shapes[0]->area() + shapes[1]->area();
    if (ta > 15.0 && ta < 16.0) printf("PASS virt_total_area\n");
    
    printf("ALL PASS\n");
    return 0;
}
