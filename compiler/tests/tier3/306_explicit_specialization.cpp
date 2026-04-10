template <typename T>
T id(T x) {
    return x;
}

template <>
int id(int x) {
    return x + 1;
}

int main() {
    return id(1) == 2 ? 0 : 1;
}
