class E {};

int main() {
    try {
        throw E();
    } catch (E&) {
        return 0;
    }
    return 1;
}
