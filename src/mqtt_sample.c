//
// Created by areful on 2025/7/29.
//
#include "MQTTClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>   // Sleep

#define ADDRESS     "tcp://broker.hivemq.com:1883"
#define CLIENTID    "CLionSubPubDemo"
#define TOPIC       "test/topic"
#define QOS         1
#define TIMEOUT     10000L

volatile int received = 0;

// 收到订阅消息的回调
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    printf("Message arrived on topic \"%s\": %.*s\n",
           topicName, message->payloadlen, (char *)message->payload);
    received = 1;          // 置标志，主线程收到后退出
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    // 1. 创建客户端
    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // 2. 设置回调
    MQTTClient_setCallbacks(client, NULL, NULL, msgarrvd, NULL);

    // 3. 连接
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect\n");
        return EXIT_FAILURE;
    }

    // 4. 订阅
    MQTTClient_subscribe(client, TOPIC, QOS);
    printf("Subscribed to topic \"%s\"\n", TOPIC);

    // 5. 发布（让本程序自己产生一条消息）
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void *)"Hello from CLion!";
    pubmsg.payloadlen = (int)strlen((char *)pubmsg.payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, NULL);

    // 6. 等待收到消息
    while (!received) {
        Sleep(100);      // 避免 CPU 空转
    }

    // 7. 清理
    MQTTClient_unsubscribe(client, TOPIC);
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
    printf("Done.\n");
    return EXIT_SUCCESS;
}