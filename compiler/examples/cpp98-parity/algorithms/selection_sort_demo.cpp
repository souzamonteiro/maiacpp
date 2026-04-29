#include <iostream>

enum { SELECTION_SORT_SIZE = 5 };

void selection_sort(int *arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        int best = i;
        for (int j = i + 1; j < size; ++j) {
            if (arr[j] < arr[best]) {
                best = j;
            }
        }
        if (best != i) {
            int temp = arr[i];
            arr[i] = arr[best];
            arr[best] = temp;
        }
    }
}

int main() {
    int values[SELECTION_SORT_SIZE] = {42, 7, 19, 3, 11};
    selection_sort(values, SELECTION_SORT_SIZE);
    for (int i = 0; i < SELECTION_SORT_SIZE; ++i) {
        std::cout << values[i] << std::endl;
    }
    return 0;
}