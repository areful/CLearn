//
// Created by areful on 2025/7/23.
//
#include <stdio.h>
#include "regexp.h"

int main() {
    // 测试用例
    const char *tests[][2] = {
            {"a.b", "acb"},
            {"a*b", "aaab"},
            {"a+b", "aab"},
            {"^abc", "abc"},
            {"abc$", "xyzabc"},
            {"a.b", "ab"},
            {"a*b", "b"},
            {"a+b", "b"}
    };

    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        printf("Pattern: %s, Text: %s, Match: %s\n",
               tests[i][0], tests[i][1],
               match(tests[i][0], tests[i][1]) ? "Yes" : "No");
    }

    return 0;
}