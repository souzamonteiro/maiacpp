enum Color {
    RED,
    GREEN = 5,
    BLUE
};

int main() {
    Color c = BLUE;
    return c == BLUE ? 0 : 1;
}
