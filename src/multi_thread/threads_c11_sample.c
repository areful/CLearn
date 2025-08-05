//
// Created by areful on 2025/8/5.
//
#include <stdio.h>
#include <stdatomic.h>
#include "threads_c11.h"

#define THREAD_NUMS 32

atomic_int acnt;
int cnt;

int f(void* thr_data)
{
    for(int n = 0; n < 1000; ++n) {
        atomic_fetch_add_explicit(&acnt, 1, memory_order_relaxed); // atomic
        ++cnt; // undefined behavior, in practice some updates missed
    }
    return 0;
}

int main(int argc, char** argv)
{
    thrd_t thr[THREAD_NUMS];
    for(int n = 0; n < THREAD_NUMS; ++n)
        thrd_create(&thr[n], f, NULL);
    for(int n = 0; n < THREAD_NUMS; ++n)
        thrd_join(thr[n], NULL);

    printf("The atomic counter is %u\n", acnt);
    printf("The non-atomic counter is %u\n", cnt);

    return 0;
}