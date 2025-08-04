#include <stdio.h>
#include <windows.h>

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    printf("Thread %lu is running.\n", GetCurrentThreadId());
    return 0;
}

int main() {
    HANDLE hThread;
    DWORD dwThreadId;

    DWORD currentId = GetCurrentThreadId();

    printf("main Thread %lu is running.\n", currentId);

    // 创建线程
    hThread = CreateThread(NULL, 0, ThreadFunction, NULL, 0, &dwThreadId);
    if (hThread == NULL) {
        printf("Failed to create thread.\n");
        return 1;
    }

    // 等待线程结束
    WaitForSingleObject(hThread, INFINITE);

    // 关闭线程句柄
    CloseHandle(hThread);

    HWND hFore = GetForegroundWindow();
    DWORD foreId = GetWindowThreadProcessId(hFore, NULL);

    // 附加线程
    if (!AttachThreadInput(foreId, currentId, TRUE)) {
        printf("Failed to attach thread.\n");
        return 1;
    }

    printf("Thread attached.\n");

    if(!AttachThreadInput(foreId, currentId, FALSE)) {
        printf("Failed to detach thread.\n");
        return 1;
    }

    printf("Thread detached.\n");
    return 0;
}
