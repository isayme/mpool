#include <stdio.h>
#include <string.h>
#include "mpool.h"

#define LOG(format, ...) \
    do { \
        fprintf(stderr, "[%s:%d] " format, \
            __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)

int main(int argc, char **argv)
{
    mpool *mp = NULL;
    int i;
    char *(p[10]);

    mp = mpool_create(5, 10);
    if (NULL == mp) {
        LOG("mpool_create failed!\n");
        return -1;
    } else {
        LOG("mpool_create success!\n");
    }

    for (i = 0; i < 10; i++) {
        p[i] = mpool_malloc(mp);
        if (p[i]) {
            LOG("mpool_malloc [%d] success!\n", i);
            memset(p[i], 0, 10);
        } else {
            LOG("mpool_malloc [%d] failed!\n", i);
        }
    }

    for (i = 0; i < 10; i++) {
        if (0 == mpool_free(mp, p[i])) {
            LOG("mpool_free [%d] success!\n", i);
        } else {
            LOG("mpool_free [%d] failed!\n", i);
        }
    }

    if (mpool_destroy(mp)) {
        LOG("mpool_destroy failed!\n");
        return -1;
    } else {
        LOG("mpool_destroy success!\n");
    }

    if (mpool_destroy(mp)) {
        LOG("mpool_destroy failed!\n");
        return -1;
    } else {
        LOG("mpool_destroy success!\n");
    }

    return 0;
}
