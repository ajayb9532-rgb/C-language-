#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isPalindrome(const char *str) {
    size_t left = 0;
    size_t right = strlen(str);
    if (right == 0) return 1;
    right--;

    while (left < right) {
        if (tolower((unsigned char)str[left]) != tolower((unsigned char)str[right])) {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
}

int main(void) {
    const char *words[] = {"Level", "GitHub", "Radar"};
    for (int i = 0; i < 3; i++) {
        printf(""%s" -> %s\n", words[i], isPalindrome(words[i]) ? "Palindrome" : "Not a palindrome");
    }
    return 0;
}
