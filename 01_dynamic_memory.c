#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int capacity = 3;
    int *arr = malloc((size_t)capacity * sizeof(int));
    if (!arr) {
        perror("malloc failed");
        return 1;
    }

    for (int i = 0; i < capacity; i++) {
        arr[i] = (i + 1) * 100;
    }

    // Expand capacity using realloc
    int new_capacity = 6;
    int *temp = realloc(arr, (size_t)new_capacity * sizeof(int));
    if (!temp) {
        free(arr);
        perror("realloc failed");
        return 1;
    }
    arr = temp;

    for (int i = capacity; i < new_capacity; i++) {
        arr[i] = (i + 1) * 100;
    }

    printf("Resized dynamic array: ");
    for (int i = 0; i < new_capacity; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
