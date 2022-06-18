#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

#ifndef STL_FOR_C_IMPLEMENTATION_ONLY
    #define STL_FOR_C_IMPLEMENTATION_ONLY
#endif
#include "include/concurrent_stack.h"

#define MAKE_NODE_NAME(x) concurrent_stack_node_ ## x
#define NODE_NAME(x) MAKE_NODE_NAME(x)
#define NODE NODE_NAME(CONCURRENT_STACK_TYPE)

#define MAKE_CONCURRENT_STACK_NAME(x) concurrent_stack_ ## x
#define CONCURRENT_STACK_NAME(x) MAKE_CONCURRENT_STACK_NAME(x)
#define CONCURRENT_STACK CONCURRENT_STACK_NAME(CONCURRENT_STACK_TYPE)

#define MAKE_CONCURRENT_STACK_NEW_NAME(x) x ## _new()
#define GEN_NEW_NAME(x) MAKE_CONCURRENT_STACK_NEW_NAME(x)
CONCURRENT_STACK* GEN_NEW_NAME(CONCURRENT_STACK)
{
    return (CONCURRENT_STACK*)calloc(sizeof(CONCURRENT_STACK), 1);
}

#define MAKE_CONCURRENT_STACK_NEW_NAME2(x, y) x ## _new2(y* data, INDEX_TYPE length)
#define GEN_NEW_NAME2(x, y) MAKE_CONCURRENT_STACK_NEW_NAME2(x, y)
CONCURRENT_STACK* GEN_NEW_NAME2(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    CONCURRENT_STACK* stack = (CONCURRENT_STACK*)malloc(sizeof(CONCURRENT_STACK));
    NODE* lastNode = NULL;
    for (INDEX_TYPE i = 0; i < length; ++i)
    {
        NODE* newNode = (NODE*)malloc(sizeof(NODE));
        newNode->m_value = data[i];
        newNode->m_next = lastNode;
        lastNode = newNode;
    }

    stack->m_head = lastNode;
    return stack;
}

#define MAKE_CONCURRENT_STACK_DESTROY_NAME(x) x ## _destroy(x* this)
#define GEN_DESTROY_NAME(x) MAKE_CONCURRENT_STACK_DESTROY_NAME(x)
bool GEN_DESTROY_NAME(CONCURRENT_STACK)
{
    for (NODE* ptr = this->m_head; ptr != NULL;)
    {
        NODE* next = (NODE*)ptr->m_next;
        free(ptr);
        ptr = next;
    }
    free(this);
    return false;
}

#define MAKE_CONCURRENT_STACK_ISEMPTY_NAME(x) x ## _isempty(x* this)
#define GEN_ISEMPTY_NAME(x) MAKE_CONCURRENT_STACK_ISEMPTY_NAME(x)
bool GEN_ISEMPTY_NAME(CONCURRENT_STACK)
{
    if (this == NULL) return true;
    return this->m_head == NULL;
}

#define MAKE_CONCURRENT_STACK_COUNT_NAME(x) x ## _count(x* this)
#define GEN_COUNT_NAME(x) MAKE_CONCURRENT_STACK_COUNT_NAME(x)
INDEX_TYPE GEN_COUNT_NAME(CONCURRENT_STACK)
{
    if (this == NULL) return 0;
    INDEX_TYPE count = 0;
    for (NODE* current = this->m_head; current != NULL; current = current->m_next)
    {
        ++count;
    }
    return count;
}

#define MAKE_CONCURRENT_STACK_CLEAR_NAME(x) x ## _clear(x* this)
#define GEN_CLEAR_NAME(x) MAKE_CONCURRENT_STACK_CLEAR_NAME(x)
bool GEN_CLEAR_NAME(CONCURRENT_STACK)
{
    if (this == NULL) return true;
    void* nullPtr = NULL;
    NODE* original = NULL;
    original = atomic_exchange(&this->m_head, nullPtr);
    if (original == NULL)
        return false;
    for (NODE* current = original; current != NULL;)
    {
        NODE* next = current->m_next;
        free(current);
        current = next;
    }
    return false;
}

#define MAKE_CONCURRENT_STACK_PUSH_NAME(x, y) x ## _push(x* this, y item)
#define GEN_PUSH_NAME(x, y) MAKE_CONCURRENT_STACK_PUSH_NAME(x, y)
bool GEN_PUSH_NAME(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    if (this == NULL) return true;
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    newNode->m_next = atomic_load(&this->m_head);
    newNode->m_value = item;
    if (!atomic_compare_exchange_strong(&this->m_head, (NODE**) &newNode->m_next, newNode))
    {
        free(newNode);
        return true;
    }
    return false;
}

#define MAKE_CONCURRENT_STACK_PUSHRANGE_NAME(x, y) x ## _pushrange(x* this, y* items, INDEX_TYPE length)
#define GEN_PUSHRANGE_NAME(x, y) MAKE_CONCURRENT_STACK_PUSHRANGE_NAME(x, y)
bool GEN_PUSHRANGE_NAME(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    if (this == NULL || length <= 0) return true;
    NODE* head = (NODE*)calloc(sizeof(NODE), 1);
    NODE* curr = head;
    head->m_value = items[length - 1];
    for (INDEX_TYPE i = length - 2; i > -1; --i)
    {
        curr->m_next = calloc(sizeof(NODE), 1);
        curr = curr->m_next;
        curr->m_value = items[i];
    }
    curr->m_next = this->m_head;

    if (!atomic_compare_exchange_strong(&this->m_head, (NODE**)&curr->m_next, head))
    {
        for (curr = head; curr != NULL;)
        {
            NODE* next = curr->m_next;
            free(curr);
            curr = next;
        }
        return true;
    }
    return false;
}

#define MAKE_CONCURRENT_STACK_TRYPEEK_NAME(x, y) x ## _trypeek(x* this, y* result)
#define GEN_TRYPEEK_NAME(x, y) MAKE_CONCURRENT_STACK_TRYPEEK_NAME(x, y)
bool GEN_TRYPEEK_NAME(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    if (this == NULL) return true;
    NODE* node = (NODE*) atomic_load(&this->m_head);
    *result = node->m_value;
    return false;
}

#define MAKE_CONCURRENT_STACK_TRYPOP_NAME(x, y) x ## _trypop(x* this, y* result)
#define GEN_TRYPOP_NAME(x, y) MAKE_CONCURRENT_STACK_TRYPOP_NAME(x, y)
bool GEN_TRYPOP_NAME(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    if (this == NULL) return true;
    NODE* head = this->m_head;
    if (head == NULL)
    {
        return true;
    }
    if (!atomic_compare_exchange_strong(&this->m_head, &head, (NODE*)head->m_next))
    {
        return true;
    }
    *result = head->m_value;
    return false;
}


#define MAKE_CONCURRENT_STACK_TRYPOPRANGE_NAME(x, y) x ## _trypoprange(x* this, y* result, INDEX_TYPE* result_length, INDEX_TYPE count)
#define GEN_TRYPOPRANGE_NAME(x, y) MAKE_CONCURRENT_STACK_TRYPOPRANGE_NAME(x, y)
bool GEN_TRYPOPRANGE_NAME(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    if (this == NULL || count <= 0) return true;

    NODE* head = atomic_load(&this->m_head);
    NODE* tail = head;
    INDEX_TYPE actualCount = 0;
    if (head == NULL)
    {
        return true;
    }
    for (INDEX_TYPE i = 0; tail->m_next != NULL && i < count; ++i)
    {
        tail = tail->m_next;
        ++actualCount;
    }
    if (!atomic_compare_exchange_strong(&this->m_head, &head, tail))
    {
        return true;
    }
    INDEX_TYPE index = 0;
    NODE* curr = head;
    for (; curr != tail; curr = curr->m_next)
    {
        result[index++] = curr->m_value;
    }
    result[index] = curr->m_value;
    *result_length = actualCount;
    return false;
}

#undef GEN_TRYPOPRANGE_NAME
#undef MAKE_CONCURRENT_STACK_TRYPOPRANGE_NAME
#undef GEN_TRYPOP_NAME
#undef MAKE_CONCURRENT_STACK_TRYPOP_NAME
#undef GEN_TRYPEEK_NAME
#undef MAKE_CONCURRENT_STACK_TRYPEEK_NAME
#undef GEN_PUSHRANGE_NAME
#undef MAKE_CONCURRENT_STACK_PUSHRANGE_NAME
#undef GEN_PUSH_NAME
#undef MAKE_CONCURRENT_STACK_PUSH_NAME
#undef GEN_CLEAR_NAME
#undef MAKE_CONCURRENT_STACK_CLEAR_NAME
#undef GEN_COUNT_NAME
#undef MAKE_CONCURRENT_STACK_COUNT_NAME
#undef GEN_ISEMPTY_NAME
#undef MAKE_CONCURRENT_STACK_ISEMPTY_NAME
#undef GEN_DESTROY_NAME
#undef MAKE_CONCURRENT_STACK_DESTROY_NAME
#undef GEN_NEW_NAME2
#undef MAKE_CONCURRENT_STACK_NEW_NAME2
#undef GEN_NEW_NAME
#undef MAKE_CONCURRENT_STACK_NEW_NAME

#undef MAKE_NODE_NAME
#undef NODE_NAME
#undef NODE
#undef MAKE_CONCURRENT_STACK_NAME
#undef CONCURRENT_STACK_NAME
#undef CONCURRENT_STACK

#undef CONCURRENT_STACK_TYPE