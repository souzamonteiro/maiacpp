class C {
public:
    explicit C(int x) : value(x) {}
    int get() const { return value; }
private:
    int value;
};
