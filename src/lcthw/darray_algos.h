#ifndef _lcthw_darray_algos_h
#define _lcthw_darray_algos_h

#include <lcthw/darray.h>
#include <stdlib.h>

typedef int (*darray_compare) (const void *a, const void *b);

int darray_qsort(DArray *array, darray_compare cmp);

int darray_heapsort(DArray *array, darray_compare cmp);

int darray_mergesort(DArray *array, darray_compare cmp);

#endif