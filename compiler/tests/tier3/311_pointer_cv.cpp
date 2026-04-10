int main() {
    int value = 7;
    const int* p = &value;
    int* const q = &value;
    return (*p == *q) ? 0 : 1;
}
