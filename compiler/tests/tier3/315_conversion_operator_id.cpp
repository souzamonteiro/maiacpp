class Num {
public:
    Num(int v) : v_(v) {}
    operator int() const { return v_; }

private:
    int v_;
};

int main() {
    Num n(3);
    int x = n;
    return x == 3 ? 0 : 1;
}
