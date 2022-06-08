#ifndef EVENT_ARG_TYPE
    #error EVENT_ARG_TYPE need to be defined with a type specified (Ex. int, long, double, float, struct TestStruct, ...)
    #define EVENT_ARG_TYPE int32_t
#endif

#ifndef INDEX_TYPE
    #define INDEX_TYPE int64_t
#endif
#include <stdbool.h>

#define MAKE_EVENT_ARG_NAME(x) event_arg_ ## x
#define EVENT_ARG_NAME(x) MAKE_EVENT_ARG_NAME(x)
#define EVENT_ARG EVENT_ARG_NAME(EVENT_ARG_TYPE)

#define MAKE_EVENT_CALLBACK_NAME(x) callback_## x
#define EVENT_CALLBACK_NAME(x) MAKE_EVENT_CALLBACK_NAME(x)
typedef void (*EVENT_CALLBACK_NAME(EVENT_ARG_TYPE))(EVENT_ARG_TYPE arg);

#define MAKE_EVENT_NAME(x) event_ ## x
#define EVENT_NAME(x) MAKE_EVENT_NAME(x)
#define EVENT EVENT_NAME(EVENT_ARG_TYPE)

typedef struct EVENT *EVENT;
#include <stdint.h>
#include "include/data/pregen/list_intptr_t.h"

#define MAKE_EVENT_NEW_NAME(x) event_new_ ## x
#define EVENT_NEW_NAME(x) MAKE_EVENT_NEW_NAME(x)
EVENT EVENT_NEW_NAME(EVENT)()
{
    return (EVENT)list_intptr_t_new();
}
#undef EVENT_NEW_NAME
#undef MAKE_EVENT_NEW_NAME

#define MAKE_EVENT_DESTROY_NAME(x) event_destroy_ ## x
#define EVENT_DESTROY_NAME(x) MAKE_EVENT_DESTROY_NAME(x)
void EVENT_DESTROY_NAME(EVENT)(EVENT event)
{
    list_intptr_t_destroy((list_intptr_t*)event);
}
#undef EVENT_DESTROY_NAME
#undef MAKE_EVENT_DESTROY_NAME

#define MAKE_EVENT_INVOKE_NAME(x) event_invoke_ ## x
#define EVENT_INVOKE_NAME(x) MAKE_EVENT_INVOKE_NAME(x)
void EVENT_INVOKE_NAME(EVENT)(EVENT event, EVENT_ARG_TYPE arg)
{
    list_intptr_t* list = ((list_intptr_t*)event);
    for (INDEX_TYPE i = 0; i < list->size; ++i)
    {
        ((EVENT_CALLBACK_NAME(EVENT_ARG_TYPE))list->items[i])(arg);
    }
}
#undef EVENT_INVOKE_NAME
#undef MAKE_EVENT_INVOKE_NAME

#define MAKE_EVENT_ADD_NAME(x) event_add_ ## x
#define EVENT_ADD_NAME(x) MAKE_EVENT_ADD_NAME(x)
void EVENT_ADD_NAME(EVENT)(EVENT event, EVENT_CALLBACK_NAME(EVENT_ARG_TYPE) callback)
{
    list_intptr_t_add((list_intptr_t*)event, (intptr_t) callback);
}
#undef EVENT_ADD_NAME
#undef MAKE_EVENT_ADD_NAME

#define MAKE_EVENT_REMOVE_NAME(x) event_remove_ ## x
#define EVENT_REMOVE_NAME(x) MAKE_EVENT_REMOVE_NAME(x)
void EVENT_REMOVE_NAME(EVENT)(EVENT event, EVENT_CALLBACK_NAME(EVENT_ARG_TYPE) callback)
{
    list_intptr_t_remove_item((list_intptr_t*)event, (intptr_t) callback);
}
#undef EVENT_REMOVE_NAME
#undef MAKE_EVENT_REMOVE_NAME

#define MAKE_EVENT_CLEAR_NAME(x) event_clear_ ## x
#define EVENT_CLEAR_NAME(x) MAKE_EVENT_CLEAR_NAME(x)
void EVENT_CLEAR_NAME(EVENT)(EVENT event)
{
    list_intptr_t_clear((list_intptr_t*)event);
}
#undef EVENT_CLEAR_NAME
#undef MAKE_EVENT_CLEAR_NAME
#undef EVENT_ARG_TYPE