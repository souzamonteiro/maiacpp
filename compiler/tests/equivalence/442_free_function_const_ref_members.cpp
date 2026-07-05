class Vec2 {
public:
    Vec2(double x_, double y_) : x(x_), y(y_) {}

    double x;
    double y;
};

double dot_pair(const Vec2& lhs, const Vec2& rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

int same_pair(const Vec2& lhs, const Vec2& rhs) {
    if (lhs.x == rhs.x && lhs.y == rhs.y) {
        return 1;
    }
    return 0;
}

int main() {
    Vec2 a(3.0, 4.0);
    Vec2 b(1.0, 0.0);
    Vec2 c(3.0, 4.0);

    if (dot_pair(a, b) != 3.0) return 11;
    if (same_pair(a, c) != 1) return 12;
    if (same_pair(a, b) != 0) return 13;

    return 0;
}
