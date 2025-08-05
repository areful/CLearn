//
// Created by areful on 2025/8/5.
//
#include <stdio.h>

// 定义回调函数类型
typedef void (*CallbackFunction)(int count);

// 计数器结构体，包含回调函数指针
typedef struct {
    int count;
    int threshold;
    CallbackFunction callback;
} Counter;

// 初始化计数器
void initCounter(Counter *counter, int threshold, CallbackFunction callback) {
    counter->count = 0;
    counter->threshold = threshold;
    counter->callback = callback;
}

// 增加计数并检查是否触发回调
void incrementCounter(Counter *counter) {
    counter->count++;
    printf("Current count: %d\n", counter->count);

    // 当计数达到阈值时触发回调
    if (counter->count >= counter->threshold && counter->callback != NULL) {
        counter->callback(counter->count);
    }
}

// 示例回调函数
void onThresholdReached(int count) {
    printf("Threshold reached! Final count: %d\n", count);
}

int main() {
    Counter counter;

    // 初始化计数器，设置阈值为5，回调函数为onThresholdReached
    initCounter(&counter, 5, onThresholdReached);

    // 模拟计数
    for (int i = 0; i < 7; i++) {
        incrementCounter(&counter);
    }

    return 0;
}