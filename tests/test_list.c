#define LIST_TYPE int
#include "src/list_impl.c"
#include "stdint.h"

typedef struct {
    int a;
    uint32_t b;
    double c;
} TestStruct;

#define LIST_TYPE TestStruct
#include "src/list_impl.c"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int Test_AddItem()
{
    list_int* list = list_int_new();
    list_int_add(list, 3);
    if (list->items[0] != 3)
        return 1;

    list_int_add(list, 2);
    if (list->items[1] != 2)
        return 1;

    list_int_add(list, 1);
    if (list->items[2] != 1)
        return 1;
    if (list->capacity < list->size)
        return 1;
    list_int_destroy(list);
    return 0;
}

int Test_IndexOf1()
{
    list_int* list = list_int_new();
    list_int_add(list, 1);
    list_int_add(list, 2);
    list_int_add(list, 3);
    if (list_int_indexof(list, 1) != 0)
        return 1;
    if (list_int_indexof(list, 2) != 1)
        return 2;
    if (list_int_indexof(list, 3) != 2)
        return 3;
    list_int_destroy(list);
    return 0;
}

int Test_IndexOf2()
{
    list_int* list = list_int_new();
    list_int_add(list, 1);
    list_int_add(list, 2);
    list_int_add(list, 3);
    if (list_int_indexof2(list, 1, 1) != -1)
        return 1;
    if (list_int_indexof2(list, 1, 0) != 0)
        return 2;
    if (list_int_indexof2(list, 2, 1) != 1)
        return 3;
    if (list_int_indexof2(list, 2, 2) != -1)
        return 4;
    if (list_int_indexof2(list, 3, 2) != 2)
        return 5;
    if (list_int_indexof2(list, 3, 3) != -1)
        return 6;
    list_int_destroy(list);
    return 0;
}

int Test_IndexOf3()
{
    list_int* list = list_int_new();
    list_int_add(list, 1);
    list_int_add(list, 2);
    list_int_add(list, 3);
    if (list_int_indexof3(list, 1, 0, 0) != -1)
        return 1;
    if (list_int_indexof3(list, 1, 0, 4) != 0)
        return 2;
    if (list_int_indexof3(list, 1, 0, 1) != 0)
        return 3;
    if (list_int_indexof3(list, 1, 1, 1) != -1)
        return 4;
    
    if (list_int_indexof3(list, 2, 1, 1) != 1)
        return 5;
    if (list_int_indexof3(list, 2, 1, 0) != -1)
        return 6;
    if (list_int_indexof3(list, 2, 0, 1) != -1)
        return 7;
    if (list_int_indexof3(list, 2, 0, 2) != 1)
        return 8;
    if (list_int_indexof3(list, 2, 2, 2) != -1)
        return 9;

    if (list_int_indexof3(list, 3, 2, 1) != 2)
        return 10;
    if (list_int_indexof3(list, 3, 1, 1) != -1)
        return 11;
    if (list_int_indexof3(list, 3, 2, 0) != -1)
        return 12;
    if (list_int_indexof3(list, 3, 0, 3) != 2)
        return 13;
    if (list_int_indexof3(list, 3, 1, 2) != 2)
        return 14;
    list_int_destroy(list);
    return 0;
}

int Test_Get()
{
    list_int* list = list_int_new();
    list_int_add(list, 1);
    list_int_add(list, 2);
    list_int_add(list, 3);
    int result;
    if (!list_int_get(list, 0, &result)) return 1;
    if (result != 1) return 2;
    result = -1;
    if (!list_int_get(list, 1, &result)) return 3;
    if (result != 2) return 4;
    result = -1;
    if (!list_int_get(list, 2, &result)) return 5;
    if (result != 3) return 6;
    list_int_destroy(list);
    return 0;
}

int Test_GetRange()
{
    list_int* list = list_int_new_withcapacity(1024);
    for (int i = 0; i < 1024; ++i)
        list_int_add(list, i);
    int* result;
    if (!list_int_getrange(list, 0, 1024, &result))
        return 1;
    if (result == NULL)
        return 2;
    if (memcmp(result, list->items, list->size * sizeof(int)) != 0)
        return 3;
    free(result);
    if (!list_int_getrange(list, 1, 1, &result))
        return 4;
    if (result[0] != 1)
        return 5;
    free(result);
    list_int_destroy(list);
    return 0;
}

int Test_Reallocation()
{
    list_int* list = list_int_new_withcapacity(1024);
    for (int i = 0; i < 1024; ++i)
        list_int_add(list, i);
    if (list->capacity < list->size) return 1;
        for (int i = 0; i < 2048; ++i)
        list_int_add(list, i);
    if (list->capacity < list->size) return 2;
    for (int i = 0; i < 4096; ++i)
        list_int_add(list, i);
    if (list->capacity < list->size) return 3;
    list_int_destroy(list);
    return 0;
}

int Test_AddRange()
{
    list_int* list = list_int_new();
    int* sample = (int*)malloc(1024 * sizeof(int));
    for (int i = 0; i < 1024; ++i)
        sample[i] = i + 1;
    if (!list_int_addrange(list, &sample[512], 512)) return 1;
    if (list->items[0] != 513) return 2;
    if (list->items[511] != 1024) return 3;
    if (!list_int_addrange(list, sample, 512)) return 4;
    if (list->items[512] != 1) return 5;
    if (list->items[1023] != 512) return 6;
    list_int_destroy(list);
    free(sample);
    return 0;
}

int Test_AddRange_At()
{
    list_int* list = list_int_new();
    int* sample = (int*)malloc(1024 * sizeof(int));
    for (int i = 0; i < 1024; ++i)
        sample[i] = i;
    
    // 0 ... 512
    if (!list_int_addrange_at(list, 0, sample, 512)) return 1;
    if (list->items[0] != 0) return 2;
    if (list->items[511] != 511) return 3;
    // 0 .. 255, 512 .. 1023, 256 .. 511
    if (!list_int_addrange_at(list, 256, &sample[512], 512)) return 4;
    if (list->items[256] != 512) return 5;
    if (list->items[255] != 255) return 6;
    if (list->items[768] != 256) return 7;
    if (list->items[1023] != 511) return 8;
    free(sample);
    list_int_destroy(list);
    return 0;
}

int Test_Remove()
{
    list_int* list = list_int_new();
    list_int_add(list, 1);
    list_int_add(list, 2);
    list_int_add(list, 3);
    if (!list_int_remove(list, 0)) return 1;
    if (list->size != 2) return 2;
    if (list->items[0] != 2) return 3;
    if (list->items[1] != 3) return 4;
    if (!list_int_remove(list, 0)) return 5;
    if (list->items[0] != 3) return 6;
    if (list->size != 1) return 7;
    if (!list_int_remove(list, 0)) return 8;
    if (list->size != 0) return 9;
    if (!list_int_remove(list, 0)) return 10;
    list_int_destroy(list);
    return 0;
}

int Test_RemoveRange()
{
    list_int* list = list_int_new();
    // 0 .. 1023
    for (int i = 0; i < 1024; ++i)
        list_int_add(list, i);
    
    // 2 .. 1023
    if (!list_int_removerange(list, 0, 2)) return 1;
    if (list->items[0] != 2) return 2;
    if (list->size != 1022) return 3;

    // 2 .. 102, 104 .. 1023
    if (!list_int_removerange(list, 100, 2)) return 4;
    if (list->items[99] != 101) return 5;
    if (list->items[100] != 104) return 6;
    if (list->items[101] != 105) return 7;
    if (list->size != 1020) return 8;

    if (!list_int_removerange(list, 1019, 100)) return 9;
    if (list->size != 1019) return 10;

    list_int_destroy(list);
    return 0;
}

int Test_RemoveItem()
{
    list_int* list = list_int_new();
    list_int_add(list, 1);
    list_int_add(list, 2);
    list_int_add(list, 3);

    if (!list_int_remove_item(list, 2)) return 1;
    if (list->size != 2) return 2;
    if (list->items[0] != 1) return 3;
    if (list->items[1] != 3) return 4;

    if (!list_int_remove_item(list, 1)) return 5;
    if (list->size != 1) return 6;
    if (list->items[0] != 3) return 7;
    
    if (!list_int_remove_item(list, 3)) return 8;
    
    if (list->size != 0) return 9;
    if (list_int_remove_item(list, 0)) return 10;

    list_int_destroy(list);
    return 0;
}

int Test_StructList()
{
    list_TestStruct* list = list_TestStruct_new();
    TestStruct val = { .a = 1, .b = 2, .c = 3};
    if (!list_TestStruct_add(list, val)) return 1;
    val = (TestStruct) { .a = 2l, .b = 3u, .c = 1};
    if (!list_TestStruct_add(list, val)) return 2;
    val = (TestStruct) { .a = 3l, .b = 2u, .c = 1};
    if (!list_TestStruct_add(list, val)) return 3;
    if (list_TestStruct_indexof(list, val) != 2) return 4;

    list_TestStruct_destroy(list);
    return 0;
}

int Test_IncorrectData()
{
    list_int* list = list_int_new();
    int32_t output;
    if (list_int_get(list, 0, &output) == true) return 1;
    list_int_destroy(list);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc > 0)
    {
        for (int i = 0; i < argc; ++i)
        {
            if (strcmp(argv[i], "test_additem") == 0)
            {
                return Test_AddItem();
            }
            if (strcmp(argv[i], "test_indexof1") == 0)
            {
                return Test_IndexOf1();
            }
            if (strcmp(argv[i], "test_indexof2") == 0)
            {
                return Test_IndexOf2();
            }
            if (strcmp(argv[i], "test_indexof3") == 0)
            {
                return Test_IndexOf3();
            }
            if (strcmp(argv[i], "test_get") == 0)
            {
                return Test_Get();
            }
            if (strcmp(argv[i], "test_getrange") == 0)
            {
                return Test_GetRange();
            }
            if (strcmp(argv[i], "test_reallocation") == 0)
            {
                return Test_Reallocation();
            }
            if (strcmp(argv[i], "test_addrange") == 0)
            {
                return Test_AddRange();
            }
            if (strcmp(argv[i], "test_addrange_at") == 0)
            {
                return Test_AddRange_At();
            }
            if (strcmp(argv[i], "test_remove") == 0)
            {
                return Test_Remove();
            }
            if (strcmp(argv[i], "test_removerange") == 0)
            {
                return Test_RemoveRange();
            }
            if (strcmp(argv[i], "test_removeitem") == 0)
            {
                return Test_RemoveItem();
            }
            if (strcmp(argv[i], "test_structlist") == 0)
            {
                return Test_StructList();
            }
            if (strcmp(argv[i], "test_incorrectdata") == 0)
            {
                return Test_IncorrectData();
            }
        }
    }
    else
    {
        printf("Please specify which test to run.\n");
    }
    return 0;
}