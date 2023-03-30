#ifndef _lcthw_hashmap_h
#define _lcthw_hashmap_h

#include <stdint.h>
#include <lcthw/darray.h>

#define DEFAULT_BUCKET_NUMBER 100


typedef int (*hashmap_compare)(void *a, void *b);
typedef uint32_t (*hashmap_hash)(void *key);

// 散列表结构，包含若干桶，以及比较和哈希函数指针
typedef struct Hashmap{
    DArray *buckets; // 桶的默认数量见DEFAULT_BUCKET_NUMBER，每个桶为一个含HashmapNode的DArray，双层数组
    hashmap_compare compare;
    hashmap_hash hash;
} Hashmap;

typedef struct HashmapNode{
    void *key;
    void *data;
    uint32_t hash; // 记录一份hash值，以快速比对
} HashmapNode;

#endif