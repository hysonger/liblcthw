#ifndef _lcthw_darray_h
#define _lcthw_darray_h

#include <stdlib.h>
#include <assert.h>
#include <lcthw/dbg.h>

typedef struct DArray{
    unsigned int end; // =count !!!
    unsigned int max;
    size_t element_size;
    size_t expand_rate;
    void **contents; // 指向指针的指针
} DArray;

DArray *darray_create(size_t element_size, size_t initial_max);

void darray_destroy(DArray *array);

void darray_clear(DArray *array);

int darray_expand(DArray *array);

int darray_contract(DArray *array);

int darray_push(DArray *array, void *elem);

void *darray_pop(DArray *array);

void darray_clear_destroy(DArray *array);

#define darray_last(A) ((A)->contents[(A)->end - 1])
#define darray_first(A) ((A)->contents[0])
// 我认为底下这几个b没有用，还不如自己写->来的快
//#define darray_end(A) ((A)->end)
//#define darray_count(A) darray_end(A)
//#define darray_max(A) ((A)->max)

#define DEFAULT_EXPAND_RATE 300

// 不同于书，我认为这四个内部宏应当暴露给外部，代码已移动至darray.c

int darray_set(DArray *array, unsigned int i, void *elem);

void *darray_get(DArray *array, unsigned int i);

void *darray_remove(DArray *array, unsigned int i);

void *darray_new(DArray *array);

#endif