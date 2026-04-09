float neg(float v) {
    return -v;
}

int neg(int v) {
    return -v;
}

int apply(int x) {
    return neg(-x);
}

int main() {
    return apply(3);
}
