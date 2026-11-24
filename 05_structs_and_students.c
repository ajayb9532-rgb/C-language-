#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float gpa;
} Student;

void printStudent(const Student *s) {
    printf("ID: %d | Name: %-15s | GPA: %.2f\n", s->id, s->name, s->gpa);
}

int main(void) {
    Student roster[3] = {
        {101, "Aarav Sharma", 3.85f},
        {102, "Priya Patel", 3.92f},
        {103, "Rohan Verma", 3.68f}
    };

    printf("--- Student Roster ---\n");
    for (int i = 0; i < 3; i++) {
        printStudent(&roster[i]);
    }

    return 0;
}
