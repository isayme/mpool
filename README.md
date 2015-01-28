# mpool
A `thread safe` memory pool in linux c.

# APIs
only 4 APIs for this great(?) memory pool.

## mpool *mpool_create(size_t nblks, size_t blksize) ##
create memory pool, return the pointer to the memory pool if success otherwise return `NULL`.
`nblks` is the number of memory blocks;
`blksize` is the size of each memory block, 

## int mpool_destroy(mpool *mp) ##
destroy memory pool. return `0` if success, 
and return `-1` when any of below condition occurs:

 - NULL == mp;
 - memory pool already destroyed;

## void *mpool_malloc(mpool *mp) ##
malloc a memory block. return a pointer to the alloacted memory if success,
and return `NULL` when any of below condition occurs:

 - NULL == mp;
 - memory pool already destroyed;
 - there no free memory block left in memory pool pointed to by `mp`.

## int mpool_free(mpool *mp, void *ptr) ##
free the memory space pointed to by `ptr` back to memory pool `mp`, return `0` if success,
and return `-1` when any of below condition occurs:

- NULL == mp;
- NULL == ptr;
- memory pool already destroyed;
- the memory pool already full.


# Contact #
EMAIL: [isaymeorg@gmail.com](mailto:isaymeorg@gmail.com)
