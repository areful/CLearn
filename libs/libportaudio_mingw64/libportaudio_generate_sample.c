//
// Created by areful on 2025/8/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define DURATION 2.0 // 持续时间（秒）
#define FREQUENCY 440.0 // 频率（Hz，A4音）
#define AMPLITUDE 32760 // 16-bit PCM 最大幅度

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    // Windows 设置控制台为 UTF-8（Linux/macOS 通常无需此步骤）
    SetConsoleOutputCP(65001);
#endif

    FILE *file = fopen("audio.bin", "wb");
    if (!file) {
        fprintf(stderr, "无法创建音频文件\n");
        return 1;
    }

    // 计算样本数
    size_t num_samples = (size_t)(DURATION * SAMPLE_RATE);
    int16_t *buffer = (int16_t *)malloc(num_samples * CHANNELS * sizeof(int16_t));
    if (!buffer) {
        fprintf(stderr, "内存分配失败\n");
        fclose(file);
        return 1;
    }

    // 生成正弦波（立体声，双声道相同数据）
    for (size_t i = 0; i < num_samples; i++) {
        double t = (double)i / SAMPLE_RATE;
        int16_t sample = (int16_t)(AMPLITUDE * sin(2.0 * M_PI * FREQUENCY * t));
        buffer[i * CHANNELS] = sample;      // 左声道
        buffer[i * CHANNELS + 1] = sample;  // 右声道
    }

    // 写入二进制文件
    fwrite(buffer, sizeof(int16_t), num_samples * CHANNELS, file);
    fclose(file);
    free(buffer);
    printf("已生成 audio.bin\n");
    return 0;
}