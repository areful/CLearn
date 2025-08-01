//
// Created by areful on 2025/7/23.
//

#include "libutf8.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// UTF-8 BOM: EF BB BF
static const uint8_t UTF8_BOM[] = {0xEF, 0xBB, 0xBF};
#define UTF8_BOM_LENGTH 3

// 检查是否是有效的UTF-8字符
bool is_valid_utf8_char(const uint8_t *str, size_t len, size_t *char_len) {
    if (len == 0) return false;

    // 单字节字符 (ASCII)
    if (str[0] < 0x80) {
        *char_len = 1;
        return true;
    }

    // 多字节字符
    if ((str[0] & 0xE0) == 0xC0 && len >= 2) { // 2字节
        if ((str[1] & 0xC0) == 0x80) {
            *char_len = 2;
            return true;
        }
    } else if ((str[0] & 0xF0) == 0xE0 && len >= 3) { // 3字节
        if ((str[1] & 0xC0) == 0x80 && (str[2] & 0xC0) == 0x80) {
            *char_len = 3;
            return true;
        }
    } else if ((str[0] & 0xF8) == 0xF0 && len >= 4) { // 4字节
        if ((str[1] & 0xC0) == 0x80 && (str[2] & 0xC0) == 0x80 &&
            (str[3] & 0xC0) == 0x80) {
            *char_len = 4;
            return true;
        }
    }

    return false;
}

// 检查字符串是否是有效的UTF-8
bool is_valid_utf8(const uint8_t *str, size_t len) {
    size_t i = 0;
    while (i < len) {
        size_t char_len;
        if (!is_valid_utf8_char(str + i, len - i, &char_len)) {
            return false;
        }
        i += char_len;
    }
    return true;
}

// 计算UTF-8字符串的字符数（非字节数）
size_t utf8_char_count(const uint8_t *str, size_t len) {
    size_t count = 0;
    size_t i = 0;

    while (i < len) {
        size_t char_len;
        if (is_valid_utf8_char(str + i, len - i, &char_len)) {
            count++;
            i += char_len;
        } else {
            return 0; // 无效的UTF-8序列
        }
    }
    return count;
}

// 检查并跳过BOM
const uint8_t *skip_utf8_bom(const uint8_t *str, size_t len, size_t *out_len) {
    if (len >= UTF8_BOM_LENGTH &&
        memcmp(str, UTF8_BOM, UTF8_BOM_LENGTH) == 0) {
        *out_len = len - UTF8_BOM_LENGTH;
        return str + UTF8_BOM_LENGTH;
    }
    *out_len = len;
    return str;
}
