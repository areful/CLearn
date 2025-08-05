#include <stdio.h>
#include <stdlib.h>

// 定义链表节点结构体
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// 定义链表结构体
typedef struct {
    Node *head;
} LinkedList;

// 初始化链表
void initList(LinkedList *list) {
    list->head = NULL;
}

// 插入节点到链表头部
void insertFront(LinkedList *list, int data) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->data = data;
    newNode->next = list->head;
    list->head = newNode;
}

// 插入节点到链表尾部
void insertBack(LinkedList *list, int data) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->data = data;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
        return;
    }

    Node *current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

// 删除头部节点
void deleteFront(LinkedList *list) {
    if (list->head == NULL) {
        printf("List is empty, cannot delete front node\n");
        return;
    }

    Node *temp = list->head;
    list->head = list->head->next;
    free(temp);
}

// 删除尾部节点
void deleteBack(LinkedList *list) {
    if (list->head == NULL) {
        printf("List is empty, cannot delete back node\n");
        return;
    }

    if (list->head->next == NULL) {
        free(list->head);
        list->head = NULL;
        return;
    }

    Node *current = list->head;
    Node *prev = NULL;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    if (prev != NULL) {
        prev->next = NULL;
    }
    free(current);
}

// 删除指定数据的节点
void deleteNode(LinkedList *list, int data) {
    Node *current = list->head;
    Node *prev = NULL;

    while (current != NULL && current->data != data) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Node with data %d not found\n", data);
        return;
    }

    if (prev == NULL) {
        list->head = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
}

// 遍历打印链表
void printList(LinkedList *list) {
    Node *current = list->head;
    printf("List: ");
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// 释放链表内存
void freeList(LinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
}

int main() {
    LinkedList list;
    initList(&list);

    // 测试插入操作
    insertFront(&list, 1);
    insertBack(&list, 3);
    insertFront(&list, 0);
    insertBack(&list, 4);
    printList(&list); // 输出: List: 0 -> 1 -> 3 -> 4 -> NULL

    // 测试删除操作
    deleteNode(&list, 1);
    printList(&list); // 输出: List: 0 -> 3 -> 4 -> NULL

    deleteFront(&list);
    printList(&list); // 输出: List: 3 -> 4 -> NULL

    deleteBack(&list);
    printList(&list); // 输出: List: 3 -> NULL

    // 释放链表
    freeList(&list);
    printList(&list); // 输出: List: NULL

    return 0;
}
