//
// Created by areful on 2025/7/28.
//
#include <stdio.h>
#include "openssl/evp.h"

int main(void) {
    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, "hello openssl", 13);
    EVP_DigestFinal_ex(ctx, md, &md_len);
    EVP_MD_CTX_free(ctx);

    printf("SHA256: ");
    for (unsigned int i = 0; i < md_len; ++i) printf("%02x", md[i]);
    puts("");
    return 0;
}