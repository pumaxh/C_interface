#ifndef ARENA_H
#define ARENA_H

#include "exception.h"

extern const Except_T Arena_NewFailed;
extern const Except_T Arena_Failed;

#define T Arena_T

struct Arena_T{
    struct Arena_T *prev;
    char *avail;
    char *limit;
};

typedef struct Arena_T Arena_T;

extern T *Arena_new(void);
extern void  Arena_dispose(T **ap);
extern void *Arena_alloc(T *arena, long nbytes, const char *file, int line);
extern void *Arena_calloc(T *arena, long count, long nbytes, const char *file, int line);
extern void Arena_free(T *arena);
#undef T
#endif // ARENA_H
