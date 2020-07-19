#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "exception.h"
#include "mem.h"

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

#define NDESCRIPTORS                 512
#define MEMCHK_HASH(p, t)  ((((unsigned long) (p)) >> 3) & (sizeof(t)/sizeof((t)[0]) - 1))
#define MEMCHK_IS_VALID(ptr)         (((unsigned long) ptr) % (sizeof(union align)) != 0 \
                                      || (bp = find(ptr)) == NULL || bp->free)
#define MEM_ROUND_UP(nbytes)        (((nbytes + sizeof(union align) - 1) / (sizeof(union align)))*(sizeof(union align)))
#define MEM_CHK_NALLOC              ((4096 + sizeof(union align) - 1) / (sizeof(union align))) * (sizeof(union align))

//data

struct descriptor
{
    struct descriptor *free;
    struct descriptor *link;
    const void *ptr;
    long size;
    const char *file;
    int line;
} *htab[2048];

typedef struct descriptor descriptor_t;

descriptor_t freelist = { &freelist, NULL, NULL, 0, __FILE__, 0 };

#ifdef MEM_CHK

static descriptor_t *find(const void *ptr)
{
    descriptor_t *bp = htab[MEMCHK_HASH(ptr, htab)];
    while (bp && bp->ptr != ptr)
    {
        bp = bp->link;
    }
    return bp;
}

void Mem_free(void *ptr, const char *file, int line)
{
    if(ptr)
    {
        descriptor_t *bp;

        /* buffer size point by ptr can be divide not ptr */
        /*if(((unsigned long) ptr) % (sizeof(union align)) != 0)
        {
            Except_raise(&Mem_Failed, file, line);
        }*/

        if((bp = find(ptr)) == NULL || bp->free)
        {
            Except_raise(&Mem_Failed, file, line);
        }

        bp->free = freelist.free;
        freelist.free = bp;

    }
}

void *Mem_resize(void *ptr, long nbytes, const char *file, int line)
{
    descriptor_t *bp;
    void *newptr;

    assert(ptr);
    assert(nbytes);
    if(((unsigned long) ptr) % (sizeof(union align)) != 0
            || (bp = find(ptr)) == NULL || bp->free){
        Except_raise(&Mem_Failed, file, line);
    }

    newptr = Mem_alloc(nbytes, file, line);
    memcpy(newptr, ptr, nbytes < bp->size ? nbytes : bp->size);
    Mem_free(ptr, file, line);
    return newptr;
}

void *Mem_calloc(long count, long nbytes, const char *file, int line)
{
    void *ptr;

    assert(count > 0);
    assert(nbytes > 0);

    ptr = Mem_alloc(count*nbytes, file, line);
    memset(ptr, '\0', count*nbytes);
    return ptr;
}

static descriptor_t *dalloc(void *ptr, long size, const char *file, int line)
{
    static descriptor_t *avail;
    static int nleft;

    if(nleft <= 0)
    {
        avail = malloc(NDESCRIPTORS*sizeof(*avail));
        if(avail == NULL)
        {
            return NULL;
        }
        nleft = NDESCRIPTORS;
    }
    avail->ptr = ptr;
    avail->size = size;
    avail->file = file;
    avail->line = line;
    avail->free = avail->link = NULL;
    nleft--;
    return avail++;
}

void *Mem_alloc(long nbytes, const char *file, int line)
{
    descriptor_t *bp;
    void *ptr;

    assert(nbytes > 0);
    nbytes = MEM_ROUND_UP(nbytes);
    for(bp = freelist.free; bp != NULL; bp = bp->free)
    {
        if(bp->size > nbytes)
        {
            bp->size -= nbytes;
            ptr = (char *)bp->ptr + bp->size;
            if((bp = dalloc(ptr, nbytes, file, line)) != NULL)
            {
                unsigned h = MEMCHK_HASH(ptr, htab);
                htab[h] = bp;
                return ptr;
            }
            else
            {
                RAISE(Mem_Failed);
            }
        }

        if(bp == &freelist)
        {
            descriptor_t *newptr;
            if((ptr = malloc(nbytes + MEM_CHK_NALLOC)) == NULL
             || (newptr = dalloc(ptr, nbytes + MEM_CHK_NALLOC,
                                 __FILE__, __LINE__)) == NULL)
            {
                RAISE(Mem_Failed);
            }
            newptr->free = freelist.free;
            freelist.free = newptr;
        }
    }
    assert(0);
    return NULL;
}

#endif

