#include "stdint.h"
#define CONCURRENT_STACK_TYPE int32_t
#include "src/concurrent_stack_impl.c"

typedef struct {
    int a;
    uint32_t b;
    double c;
} TestStruct;

#define CONCURRENT_STACK_TYPE TestStruct
#include "src/concurrent_stack_impl.c"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int Test_Push_And_Pop()
{
    concurrent_stack_int32_t* stack = concurrent_stack_int32_t_new();
    int32_t result = 0;
    if (!concurrent_queue_int32_t_isempty(queue)) return 1;
    if (concurrent_queue_int32_t_getcount(queue) != 0) return 2;
    if (concurrent_queue_int32_t_trypeek(queue, &result)) return 3;
    if (concurrent_queue_int32_t_trydequeue(queue, &result)) return 4;
    if (concurrent_queue_int32_t_enqueue(queue, 1)) return 5;
    if (concurrent_queue_int32_t_isempty(queue)) return 6;
    if (concurrent_queue_int32_t_getcount(queue) != 1) return 7;
    for (int i = 2; i <= 1024; ++i)
    {
        if (concurrent_queue_int32_t_enqueue(queue, i)) return 8;
        if (concurrent_queue_int32_t_getcount(queue) != i) return 9;
    }

    result = 0;
    for (int i = 1; i <= 1024; ++i)
    {
        result = 0;
        if (concurrent_queue_int32_t_trypeek(queue, &result)) return 10;
        if (result != i) return 11;
        if (concurrent_queue_int32_t_getcount(queue) != 1024 - (i - 1)) return 12;
        result = 0;
        if (concurrent_queue_int32_t_trydequeue(queue, &result)) return 13;
        if (result != i) return 14;
        if (concurrent_queue_int32_t_getcount(queue) != 1024 - i) return 15;
    }

    concurrent_queue_int32_t_delete(queue);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc > 0)
    {
        for (int i = 0; i < argc; ++i)
        {
            if (strcmp(argv[i], "test_enqueue_and_dequeue") == 0)
            {
                return Test_Enqueue_And_Dequeue();
            }
        }
    }
    else
    {
        printf("Please specify which test to run.\n");
    }
    return 0;
}