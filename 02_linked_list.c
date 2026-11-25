#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *pushFront(Node *head, int value) {
    Node *newNode = malloc(sizeof(Node));
    if (!newNode) return head;
    newNode->data = value;
    newNode->next = head;
    return newNode;
}

void printList(const Node *head) {
    while (head) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

void freeList(Node *head) {
    while (head) {
        Node *tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main(void) {
    Node *list = NULL;
    list = pushFront(list, 30);
    list = pushFront(list, 20);
    list = pushFront(list, 10);

    printf("Singly Linked List: ");
    printList(list);

    freeList(list);
    return 0;
}
