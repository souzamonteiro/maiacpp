class Circle {
public:
    explicit Circle(double r) : radius(r) {}
    double area() const { return radius * radius; }

private:
    // member comment line with namespace and using words
    double radius;
};
