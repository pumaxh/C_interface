#include <stdarg.h>
#include <stddef.h>
#include <assert.h>

#include "mem.h"
#include "list.h"

#define T List_T
#define PT List_T_p

//function
PT List_push(PT list, void *x)
{
    PT p;
    NEW(p);
    p->first = x;
    p->rest  = list;
    return p;
}

PT List_list(void *x, ...)
{
    va_list ap;
    PT list, *p = &list;

    va_start(ap, x);
    for(; x; x = va_arg(ap, void *))
    {
        NEW(*p);
        (*p)->first = x;
        p = &(*p)->rest;
    }
    *p = NULL;
    va_end(ap);
    return list;
}

PT List_append(PT list, PT tail)
{
    PT *p = &list;
     while(*p)
     {
         p = &(*p)->rest;
     }
     *p = tail;
     return list;
}

PT List_copy(PT list)
{
    PT head, *p = &head;

    for(; list; list= list->rest)
    {
        NEW(*p);
        (*p)->first = list->first;
        p = &((*p)->rest);
    }
    *p = NULL;
    return head;
}

PT List_pop(PT list, void **x)
{
    if(list)
    {
        PT head = list->rest;
        if(x)
        {
            *x = list->first;
        }
        FREE(list);
        return head;
    }
    else
        return list;
}

PT List_revers(PT list)
{
    PT head = NULL, next;

    for(; list; list = next)
    {
        next = list->rest;
        list->rest = head;
        head = list;
    }
    return head;
}

int List_length(PT list)
{
    int n;
    for(n = 0; list; list = list->rest)
    {
        n++;
    }
    return n;
}

void List_free(PT * list)
{
    PT next;

    assert(list);
    for(; *list; *list = next)
    {
        next = (*list)->rest;
        FREE(*list);
    }
}

void List_map(PT list,
              void apply(void **x, void *cl),
              void *cl)
{
    assert(apply);
    for(;list; list = list->rest)
    {
        apply(&(list->first), cl);
    }
}

void **List_toArray(PT list, void *end)
{
    int i, n = List_length(list);
    void **array = ALLOC((n+1)*sizeof(*array));
    for(i=0; i < n; i++)
    {
        array[i] = list->first;
        list = list->rest;
    }
    array[i] = end;
    return array;
}
