#include <stdio.h>
#include <stdlib.h>

// 定义链表节点结构体
struct Node {
    int data;
    struct Node* next;
};

// 在链表末尾插入新节点
void append(struct Node** head_ref, int new_data) {
    // 为新节点分配内存
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    struct Node* last = *head_ref; // 用于遍历链表找到最后一个节点

    // 设置新节点的数据
    new_node->data = new_data;
    new_node->next = NULL;

    // 如果链表为空，则将新节点作为头节点
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    // 否则找到链表的最后一个节点，并将新节点连接到其后
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = new_node;
}

// 释放链表内存
void free_list(struct Node* head) {
    struct Node* current = head;
    struct Node* next;

    // 遍历链表并释放每个节点的内存
    while (current != NULL) {
        next = current->next; // 保存下一个节点的指针
        free(current); // 释放当前节点的内存
        current = next; // 移动到下一个节点
    }
}

// 打印链表的内容
void print_list(struct Node* node) {
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

int main() {
    struct Node* head = NULL; // 链表头指针

    // 在链表末尾插入节点
    append(&head, 1);
    append(&head, 2);
    append(&head, 3);
    append(&head, 4);

    // 打印链表内容
    printf("Linked List: ");
    print_list(head);

    // 释放链表内存
    free_list(head);

    return 0;
}
