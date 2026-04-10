namespace N {
int add(int a, int b) {
    return a + b;
}

float add(float a, float b) {
    return a + b;
}
}

int sum2(int x, int y) {
    return N::add(x, y);
}

int main() {
    return sum2(1, 2) == 3 ? 0 : 1;
}
