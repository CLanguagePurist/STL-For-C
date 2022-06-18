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
        }
    }
    else
    {
        printf("Please specify which test to run.\n");
    }
    return 0;
}