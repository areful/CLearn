//
// Created by jian.gao on 2024-4-9.
//
#include <stdio.h>
#include <string.h>

void copy_string(char *source, char *destination) {
    // 手动复制字符串，直到遇到字符串结束符 '\0'
    while (*source != '\0') {
        *destination = *source;
        source++;
        destination++;
    }
    *destination = '\0'; // 添加目标字符串结束符
}

void copy_method0() {
    char source[] = "Hello, world!";
    char destination[20];

    // 使用自定义函数复制字符串
    copy_string(source, destination);

    printf("Source: %s\n", source);
    printf("Destination: %s\n", destination);
}

void copy_method1() {
    char source[] = "Hello, world!";
    char destination[20];

    // 使用 strncpy() 函数复制字符串
    strncpy(destination, source, sizeof(destination) - 1);
    destination[sizeof(destination) - 1] = '\0'; // 确保目标字符串以空字符结尾

    printf("Source: %s\n", source);
    printf("Destination: %s\n", destination);
}

void copy_method2() {
    char source[] = "Hello, world!";
    char destination[20];

    // 使用 snprintf() 函数复制字符串
    snprintf(destination, sizeof(destination), "%s", source);

    printf("Source: %s\n", source);
    printf("Destination: %s\n", destination);
}

int main() {
    copy_method0(); // 使用自定义函数复制字符串
    copy_method1(); // 使用 strncpy() 函数复制字符串
    copy_method2(); // 使用 snprintf() 函数复制字符串

    return 0;
}