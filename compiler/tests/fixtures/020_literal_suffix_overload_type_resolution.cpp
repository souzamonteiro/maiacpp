double add(double a, double b) {
    return a + b;
}

float add(float a, float b) {
    return a + b;
}

float sum2(float x, float y) {
    return add(1.0f, y);
}

int main() {
    return (int)sum2(1.0f, 2.0f);
}
