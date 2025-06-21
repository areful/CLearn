#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define ESP8266_BAUD CBR_115200
#define RESPONSE_TIMEOUT_MS 10000
#define BUFFER_SIZE 4096

HANDLE hSerial;

// 获取当前时间(毫秒)
long long current_timestamp_ms() {
    return GetTickCount();
}

// 初始化串口
int init_serial(const char *com_port) {
    hSerial = CreateFileA(
            com_port,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("无法打开串口 %s, 错误: %d\n", com_port, GetLastError());
        return 0;
    }

    // 配置串口参数
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("获取串口状态失败\n");
        CloseHandle(hSerial);
        return 0;
    }

    dcbSerialParams.BaudRate = ESP8266_BAUD;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("设置串口参数失败\n");
        CloseHandle(hSerial);
        return 0;
    }

    // 设置超时
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("设置串口超时失败\n");
        CloseHandle(hSerial);
        return 0;
    }

    printf("串口 %s 初始化成功\n", com_port);
    return 1;
}

// 发送AT指令并等待响应
int send_at_command(const char *cmd, const char *expected_response, unsigned int timeout_ms) {
    char buffer[BUFFER_SIZE] = {0};
    DWORD bytes_read = 0;
    DWORD bytes_written = 0;
    long long start_time = current_timestamp_ms();
    int total_bytes = 0;

    // 清空接收缓冲区
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    // 发送命令
    printf("发送: %s\n", cmd);
    char full_cmd[256];
    snprintf(full_cmd, sizeof(full_cmd), "%s\r\n", cmd);

    if (!WriteFile(hSerial, full_cmd, strlen(full_cmd), &bytes_written, NULL)) {
        printf("发送命令失败\n");
        return 0;
    }

    // 等待响应
    while ((current_timestamp_ms() - start_time) < timeout_ms) {
        if (ReadFile(hSerial, buffer + total_bytes, BUFFER_SIZE - total_bytes - 1, &bytes_read, NULL)) {
            if (bytes_read > 0) {
                total_bytes += bytes_read;
                buffer[total_bytes] = '\0';

                // 检查是否收到完整响应
                if (expected_response == NULL || strstr(buffer, expected_response) != NULL) {
                    printf("接收: %s\n", buffer);
                    return 1;
                }
            }
        }

        Sleep(10);
    }

    printf("接收(超时): %s\n", buffer);
    return 0;
}

// 连接WiFi网络
int connect_to_wifi() {
    const char *ssid = "Piao";
    const char *password = "yanliping123456";
    char cmd[128];

    // 设置WiFi模式为STA
    if (!send_at_command("AT+CWMODE=1", "OK", 2000)) {
        printf("设置WiFi模式失败\n");
        return 0;
    }

    // 连接WiFi
    printf("正在连接WiFi: %s...\n", ssid);
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
    if (!send_at_command(cmd, "OK", 15000)) {  // 连接WiFi需要更长时间
        printf("连接WiFi失败\n");
        return 0;
    }

    printf("成功连接到WiFi: %s\n", ssid);
    return 1;
}

// 获取HTTP响应内容
int get_http_response(char *response, int max_len) {
    DWORD bytes_read = 0;
    long long start_time = current_timestamp_ms();
    int total_bytes = 0;

    memset(response, 0, max_len);

    while ((current_timestamp_ms() - start_time) < RESPONSE_TIMEOUT_MS) {
        if (ReadFile(hSerial, response + total_bytes, max_len - total_bytes - 1, &bytes_read, NULL)) {
            if (bytes_read > 0) {
                total_bytes += bytes_read;
                response[total_bytes] = '\0';

                // 检查是否收到完整响应(包含CLOSED)
                if (strstr(response, "CLOSED") != NULL) {
                    return 1;
                }
            }
        }

        Sleep(10);
    }

    return 0;
}

// 发送HTTP请求
int send_http_request(const char *host, int port, const char *path) {
    char cmd[128];
    char http_request[256];
    int request_length;
    char response[BUFFER_SIZE];

    // 建立TCP连接
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"%s\",%d", host, port);
    if (!send_at_command(cmd, "OK", 3000)) {
        printf("TCP连接失败\n");
        return 0;
    }

    // 构造HTTP GET请求
    snprintf(http_request, sizeof(http_request),
             "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
             path, host);
    request_length = strlen(http_request);

    // 准备发送数据
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%d", request_length);
    if (!send_at_command(cmd, ">", 2000)) {
        printf("准备发送数据失败\n");
        return 0;
    }

    // 发送HTTP请求
    printf("发送HTTP请求:\n%s\n", http_request);
    DWORD bytes_written;
    WriteFile(hSerial, http_request, request_length, &bytes_written, NULL);

    // 获取HTTP响应
    printf("等待HTTP响应...\n");
    if (get_http_response(response, BUFFER_SIZE)) {
        printf("完整HTTP响应:\n%s\n", response);
    } else {
        printf("不完整HTTP响应:\n%s\n", response);
    }

    // 关闭TCP连接
    send_at_command("AT+CIPCLOSE", "OK", 2000);

    return 1;
}

int main() {
    const char *com_port = "COM3"; // 修改为你的COM端口
    const char *host = "example.com";
    int port = 80;
    const char *path = "/";

    // 初始化串口
    if (!init_serial(com_port)) {
        return 1;
    }

    // 测试AT指令
    if (!send_at_command("AT", "OK", 2000)) {
        printf("ESP8266未响应\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 连接WiFi
    if (!connect_to_wifi()) {
        CloseHandle(hSerial);
        return 1;
    }

    // 发送HTTP请求
    if (!send_http_request(host, port, path)) {
        CloseHandle(hSerial);
        return 1;
    }

    // 关闭串口
    CloseHandle(hSerial);
    return 0;
}