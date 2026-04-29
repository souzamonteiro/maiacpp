#include <iostream>

void draw_triangle(int rows, int level) {
    if (level > rows) {
        return;
    }
    for (int i = 0; i < level; ++i) {
        std::cout << '^';
    }
    std::cout << std::endl;
    draw_triangle(rows, level + 1);
}

int main() {
    draw_triangle(5, 1);
    return 0;
}