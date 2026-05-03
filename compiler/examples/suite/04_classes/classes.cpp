// 04_classes — Exercises C++98 class features:
//   member-initialiser lists, copy constructor, const methods
#include <stdio.h>

class Vec2 {
public:
    double x, y;
    Vec2(double x_, double y_) : x(x_), y(y_) {}
    Vec2(const Vec2& o) : x(o.x), y(o.y) {}
    Vec2& operator=(const Vec2& o) {
        if (this != &o) { x = o.x; y = o.y; }
        return *this;
    }
    double dot(const Vec2& o) const { return x * o.x + y * o.y; }
    double lengthSq() const { return x * x + y * y; }
};

int main() {
    Vec2 a(3.0, 4.0);
    if (a.x == 3.0) printf("PASS ctor_x\n");
    if (a.y == 4.0) printf("PASS ctor_y\n");

    Vec2 b(a);
    if (b.x == 3.0 && b.y == 4.0) printf("PASS copy_ctor\n");

    Vec2 c(0.0, 0.0);
    c = a;
    if (c.x == 3.0 && c.y == 4.0) printf("PASS assign_op\n");

    Vec2 unit(1.0, 0.0);
    if (a.dot(unit) == 3.0) printf("PASS dot_x_axis\n");
    if (a.lengthSq() == 25.0) printf("PASS length_sq\n");

    printf("ALL PASS\n");
    return 0;
}

