#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nettle/aes.h>
#include <nettle/cbc.h>

void print_hex(const char* label, const uint8_t* data, size_t length) {
    printf("%s: ", label);
    for (size_t i = 0; i < length; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    // AES-256 密钥 (32字节)
    const uint8_t key[AES256_KEY_SIZE] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
            0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };

    // 初始化向量 (IV) (16字节)
    uint8_t iv[AES_BLOCK_SIZE] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };

    // 明文数据
    const char* plaintext = "This is a secret message that needs to be encrypted!";
    size_t plaintext_len = strlen(plaintext);

    // 计算填充后的长度
    size_t padded_len = ((plaintext_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // 分配内存
    uint8_t* padded_plaintext = malloc(padded_len);
    uint8_t* ciphertext = malloc(padded_len);
    uint8_t* decrypted = malloc(padded_len);

    if (!padded_plaintext || !ciphertext || !decrypted) {
        perror("Memory allocation failed");
        return 1;
    }

    // 复制明文并填充
    memcpy(padded_plaintext, plaintext, plaintext_len);
    uint8_t pad_value = AES_BLOCK_SIZE - (plaintext_len % AES_BLOCK_SIZE);
    memset(padded_plaintext + plaintext_len, pad_value, pad_value);

    printf("Original message: %s\n", plaintext);
    print_hex("Plaintext (with padding)", padded_plaintext, padded_len);

    // 加密
    struct aes256_ctx enc_ctx;
    aes256_set_encrypt_key(&enc_ctx, key);

    // 保存 IV 的副本，因为 cbc_encrypt 会修改它
    uint8_t iv_enc[AES_BLOCK_SIZE];
    memcpy(iv_enc, iv, AES_BLOCK_SIZE);

    cbc_encrypt(&enc_ctx, (nettle_cipher_func*)aes256_encrypt,
                AES_BLOCK_SIZE, iv_enc,
                padded_len, ciphertext, padded_plaintext);

    print_hex("Ciphertext", ciphertext, padded_len);

    // 解密
    struct aes256_ctx dec_ctx;
    aes256_set_decrypt_key(&dec_ctx, key);

    // 必须使用加密时的 IV
    uint8_t iv_dec[AES_BLOCK_SIZE];
    memcpy(iv_dec, iv, AES_BLOCK_SIZE);  // 恢复初始 IV

    cbc_decrypt(&dec_ctx, (nettle_cipher_func*)aes256_decrypt,
                AES_BLOCK_SIZE, iv_dec,
                padded_len, decrypted, ciphertext);

    // 移除填充
    uint8_t last_byte = decrypted[padded_len - 1];
    if (last_byte <= AES_BLOCK_SIZE) {
        int valid_pad = 1;
        for (size_t i = padded_len - last_byte; i < padded_len; i++) {
            if (decrypted[i] != last_byte) {
                valid_pad = 0;
                break;
            }
        }
        if (valid_pad) {
            padded_len -= last_byte;
        }
    }

    decrypted[padded_len] = '\0';
    printf("Decrypted message: %s\n", (char*)decrypted);

    free(padded_plaintext);
    free(ciphertext);
    free(decrypted);

    return 0;
}