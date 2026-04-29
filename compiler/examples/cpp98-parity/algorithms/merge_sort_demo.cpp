#include <iostream>

enum { MERGE_SORT_SIZE = 8 };

void merge(int *arr, int left, int mid, int right) {
    int temp[MERGE_SORT_SIZE];
    int i = left;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (int idx = 0; idx < k; ++idx) {
        arr[left + idx] = temp[idx];
    }
}

void merge_sort(int *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main() {
    int values[MERGE_SORT_SIZE] = {9, 1, 8, 2, 7, 3, 6, 4};
    merge_sort(values, 0, MERGE_SORT_SIZE - 1);
    for (int i = 0; i < MERGE_SORT_SIZE; ++i) {
        std::cout << values[i] << std::endl;
    }
    return 0;
}