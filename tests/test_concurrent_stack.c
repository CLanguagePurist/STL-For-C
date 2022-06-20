#include "stdint.h"
#define CONCURRENT_STACK_TYPE int32_t
#include "src/concurrent_stack_impl.c"

typedef struct {
    int a;
    uint32_t b;
    double c;
} TestStruct;

//#define CONCURRENT_STACK_TYPE TestStruct
//#include "src/concurrent_stack_impl.c"
#include "STL_CThread/source/tinycthread.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int Test_Push_And_Pop()
{
    concurrent_stack_int32_t* stack = concurrent_stack_int32_t_new();
    int32_t result = 0;

    for (int i = 0; i < 1024; ++i)
    {
        if (concurrent_stack_int32_t_push(stack, i)) return 1;
    }

    for (int i = 1023; i > -1; --i)
    {
        if (concurrent_stack_int32_t_trypop(stack, &result)) return 2;
        if (result != i) return 3;
    }

    concurrent_stack_int32_t_destroy(stack);
    return 0;
}

int Test_PushRange_And_PopRange()
{
    concurrent_stack_int32_t* stack = concurrent_stack_int32_t_new();
    int32_t input[1024];
    for (int i = 0; i < 1024; ++i)
        input[i] = i + 1;
    int32_t output[1024];
    int64_t resultLength = 0;
    concurrent_stack_int32_t_pushrange(stack, input, 1024);
    for (int i = 1024; i > 0; --i)
    {
        int32_t item = 0;
        concurrent_stack_int32_t_trypop(stack, &item);
        if (item != i) return 1;
    }
    concurrent_stack_int32_t_pushrange(stack, input, 1024);
    concurrent_stack_int32_t_trypoprange(stack, output, &resultLength, 1024);

    for (int i = 0; i < 1024; ++i)
    {
        if (output[i] != 1024 - i) return 2;
    }

    concurrent_stack_int32_t_destroy(stack);
    return 0;
}


static concurrent_stack_int32_t *global_stack;
static _Atomic int32_t completedCount = 0;

int push_to_global_stack(void *arg)
{
    int32_t index = ((int32_t *)arg)[0];
    int32_t length = ((int32_t *)arg)[1];
    index *= length;
    int32_t endIndex = index + length;
    for (int32_t i = index; i < endIndex; ++i)
    {
        if (i % 2 == 0)
        {
            if (concurrent_stack_int32_t_push(global_stack, i))
                return 1;
        }
        else
        {
            if (concurrent_stack_int32_t_pushrange(global_stack, &i, 1))
                return 1;
        }
    }
    atomic_fetch_add(&completedCount, 1);
    return 0;
}

int Test_Multithread_Push()
{
    global_stack = concurrent_stack_int32_t_new();
    thrd_t *threads = (thrd_t *)malloc(sizeof(thrd_t) * 8);
    int32_t args[16];
    // Dispatch 8 Threads
    for (int32_t threadIndex = 0; threadIndex < 8; ++threadIndex)
    {
        args[threadIndex * 2] = threadIndex;
        args[threadIndex * 2 + 1] = 1048576;
        thrd_create(&threads[threadIndex], push_to_global_stack, &args[threadIndex * 2]);
    }

    while (completedCount != 8)
    {
        thrd_yield();
    }

    if (concurrent_stack_int32_t_count(global_stack) != 8388608)
    {
        return 1;
    }

    int32_t *validator = (int32_t *)calloc(8388608, sizeof(int32_t));
    while (!concurrent_stack_int32_t_isempty(global_stack))
    {
        int32_t res = 0;
        if (concurrent_stack_int32_t_trypop(global_stack, &res))
        {
            return 2;
        }
        validator[res] = 1;
    }

    for (int32_t i = 8388607; i > -1; --i)
    {
        if (validator[i] == 0)
        {
            return 3;
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

static _Atomic int32_t* popResult;

int pop_from_global_stack(void *arg)
{
    int32_t length = ((int32_t *)arg)[0];
    int32_t i = 0;
    for (; i < length; ++i)
    {
        int32_t res = 0;
        if (i % 2 == 0)
        {
            if (concurrent_stack_int32_t_trypop(global_stack, &res))
            {
                return 1;
            }
        }
        else
        {
            int64_t resCount = 0;
            if (concurrent_stack_int32_t_trypoprange(global_stack, &res, &resCount, 1))
            {
                return 1;
            }
            if (resCount != 1)
            {
                return 1;
            }
        }
        popResult[res] = 1;
    }
    atomic_fetch_add(&completedCount, 1);
    return 0;
}

int Test_Multithread_Pop()
{
    global_stack = concurrent_stack_int32_t_new();
    thrd_t *threads = (thrd_t *)malloc(sizeof(thrd_t) * 8);
    popResult = (_Atomic(int32_t) *)calloc(8388608, sizeof(int32_t));
    for (int32_t i = 0; i < 8388608; ++i)
    {
        if (concurrent_stack_int32_t_push(global_stack, i)) return 1;
    }
    int32_t* args = (int32_t*)malloc(sizeof(int32_t));
    *args = 1048576;
    // Dispatch 8 Threads
    for (int32_t threadIndex = 0; threadIndex < 8; ++threadIndex)
    {
        thrd_create(&threads[threadIndex], pop_from_global_stack, args);
    }

    while (completedCount != 8)
    {
        thrd_yield();
    }
    free(args);
    
    for (int32_t i = 8388607; i > -1; --i)
    {
        if (popResult[i] == 0)
        {
            return 2;
        }
    }
    for (int threadId = 0; threadId < 8; ++threadId)
    {
        thrd_detach(threads[threadId]);
    }
    free(popResult);
    completedCount = 0;
    free(threads);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc > 0)
    {
        for (int i = 0; i < argc; ++i)
        {
            if (strcmp(argv[i], "test_push_and_pop") == 0)
            {
                return Test_Push_And_Pop();
            }
            if (strcmp(argv[i], "test_pushrange_and_poprange") == 0)
            {
                return Test_PushRange_And_PopRange();
            }
            if (strcmp(argv[i], "test_multithread_push") == 0)
            {
                return Test_Multithread_Push();
            }
            if (strcmp(argv[i], "test_multithread_pop") == 0)
            {
                return Test_Multithread_Pop();
            }
        }
    }
    else
    {
        printf("Please specify which test to run.\n");
    }
    return 0;
}