#ifndef CONCURRENT_STACK_TYPE
    #error There must be a provided type for CONCURRENT_STACK_TYPE and it must be defined prior to using this code!
    #define CONCURRENT_STACK_TYPE int32_t
#endif

#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include "include/concurrent_stack.h"

#ifndef BACKOFF_MAX_YIELDS
    #define BACKOFF_MAX_YIELDS 8
#endif

#define MAKE_NODE_NAME(x) concurrent_stack_node_ ## x
#define NODE_NAME(x) MAKE_NODE_NAME(x)
#define NODE NODE_NAME(CONCURRENT_STACK_TYPE)

#define MAKE_CONCURRENT_STACK_NAME(x) concurrent_stack_ ## x
#define CONCURRENT_STACK_NAME(x) MAKE_CONCURRENT_STACK_NAME(x)
#define CONCURRENT_STACK CONCURRENT_STACK_NAME(CONCURRENT_STACK_TYPE)

// Node implementation
typedef struct {
    CONCURRENT_STACK_TYPE m_value;
    NODE* m_next;
} NODE;

// Concurrent Stack Implementation
typedef struct  {
    NODE* m_head;
} CONCURRENT_STACK;


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
    __atomic_exchange(&this->m_head, (volatile void**)&nullPtr, &original, __ATOMIC_SEQ_CST);
    if (original == NULL)
        return false;
    for (NODE* current = original; current != NULL; current = current->m_next)
    {
        free(current);
    }
    return false;
}

#define MAKE_CONCURRENT_STACK_PUSH_NAME(x, y) x ## _push(x* this, y item)
#define GEN_PUSH_NAME(x, y) MAKE_CONCURRENT_STACK_PUSH_NAME(x, y)
bool GEN_PUSH_NAME(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    if (this == NULL) return true;
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if (!__atomic_compare_exchange_n(&this->m_head, &newNode->m_next, newNode, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {
        atomic_signal_fence(memory_order_acq_rel);
    }
    return false;
}

#define MAKE_CONCURRENT_STACK_PUSHRANGE_NAME(x, y) x ## _pushrange(x* this, y* items, INDEX_TYPE length)
#define GEN_PUSHRANGE_NAME(x, y) MAKE_CONCURRENT_STACK_PUSHRANGE_NAME(x, y)
bool GEN_PUSHRANGE_NAME(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    if (this == NULL || length <= 0) return true;
    NODE* head = (NODE*)malloc(sizeof(NODE));
    NODE* tail = head;
    head->m_value = items[0];
    for (INDEX_TYPE i = 1; i < length; ++i)
    {
        NODE* node = (NODE*)malloc(sizeof(NODE));
        node->m_next = head;
        node->m_value = items[i];
        head = node;
    }
    tail->m_next = this->m_head;

    while (!__atomic_compare_exchange_n(&this->m_head, &tail->m_next, head, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {
        atomic_signal_fence(memory_order_acq_rel);
    }
    return false;
}

#define MAKE_CONCURRENT_STACK_TRYPEEK_NAME(x, y) x ## _trypeek(x* this, y* result)
#define GEN_TRYPEEK_NAME(x, y) MAKE_CONCURRENT_STACK_TRYPEEK_NAME(x, y)
bool GEN_TRYPEEK_NAME(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    if (this == NULL) return true;
    NODE node = {};
    __atomic_load(this->m_head, &node, __ATOMIC_SEQ_CST);
    *result = node.m_value;
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
    while (!__atomic_compare_exchange_n(&this->m_head, head, &head->m_next, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {
        atomic_signal_fence(memory_order_acq_rel);
    }
    *result = head->m_value;
    return false;
}


#define MAKE_CONCURRENT_STACK_TRYPOPRANGE_NAME(x, y) x ## _trypoprange(x* this, y* result, INDEX_TYPE* result_length, INDEX_TYPE count)
#define GEN_TRYPOPRANGE_NAME(x, y) MAKE_CONCURRENT_STACK_TRYPOPRANGE_NAME(x, y)
bool GEN_TRYPOPRANGE_NAME(CONCURRENT_STACK, CONCURRENT_STACK_TYPE)
{
    if (this == NULL || count <= 0) return true;

    NODE* poppedHead = NULL;
    NODE* next = NULL;
    __atomic_load(&this->m_head, &poppedHead, __ATOMIC_SEQ_CST);
    if (poppedHead == NULL)
    {
        return true;
    }
    if (!__atomic_compare_exchange_n(&this->m_head, poppedHead, &poppedHead->m_next, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {
        return true;
    }
    INDEX_TYPE nodesCount = 1;
    for (; nodesCount < count && next != NULL; ++nodesCount)
    {
        next = next->m_next;
    }
    if (!__atomic_compare_exchange_n(&this->m_head, poppedHead, &next, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {
        return true;
    }
    result[0] = poppedHead->m_value;
    poppedHead = poppedHead->m_next;
    for (INDEX_TYPE i = 1; i < nodesCount; ++i)
    {
        result[i] = poppedHead->m_value;
        poppedHead = poppedHead->m_next;
    }
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
#undef GEN_NEW_NAME2
#undef MAKE_CONCURRENT_STACK_NEW_NAME2
#undef GEN_NEW_NAME
#undef MAKE_CONCURRENT_STACK_NEW_NAME

#undef MAKE_CONCURRENT_STACK_NAME
#undef CONCURRENT_STACK_NAME
#undef CONCURRENT_STACK

#undef DEFAULT_SEGMENT_SIZE
#undef INDEX_TYPE
#undef CONCURRENT_STACK_TYPE