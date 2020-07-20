#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "exception.h"
#include "arena.h"

const Except_T Arena_NewFailed= {"Arena Creation Failed"};
const Except_T Arena_Failed = {"Arena Allocation Failed"};

// macros 71
#define THRESHOLD  10
#define T  Arena_T

// data 70
static T *freechunks;
static int nfree;

// types 67

union align {
    int i;
    long l;
    long *lp;
    void *p;
    void (*fp)(void);
    float f;
    double d;
    long double ld;
};

union header {
  struct T b;
  union align a;
};

// function 68
T *Arena_new()
{
    T *arena = malloc(sizeof(*arena));
    if( arena == NULL)
    {
        RAISE(Arena_NewFailed);
    }
    arena->prev = NULL;
    arena->limit = arena->avail = NULL;
    return arena;
}

void Arena_dispose(T **ap)
{
    assert(ap && *ap);
    Arena_free(*ap);
    free(*ap);
    *ap = NULL;
}

void *Arena_alloc(Arena_T *arena, long nbytes, const char *file, int line)
{
    assert(arena);
    assert(nbytes > 0);

    nbytes = ((nbytes + sizeof(union align) - 1) / (sizeof(union align))) * sizeof(union align);
    while(nbytes > arena->limit - arena->avail)
    {
        T *ptr;
        char *limit;
        //a new chunk 70
        if((ptr = freechunks) != NULL)
        {
            freechunks = freechunks->prev;
            nfree--;
            limit = ptr->limit;
        }
        else
        {
            long m = sizeof(union header) + nbytes + 10*1024;
            ptr = malloc(m);
            if(ptr == NULL)
            {
                if(file == NULL)
                {
                    RAISE(Arena_Failed);
                }
                else
                {
                    Except_raise(&Arena_Failed, file, line);
                }
            }
            limit = (char *)ptr + m;
        }
        *ptr = *arena;
        arena->avail = (char *)((union header *)ptr + 1);
        arena->limit = limit;
        arena->prev = ptr;
    }
    arena->avail += nbytes;
    return arena->avail - nbytes;
}

void *Arena_calloc(T *arena, long count, long nbytes, const char *file, int line)

{
    void *ptr;

    assert(count > 0);
    ptr = Arena_alloc(arena, count*nbytes, file, line);
    memset(ptr, '\0', count*nbytes);
    return ptr;
}

/* Arena free all memery in arena */
void Arena_free(T *arena)
{
    assert(arena);
    while(arena->prev)
    {
        struct T tmp = *(arena->prev);
        // free the chunk described by arena 71
        if(nfree < THRESHOLD)
        {
            arena->prev->prev = freechunks;
            freechunks = arena->prev;
            nfree++;
            freechunks->limit = arena->limit;
        }
        else
        {
            free(arena->prev);
        }
        *arena = tmp;
    }
    assert(arena->limit == NULL);
    assert(arena->avail == NULL);
}
