#ifndef CONCURRENT_QUEUE_TYPE
    #error There must be a provided type for CONCURRENT_QUEUE_TYPE and it must be defined prior to using this code!
    #define CONCURRENT_QUEUE_TYPE int32_t
#endif

#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include "include/concurrent_queue.h"

#ifndef DEFAULT_SEGMENT_SIZE
    #define DEFAULT_SEGMENT_SIZE 32
#endif

#define MAKE_SEGMENT_NAME(x) concurrent_queue_segment_ ## x
#define SEGMENT_NAME(x) MAKE_SEGMENT_NAME(x)
#define SEGMENT SEGMENT_NAME(CONCURRENT_QUEUE_TYPE)

#define MAKE_CONCURRENT_QUEUE_NAME(x) concurrent_queue_ ## x
#define CONCURRENT_QUEUE_NAME(x) MAKE_CONCURRENT_QUEUE_NAME(x)
#define CONCURRENT_QUEUE CONCURRENT_QUEUE_NAME(CONCURRENT_QUEUE_TYPE)

// Segment Implementation

#define MAKE_SEGMENT_NEW_NAME(x, y) x ## _new(INDEX_TYPE index, y* source)
#define GEN_NEW_NAME(x, y) MAKE_SEGMENT_NEW_NAME(x, y)
// segment_x* segment_x_new(INDEX_TYPE index, concurrentqueue_x* source)
SEGMENT* GEN_NEW_NAME(SEGMENT, CONCURRENT_QUEUE)
{
    SEGMENT* output = (SEGMENT*)malloc(sizeof(SEGMENT));
    output->m_array = (CONCURRENT_QUEUE_TYPE*)calloc(DEFAULT_SEGMENT_SIZE, sizeof(CONCURRENT_QUEUE_TYPE));
    output->m_state = (bool*)calloc(DEFAULT_SEGMENT_SIZE, sizeof(bool));
    output->m_high = -1;
    output->m_low = 0;
    output->m_index = index;
    output->m_source = source;
    output->m_next = NULL;
    return output;
}
#undef MAKE_SEGMENT_NEW_NAME
#undef GEN_NEW_NAME

#define MAKE_SEGMENT_DELETE_NAME(x) x ## _delete(x* this)
#define GEN_DELETE_NAME(x) MAKE_SEGMENT_DELETE_NAME(x)
// void segment_x_next(segment_x* this)
void GEN_DELETE_NAME(SEGMENT)
{
    if (this == NULL) return;
    free((void*)this->m_array);
    free((void*)this->m_state);
    free(this);
}
#undef MAKE_SEGMENT_DELETE_NAME
#undef GEN_DELETE_NAME

#define MAKE_SEGMENT_NEXT_NAME(x) x ## _next(x* this)
#define GEN_NEXT_NAME(x) MAKE_SEGMENT_NEXT_NAME(x)
// segment_x* segment_x_next(segment_x* this)
SEGMENT* GEN_NEXT_NAME(SEGMENT)
{
    return (SEGMENT*)this->m_next;
}
#undef MAKE_SEGMENT_NEXT_NAME
#undef GEN_NEXT_NAME

#define MAKE_SEGMENT_ISEMPTY_NAME(x) x ## _isempty(x* this)
#define GEN_ISEMPTY_NAME(x) MAKE_SEGMENT_ISEMPTY_NAME(x)
// bool segment_x_isempty(segment_x* this)
bool GEN_ISEMPTY_NAME(SEGMENT)
{
    #define MAKE_SEGMENT_LOW_NAME(x) x ## _low
    #define GEN_LOW_NAME(x) MAKE_SEGMENT_LOW_NAME(x)
    #define MAKE_SEGMENT_HIGH_NAME(x) x ## _high
    #define GEN_HIGH_NAME(x) MAKE_SEGMENT_HIGH_NAME(x)
    if (this == NULL) return true;
    return (GEN_LOW_NAME(SEGMENT)(this) > GEN_HIGH_NAME(SEGMENT)(this));

    #undef MAKE_SEGMENT_LOW_NAME
    #undef GEN_LOW_NAME
    #undef MAKE_SEGMENT_HIGH_NAME
    #undef GEN_HIGH_NAME
}
#undef MAKE_SEGMENT_ISEMPTY_NAME
#undef GEN_ISEMPTY_NAME

#define MAKE_SEGMENT_UNSAFE_ADD_NAME(x, y) x ## _unsafe_add(x* this, y value)
#define GEN_UNSAFE_ADD_NAME(x, y) MAKE_SEGMENT_UNSAFE_ADD_NAME(x, y)
// void segment_x_unsafe_add(segment_x* this, x value)
void GEN_UNSAFE_ADD_NAME(SEGMENT, CONCURRENT_QUEUE_TYPE)
{
    if (this == NULL) return;
    __atomic_fetch_add(&this->m_high, 1, __ATOMIC_SEQ_CST);
    this->m_array[this->m_high] = value;
    this->m_state[this->m_high] = true;
}
#undef MAKE_SEGMENT_UNSAFE_ADD_NAME
#undef GEN_UNSAFE_ADD_NAME

#define MAKE_SEGMENT_UNSAFE_GROW_NAME(x) x ## _unsafe_grow(x* this)
#define GEN_UNSAFE_GROW_NAME(x) MAKE_SEGMENT_UNSAFE_GROW_NAME(x)
// segment_x* segment_x_unsafe_grow(segment_x* this)
SEGMENT* GEN_UNSAFE_GROW_NAME(SEGMENT)
{
    #define MAKE_SEGMENT_NEW_NAME(x) x ## _new
    #define GEN_SEGMENT_NEW_NAME(x) MAKE_SEGMENT_NEW_NAME(x)
    if (this == NULL) return NULL;
    SEGMENT* newSegment = GEN_SEGMENT_NEW_NAME(SEGMENT)(this->m_index + 1, (CONCURRENT_QUEUE*) this->m_source);
    this->m_next = newSegment;
    return newSegment;

    #undef MAKE_SEGMENT_NEW_NAME
    #undef GEN_SEGMENT_NEW_NAME
}
#undef MAKE_SEGMENT_UNSAFE_GROW_NAME
#undef GEN_UNSAFE_GROW_NAME

#define MAKE_SEGMENT_GROW_NAME(x) x ## _grow(x* this)
#define GEN_GROW_NAME(x) MAKE_SEGMENT_GROW_NAME(x)
// void segment_x_grow(segment_x* this)
void GEN_GROW_NAME(SEGMENT)
{
    #define MAKE_SEGMENT_NEW_NAME(x) x ## _new
    #define GEN_NEW_NAME(x) MAKE_SEGMENT_NEW_NAME(x)
    if (this == NULL) return;
    SEGMENT* newSegment = GEN_NEW_NAME(SEGMENT)(this->m_index + 1, (CONCURRENT_QUEUE*) this->m_source);
    this->m_next = newSegment;
    ((CONCURRENT_QUEUE*)this->m_source)->m_tail = (SEGMENT*)this->m_next;

    #undef MAKE_SEGMENT_NEW_NAME
    #undef GEN_NEW_NAME
}
#undef MAKE_SEGMENT_GROW_NAME
#undef GEN_GROW_NAME

#define MAKE_SEGMENT_TRYAPPEND_NAME(x, y) x ## _tryappend(x* this, y value)
#define GEN_TRYAPPEND_NAME(x, y) MAKE_SEGMENT_TRYAPPEND_NAME(x, y)
// void segment_x_tryappend(segment_x* this, x value)
bool GEN_TRYAPPEND_NAME(SEGMENT, CONCURRENT_QUEUE_TYPE)
{
    if (this == NULL) return false;
    if (this->m_high >= DEFAULT_SEGMENT_SIZE - 1)
    {
        return false;
    }

    long newHigh = DEFAULT_SEGMENT_SIZE;
    INDEX_TYPE a = __atomic_fetch_add(&this->m_high, 1, __ATOMIC_SEQ_CST);
    newHigh = a + 1;
    if (newHigh <= DEFAULT_SEGMENT_SIZE - 1)
    {
        this->m_array[newHigh] = value;
        this->m_state[newHigh] = true;
    }

    if (newHigh == DEFAULT_SEGMENT_SIZE - 1)
    {
        #define MAKE_SEGMENT_GROW_NAME(x) x ## _grow
        #define GEN_GROW_NAME(x) MAKE_SEGMENT_GROW_NAME(x)

        GEN_GROW_NAME(SEGMENT)(this);

        #undef MAKE_SEGMENT_GROW_NAME
        #undef GEN_GROW_NAME
    }
    return newHigh <= DEFAULT_SEGMENT_SIZE - 1;
}
#undef MAKE_SEGMENT_TRYAPPEND_NAME
#undef GEN_TRYAPPEND_NAME

#define MAKE_SEGMENT_TRYREMOVE_NAME(x, y) x ## _tryremove(x* this, y *result)
#define GEN_TRYREMOVE_NAME(x, y) MAKE_SEGMENT_TRYREMOVE_NAME(x, y)
// void segment_x_tryappend(segment_x* this, x* result)
bool GEN_TRYREMOVE_NAME(SEGMENT, CONCURRENT_QUEUE_TYPE)
{
    #define MAKE_SEGMENT_LOW_NAME(x) x ## _low
    #define GEN_LOW_NAME(x) MAKE_SEGMENT_LOW_NAME(x)
    #define MAKE_SEGMENT_HIGH_NAME(x) x ## _high
    #define GEN_HIGH_NAME(x) MAKE_SEGMENT_HIGH_NAME(x)
    if (this == NULL) return false;
    INDEX_TYPE lowLocal = GEN_LOW_NAME(SEGMENT)(this);
    INDEX_TYPE highLocal = GEN_HIGH_NAME(SEGMENT)(this);
    while (lowLocal <= highLocal)
    {
        if (__atomic_compare_exchange_n (&this->m_low,
						       &lowLocal,
						       lowLocal + 1,
						       0,
						       __ATOMIC_SEQ_CST,
						       __ATOMIC_SEQ_CST))
        {
            while (this->m_state[lowLocal] == false)
            {
                atomic_signal_fence(memory_order_acq_rel);
            }

            *result = this->m_array[lowLocal];

            if (((CONCURRENT_QUEUE*)this->m_source)->m_numSnapshotTakers <= 0)
            {
                #ifdef CONCURRENT_QUEUE_ZERO_OUT_RESULT
                    memset((void*)&this->m_array[lowLocal], 0, sizeof(CONCURRENT_QUEUE_TYPE));
                #endif
            }

            if (lowLocal + 1 >= DEFAULT_SEGMENT_SIZE)
            {
                while (this->m_next == NULL)
                {
                    atomic_signal_fence(memory_order_acq_rel);
                }
                ((CONCURRENT_QUEUE*)this->m_source)->m_head = (SEGMENT*)this->m_next;
            }
            return true;
        }
        else
        {
            atomic_signal_fence(memory_order_acq_rel);

            lowLocal = GEN_LOW_NAME(SEGMENT)(this);
            highLocal = GEN_HIGH_NAME(SEGMENT)(this);
        }
    }
    #ifdef CONCURRENT_QUEUE_ZERO_OUT_RESULT
        memset(result, 0, sizeof(CONCURRENT_QUEUE_TYPE));
    #endif
    return false;

    #undef MAKE_SEGMENT_LOW_NAME
    #undef GEN_LOW_NAME
    #undef MAKE_SEGMENT_HIGH_NAME
    #undef GEN_HIGH_NAME
}
#undef MAKE_SEGMENT_TRYREMOVE_NAME
#undef GEN_TRYREMOVE_NAME

#define MAKE_SEGMENT_TRYPEEK_NAME(x, y) x ## _trypeek(x* this, y *result)
#define GEN_TRYPEEK_NAME(x, y) MAKE_SEGMENT_TRYPEEK_NAME(x, y)
// void segment_x_trypeek(segment_x* this, x value)
bool GEN_TRYPEEK_NAME(SEGMENT, CONCURRENT_QUEUE_TYPE)
{
    #define MAKE_SEGMENT_LOW_NAME(x) x ## _low
    #define GEN_LOW_NAME(x) MAKE_SEGMENT_LOW_NAME(x)
    #define MAKE_SEGMENT_HIGH_NAME(x) x ## _high
    #define GEN_HIGH_NAME(x) MAKE_SEGMENT_HIGH_NAME(x)
    if (this == NULL) return false;
    #ifdef CONCURRENT_QUEUE_ZERO_OUT_RESULT
        memset(result, 0, sizeof(CONCURRENT_QUEUE_TYPE));
    #endif
    INDEX_TYPE lowLocal = GEN_LOW_NAME(SEGMENT)(this);
    if (lowLocal > GEN_HIGH_NAME(SEGMENT)(this))
    {
        return false;
    }
    while (this->m_state[lowLocal] == false)
    {
        atomic_signal_fence(memory_order_acq_rel);
    }

    *result = (CONCURRENT_QUEUE_TYPE) this->m_array[lowLocal];
    return true;
    
    #undef MAKE_SEGMENT_LOW_NAME
    #undef GEN_LOW_NAME
    #undef MAKE_SEGMENT_HIGH_NAME
    #undef GEN_HIGH_NAME
}
#undef MAKE_SEGMENT_TRYPEEK_NAME
#undef GEN_TRYPEEK_NAME

#define MAKE_SEGMENT_LOW_NAME(x) x ## _low(x* this)
#define GEN_LOW_NAME(x) MAKE_SEGMENT_LOW_NAME(x)
// INDEX_TYPE segment_x_low(segment_x* this)
INDEX_TYPE GEN_LOW_NAME(SEGMENT)
{
    if (this == NULL) return -1;
    INDEX_TYPE low = this->m_low;
    if (low < DEFAULT_SEGMENT_SIZE)
        return low;
    else
        return DEFAULT_SEGMENT_SIZE;
}
#undef MAKE_SEGMENT_LOW_NAME
#undef GEN_LOW_NAME

#define MAKE_SEGMENT_HIGH_NAME(x) x ## _high(x* this)
#define GEN_HIGH_NAME(x) MAKE_SEGMENT_HIGH_NAME(x)
// INDEX_TYPE segment_x_high(segment_x* this)
INDEX_TYPE GEN_HIGH_NAME(SEGMENT)
{
    if (this == NULL) return -1;
    INDEX_TYPE high = this->m_high;
    if (high < DEFAULT_SEGMENT_SIZE - 1)
        return high;
    else
        return DEFAULT_SEGMENT_SIZE - 1;
}
#undef MAKE_SEGMENT_HIGH_NAME
#undef GEN_HIGH_NAME

#define MAKE_CONCURRENTQUEUE_NEW_NAME(x) x ## _new()
#define GEN_NEW_NAME(x) MAKE_CONCURRENTQUEUE_NEW_NAME(x)
// concurrentqueue_x* concurrentqueue_x_new()
CONCURRENT_QUEUE* GEN_NEW_NAME(CONCURRENT_QUEUE)
{
    #define MAKE_SEGMENT_NEW_NAME(x) x ## _new
    #define GEN_SEG_NEW_NAME(x) MAKE_SEGMENT_NEW_NAME(x)
    CONCURRENT_QUEUE* newVal = (CONCURRENT_QUEUE*)malloc(sizeof(CONCURRENT_QUEUE));
    newVal->m_tail = GEN_SEG_NEW_NAME(SEGMENT)(0, newVal);
    newVal->m_head = newVal->m_tail;
    newVal->m_numSnapshotTakers = 0;
    return newVal;
    
    #undef MAKE_SEGMENT_NEW_NAME
    #undef GEN_SEG_NEW_NAME
}
#undef MAKE_CONCURRENTQUEUE_NEW_NAME
#undef GEN_NEW_NAME

#define MAKE_CONCURRENTQUEUE_DELETE_NAME(x) x ## _delete(x* this)
#define GEN_DELETE_NAME(x) MAKE_CONCURRENTQUEUE_DELETE_NAME(x)
// concurrentqueue_x concurrentqueue_x_delete(concurrentqueue_x* this)
void GEN_DELETE_NAME(CONCURRENT_QUEUE)
{
    if (this == NULL) return;
    if (this->m_head == this->m_tail)
        free(this->m_head);
    else
    {
        free(this->m_head);
        free(this->m_tail);
    }
    free(this);
}
#undef MAKE_CONCURRENTQUEUE_DELETE_NAME
#undef GEN_DELETE_NAME

#define MAKE_CONCURRENTQUEUE_ISEMPTY_NAME(x) x ## _isempty(x* this)
#define GEN_ISEMPTY_NAME(x) MAKE_CONCURRENTQUEUE_ISEMPTY_NAME(x)
// bool concurrentqueue_x_isempty(concurrentqueue_x* this)
bool GEN_ISEMPTY_NAME(CONCURRENT_QUEUE)
{
    #define MAKE_SEGMENT_ISEMPTY_NAME(x) x ## _isempty
    #define GEN_SEG_ISEMPTY_NAME(x) MAKE_SEGMENT_ISEMPTY_NAME(x)
    if (this == NULL) return true;
    SEGMENT* head = this->m_head;
    if (GEN_SEG_ISEMPTY_NAME(SEGMENT)(head) == false)
        return false;
    else if (head->m_next == NULL)
        return true;
    else {
        while (GEN_SEG_ISEMPTY_NAME(SEGMENT)(head) == true)
        {
            if (head->m_next == NULL)
                return true;

            atomic_signal_fence(memory_order_acq_rel);

            head = this->m_head;
        }
        return false;
    }
    #undef MAKE_SEGMENT_ISEMPTY_NAME
    #undef GEN_SEG_ISEMPTY_NAME
}
#undef MAKE_CONCURRENTQUEUE_ISEMPTY_NAME
#undef GEN_ISEMPTY_NAME

#define MAKE_CONCURRENTQUEUE_GETHEADTAILPOSITIONS_NAME(x) x ## _getheadtailpositions(x* this, SEGMENT** head, SEGMENT** tail, INDEX_TYPE* headLow, INDEX_TYPE* tailHigh)
#define GEN_GETHEADTAILPOSITIONS_NAME(x) MAKE_CONCURRENTQUEUE_GETHEADTAILPOSITIONS_NAME(x)
// void concurrentqueue_x_getheadtailpositions(concurrentqueue_x* this, segment_x** head, segment_x** tail, INDEX_TYPE* headLow, INDEX_TYPE* tailHigh)
void GEN_GETHEADTAILPOSITIONS_NAME(CONCURRENT_QUEUE)
{
    #define MAKE_SEGMENT_LOW_NAME(x) x ## _low
    #define GEN_LOW_NAME(x) MAKE_SEGMENT_LOW_NAME(x)
    #define MAKE_SEGMENT_HIGH_NAME(x) x ## _high
    #define GEN_HIGH_NAME(x) MAKE_SEGMENT_HIGH_NAME(x)
    if (this == NULL) return;
    *head = this->m_head;
    *tail = this->m_tail;
    *headLow = GEN_LOW_NAME(SEGMENT)(*head);
    *tailHigh = GEN_HIGH_NAME(SEGMENT)(*tail);
    while (*head != this->m_head || *tail != this->m_tail ||
            *headLow != GEN_LOW_NAME(SEGMENT)(*head) ||
            *tailHigh != GEN_HIGH_NAME(SEGMENT)(*tail) ||
            (*head)->m_index > (*tail)->m_index)
    {

        atomic_signal_fence(memory_order_acq_rel);
        *head = this->m_head;
        *tail = this->m_tail;
        *headLow = GEN_LOW_NAME(SEGMENT)(*head);
        *tailHigh = GEN_HIGH_NAME(SEGMENT)(*tail);
    }

    #undef MAKE_SEGMENT_LOW_NAME
    #undef GEN_LOW_NAME
    #undef MAKE_SEGMENT_HIGH_NAME
    #undef GEN_HIGH_NAME
}
#undef MAKE_CONCURRENTQUEUE_GETHEADTAILPOSITIONS_NAME
#undef GEN_GETHEADTAILPOSITIONS_NAME

#define MAKE_CONCURRENTQUEUE_GETCOUNT_NAME(x) x ## _getcount(x* this)
#define GEN_GETCOUNT_NAME(x) MAKE_CONCURRENTQUEUE_GETCOUNT_NAME(x)
// INDEX_TYPE concurrentqueue_x_getcount(concurrentqueue_x* this)
INDEX_TYPE GEN_GETCOUNT_NAME(CONCURRENT_QUEUE)
{
    #define MAKE_CONCURRENTQUEUE_GETHEADTAILPOSITIONS_NAME(x) x ## _getheadtailpositions
    #define GEN_GETHEADTAILPOSITIONS_NAME(x) MAKE_CONCURRENTQUEUE_GETHEADTAILPOSITIONS_NAME(x)
    if (this == NULL) return -1;
    SEGMENT* head = NULL;
    SEGMENT* tail = NULL;
    INDEX_TYPE headLow = 0;
    INDEX_TYPE tailHigh = 0;
    GEN_GETHEADTAILPOSITIONS_NAME(CONCURRENT_QUEUE)(this, &head, &tail, &headLow, &tailHigh);
    if (head == tail)
    {
        return tailHigh - headLow + 1;
    }

    INDEX_TYPE count = DEFAULT_SEGMENT_SIZE - headLow;
    count += DEFAULT_SEGMENT_SIZE * ((INDEX_TYPE)(tail->m_index - head->m_index - 1));
    count += tailHigh + 1;
    return count;

    #undef MAKE_CONCURRENTQUEUE_GETHEADTAILPOSITIONS_NAME
    #undef GEN_GETHEADTAILPOSITIONS_NAME
}
#undef MAKE_CONCURRENTQUEUE_GETCOUNT_NAME
#undef GEN_GETCOUNT_NAME

#define MAKE_CONCURRENTQUEUE_ENQUEUE_NAME(x) x ## _enqueue(x* this, CONCURRENT_QUEUE_TYPE item)
#define GEN_ENQUEUE_NAME(x) MAKE_CONCURRENTQUEUE_ENQUEUE_NAME(x)
// void concurrentqueue_x_enqueue(concurrentqueue_x* this, x item)
void GEN_ENQUEUE_NAME(CONCURRENT_QUEUE)
{
    #define MAKE_SEGMENT_TRYAPPEND_NAME(x) x ## _tryappend
    #define GEN_SEGMENT_TRYAPPEND_NAME(x) MAKE_SEGMENT_TRYAPPEND_NAME(x)
    if (this == NULL) return;
    while (true)
    {
        SEGMENT* tail = this->m_tail;
        if (GEN_SEGMENT_TRYAPPEND_NAME(SEGMENT)(tail, item) == true)
        {
            return;
        }

        atomic_signal_fence(memory_order_acq_rel);
    }

    #undef MAKE_SEGMENT_TRYAPPEND_NAME
    #undef GEN_TRYAPPEND_NAME
}
#undef MAKE_CONCURRENTQUEUE_ENQUEUE_NAME
#undef GEN_ENQUEUE_NAME

#define MAKE_CONCURRENTQUEUE_TRYDEQUEUE_NAME(x) x ## _trydequeue(x* this, CONCURRENT_QUEUE_TYPE* result)
#define GEN_TRYDEQUEUE_NAME(x) MAKE_CONCURRENTQUEUE_TRYDEQUEUE_NAME(x)
// bool concurrentqueue_x_trydequeue(concurrentqueue_x* this, x* result)
bool GEN_TRYDEQUEUE_NAME(CONCURRENT_QUEUE)
{
    #define MAKE_CONCURRENTQUEUE_ISEMPTY_NAME(x) x ## _isempty
    #define GEN_ISEMPTY_NAME(x) MAKE_CONCURRENTQUEUE_ISEMPTY_NAME(x)
    #define MAKE_SEGMENT_TRYREMOVE_NAME(x) x ## _tryremove
    #define GEN_TRYREMOVE_NAME(x) MAKE_SEGMENT_TRYREMOVE_NAME(x)
    if (this == NULL) return false;
    while (!GEN_ISEMPTY_NAME(CONCURRENT_QUEUE)(this))
    {
        SEGMENT* head = this->m_head;
        if (GEN_TRYREMOVE_NAME(SEGMENT)(head, result))
            return true;
    }
    #ifdef CONCURRENT_QUEUE_ZERO_OUT_RESULT
        memset(result, 0, sizeof(CONCURRENT_QUEUE_TYPE));
    #endif
    return false;

    #undef MAKE_CONCURRENTQUEUE_ISEMPTY_NAME
    #undef GEN_ISEMPTY_NAME
    #undef MAKE_SEGMENT_TRYREMOVE_NAME
    #undef GEN_TRYREMOVE_NAME
}
#undef MAKE_CONCURRENTQUEUE_TRYDEQUEUE_NAME
#undef GEN_TRYDEQUEUE_NAME

#define MAKE_CONCURRENTQUEUE_TRYPEEK_NAME(x) x ## _trypeek(x* this, CONCURRENT_QUEUE_TYPE* result)
#define GEN_TRYPEEK_NAME(x) MAKE_CONCURRENTQUEUE_TRYPEEK_NAME(x)
// bool concurrentqueue_x_trypeek(concurrentqueue_x* this, x* result)
bool GEN_TRYPEEK_NAME(CONCURRENT_QUEUE)
{
    #define MAKE_CONCURRENTQUEUE_ISEMPTY_NAME(x) x ## _isempty
    #define GEN_ISEMPTY_NAME(x) MAKE_CONCURRENTQUEUE_ISEMPTY_NAME(x)
    #define MAKE_SEGMENT_TRYPEEK_NAME(x) x ## _trypeek
    #define GEN_SEG_TRYPEEK_NAME(x) MAKE_SEGMENT_TRYPEEK_NAME(x)
    if (this == NULL) return false;
    __atomic_fetch_add(&this->m_numSnapshotTakers, 1, __ATOMIC_SEQ_CST);

    while (!GEN_ISEMPTY_NAME(CONCURRENT_QUEUE)(this))
    {
        SEGMENT* head = this->m_head;
        if (GEN_SEG_TRYPEEK_NAME(SEGMENT)(head, result))
        {
            __atomic_fetch_sub(&this->m_numSnapshotTakers, 1, __ATOMIC_SEQ_CST);
            return true;
        }
    }
    #ifdef CONCURRENT_QUEUE_ZERO_OUT_RESULT
        memset(result, 0, sizeof(CONCURRENT_QUEUE_TYPE));
    #endif
    __atomic_fetch_sub(&this->m_numSnapshotTakers, 1, __ATOMIC_SEQ_CST);
    return false;

    #undef MAKE_CONCURRENTQUEUE_ISEMPTY_NAME
    #undef GEN_ISEMPTY_NAME
    #undef MAKE_SEGMENT_TRYPEEK_NAME
    #undef GEN_SEG_TRYPEEK_NAME
}
#undef MAKE_CONCURRENTQUEUE_TRYPEEK_NAME
#undef GEN_TRYPEEK_NAME

#undef MAKE_SEGMENT_DELETE_NAME
#undef GEN_DELETE_NAME

#undef MAKE_SEGMENT_NAME
#undef SEGMENT_NAME
#undef SEGMENT

#undef MAKE_CONCURRENT_QUEUE_NAME
#undef CONCURRENT_QUEUE_NAME
#undef CONCURRENT_QUEUE

#undef DEFAULT_SEGMENT_SIZE
#undef INDEX_TYPE
#undef CONCURRENT_QUEUE_TYPE