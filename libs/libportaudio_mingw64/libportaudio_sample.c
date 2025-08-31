//
// Created by areful on 2025/8/22.
//
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define SAMPLE_RATE 44100
#define CHANNELS 2
#define SAMPLE_FORMAT paInt16 // 16-bit PCM
#define FRAMES_PER_BUFFER 256

// 音频数据结构
typedef struct {
    int16_t *data;
    size_t length;
    size_t index;
} AudioData;

// 回调函数，用于流式播放音频数据
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData) {
    AudioData *audio = (AudioData *)userData;
    int16_t *out = (int16_t *)outputBuffer;
    size_t i;

    // 复制数据到输出缓冲区
    for (i = 0; i < framesPerBuffer * CHANNELS; i++) {
        if (audio->index < audio->length) {
            out[i] = audio->data[audio->index++];
        } else {
            out[i] = 0; // 数据耗尽，填充静音
        }
    }

    return audio->index < audio->length ? paContinue : paComplete;
}

int main() {
#ifdef _WIN32
    // Windows 设置控制台为 UTF-8（Linux/macOS 通常无需此步骤）
    SetConsoleOutputCP(65001);
#endif

    PaError err;
    PaStream *stream;
    PaStreamParameters outputParameters;
    AudioData audioData = {0};
    FILE *file;
    size_t fileSize;

    // 初始化 PortAudio
    err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stderr, "PortAudio 初始化错误: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    // 读取二进制音频数据
    file = fopen("audio.bin", "rb");
    if (!file) {
        fprintf(stderr, "无法打开音频文件\n");
        Pa_Terminate();
        return 1;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file) / sizeof(int16_t); // 假设 16-bit 样本
    fseek(file, 0, SEEK_SET);

    // 分配内存存储音频数据
    audioData.data = (int16_t *)malloc(fileSize * sizeof(int16_t));
    if (!audioData.data) {
        fprintf(stderr, "内存分配失败\n");
        fclose(file);
        Pa_Terminate();
        return 1;
    }

    // 读取文件
    audioData.length = fread(audioData.data, sizeof(int16_t), fileSize, file);
    audioData.index = 0;
    fclose(file);

    if (audioData.length == 0) {
        fprintf(stderr, "读取音频数据失败\n");
        free(audioData.data);
        Pa_Terminate();
        return 1;
    }

    // 配置音频流
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr, "未找到音频设备\n");
        free(audioData.data);
        Pa_Terminate();
        return 1;
    }
    outputParameters.channelCount = CHANNELS;
    outputParameters.sampleFormat = SAMPLE_FORMAT;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // 打开音频流
    err = Pa_OpenStream(
            &stream,
            NULL, // 无输入
            &outputParameters,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,
            audioCallback,
            &audioData);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio 打开流错误: %s\n", Pa_GetErrorText(err));
        free(audioData.data);
        Pa_Terminate();
        return 1;
    }

    // 启动音频流
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio 启动流错误: %s\n", Pa_GetErrorText(err));
        free(audioData.data);
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }

    // 等待播放完成
    while (Pa_IsStreamActive(stream)) {
        Pa_Sleep(100);
    }

    // 清理
    Pa_CloseStream(stream);
    free(audioData.data);
    Pa_Terminate();
    printf("播放完成\n");
    return 0;
}