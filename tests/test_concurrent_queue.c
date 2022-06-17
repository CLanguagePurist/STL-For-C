#include "stdint.h"
#define CONCURRENT_QUEUE_TYPE int32_t
#include "src/concurrent_queue_impl.c"

typedef struct
{
    int a;
    uint32_t b;
    double c;
} TestStruct;

#define CONCURRENT_QUEUE_TYPE TestStruct
#include "src/concurrent_queue_impl.c"

#include "tinycthread.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int Test_Enqueue_And_Dequeue()
{
    concurrent_queue_int32_t *queue = concurrent_queue_int32_t_new();
    int32_t result = 0;
    if (!concurrent_queue_int32_t_isempty(queue))
        return 1;
    if (concurrent_queue_int32_t_getcount(queue) != 0)
        return 2;
    if (concurrent_queue_int32_t_trypeek(queue, &result))
        return 3;
    if (concurrent_queue_int32_t_trydequeue(queue, &result))
        return 4;
    if (concurrent_queue_int32_t_enqueue(queue, 1))
        return 5;
    if (concurrent_queue_int32_t_isempty(queue))
        return 6;
    if (concurrent_queue_int32_t_getcount(queue) != 1)
        return 7;
    for (int i = 2; i <= 1024; ++i)
    {
        if (concurrent_queue_int32_t_enqueue(queue, i))
            return 8;
        if (concurrent_queue_int32_t_getcount(queue) != i)
            return 9;
    }

    result = 0;
    for (int i = 1; i <= 1024; ++i)
    {
        result = 0;
        if (concurrent_queue_int32_t_trypeek(queue, &result))
            return 10;
        if (result != i)
            return 11;
        if (concurrent_queue_int32_t_getcount(queue) != 1024 - (i - 1))
            return 12;
        result = 0;
        if (concurrent_queue_int32_t_trydequeue(queue, &result))
            return 13;
        if (result != i)
            return 14;
        if (concurrent_queue_int32_t_getcount(queue) != 1024 - i)
            return 15;
    }

    concurrent_queue_int32_t_delete(queue);
    return 0;
}

static concurrent_queue_int32_t *global_queue;
static volatile int32_t completedCount = 0;

int enqueue_to_global_queue(void *arg)
{
    int32_t index = ((int32_t *)arg)[0];
    int32_t length = ((int32_t *)arg)[1];
    index *= length;
    int32_t endIndex = index + length;
    for (int32_t i = index; i < endIndex; ++i)
    {
        concurrent_queue_int32_t_enqueue(global_queue, i);
    }
    ++completedCount;
    return 0;
}

int Test_Multithread_Enqueue()
{
    global_queue = concurrent_queue_int32_t_new();
    thrd_t *threads = (thrd_t *)malloc(sizeof(thrd_t) * 8);
    int32_t args[16];
    // Dispatch 8 Threads
    for (int32_t threadIndex = 0; threadIndex < 8; ++threadIndex)
    {
        args[threadIndex * 2] = threadIndex;
        args[threadIndex * 2 + 1] = 1048576;
        thrd_create(&threads[threadIndex], enqueue_to_global_queue, &args[threadIndex * 2]);
    }

    while (completedCount != 8)
    {
        thrd_yield();
    }

    int32_t *validator = (int32_t *)calloc(1048576 * 8, sizeof(int32_t));
    while (!concurrent_queue_int32_t_isempty(global_queue))
    {
        int32_t res = 0;
        if (concurrent_queue_int32_t_trydequeue(global_queue, &res))
        {
            return 1;
        }
        validator[res] = 1;
    }

    for (int32_t i = 0; i < 1048576 * 8; ++i)
    {
        if (validator[i] == 0)
        {
            return 2;
        }
    }
    for (int threadId = 0; threadId < 8; ++threadId)
    {
        thrd_detach(threads[threadId]);
    }
    free(validator);
    completedCount = 0;
    free(threads);
    return 0;
}

static int* dequeueResult;

int dequeue_from_global_queue(void *arg)
{
    int32_t length = ((int32_t *)arg)[0];
    for (int32_t i = 0; i < length; ++i)
    {
        int32_t res = 0;
        if (concurrent_queue_int32_t_trydequeue(global_queue, &res))
            return 1;
        dequeueResult[res] = 1;
    }
    ++completedCount;
    return 0;
}

int Test_Multithread_Dequeue()
{
    global_queue = concurrent_queue_int32_t_new();
    thrd_t *threads = (thrd_t *)malloc(sizeof(thrd_t) * 8);
    dequeueResult = (int32_t *)calloc(1048576 * 8, sizeof(int32_t));
    for (int32_t i = 0; i < 1048576 * 8; ++i)
    {
        if (concurrent_queue_int32_t_enqueue(global_queue, i)) return 1;
    }
    int32_t args = 1048576;
    // Dispatch 8 Threads
    for (int32_t threadIndex = 0; threadIndex < 8; ++threadIndex)
    {
        thrd_create(&threads[threadIndex], dequeue_from_global_queue, &args);
    }

    while (completedCount != 8)
    {
        thrd_yield();
    }

    for (int32_t i = 0; i < 1048576 * 8; ++i)
    {
        if (dequeueResult[i] == 0)
        {
            return 2;
        }
    }
    for (int threadId = 0; threadId < 8; ++threadId)
    {
        thrd_detach(threads[threadId]);
    }
    free(dequeueResult);
    completedCount = 0;
    free(threads);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc > 0)
    {
        for (int i = 0; i < argc; ++i)
        {
            if (strcmp(argv[i], "test_enqueue_and_dequeue") == 0)
            {
                return Test_Enqueue_And_Dequeue();
            }
            if (strcmp(argv[i], "test_multithread_enqueue") == 0)
            {
                return Test_Multithread_Enqueue();
            }
            if (strcmp(argv[i], "test_multithread_dequeue") == 0)
            {
                return Test_Multithread_Dequeue();
            }
        }
    }
    else
    {
        printf("Please specify which test to run.\n");
    }
    return 0;
}