//
// Created by areful on 2025/8/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define DURATION 2.0
#define FREQUENCY 440.0
#define AMPLITUDE 32760

// WAV 头部结构
typedef struct {
    char chunkId[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1Id[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2Id[4];
    uint32_t subchunk2Size;
} WavHeader;

int main() {
#ifdef _WIN32
    // Windows 设置控制台为 UTF-8（Linux/macOS 通常无需此步骤）
    SetConsoleOutputCP(65001);
#endif

    FILE *file = fopen("audio.wav", "wb");
    if (!file) {
        fprintf(stderr, "无法创建音频文件\n");
        return 1;
    }

    size_t num_samples = (size_t) (DURATION * SAMPLE_RATE);
    int16_t *buffer = (int16_t *) malloc(num_samples * CHANNELS * sizeof(int16_t));
    if (!buffer) {
        fprintf(stderr, "内存分配失败\n");
        fclose(file);
        return 1;
    }

    // 生成正弦波
    for (size_t i = 0; i < num_samples; i++) {
        double t = (double) i / SAMPLE_RATE;
        int16_t sample = (int16_t)(AMPLITUDE * sin(2.0 * M_PI * FREQUENCY * t));
        buffer[i * CHANNELS] = sample;
        buffer[i * CHANNELS + 1] = sample;
    }

    // 设置 WAV 头部
    WavHeader header = {
            .chunkId = {'R', 'I', 'F', 'F'},
            .chunkSize = 36 + num_samples * CHANNELS * sizeof(int16_t),
            .format = {'W', 'A', 'V', 'E'},
            .subchunk1Id = {'f', 'm', 't', ' '},
            .subchunk1Size = 16,
            .audioFormat = 1, // PCM
            .numChannels = CHANNELS,
            .sampleRate = SAMPLE_RATE,
            .byteRate = SAMPLE_RATE * CHANNELS * sizeof(int16_t),
            .blockAlign = CHANNELS * sizeof(int16_t),
            .bitsPerSample = 16,
            .subchunk2Id = {'d', 'a', 't', 'a'},
            .subchunk2Size = num_samples * CHANNELS * sizeof(int16_t)
    };

    // 写入 WAV 头部和数据
    fwrite(&header, sizeof(WavHeader), 1, file);
    fwrite(buffer, sizeof(int16_t), num_samples * CHANNELS, file);
    fclose(file);
    free(buffer);
    printf("已生成 audio.wav\n");
    return 0;
}