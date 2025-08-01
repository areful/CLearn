//
// Created by areful on 2025/7/23.
//

#ifndef CSAMPLE_REG_EXP_H
#define CSAMPLE_REG_EXP_H

// 检查单个字符是否匹配
int match_char(char pattern, char text);

// 匹配当前位置的模式
int match_here(const char *pattern, const char *text);

// 主匹配函数
int match(const char *pattern, const char *text);

#endif //CSAMPLE_REG_EXP_H
