template <typename T>
T addT(T a, T b) {
    return a + b;
}

template int addT(int, int);

int main() {
    return addT(1, 2) == 3 ? 0 : 1;
}
