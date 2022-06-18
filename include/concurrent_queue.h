/*
    Concurrent queue implementation rewritten for C Language
*/

#ifndef CONCURRENT_QUEUE_TYPE
    #error There must be a provided type for CONCURRENT_QUEUE_TYPE and it must be defined prior to using this header!
    #define CONCURRENT_QUEUE_TYPE int32_t // To make Code Analysis on VSCode happy, code shouldn't compile pass above error.
#endif

#include <stdint.h>
#include <stdbool.h>
#define MAKE_SEGMENT_NAME(x) concurrent_queue_segment_ ## x
#define SEGMENT_NAME(x) MAKE_SEGMENT_NAME(x)
#define SEGMENT SEGMENT_NAME(CONCURRENT_QUEUE_TYPE)

#define MAKE_CONCURRENT_QUEUE_NAME(x) concurrent_queue_ ## x
#define CONCURRENT_QUEUE_NAME(x) MAKE_CONCURRENT_QUEUE_NAME(x)
#define CONCURRENT_QUEUE CONCURRENT_QUEUE_NAME(CONCURRENT_QUEUE_TYPE)

#define MAKE_CONCURRENT_QUEUE_NAME_PTR(x) concurrent_queue ## x _PTR
#define CONCURRENT_QUEUE_NAME_PTR(x) MAKE_CONCURRENT_QUEUE_NAME_PTR(x)
#define CONCURRENT_QUEUE_PTR CONCURRENT_QUEUE_NAME_PTR(CONCURRENT_QUEUE_TYPE)

#ifndef INDEX_TYPE
    #define INDEX_TYPE int64_t
#endif

#ifdef STL_FOR_C_IMPLEMENTATION_ONLY
typedef struct {
    _Atomic(CONCURRENT_QUEUE_TYPE*) m_array;
    _Atomic(bool*) m_state;
    _Atomic(void*) m_next;
    INDEX_TYPE _Atomic m_index;
    INDEX_TYPE _Atomic m_low;
    INDEX_TYPE _Atomic m_high;
    _Atomic(void*) m_source;
} SEGMENT;

typedef struct {
    SEGMENT* m_head;
    SEGMENT* m_tail;
} CONCURRENT_QUEUE;

#define MAKE_SEGMENT_NEW_NAME(x, y) x ## _new(INDEX_TYPE index, y* source)
#define GEN_NEW_NAME(x, y) MAKE_SEGMENT_NEW_NAME(x, y)
// segment_x* segment_x_new(INDEX_TYPE index, concurrentqueue_x* source)
SEGMENT* GEN_NEW_NAME(SEGMENT, CONCURRENT_QUEUE);
#undef MAKE_SEGMENT_NEW_NAME
#undef GEN_NEW_NAME

#define MAKE_SEGMENT_DELETE_NAME(x) x ## _delete(x* this)
#define GEN_DELETE_NAME(x) MAKE_SEGMENT_DELETE_NAME(x)
// void segment_x_next(segment_x* this)
void GEN_DELETE_NAME(SEGMENT);
#undef MAKE_SEGMENT_DELETE_NAME
#undef GEN_DELETE_NAME

#define MAKE_SEGMENT_NEXT_NAME(x) x ## _next(x* this)
#define GEN_NEXT_NAME(x) MAKE_SEGMENT_NEXT_NAME(x)
// segment_x* segment_x_next(segment_x* this)
SEGMENT* GEN_NEXT_NAME(SEGMENT);
#undef MAKE_SEGMENT_NEXT_NAME
#undef GEN_NEXT_NAME

#define MAKE_SEGMENT_ISEMPTY_NAME(x) x ## _isempty(x* this)
#define GEN_ISEMPTY_NAME(x) MAKE_SEGMENT_ISEMPTY_NAME(x)
// bool segment_x_isempty(segment_x* this)
bool GEN_ISEMPTY_NAME(SEGMENT);
#undef MAKE_SEGMENT_ISEMPTY_NAME
#undef GEN_ISEMPTY_NAME

#define MAKE_SEGMENT_UNSAFE_ADD_NAME(x, y) x ## _unsafe_add(x* this, y value)
#define GEN_UNSAFE_ADD_NAME(x, y) MAKE_SEGMENT_UNSAFE_ADD_NAME(x, y)
// bool segment_x_unsafe_add(segment_x* this, x value)
bool GEN_UNSAFE_ADD_NAME(SEGMENT, CONCURRENT_QUEUE_TYPE);
#undef MAKE_SEGMENT_UNSAFE_ADD_NAME
#undef GEN_UNSAFE_ADD_NAME

#define MAKE_SEGMENT_UNSAFE_GROW_NAME(x) x ## _unsafe_grow(x* this)
#define GEN_UNSAFE_GROW_NAME(x) MAKE_SEGMENT_UNSAFE_GROW_NAME(x)
// segment_x* segment_x_unsafe_grow(segment_x* this)
SEGMENT* GEN_UNSAFE_GROW_NAME(SEGMENT);
#undef MAKE_SEGMENT_UNSAFE_GROW_NAME
#undef GEN_UNSAFE_GROW_NAME

#define MAKE_SEGMENT_GROW_NAME(x) x ## _grow(x* this)
#define GEN_GROW_NAME(x) MAKE_SEGMENT_GROW_NAME(x)
// bool segment_x_grow(segment_x* this)
bool GEN_GROW_NAME(SEGMENT);
#undef MAKE_SEGMENT_GROW_NAME
#undef GEN_GROW_NAME

#define MAKE_SEGMENT_TRYAPPEND_NAME(x, y) x ## _tryappend(x* this, y value)
#define GEN_TRYAPPEND_NAME(x, y) MAKE_SEGMENT_TRYAPPEND_NAME(x, y)
// void segment_x_tryappend(segment_x* this, x value)
bool GEN_TRYAPPEND_NAME(SEGMENT, CONCURRENT_QUEUE_TYPE);
#undef MAKE_SEGMENT_TRYAPPEND_NAME
#undef GEN_TRYAPPEND_NAME

#define MAKE_SEGMENT_TRYREMOVE_NAME(x, y) x ## _tryremove(x* this, y *result)
#define GEN_SEGMENT_TRYREMOVE_NAME(x, y) MAKE_SEGMENT_TRYREMOVE_NAME(x, y)
// void segment_x_tryappend(segment_x* this, x* result)
bool GEN_SEGMENT_TRYREMOVE_NAME(SEGMENT, CONCURRENT_QUEUE_TYPE);
#undef MAKE_SEGMENT_TRYREMOVE_NAME
#undef GEN_SEGMENT_TRYREMOVE_NAME

#define MAKE_SEGMENT_TRYPEEK_NAME(x, y) x ## _trypeek(x* this, y *result)
#define GEN_TRYPEEK_NAME(x, y) MAKE_SEGMENT_TRYPEEK_NAME(x, y)
// void segment_x_trypeek(segment_x* this, x value)
bool GEN_TRYPEEK_NAME(SEGMENT, CONCURRENT_QUEUE_TYPE);
#undef MAKE_SEGMENT_TRYPEEK_NAME
#undef GEN_TRYPEEK_NAME

#define MAKE_SEGMENT_LOW_NAME(x) x ## _low(x* this)
#define GEN_LOW_NAME(x) MAKE_SEGMENT_LOW_NAME(x)
// INDEX_TYPE segment_x_low(segment_x* this)
INDEX_TYPE GEN_LOW_NAME(SEGMENT);
#undef MAKE_SEGMENT_LOW_NAME
#undef GEN_LOW_NAME

#define MAKE_SEGMENT_HIGH_NAME(x) x ## _high(x* this)
#define GEN_HIGH_NAME(x) MAKE_SEGMENT_HIGH_NAME(x)
// INDEX_TYPE segment_x_high(segment_x* this)
INDEX_TYPE GEN_HIGH_NAME(SEGMENT);
#undef MAKE_SEGMENT_HIGH_NAME
#undef GEN_HIGH_NAME

#else
typedef void* CONCURRENT_QUEUE;
#endif

#define MAKE_CONCURRENTQUEUE_NEW_NAME(x) x ## _new()
#define GEN_NEW_NAME(x) MAKE_CONCURRENTQUEUE_NEW_NAME(x)
// concurrentqueue_x* concurrentqueue_x_new()
CONCURRENT_QUEUE* GEN_NEW_NAME(CONCURRENT_QUEUE);
#undef MAKE_CONCURRENTQUEUE_NEW_NAME
#undef GEN_NEW_NAME

#define MAKE_CONCURRENTQUEUE_DELETE_NAME(x) x ## _delete(x* this)
#define GEN_DELETE_NAME(x) MAKE_CONCURRENTQUEUE_DELETE_NAME(x)
// concurrentqueue_x concurrentqueue_x_delete(concurrentqueue_x* this)
void GEN_DELETE_NAME(CONCURRENT_QUEUE);
#undef MAKE_CONCURRENTQUEUE_DELETE_NAME
#undef GEN_DELETE_NAME

#define MAKE_CONCURRENTQUEUE_ISEMPTY_NAME(x) x ## _isempty(x* this)
#define GEN_ISEMPTY_NAME(x) MAKE_CONCURRENTQUEUE_ISEMPTY_NAME(x)
// bool concurrentqueue_x_isempty(concurrentqueue_x* this)
bool GEN_ISEMPTY_NAME(CONCURRENT_QUEUE);
#undef MAKE_CONCURRENTQUEUE_ISEMPTY_NAME
#undef GEN_ISEMPTY_NAME

#define MAKE_CONCURRENTQUEUE_GETHEADTAILPOSITIONS_NAME(x) x ## _getheadtailpositions(x* this, SEGMENT** head, SEGMENT** tail, INDEX_TYPE* headLow, INDEX_TYPE* tailHigh)
#define GEN_GETHEADTAILPOSITIONS_NAME(x) MAKE_CONCURRENTQUEUE_GETHEADTAILPOSITIONS_NAME(x)
// void concurrentqueue_x_getheadtailpositions(concurrentqueue_x* this, segment_x** head, segment_x** tail, INDEX_TYPE* headLow, INDEX_TYPE* tailHigh)
void GEN_GETHEADTAILPOSITIONS_NAME(CONCURRENT_QUEUE);
#undef MAKE_CONCURRENTQUEUE_GETHEADTAILPOSITIONS_NAME
#undef GEN_GETHEADTAILPOSITIONS_NAME

#define MAKE_CONCURRENTQUEUE_GETCOUNT_NAME(x) x ## _getcount(x* this)
#define GEN_GETCOUNT_NAME(x) MAKE_CONCURRENTQUEUE_GETCOUNT_NAME(x)
// INDEX_TYPE concurrentqueue_x_getcount(concurrentqueue_x* this)
INDEX_TYPE GEN_GETCOUNT_NAME(CONCURRENT_QUEUE);
#undef MAKE_CONCURRENTQUEUE_GETCOUNT_NAME
#undef GEN_GETCOUNT_NAME

#define MAKE_CONCURRENTQUEUE_ENQUEUE_NAME(x) x ## _enqueue(x* this, CONCURRENT_QUEUE_TYPE item)
#define GEN_ENQUEUE_NAME(x) MAKE_CONCURRENTQUEUE_ENQUEUE_NAME(x)
// bool concurrentqueue_x_enqueue(concurrentqueue_x* this, x item)
bool GEN_ENQUEUE_NAME(CONCURRENT_QUEUE);
#undef MAKE_CONCURRENTQUEUE_ENQUEUE_NAME
#undef GEN_ENQUEUE_NAME

#define MAKE_CONCURRENTQUEUE_TRYDEQUEUE_NAME(x) x ## _trydequeue(x* this, CONCURRENT_QUEUE_TYPE* item)
#define GEN_TRYDEQUEUE_NAME(x) MAKE_CONCURRENTQUEUE_TRYDEQUEUE_NAME(x)
// bool concurrentqueue_x_trydequeue(concurrentqueue_x* this, x* result)
bool GEN_TRYDEQUEUE_NAME(CONCURRENT_QUEUE);
#undef MAKE_CONCURRENTQUEUE_TRYDEQUEUE_NAME
#undef GEN_TRYDEQUEUE_NAME

#define MAKE_CONCURRENTQUEUE_TRYPEEK_NAME(x) x ## _trypeek(x* this, CONCURRENT_QUEUE_TYPE* item)
#define GEN_TRYPEEK_NAME(x) MAKE_CONCURRENTQUEUE_TRYPEEK_NAME(x)
// bool concurrentqueue_x_trypeek(concurrentqueue_x* this, x* result)
bool GEN_TRYPEEK_NAME(CONCURRENT_QUEUE);
#undef MAKE_CONCURRENTQUEUE_TRYPEEK_NAME
#undef GEN_TRYPEEK_NAME

#undef MAKE_CONCURRENT_QUEUE_NAME_PTR
#undef CONCURRENT_QUEUE_NAME_PTR
#undef CONCURRENT_QUEUE_PTR

#undef MAKE_SEGMENT_NAME
#undef SEGMENT_NAME
#undef SEGMENT

#undef MAKE_CONCURRENT_QUEUE_NAME
#undef CONCURRENT_QUEUE_NAME
#undef CONCURRENT_QUEUE