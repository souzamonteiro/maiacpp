#include <iostream>

void draw_frame(int width, int height) {
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            if (row == 0 || row == height - 1 || col == 0 || col == width - 1) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    draw_frame(8, 4);
    return 0;
}