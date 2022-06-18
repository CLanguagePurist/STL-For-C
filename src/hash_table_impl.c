#ifndef HASH_TABLE_VALUE_TYPE
    #error HASH_TABLE_VALUE_TYPE need to be defined with a type specified (Ex. int, long, double, float, struct TestStruct, ...)
    #define HASH_TABLE_VALUE_TYPE int32_t
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#ifndef INDEX_TYPE
    #define INDEX_TYPE int64_t
#endif

#ifndef DEFAULT_TABLE_SIZE
    #define DEFAULT_TABLE_SIZE 32
#endif

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

#define MAKE_HASH_TABLE_NAME(x) hash_table_ ## x
#define HASH_TABLE_NAME(x) MAKE_HASH_TABLE_NAME(x)
#define HASH_TABLE HASH_TABLE_NAME(HASH_TABLE_VALUE_TYPE)

#define MAKE_HASH_TABLE_ENTRY_NAME(x) hash_table_entry_ ## x
#define HASH_TABLE_ENTRY_NAME(x) MAKE_HASH_TABLE_ENTRY_NAME(x)
#define HASH_TABLE_ENTRY HASH_TABLE_ENTRY_NAME(HASH_TABLE_VALUE_TYPE)

typedef struct {
    char* key;
    HASH_TABLE_VALUE_TYPE value;
} HASH_TABLE_ENTRY;

typedef struct {
    HASH_TABLE_ENTRY* entries;
    INDEX_TYPE capacity;
    INDEX_TYPE length;
} HASH_TABLE;

#define MAKE_HASH_TABLE_NEW_NAME(x) x ## _new()
#define GEN_HASH_TABLE_NEW_NAME(x) MAKE_HASH_TABLE_NEW_NAME(x)
HASH_TABLE* GEN_HASH_TABLE_NEW_NAME(HASH_TABLE)
{
    // Allocate space for hash table struct.
    HASH_TABLE* table = malloc(sizeof(HASH_TABLE));
    if (table == NULL) {
        return NULL;
    }
    table->length = 0;
    table->capacity = DEFAULT_TABLE_SIZE;

    // Allocate (zero'd) space for entry buckets.
    table->entries = calloc(table->capacity, sizeof(HASH_TABLE_ENTRY));
    if (table->entries == NULL) {
        free(table); // error, free table before we return!
        return NULL;
    }
    return table;
}

#define MAKE_HASH_TABLE_DESTROY_NAME(x) x ## _destroy(x* this)
#define GEN_HASH_TABLE_DESTROY_NAME(x) MAKE_HASH_TABLE_DESTROY_NAME(x)
void GEN_HASH_TABLE_DESTROY_NAME(HASH_TABLE)
{
    if (this == NULL) return;
    // First free allocated keys.
    for (size_t i = 0; i < this->length; i++) {
        if (this->entries[i].key != NULL) {
            free(this->entries[i].key);
        }
    }

    // Then free entries array and table itself.
    free(this->entries);
    free(this);
}

#define MAKE_HASH_TABLE_HASH_KEY_NAME(x) x ## _hash_key
#define GEN_HASH_TABLE_HASH_KEY_NAME(x) MAKE_HASH_TABLE_HASH_KEY_NAME(x)
static INDEX_TYPE GEN_HASH_TABLE_HASH_KEY_NAME(HASH_TABLE)(const char* key)
{
    INDEX_TYPE hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (INDEX_TYPE)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

#define MAKE_HASH_TABLE_GET_NAME(x) x ## _get(x* this, const char* key)
#define GEN_HASH_TABLE_GET_NAME(x) MAKE_HASH_TABLE_GET_NAME(x)
HASH_TABLE_VALUE_TYPE* GEN_HASH_TABLE_GET_NAME(HASH_TABLE)
{
    if (this == NULL)
        return NULL;
    // AND hash with capacity-1 to ensure it's within entries array.
    INDEX_TYPE hash = GEN_HASH_TABLE_HASH_KEY_NAME(HASH_TABLE)(key);
    INDEX_TYPE index = (INDEX_TYPE)(hash & (INDEX_TYPE)(this->capacity - 1));

    // Loop till we find an empty entry.
    while (this->entries[index].key != NULL) {
        if (strcmp(key, this->entries[index].key) == 0) {
            // Found key, return value.
            return &this->entries[index].value;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= this->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return NULL;
}

#define MAKE_HASH_TABLE_REMOVE_NAME(x) x ## _remove(x* this, const char* key)
#define GEN_HASH_TABLE_REMOVE_NAME(x) MAKE_HASH_TABLE_REMOVE_NAME(x)
bool GEN_HASH_TABLE_REMOVE_NAME(HASH_TABLE)
{
    if (this == NULL)
        return false;
    // AND hash with capacity-1 to ensure it's within entries array.
    INDEX_TYPE hash = GEN_HASH_TABLE_HASH_KEY_NAME(HASH_TABLE)(key);
    INDEX_TYPE index = (INDEX_TYPE)(hash & (INDEX_TYPE)(this->capacity - 1));

    // Loop till we find an empty entry.
    while (this->entries[index].key != NULL) {
        if (strcmp(key, this->entries[index].key) == 0) {
            free(this->entries[index].key);
            INDEX_TYPE remainder = this->length - index;
            if (remainder > 0)
                memmove(&this->entries[index + 1], &this->entries[index], remainder);
            this->length--;
            return true;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= this->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return false;
}

#define MAKE_HASH_TABLE_SET_ENTRY_NAME(x) x ## _set_entry
#define GEN_HASH_TABLE_SET_ENTRY_NAME(x) MAKE_HASH_TABLE_SET_ENTRY_NAME(x)
// Internal function to set an entry (without expanding table).
static const char* GEN_HASH_TABLE_SET_ENTRY_NAME(HASH_TABLE)(HASH_TABLE_ENTRY* entries, INDEX_TYPE capacity,
        const char* key, HASH_TABLE_VALUE_TYPE value, INDEX_TYPE* plength) {
    // AND hash with capacity-1 to ensure it's within entries array.
    INDEX_TYPE hash = GEN_HASH_TABLE_HASH_KEY_NAME(HASH_TABLE)(key);
    INDEX_TYPE index = (INDEX_TYPE)(hash & (INDEX_TYPE)(capacity - 1));

    // Loop till we find an empty entry.
    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            // Found key (it already exists), update value.
            entries[index].value = value;
            return entries[index].key;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }

    // Didn't find key, allocate+copy if needed, then insert it.
    char* realKey = (char*)key;
    if (plength != NULL) {
        size_t len = strlen(key);
        if (len <= 0) {
            return NULL;
        }
        realKey = calloc(len + 1, 1);
        memcpy(realKey, key, len);
        (*plength)++;
    }
    entries[index].key = (char*)realKey;
    entries[index].value = value;
    return realKey;
}

#define MAKE_HASH_TABLE_EXPAND_NAME(x) x ## _expand
#define GEN_HASH_TABLE_EXPAND_NAME(x) MAKE_HASH_TABLE_EXPAND_NAME(x)
// Expand hash table to twice its current size. Return true on success,
// false if out of memory.
static bool GEN_HASH_TABLE_EXPAND_NAME(HASH_TABLE)(HASH_TABLE* this) {
    if (this == NULL)
        return false;
    // Allocate new entries array.
    INDEX_TYPE new_capacity = this->capacity * 2;
    if (new_capacity < this->capacity) {
        return false;  // overflow (capacity would be too big)
    }
    HASH_TABLE_ENTRY* new_entries = calloc(new_capacity, sizeof(HASH_TABLE_ENTRY));
    if (new_entries == NULL) {
        return false;
    }

    // Iterate entries, move all non-empty ones to new table's entries.
    for (INDEX_TYPE i = 0; i < this->capacity; i++) {
        HASH_TABLE_ENTRY entry = this->entries[i];
        if (entry.key != NULL) {
            GEN_HASH_TABLE_SET_ENTRY_NAME(HASH_TABLE)(new_entries, new_capacity, entry.key,
                         entry.value, NULL);
        }
    }

    // Free old entries array and update this table's details.
    free(this->entries);
    this->entries = new_entries;
    this->capacity = new_capacity;
    return true;
}

#define MAKE_HASH_TABLE_SET_NAME(x) x ## _set(x* this, const char* key, HASH_TABLE_VALUE_TYPE value)
#define GEN_HASH_TABLE_SET_NAME(x) MAKE_HASH_TABLE_SET_NAME(x)
const char* GEN_HASH_TABLE_SET_NAME(HASH_TABLE)
{
    if (this == NULL)
        return NULL;
    // If length will exceed half of current capacity, expand it.
    if (this->length >= this->capacity / 2) {
        if (!GEN_HASH_TABLE_EXPAND_NAME(HASH_TABLE)(this)) {
            return NULL;
        }
    }

    // Set entry and update length.
    return GEN_HASH_TABLE_SET_ENTRY_NAME(HASH_TABLE)(this->entries, this->capacity, key, value,
                        &this->length);
}

#define MAKE_HASH_TABLE_GETCOUNT_NAME(x) x ## _getcount(x* this)
#define GEN_HASH_TABLE_GETCOUNT_NAME(x) MAKE_HASH_TABLE_GETCOUNT_NAME(x)
INDEX_TYPE GEN_HASH_TABLE_GETCOUNT_NAME(HASH_TABLE)
{
    if (this == NULL)
        return -1;
    return this->length;
}

#define MAKE_HASH_TABLE_ITERATOR_NAME(x) hash_table_iterator_ ## x
#define HASH_TABLE_ITERATOR_NAME(x) MAKE_HASH_TABLE_ITERATOR_NAME(x)
#define HASH_TABLE_ITERATOR HASH_TABLE_ITERATOR_NAME(HASH_TABLE_VALUE_TYPE)

typedef struct {
    const char* key;
    HASH_TABLE_VALUE_TYPE value;
    HASH_TABLE* _table;
    INDEX_TYPE _index;
} HASH_TABLE_ITERATOR;

#define MAKE_HASH_TABLE_GETITERATOR_NAME(x) x ## _getiterator(x* this)
#define GEN_HASH_TABLE_GETITERATOR_NAME(x) MAKE_HASH_TABLE_GETITERATOR_NAME(x)
HASH_TABLE_ITERATOR GEN_HASH_TABLE_GETITERATOR_NAME(HASH_TABLE)
{
    assert(this != NULL);
    HASH_TABLE_ITERATOR iterator;
    iterator._table = this;
    iterator._index = 0;
    return iterator;
}

#define MAKE_HASH_TABLE_ITERATOR_NEXT_NAME(x, y) x ## _iterator_next(y* iterator)
#define GEN_HASH_TABLE_ITERATOR_NEXT_NAME(x, y) MAKE_HASH_TABLE_ITERATOR_NEXT_NAME(x, y)
bool GEN_HASH_TABLE_ITERATOR_NEXT_NAME(HASH_TABLE, HASH_TABLE_ITERATOR)
{
    if (iterator == NULL)
        return false;
    // Loop till we've hit end of entries array.
    HASH_TABLE* table = iterator->_table;
    while (iterator->_index < table->capacity) {
        size_t i = iterator->_index;
        iterator->_index++;
        if (table->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and value.
            HASH_TABLE_ENTRY entry = table->entries[i];
            iterator->key = entry.key;
            iterator->value = entry.value;
            return true;
        }
    }
    return false;
}

#undef GEN_HASH_TABLE_ITERATOR_NEXT_NAME
#undef MAKE_HASH_TABLE_ITERATOR_NEXT_NAME
#undef GEN_HASH_TABLE_GETITERATOR_NAME
#undef MAKE_HASH_TABLE_GETITERATOR_NAME
#undef HASH_TABLE_ITERATOR
#undef HASH_TABLE_ITERATOR_NAME
#undef MAKE_HASH_TABLE_ITERATOR_NAME
#undef GEN_HASH_TABLE_GETCOUNT_NAME
#undef MAKE_HASH_TABLE_GETCOUNT_NAME
#undef GEN_HASH_TABLE_SET_NAME
#undef MAKE_HASH_TABLE_SET_NAME
#undef GEN_HASH_TABLE_GET_NAME
#undef MAKE_HASH_TABLE_GET_NAME
#undef GEN_HASH_TABLE_DESTROY_NAME
#undef MAKE_HASH_TABLE_DESTROY_NAME
#undef GEN_HASH_TABLE_NEW_NAME
#undef MAKE_HASH_TABLE_NEW_NAME
#undef HASH_TABLE
#undef HASH_TABLE_NAME
#undef MAKE_HASH_TABLE_NAME
#undef HASH_TABLE_VALUE_TYPE
#undef MAKE_HASH_TABLE_SET_ENTRY_NAME
#undef GEN_HASH_TABLE_SET_ENTRY_NAME
#undef MAKE_HASH_TABLE_EXPAND_NAME
#undef GEN_HASH_TABLE_EXPAND_NAME
#undef MAKE_HASH_TABLE_HASH_KEY_NAME
#undef GEN_HASH_TABLE_HASH_KEY_NAME
#undef MAKE_HASH_TABLE_REMOVE_NAME
#undef GEN_HASH_TABLE_REMOVE_NAME