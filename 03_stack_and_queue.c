#include <stdio.h>
#define MAX 5

typedef struct {
    int items[MAX];
    int top;
} Stack;

void initStack(Stack *s) { s->top = -1; }
int push(Stack *s, int val) {
    if (s->top >= MAX - 1) return 0;
    s->items[++(s->top)] = val;
    return 1;
}
int pop(Stack *s, int *out) {
    if (s->top < 0) return 0;
    *out = s->items[(s->top)--];
    return 1;
}

int main(void) {
    Stack s;
    initStack(&s);
    push(&s, 15);
    push(&s, 25);
    push(&s, 35);

    int val;
    printf("Popping from stack: ");
    while (pop(&s, &val)) {
        printf("%d ", val);
    }
    printf("\n");
    return 0;
}
