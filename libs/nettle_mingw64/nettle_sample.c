//
// Created by areful on 2025/7/29.
//
#include "nettle/sha2.h"
#include <stdio.h>

int main(void) {
    struct sha256_ctx ctx;
    uint8_t digest[SHA256_DIGEST_SIZE];
    const uint8_t data[] = "The quick brown fox jumps over the lazy dog";

    sha256_init(&ctx);
    sha256_update(&ctx, sizeof(data) - 1, data);
    sha256_digest(&ctx, sizeof(digest), digest);

    printf("SHA256: ");
    for (int i = 0; i < sizeof(digest); ++i)
        printf("%02x", digest[i]);
    puts("");
    return 0;
}