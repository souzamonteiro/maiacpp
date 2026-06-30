class Vec2 {
public:
    Vec2(double x_, double y_) : x(x_), y(y_) {}
    Vec2(const Vec2& other) : x(other.x), y(other.y) {}

    Vec2& operator=(const Vec2& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    double dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    double x;
    double y;
};

int main() {
    Vec2 a(3.0, 4.0);
    Vec2 unitX(1.0, 0.0);
    Vec2 b(a);
    Vec2 c(0.0, 0.0);

    c = a;

    if (b.x != a.x || b.y != a.y) return 11;
    if (c.x != a.x || c.y != a.y) return 12;
    if (a.dot(unitX) != 3.0) return 13;
    if (b.dot(unitX) != 3.0) return 14;
    if (b.x != c.x || b.y != c.y) return 15;

    return 0;
}
