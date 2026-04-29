#include <iostream>

struct Point {
    Point(int point_x, int point_y) : x(point_x), y(point_y) {
    }

    int x;
    int y;
};

class Box {
public:
    Box(const Point &box_origin, int box_width, int box_height)
        : origin_(box_origin), width_(box_width), height_(box_height) {
    }

    const Point &origin() const {
        return origin_;
    }

    int area() const {
        return width_ * height_;
    }

    int perimeter() const {
        return 2 * (width_ + height_);
    }

private:
    Point origin_;
    int width_;
    int height_;
};

int main() {
    Box box(Point(3, 5), 7, 4);

    std::cout << "origin=(" << box.origin().x << ',' << box.origin().y << ')' << std::endl;
    std::cout << "area=" << box.area() << std::endl;
    std::cout << "perimeter=" << box.perimeter() << std::endl;
    return 0;
}