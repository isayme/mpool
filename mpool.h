#ifndef _MPOOL_H
#define _MPOOL_H

#include <pthread.h>

// check the validity of the memory to be freed;
#define MPOOL_ENABLE_PTR_CHECK 1

// align blksize for faster memory access;
#define MPOOL_ENABLE_BLKSIZE_ALIGN 1

typedef struct mpool {
    size_t nblks;
    size_t blksize;
    
    size_t nfree;
    void *freelist;
    
    void *addr;
#if MPOOL_ENABLE_PTR_CHECK > 0
    void *addrend;
#endif
    
    pthread_mutex_t mutex;
} mpool;

mpool *mpool_create(size_t nblks, size_t blksize);
int mpool_destroy(mpool *mp);

void *mpool_malloc(mpool *mp);
int mpool_free(mpool *mp, void *ptr);

#endif
