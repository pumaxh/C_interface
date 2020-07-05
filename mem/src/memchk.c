#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "exception.h"
#include "mem.h"

//chk types

#define hash(p, t)  ((unsigned long) ((p) >> 3) & (sizeof(t)/sizeof(((t)[0]) - 1)))

//data

static struct descriptor
{
    struct descriptor *free;
    struct descriptor *link;
    const void *ptr;
    long size;
    const char *file;
    int line;
} *htab[2048];

typedef struct descriptor descriptor_t;

static descriptor_t freelist = { &freelist };

static descriptor_t *find(const void *ptr)
{
    descriptor_t *bp = htab[hash(ptr, htab)];
    while (bp && bp->ptr != ptr)
    {
        bp = bp->link;
    }
    return bp;
}


