//
// Created by areful on 2025/7/23.
//

#include "regexp.h"

// 检查单个字符是否匹配
int match_char(char pattern, char text) {
    if (pattern == '.') return 1; // 点号匹配任意字符
    return pattern == text;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
// 匹配当前位置的模式
int match_here(const char *pattern, const char *text) {
    // 模式为空
    if (*pattern == '\0') return 1;

    // 处理 $ 结尾
    if (pattern[0] == '$' && pattern[1] == '\0') {
        return *text == '\0';
    }

    // 处理 * 量词
    if (pattern[1] == '*') {
        do {
            if (match_here(pattern + 2, text)) return 1;
        } while (*text != '\0' && match_char(pattern[0], *text++));
        return 0;
    }

    // 处理 + 量词
    if (pattern[1] == '+') {
        if (!match_char(pattern[0], *text)) return 0;
        text++;
        do {
            if (match_here(pattern + 2, text)) return 1;
        } while (*text != '\0' && match_char(pattern[0], *text++));
        return 0;
    }

    // 普通字符匹配
    if (*text != '\0' && match_char(pattern[0], *text)) {
        return match_here(pattern + 1, text + 1);
    }

    return 0;
}
#pragma clang diagnostic pop

// 主匹配函数
int match(const char *pattern, const char *text) {
    if (*pattern == '^') {
        return match_here(pattern + 1, text);
    }
    do {
        if (match_here(pattern, text)) return 1;
    } while (*text++ != '\0');
    return 0;
}
