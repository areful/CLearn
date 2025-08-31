//
// Created by areful on 2025/7/23.
//
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

// 将 Unicode 码点转换为 UTF-8 编码
void print_utf8(int codepoint) {
    if (codepoint <= 0x7F) {
        // 1 字节 UTF-8
        putchar(codepoint);
    } else if (codepoint <= 0x7FF) {
        // 2 字节 UTF-8
        putchar(0xC0 | ((codepoint >> 6) & 0x1F));
        putchar(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0xFFFF) {
        // 3 字节 UTF-8
        putchar(0xE0 | ((codepoint >> 12) & 0x0F));
        putchar(0x80 | ((codepoint >> 6) & 0x3F));
        putchar(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0x10FFFF) {
        // 4 字节 UTF-8（emoji 通常在此范围）
        putchar(0xF0 | ((codepoint >> 18) & 0x07));
        putchar(0x80 | ((codepoint >> 12) & 0x3F));
        putchar(0x80 | ((codepoint >> 6) & 0x3F));
        putchar(0x80 | (codepoint & 0x3F));
    }
}

int main() {
#ifdef _WIN32
    // Windows 设置控制台为 UTF-8（Linux/macOS 通常无需此步骤）
    SetConsoleOutputCP(65001);
#endif

    // 定义主要的 emoji Unicode 范围（基于 Unicode 15.0）
    int ranges[][2] = {
            {0x1F300, 0x1F5FF}, // 杂项符号和象形文字
            {0x1F600, 0x1F64F}, // 表情符号
            {0x1F680, 0x1F6FF}, // 交通和地图符号
            {0x1F900, 0x1F9FF}, // 补充符号和象形文字
            {0x2600,  0x26FF},   // 杂项符号
            {0x2700,  0x27BF},   // 装饰符号
            {0x2190,  0x21FF},   // 箭头
            {0x2500,  0x257F},   // 方框绘制
            {0x25A0,  0x25FF},   // 几何形状
            {0x1F100, 0x1F1FF}, // 补充私人使用区域（部分数字和符号）
            {0x1FA70, 0x1FAFF}, // 符号和象形文字扩展-A
            {0x3000,  0x303F}     // CJK 符号和标点
    };

    // 计算范围数组的行数
    int num_ranges = sizeof(ranges) / sizeof(ranges[0]);

    unsigned long total = 0;
    // 遍历每个范围
    for (int i = 0; i < num_ranges; i++) {
        for (int codepoint = ranges[i][0]; codepoint <= ranges[i][1]; codepoint++) {
            // 跳过部分已知无效或非 emoji 码点（如变体选择器或 ZWJ）
            if (codepoint == 0x200D || (codepoint >= 0xFE00 && codepoint <= 0xFE0F)) {
                continue;
            }
            // 输出 Unicode 码点和对应的 emoji
//            printf("U+%04X: ", codepoint);
            print_utf8(codepoint);
//            printf("\n");

            total++;
            if (total % 16 == 0) {
                printf("\n");
            }
        }
    }

    printf("total: %ld\n", total);
    return 0;
}