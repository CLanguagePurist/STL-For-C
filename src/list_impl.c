/*
    List Type reimplemented in C
*/

#ifndef LIST_TYPE
    #error There must be a provided type for LIST_TYPE and it must be defined prior to using this header!
#endif

#include <stdlib.h>
#include <string.h>
#include "include/data/list.h"

#define MAKE_LIST_NAME(x) list_ ## x
#define LIST_NAME(x) MAKE_LIST_NAME(x)
#define LIST LIST_NAME(LIST_TYPE)

#define MAKE_LIST_NEW_NAME(x) x ## _new()
#define GEN_LIST_NEW_NAME(x) MAKE_LIST_NEW_NAME(x)
// list_x* list_x_new()
LIST* GEN_LIST_NEW_NAME(LIST)
{
    LIST* newData = (LIST*) malloc(sizeof(LIST));
    newData->capacity = 32;
    newData->size = 0;
    newData->items = (LIST_TYPE*)malloc(sizeof(LIST_TYPE) * 32);
    return newData;
}

#define MAKE_LIST_NEW_WITHCAPACITY_NAME(x) x ## _new_withcapacity(INDEX_TYPE initialCapacity)
#define GEN_LIST_NEW_WITHCAPACITY_NAME(x) MAKE_LIST_NEW_WITHCAPACITY_NAME(x)
// list_x* list_x_new_withcapacity(INDEX_TYPE initialCapacity)
LIST* GEN_LIST_NEW_WITHCAPACITY_NAME(LIST)
{
    if (initialCapacity < 0) initialCapacity = 0;
    LIST* newData = (LIST*) malloc(sizeof(LIST));
    newData->capacity = initialCapacity;
    newData->size = 0;
    newData->items = (LIST_TYPE*)malloc(sizeof(LIST_TYPE) * initialCapacity);
    return newData;
}

#define MAKE_LIST_DESTROY_NAME(x) x ## _destroy(x* this)
#define GEN_LIST_DESTROY_NAME(x) MAKE_LIST_DESTROY_NAME(x)
// void list_x_destroy(list_x* this)
void GEN_LIST_DESTROY_NAME(LIST)
{
    if (this == NULL) return;
    free(this->items);
    free(this);
}

#define MAKE_LIST_INDEXOF_NAME(x) x ## _indexof(x* this, LIST_TYPE searchItem)
#define GEN_LIST_INDEXOF_NAME(x) MAKE_LIST_INDEXOF_NAME(x)
#define MAKE_LIST_INDEXOF_NAME_USE(x) x ## _indexof
#define GEN_LIST_INDEXOF_NAME_USE(x) MAKE_LIST_INDEXOF_NAME_USE(x)
// INDEX_TYPE list_x_indexof(list_x* this)
INDEX_TYPE GEN_LIST_INDEXOF_NAME(LIST)
{
    if (this == NULL) return -1;
    for (INDEX_TYPE i = 0; i < this->size; ++i)
    {
        if (memcmp(&this->items[i], &searchItem, sizeof(LIST_TYPE)) == 0)
            return i;
    }
    return -1;
}

#define MAKE_LIST_INDEXOF2_NAME(x) x ## _indexof2(x* this, LIST_TYPE searchItem, INDEX_TYPE start)
#define GEN_LIST_INDEXOF2_NAME(x) MAKE_LIST_INDEXOF2_NAME(x)
// INDEX_TYPE list_x_indexof2(list_x* this, INDEX_TYPE start)
INDEX_TYPE GEN_LIST_INDEXOF2_NAME(LIST)
{
    if (this == NULL) return -1;
    for (INDEX_TYPE i = start; i < this->size; ++i)
    {
        if (memcmp(&this->items[i], &searchItem, sizeof(LIST_TYPE)) == 0)
            return i;
    }
    return -1;
}

#define MAKE_LIST_INDEXOF3_NAME(x) x ## _indexof3(x* this, LIST_TYPE searchItem, INDEX_TYPE start, INDEX_TYPE length)
#define GEN_LIST_INDEXOF3_NAME(x) MAKE_LIST_INDEXOF3_NAME(x)
// INDEX_TYPE list_x_indexof3(list_x* this, INDEX_TYPE start, INDEX_TYPE length)
INDEX_TYPE GEN_LIST_INDEXOF3_NAME(LIST)
{
    if (this == NULL) return -1;
    INDEX_TYPE end = start + length;
    if (end > this->size)
        end = this->size;
    for (INDEX_TYPE i = start; i < end; ++i)
    {
        if (memcmp(&this->items[i], &searchItem, sizeof(LIST_TYPE)) == 0)
            return i;
    }
    return -1;
}

#define MAKE_LIST_GET_NAME(x) x ## _get(x* this, INDEX_TYPE index, LIST_TYPE* result)
#define GEN_LIST_GET_NAME(x) MAKE_LIST_GET_NAME(x)
// bool list_x_get(list_x* this, INDEX_TYPE index, LIST_TYPE* result)
bool GEN_LIST_GET_NAME(LIST)
{
    if (this == NULL) return false;
    if (index >= 0 && index < this->size)
    {
        *result = this->items[index];
        return true;
    }
    return false;
}

#define MAKE_LIST_GETRANGE_NAME(x) x ## _getrange(x* this, INDEX_TYPE index, INDEX_TYPE length, LIST_TYPE** result)
#define GEN_LIST_GETRANGE_NAME(x) MAKE_LIST_GETRANGE_NAME(x)
// bool list_x_getrange(list_x* this, INDEX_TYPE index, INDEX_TYPE length, LIST_TYPE** result)
bool GEN_LIST_GETRANGE_NAME(LIST)
{
    if (this == NULL) return false;
    if (index < 0 || index >= this->size || length < 0)
        return false;
    INDEX_TYPE remainder = this->size - index;
    if (length > remainder)
        return false;
    
    *result = (LIST_TYPE*) malloc(length * sizeof(LIST_TYPE));
    if (result == NULL)
        return false;
    memcpy(*result, &this->items[index], length * sizeof(LIST_TYPE));
    return true;
}
#define MAKE_LIST_REALLOCATE_NAME(x) x ## _reallocate(x* this, INDEX_TYPE newLength)
#define GEN_LIST_REALLOCATE_NAME(x) MAKE_LIST_REALLOCATE_NAME(x)
#define MAKE_LIST_REALLOCATE_NAME_USE(x) x ## _reallocate
#define GEN_LIST_REALLOCATE_NAME_USE(x) MAKE_LIST_REALLOCATE_NAME_USE(x)
// bool list_x_reallocate(list_x* this, INDEX_TYPE newLength)
bool GEN_LIST_REALLOCATE_NAME(LIST)
{
    if (this == NULL) return false;
    if (newLength <= 0)
    {
        free(this->items);
        this->items = NULL;
        this->size = 0;
        this->capacity = 0;
        return true;
    }

    if (newLength < this->capacity)
    {
        LIST_TYPE* temp = (LIST_TYPE*) realloc(this->items, newLength * sizeof(LIST_TYPE));

        if (temp == NULL) { return false; }
        
        this->items = temp;
        this->capacity = newLength;

        if (this->size > newLength) { this->size = newLength; }
        return true;
    }
    
    INDEX_TYPE newLen = this->capacity * 2;
    
    while (newLen < newLength && newLen < newLen * 2) { newLen *= 2; }
    
    if (newLen < newLength) { return false; }
    
    LIST_TYPE* temp = (LIST_TYPE*) realloc(this->items, newLen * sizeof(LIST_TYPE));
    
    if (temp == NULL) { return false; }

    this->items = temp;
    this->capacity = newLen;
    return true;
}

#define MAKE_LIST_ADD_NAME(x) x ## _add(x* this, LIST_TYPE item)
#define GEN_LIST_ADD_NAME(x) MAKE_LIST_ADD_NAME(x)
// bool list_x_add(list_x* this, LIST_TYPE item)
bool GEN_LIST_ADD_NAME(LIST)
{
    if (this == NULL) return false;
    if (this->size >= this->capacity)
    {
        bool check = GEN_LIST_REALLOCATE_NAME_USE(LIST)(this, this->size + 1);
        if (check == false) { return false; }
    }
    this->items[this->size++] = item;
    return true;
}

#define MAKE_LIST_ADDRANGE_NAME(x) x ## _addrange(x* this, LIST_TYPE* items, INDEX_TYPE items_length)
#define GEN_LIST_ADDRANGE_NAME(x) MAKE_LIST_ADDRANGE_NAME(x)
// bool list_x_addrange(list_x* this, LIST_TYPE* items, INDEX_TYPE items_length)
bool GEN_LIST_ADDRANGE_NAME(LIST)
{
    if (this == NULL) return false;
    INDEX_TYPE newSize = this->size + items_length;
    if (newSize >= this->capacity)
    {
        bool check = GEN_LIST_REALLOCATE_NAME_USE(LIST)(this, newSize);
        if (check == false) return false;
    }
    memcpy(&this->items[this->size], items, items_length * sizeof(LIST_TYPE));
    this->size += items_length;
    return true;
}

#define MAKE_LIST_ADD_AT_NAME(x) x ## _add_at(x* this, LIST_TYPE item, INDEX_TYPE index)
#define GEN_LIST_ADD_AT_NAME(x) MAKE_LIST_ADD_AT_NAME(x)
// bool list_x_add(list_x* this, LIST_TYPE item, INDEX_TYPE index)
bool GEN_LIST_ADD_AT_NAME(LIST)
{
    if (this == NULL) return false;
    if (this->size >= this->capacity)
    {
        bool check = GEN_LIST_REALLOCATE_NAME_USE(LIST)(this, this->size + 1);
        if (check == false) { return false; }
    }
    INDEX_TYPE remainder = this->size - index;
    memmove(&this->items[index + 1], &this->items[index], (remainder + 1) * sizeof(LIST_TYPE));
    this->items[index] = item;
    this->size++;
    return true;
}

#define MAKE_LIST_ADDRANGE_AT_NAME(x) x ## _addrange_at(x* this, INDEX_TYPE index, LIST_TYPE* items, INDEX_TYPE items_length)
#define GEN_LIST_ADDRANGE_AT_NAME(x) MAKE_LIST_ADDRANGE_AT_NAME(x)
// bool list_x_addrange_at(list_x* this, INDEX_TYPE index, LIST_TYPE* items, INDEX_TYPE items_length)
bool GEN_LIST_ADDRANGE_AT_NAME(LIST)
{
    if (this == NULL) return false;
    INDEX_TYPE newLen = this->size + items_length;
    if (newLen >= this->capacity)
    {
        bool check = GEN_LIST_REALLOCATE_NAME_USE(LIST)(this, newLen);
        if (check == false) { return false; }
    }
    INDEX_TYPE remainder = this->size - index;
    memmove(&this->items[index + items_length], &this->items[index], (remainder + 1) * sizeof(LIST_TYPE));
    memcpy(&this->items[index], items, items_length * sizeof(LIST_TYPE));
    this->size = newLen;
    return true;
}

#define MAKE_LIST_REMOVE_NAME(x) x ## _remove(x* this, INDEX_TYPE index)
#define GEN_LIST_REMOVE_NAME(x) MAKE_LIST_REMOVE_NAME(x)
#define MAKE_LIST_REMOVE_NAME_USE(x) x ## _remove
#define GEN_LIST_REMOVE_NAME_USE(x) MAKE_LIST_REMOVE_NAME_USE(x)
// bool list_x_remove(list_x* this, INDEX_TYPE index)
bool GEN_LIST_REMOVE_NAME(LIST)
{
    if (this == NULL) return false;
    INDEX_TYPE remainder = this->size - index;
    if (remainder > 0)
    {
        memmove(&this->items[index], &this->items[index + 1], remainder * sizeof(LIST_TYPE));
        if (this->size > 0)
            this->size -= 1;
    }
    if (this->size < this->capacity / 4)
    {
        return GEN_LIST_REALLOCATE_NAME_USE(LIST)(this, this->capacity / 2);
    }
    return true;
}


#define MAKE_LIST_REMOVERANGE_NAME(x) x ## _removerange(x* this, INDEX_TYPE index, INDEX_TYPE length)
#define GEN_LIST_REMOVERANGE_NAME(x) MAKE_LIST_REMOVERANGE_NAME(x)
// bool list_x_removerange(list_x* this, INDEX_TYPE index, INDEX_TYPE length)
bool GEN_LIST_REMOVERANGE_NAME(LIST)
{
    if (this == NULL) return false;
    if (index > this->size || index < 0)
        return false;
    if (this->size - index < length)
        length = this->size - index;
    if (length < 0) length = 0;
    INDEX_TYPE remainder = this->size - index;
    if (remainder > 0)
    {
        memmove(&this->items[index], &this->items[index + length], remainder * sizeof(LIST_TYPE));
        this->size -= length;
    }
    if (this->size < 0)
        this->size = 0;
    if (this->size < this->capacity / 4)
    {
        return GEN_LIST_REALLOCATE_NAME_USE(LIST)(this, this->capacity / 2);
    }
    return true;
}


#define MAKE_LIST_REMOVE_ITEM_NAME(x) x ## _remove_item(x* this, LIST_TYPE item)
#define GEN_LIST_REMOVE_ITEM_NAME(x) MAKE_LIST_REMOVE_ITEM_NAME(x)
// bool list_x_remove_item(list_x* this, INDEX_TYPE index)
bool GEN_LIST_REMOVE_ITEM_NAME(LIST)
{
    if (this == NULL) return false;
    INDEX_TYPE index = GEN_LIST_INDEXOF_NAME_USE(LIST)(this, item);
    if (index < 0)
        return false;
    return GEN_LIST_REMOVE_NAME_USE(LIST)(this, index);
}

#define MAKE_LIST_CLEAR_NAME(x) x ## _clear(x* this)
#define GEN_LIST_CLEAR_NAME(x) MAKE_LIST_CLEAR_NAME(x)
// bool list_x_clear(list_x* this)
bool GEN_LIST_CLEAR_NAME(LIST)
{
    if (this == NULL) return false;
    free(this->items);
    this->items = NULL;
    this->capacity = 0;
    this->size = 0;
    return true;
}

#define MAKE_LIST_COUNT_NAME(x) x ## _count(x* this)
#define GEN_LIST_COUNT_NAME(x) MAKE_LIST_COUNT_NAME(x)
// INDEX_TYPE list_x_count(list_x* this)
INDEX_TYPE GEN_LIST_COUNT_NAME(LIST)
{
    if (this == NULL) return false;
    return this->size;
}

#undef MAKE_LIST_NEW_NAME
#undef GEN_LIST_NEW_NAME
#undef MAKE_LIST_DELETE_NAME
#undef GEN_LIST_DELETE_NAME
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
#undef MAKE_LIST_NAME
#undef LIST_NAME
#undef LIST
#undef LIST_TYPE