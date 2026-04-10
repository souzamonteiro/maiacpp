extern "C" int c_add(int a, int b) {
    return a + b;
}

int main() {
    return c_add(1, 2) == 3 ? 0 : 1;
}
