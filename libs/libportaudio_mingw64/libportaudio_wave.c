//
// Created by areful on 2025/8/25.
//
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// WAV 头部结构
typedef struct {
    char chunkId[4];        // "RIFF"
    uint32_t chunkSize;
    char format[4];         // "WAVE"
    char subchunk1Id[4];    // "fmt "
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2Id[4];    // "data"
    uint32_t subchunk2Size;
} WavHeader;

// 音频数据结构
typedef struct {
    int16_t *data;
    size_t length;
    size_t index;
    uint16_t numChannels;
} AudioData;

// 回调函数
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData) {
    AudioData *audio = (AudioData *) userData;
    int16_t *out = (int16_t *) outputBuffer;
    size_t i;

    for (i = 0; i < framesPerBuffer * audio->numChannels; i++) {
        if (audio->index < audio->length) {
            out[i] = audio->data[audio->index++];
        } else {
            out[i] = 0;
        }
    }

    return audio->index < audio->length ? paContinue : paComplete;
}

int main() {
    PaError err;
    PaStream *stream;
    PaStreamParameters outputParameters;
    AudioData audioData = {0};
    FILE *file;
    WavHeader header;

    // 初始化 PortAudio
    err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stderr, "PortAudio 初始化错误: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    // 打开 WAV 文件
    file = fopen("audio.wav", "rb");
    if (!file) {
        fprintf(stderr, "无法打开音频文件\n");
        Pa_Terminate();
        return 1;
    }

    // 读取 WAV 头部
    if (fread(&header, sizeof(WavHeader), 1, file) != 1) {
        fprintf(stderr, "读取 WAV 头部失败\n");
        fclose(file);
        Pa_Terminate();
        return 1;
    }

    // 验证 WAV 文件格式
    if (strncmp(header.chunkId, "RIFF", 4) != 0 || strncmp(header.format, "WAVE", 4) != 0) {
        fprintf(stderr, "无效的 WAV 文件\n");
        fclose(file);
        Pa_Terminate();
        return 1;
    }
    if (header.audioFormat != 1) {
        fprintf(stderr, "仅支持 PCM 格式的 WAV 文件\n");
        fclose(file);
        Pa_Terminate();
        return 1;
    }
    if (header.bitsPerSample != 16) {
        fprintf(stderr, "仅支持 16-bit PCM\n");
        fclose(file);
        Pa_Terminate();
        return 1;
    }

    // 计算数据大小
    size_t fileSize = header.subchunk2Size / sizeof(int16_t);
    audioData.numChannels = header.numChannels;

    // 分配内存存储音频数据
    audioData.data = (int16_t *) malloc(fileSize * sizeof(int16_t));
    if (!audioData.data) {
        fprintf(stderr, "内存分配失败\n");
        fclose(file);
        Pa_Terminate();
        return 1;
    }

    // 读取音频数据
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
    outputParameters.channelCount = header.numChannels;
    outputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // 打开音频流
    err = Pa_OpenStream(
            &stream,
            NULL,
            &outputParameters,
            header.sampleRate,
            256,
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