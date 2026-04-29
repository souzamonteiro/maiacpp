#include <iostream>

enum { SEARCH_AND_SORT_SIZE = 8 };

void insertion_sort(int *arr, int size) {
    for (int i = 1; i < size; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

int binary_search(const int *arr, int size, int target) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return -1;
}

void print_array(const int *arr, int size) {
    for (int i = 0; i < size; ++i) {
        if (i > 0) {
            std::cout << ',';
        }
        std::cout << arr[i];
    }
    std::cout << std::endl;
}

int main() {
    int values[SEARCH_AND_SORT_SIZE] = {12, 5, 19, 2, 11, 8, 3, 17};

    insertion_sort(values, SEARCH_AND_SORT_SIZE);
    print_array(values, SEARCH_AND_SORT_SIZE);

    std::cout << "index_of_11=" << binary_search(values, SEARCH_AND_SORT_SIZE, 11) << std::endl;
    std::cout << "index_of_4=" << binary_search(values, SEARCH_AND_SORT_SIZE, 4) << std::endl;
    return 0;
}