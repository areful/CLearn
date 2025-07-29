#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MQTTAsync.h>
#include <synchapi.h>

#define ADDRESS     "tcp://broker.hivemq.com:1883"
#define CLIENTID    "ExampleClient"
#define TOPIC       "test/topic"
#define PAYLOAD     "Hello, MQTT!"
#define QOS         1
#define TIMEOUT     10000L

volatile int finished = 0;
volatile int connected = 0;

void connlost(void *context, char *cause) {
    printf("\nConnection lost, cause: %s\n", cause ? cause : "unknown");
    finished = 1;
}

void onConnect(void *context, MQTTAsync_successData *response) {
    printf("Successfully connected to MQTT broker\n");
    connected = 1;
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    pubmsg.payload = (void *)PAYLOAD;
    pubmsg.payloadlen = (int)strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    // 发布消息
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    opts.onSuccess = NULL;
    opts.onFailure = NULL;
    opts.context = client;
    int rc = MQTTAsync_sendMessage(client, TOPIC, &pubmsg, &opts);
    if (rc != MQTTASYNC_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published to %s\n", TOPIC);
    }
}

void onConnectFailure(void *context, MQTTAsync_failureData *response) {
    printf("Connection failed, rc %d, message: %s\n",
           response ? response->code : -1,
           response && response->message ? response->message : "unknown");
    finished = 1;
}

void onSubscribe(void *context, MQTTAsync_successData *response) {
    printf("Successfully subscribed to %s\n", TOPIC);
}

void onSubscribeFailure(void *context, MQTTAsync_failureData *response) {
    printf("Subscribe failed, rc %d\n", response ? response->code : -1);
    finished = 1;
}

int onMessage(void *context, char *topicName, int topicLen, MQTTAsync_message *message) {
    printf("Received message: %.*s on topic %s\n", message->payloadlen, (char *)message->payload, topicName);
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return MQTTASYNC_SUCCESS;
}

// 跟踪回调函数，用于调试 Paho MQTT 内部日志
void onTraceCallback(enum MQTTASYNC_TRACE_LEVELS level, char *message) {
    printf("Trace: %s\n", message);
}

int main(int argc, char *argv[]) {
    MQTTAsync client;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;

    // 启用 Paho MQTT 跟踪
    MQTTAsync_setTraceCallback(onTraceCallback);
    MQTTAsync_setTraceLevel(MQTTASYNC_TRACE_ERROR); // 设置为 ERROR 或 MAXIMUM 以获取更多日志

    printf("Starting MQTT client...\n");

    // 创建 MQTT 客户端
    rc = MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (rc != MQTTASYNC_SUCCESS) {
        printf("Failed to create client, return code %d\n", rc);
        return 1;
    }

    // 设置回调函数
    rc = MQTTAsync_setCallbacks(client, NULL, connlost, onMessage, NULL);
    if (rc != MQTTASYNC_SUCCESS) {
        printf("Failed to set callbacks, return code %d\n", rc);
        MQTTAsync_destroy(&client);
        return 1;
    }

    // 配置连接参数
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = onConnect;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = client;

    // 连接到 MQTT 代理
    printf("Connecting to %s...\n", ADDRESS);
    rc = MQTTAsync_connect(client, &conn_opts);
    if (rc != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect, return code %d\n", rc);
        fflush(stdout);
        MQTTAsync_destroy(&client);
        return 1;
    }

    // 等待连接
    while (!connected && !finished) {
        printf("Waiting connected… %d\n", rc);
        fflush(stdout);
        Sleep(1000);
    }

    if (finished) {
        printf("Connection failed, exiting...\n");
        fflush(stdout);
        MQTTAsync_destroy(&client);
        return 1;
    }

    // 订阅主题
    MQTTAsync_responseOptions sub_opts = MQTTAsync_responseOptions_initializer;
    sub_opts.onSuccess = onSubscribe;
    sub_opts.onFailure = onSubscribeFailure;
    printf("Subscribing to %s...\n", TOPIC);
    fflush(stdout);
    rc = MQTTAsync_subscribe(client, TOPIC, QOS, &sub_opts);
    if (rc != MQTTASYNC_SUCCESS) {
        printf("Failed to subscribe, return code %d\n", rc);
        fflush(stdout);
        finished = 1;
    }

    // 主循环，等待消息
    int timeout = 30; // 运行 30 秒后退出
    while (!finished && timeout-- > 0) {
        Sleep(1000); // 每秒检查一次
        printf("Waiting for messages (%d seconds left)...\n", timeout);
        fflush(stdout);
    }

    // 清理
    printf("Cleaning up and exiting...\n");
    fflush(stdout);
    MQTTAsync_destroy(&client);
    return rc;
}