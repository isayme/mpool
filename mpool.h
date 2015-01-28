#ifndef _MPOOL_H
#define _MPOOL_H

#include <pthread.h>

typedef struct mpool {
    size_t nblks;
    size_t blksize;
    
    size_t nfree;
    void *freelist;
    
    void *addr;
    
    pthread_mutex_t mutex;
} mpool;

mpool *mpool_create(size_t nblks, size_t blksize);
int mpool_destroy(mpool *mp);

void *mpool_malloc(mpool *mp);
int mpool_free(mpool *mp, void *ptr);

#endif
