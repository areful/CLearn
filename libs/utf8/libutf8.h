//
// Created by areful on 2025/7/23.
//

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef CLEARNING_LIBUTF8_H
#define CLEARNING_LIBUTF8_H

// 检查是否是有效的UTF-8字符
bool is_valid_utf8_char(const uint8_t *str, size_t len, size_t *char_len);

// 检查字符串是否是有效的UTF-8
bool is_valid_utf8(const uint8_t *str, size_t len);

// 计算UTF-8字符串的字符数（非字节数）
size_t utf8_char_count(const uint8_t *str, size_t len);

// 检查并跳过BOM
const uint8_t* skip_utf8_bom(const uint8_t *str, size_t len, size_t *out_len);

#endif //CLEARNING_LIBUTF8_H
