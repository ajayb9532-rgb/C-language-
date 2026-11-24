#include <stdio.h>

int main(void) {
    const char *filename = "sample_output.txt";

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Failed to open file for writing");
        return 1;
    }
    fprintf(fp, "Line 1: C File I/O example.\n");
    fprintf(fp, "Line 2: Ready for GitHub repository.\n");
    fclose(fp);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Failed to open file for reading");
        return 1;
    }

    char buffer[256];
    printf("Contents of %s:\n", filename);
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("  %s", buffer);
    }
    fclose(fp);

    return 0;
}
