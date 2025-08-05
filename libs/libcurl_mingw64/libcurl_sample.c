//
// Created by areful on 2025/7/28.
//
// # 在项目根新建 certs 目录
//      mkdir certs
// # 下载 Mozilla CA bundle（官方长期维护）
//      curl -L https://curl.se/ca/cacert.pem -o certs/cacert.pem
// 项目中使用：
//      curl_easy_setopt(curl, CURLOPT_CAINFO, "certs/cacert.pem");
//
#include <stdio.h>
#include <curl/curl.h>

size_t write_cb(void *ptr, size_t size, size_t nmemb, void *userdata) {
    fwrite(ptr, size, nmemb, stdout);
    return size * nmemb;
}

int main(void) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *h = curl_easy_init();
    if (h) {
        curl_easy_setopt(h, CURLOPT_CAINFO, "certs/cacert.pem");

        curl_easy_setopt(h, CURLOPT_URL, "https://httpbin.org/get");
        curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_cb);
        CURLcode rc = curl_easy_perform(h);
        if (rc != CURLE_OK)
            fprintf(stderr, "curl error: %s\n", curl_easy_strerror(rc));
        curl_easy_cleanup(h);
    }
    curl_global_cleanup();
    return 0;
}