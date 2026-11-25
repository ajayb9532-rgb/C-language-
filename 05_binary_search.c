#include <stdio.h>

int binarySearch(const int arr[], int size, int target) {
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int main(void) {
    int sorted[] = {4, 11, 19, 27, 35, 48, 62, 79};
    int n = sizeof(sorted) / sizeof(sorted[0]);
    int target = 35;

    int idx = binarySearch(sorted, n, target);
    if (idx != -1) {
        printf("Found %d at index %d\n", target, idx);
    } else {
        printf("%d not found\n", target);
    }
    return 0;
}
