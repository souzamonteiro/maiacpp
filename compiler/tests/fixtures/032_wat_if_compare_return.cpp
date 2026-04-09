int classify_positive(int x) {
    if (x > 0) {
        return 1;
    }
    return 0;
}

int main() {
    return classify_positive(2);
}
