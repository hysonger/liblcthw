#ifndef _lcthw_radixmap_h
#define _lcthw_radixmap_h

#include <stdint.h>
#include <stdlib.h> // size_t在这里面！
#include <stdio.h>
#include <assert.h>
#include <lcthw/dbg.h>

typedef union RMElement {
    uint64_t raw;
    struct{
        uint32_t key;
        uint32_t value;
    }data;
}RMElement;


typedef struct RadixMap{
    size_t max;
    size_t end; // 有一项=从1开始
    uint32_t counter;
    RMElement *contents;
    RMElement *temp;
}RadixMap;


RadixMap *radixmap_create(size_t max);


void radixmap_destroy(RadixMap *map);


void radixmap_sort(RadixMap *map);


RMElement *radixmap_find(RadixMap *map, uint32_t key);


int radixmap_add(RadixMap *map, uint32_t key, uint32_t value);


int radixmap_delete(RadixMap *map, RMElement *elem);


// 把指向64位整数x的指针转为8位，就会事实上变成一个8*8的数组！ little trick
#define ByteOf(x, y) (((uint8_t*)x)[(y)])


#endif