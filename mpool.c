#include <stdlib.h>
#include <string.h>

#include "mpool.h"

mpool *mpool_create(size_t nblks, size_t blksize)
{
    mpool *mp = NULL;
    void **pplink = NULL;
    void *pblk = NULL;
    size_t i;
    
    if (0 >= nblks) {
        return NULL;
    }

    blksize = (blksize > sizeof(void *)) ? blksize : sizeof(void *);  
#if MPOOL_ENABLE_BLKSIZE_ALIGN > 0
    blksize = (blksize + sizeof(void *) - 1) / sizeof(void *) * sizeof(void *);
#endif
    
    mp = calloc(1, sizeof(mpool));
    if (NULL == mp) {
        return NULL;
    }
    
    pthread_mutex_init(&mp->mutex, NULL);
    
    mp->nblks = nblks;
    mp->nfree = nblks;
    mp->blksize = blksize;
    mp->addr = malloc(nblks * blksize);
    if (NULL == mp->addr) {
        free(mp);
        return NULL;
    }

#if MPOOL_ENABLE_PTR_CHECK > 0
    mp->addrend = mp->addr + nblks * blksize;
#endif

    mp->freelist = mp->addr;
    
    pblk = mp->addr;
    pplink = (void **)pblk;
    for (i = 1; i < nblks; i++) {
        pblk += blksize;
        *pplink = pblk;
        pplink = (void **)pblk;
    }
    *pplink = (void *)0;

    return mp;
}

int mpool_destroy(mpool *mp)
{
    if (NULL == mp
        || NULL == mp->addr
        || 0 != pthread_mutex_lock(&mp->mutex)) {
        return -1;
    }

    free(mp->addr);
    mp->addr = NULL;
    pthread_mutex_unlock(&mp->mutex);
    pthread_mutex_destroy(&mp->mutex);
    free(mp);
    
    return 0;
}

void *mpool_malloc(mpool *mp)
{
    void *ptr = NULL;
    
    if (NULL == mp
        || NULL == mp->addr
        || 0 != pthread_mutex_lock(&mp->mutex)) {
        return NULL;
    }

    if (mp->nfree > 0) {
        ptr = mp->freelist;
        mp->freelist = *(void **)ptr;
        mp->nfree--;
        
        pthread_mutex_unlock(&mp->mutex);
        return ptr;
    }
    
    pthread_mutex_unlock(&mp->mutex);
    return NULL;
}

int mpool_free(mpool *mp, void *ptr)
{
    if (NULL == mp
        || NULL == mp->addr
        || NULL == ptr
#if MPOOL_ENABLE_PTR_CHECK > 0
        || mp->addr > ptr
        || mp->addrend <= ptr
        || 0 != ((ptr - mp->addr) % mp->blksize)
#endif
        || 0 != pthread_mutex_lock(&mp->mutex)) {
        return -1;
    }

    if (mp->nfree >= mp->nblks) {
        pthread_mutex_unlock(&mp->mutex);
        return -1;
    }
    
    *(void **)ptr = mp->freelist;
    mp->freelist = ptr;
    mp->nfree++;
    
    pthread_mutex_unlock(&mp->mutex);
    return 0;
}
