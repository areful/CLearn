//
// Created by areful on 2025/7/23.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <libutf8.h>

// 示例：处理字符串
int main() {
    // 测试字符串（包含中文和BOM）
    const char *test_str = "\xEF\xBB\xBFHello, 世界!";
    size_t len = strlen(test_str);
    size_t new_len;

    // 检查BOM并获取跳过BOM后的字符串
    const uint8_t *str_no_bom = skip_utf8_bom((const uint8_t *) test_str, len, &new_len);

    printf("Original length (bytes): %zu\n", len);
    printf("Length after BOM skip (bytes): %zu\n", new_len);

    // 验证UTF-8
    if (is_valid_utf8(str_no_bom, new_len)) {
        printf("Valid UTF-8 string\n");

        // 计算字符数
        size_t char_count = utf8_char_count(str_no_bom, new_len);
        printf("Character count: %zu\n", char_count);

        // 打印每个字符的字节
        size_t i = 0;
        while (i < new_len) {
            size_t char_len;
            if (is_valid_utf8_char(str_no_bom + i, new_len - i, &char_len)) {
                printf("Char at %zu: ", i);
                for (size_t j = 0; j < char_len; j++) {
                    printf("%02X ", str_no_bom[i + j]);
                }
                printf("\n");
                i += char_len;
            }
        }
    } else {
        printf("Invalid UTF-8 string\n");
    }

    return 0;
}