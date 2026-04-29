#include <iostream>

enum { QUICKSORT_SIZE = 6 };

void swap_values(int &left, int &right) {
    int temp = left;
    left = right;
    right = temp;
}

int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            swap_values(arr[i], arr[j]);
        }
    }
    swap_values(arr[i + 1], arr[high]);
    return i + 1;
}

void quicksort(int *arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}

int main() {
    int values[QUICKSORT_SIZE] = {14, 3, 11, 7, 2, 9};
    quicksort(values, 0, QUICKSORT_SIZE - 1);
    for (int i = 0; i < QUICKSORT_SIZE; ++i) {
        std::cout << values[i] << std::endl;
    }
    return 0;
}