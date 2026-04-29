#include <cstring>
#include <iostream>

bool is_palindrome(const char *text) {
    int left = 0;
    int right = static_cast<int>(std::strlen(text)) - 1;

    while (left < right) {
        if (text[left] != text[right]) {
            return false;
        }
        ++left;
        --right;
    }

    return true;
}

int main() {
    const char *words[5];

    words[0] = "level";
    words[1] = "robot";
    words[2] = "radar";
    words[3] = "maiac";
    words[4] = "civic";

    for (int i = 0; i < 5; ++i) {
        std::cout << words[i] << " => " << (is_palindrome(words[i]) ? "yes" : "no") << std::endl;
    }

    return 0;
}