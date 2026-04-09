long add(long a, long b) {
    return a + b;
}

double add(double a, double b) {
    return a + b;
}

double sum2(float x, float y) {
    return add(x, y);
}

int main() {
    return (int)sum2(1.0f, 2.0f);
}
