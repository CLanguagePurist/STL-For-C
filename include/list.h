/*
    Concurrent queue implementation rewritten for C Language
*/

#ifndef LIST_TYPE
    #error There must be a provided type for LIST_TYPE and it must be defined prior to using this header!
    #define LIST_TYPE int32_t
#endif

#include <stdint.h>
#include <stdbool.h>

#define MAKE_LIST_NAME(x) list_ ## x
#define LIST_NAME(x) MAKE_LIST_NAME(x)
#define LIST LIST_NAME(LIST_TYPE)

#ifndef INDEX_TYPE
    #define INDEX_TYPE int64_t
#endif

typedef struct {
    INDEX_TYPE capacity;
    INDEX_TYPE size;
    LIST_TYPE* items;
} LIST;

#define MAKE_LIST_NEW_NAME(x) x ## _new()
#define GEN_LIST_NEW_NAME(x) MAKE_LIST_NEW_NAME(x)
// list_x* list_x_new()
LIST* GEN_LIST_NEW_NAME(LIST);

#define MAKE_LIST_NEW_WITHCAPACITY_NAME(x) x ## _new_withcapacity(INDEX_TYPE initialCapacity)
#define GEN_LIST_NEW_WITHCAPACITY_NAME(x) MAKE_LIST_NEW_WITHCAPACITY_NAME(x)
// list_x* list_x_new_withcapacity(INDEX_TYPE initialCapacity)
LIST* GEN_LIST_NEW_WITHCAPACITY_NAME(LIST);

#define MAKE_LIST_DESTROY_NAME(x) x ## _destroy(x* this)
#define GEN_LIST_DESTROY_NAME(x) MAKE_LIST_DESTROY_NAME(x)
// void list_x_destroy(list_x* this)
void GEN_LIST_DESTROY_NAME(LIST);

#define MAKE_LIST_INDEXOF_NAME(x) x ## _indexof(x* this, LIST_TYPE searchItem)
#define GEN_LIST_INDEXOF_NAME(x) MAKE_LIST_INDEXOF_NAME(x)
// INDEX_TYPE list_x_indexof(list_x* this)
INDEX_TYPE GEN_LIST_INDEXOF_NAME(LIST);

#define MAKE_LIST_INDEXOF2_NAME(x) x ## _indexof2(x* this, LIST_TYPE searchItem, INDEX_TYPE start)
#define GEN_LIST_INDEXOF2_NAME(x) MAKE_LIST_INDEXOF2_NAME(x)
// INDEX_TYPE list_x_indexof2(list_x* this, INDEX_TYPE start)
INDEX_TYPE GEN_LIST_INDEXOF2_NAME(LIST);

#define MAKE_LIST_INDEXOF3_NAME(x) x ## _indexof3(x* this, LIST_TYPE searchItem, INDEX_TYPE start, INDEX_TYPE length)
#define GEN_LIST_INDEXOF3_NAME(x) MAKE_LIST_INDEXOF3_NAME(x)
// INDEX_TYPE list_x_indexof3(list_x* this, INDEX_TYPE start, INDEX_TYPE length)
INDEX_TYPE GEN_LIST_INDEXOF3_NAME(LIST);

#define MAKE_LIST_GET_NAME(x) x ## _get(x* this, INDEX_TYPE index, LIST_TYPE* result)
#define GEN_LIST_GET_NAME(x) MAKE_LIST_GET_NAME(x)
// bool list_x_get(list_x* this, INDEX_TYPE index, LIST_TYPE* result)
bool GEN_LIST_GET_NAME(LIST);

#define MAKE_LIST_GETRANGE_NAME(x) x ## _getrange(x* this, INDEX_TYPE index, INDEX_TYPE length, LIST_TYPE** result)
#define GEN_LIST_GETRANGE_NAME(x) MAKE_LIST_GETRANGE_NAME(x)
// bool list_x_getrange(list_x* this, INDEX_TYPE index, INDEX_TYPE length, LIST_TYPE** result)
bool GEN_LIST_GETRANGE_NAME(LIST);

#define MAKE_LIST_ADD_NAME(x) x ## _add(x* this, LIST_TYPE item)
#define GEN_LIST_ADD_NAME(x) MAKE_LIST_ADD_NAME(x)
// bool list_x_add(list_x* this, LIST_TYPE item)
bool GEN_LIST_ADD_NAME(LIST);

#define MAKE_LIST_ADDRANGE_NAME(x) x ## _addrange(x* this, LIST_TYPE* items, INDEX_TYPE items_length)
#define GEN_LIST_ADDRANGE_NAME(x) MAKE_LIST_ADDRANGE_NAME(x)
// bool list_x_addrange(list_x* this, LIST_TYPE* items, INDEX_TYPE items_length)
bool GEN_LIST_ADDRANGE_NAME(LIST);

#define MAKE_LIST_ADD_AT_NAME(x) x ## _add_at(x* this, LIST_TYPE item, INDEX_TYPE index)
#define GEN_LIST_ADD_AT_NAME(x) MAKE_LIST_ADD_AT_NAME(x)
// bool list_x_add(list_x* this, LIST_TYPE item, INDEX_TYPE index)
bool GEN_LIST_ADD_AT_NAME(LIST);

#define MAKE_LIST_ADDRANGE_AT_NAME(x) x ## _addrange_at(x* this, INDEX_TYPE index, LIST_TYPE* items, INDEX_TYPE items_length)
#define GEN_LIST_ADDRANGE_AT_NAME(x) MAKE_LIST_ADDRANGE_AT_NAME(x)
// bool list_x_addrange_at(list_x* this, INDEX_TYPE index, LIST_TYPE* items, INDEX_TYPE items_length)
bool GEN_LIST_ADDRANGE_AT_NAME(LIST);

#define MAKE_LIST_REMOVE_NAME(x) x ## _remove(x* this, INDEX_TYPE index)
#define GEN_LIST_REMOVE_NAME(x) MAKE_LIST_REMOVE_NAME(x)
// bool list_x_remove(list_x* this, INDEX_TYPE index)
bool GEN_LIST_REMOVE_NAME(LIST);


#define MAKE_LIST_REMOVERANGE_NAME(x) x ## _removerange(x* this, INDEX_TYPE index, INDEX_TYPE length)
#define GEN_LIST_REMOVERANGE_NAME(x) MAKE_LIST_REMOVERANGE_NAME(x)
// bool list_x_removerange(list_x* this, INDEX_TYPE index, INDEX_TYPE length)
bool GEN_LIST_REMOVERANGE_NAME(LIST);


#define MAKE_LIST_REMOVE_ITEM_NAME(x) x ## _remove_item(x* this, LIST_TYPE item)
#define GEN_LIST_REMOVE_ITEM_NAME(x) MAKE_LIST_REMOVE_ITEM_NAME(x)
// bool list_x_remove_item(list_x* this, INDEX_TYPE index)
bool GEN_LIST_REMOVE_ITEM_NAME(LIST);

#define MAKE_LIST_CLEAR_NAME(x) x ## _clear(x* this)
#define GEN_LIST_CLEAR_NAME(x) MAKE_LIST_CLEAR_NAME(x)
// bool list_x_clear(list_x* this)
bool GEN_LIST_CLEAR_NAME(LIST);

#define MAKE_LIST_COUNT_NAME(x) x ## _count(x* this)
#define GEN_LIST_COUNT_NAME(x) MAKE_LIST_COUNT_NAME(x)
// INDEX_TYPE list_x_count(list_x* this)
INDEX_TYPE GEN_LIST_COUNT_NAME(LIST);

#undef MAKE_LIST_NEW_NAME
#undef GEN_LIST_NEW_NAME
#undef MAKE_LIST_DESTROY_NAME
#undef GEN_LIST_DESTROY_NAME
#undef MAKE_LIST_NEW_WITHCAPACITY_NAME
#undef GEN_LIST_NEW_WITHCAPACITY_NAME
#undef MAKE_LIST_INDEXOF_NAME
#undef GEN_LIST_INDEXOF_NAME
#undef MAKE_LIST_INDEXOF2_NAME
#undef GEN_LIST_INDEXOF2_NAME
#undef MAKE_LIST_INDEXOF3_NAME
#undef GEN_LIST_INDEXOF3_NAME
#undef MAKE_LIST_GET_NAME
#undef GEN_LIST_GET_NAME
#undef MAKE_LIST_GETRANGE_NAME
#undef GEN_LIST_GETRANGE_NAME
#undef MAKE_LIST_ADD_NAME
#undef GEN_LIST_ADD_NAME
#undef MAKE_LIST_ADDRANGE_NAME
#undef GEN_LIST_ADDRANGE_NAME
#undef MAKE_LIST_ADD_AT_NAME
#undef GEN_LIST_ADD_AT_NAME
#undef MAKE_LIST_ADDRANGE_AT_NAME
#undef GEN_LIST_ADDRANGE_AT_NAME
#undef MAKE_LIST_REMOVE_NAME
#undef GEN_LIST_REMOVE_NAME
#undef MAKE_LIST_REMOVERANGE_NAME
#undef GEN_LIST_REMOVERANGE_NAME
#undef MAKE_LIST_REMOVE_ITEM_NAME
#undef GEN_LIST_REMOVE_ITEM_NAME
#undef MAKE_LIST_CLEAR_NAME
#undef GEN_LIST_CLEAR_NAME
#undef MAKE_LIST_COUNT_NAME
#undef GEN_LIST_COUNT_NAME
#undef MAKE_LIST_NAME
#undef LIST_NAME