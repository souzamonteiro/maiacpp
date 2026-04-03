#include <ostream>

class X {
public:
    friend std::ostream& operator<<(std::ostream& os, const X& x);
private:
    int v;
};

std::ostream& operator<<(std::ostream& os, const X& x) {
    return os;
}
