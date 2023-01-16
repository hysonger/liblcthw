#include <lcthw/darray_algos.h>
//#include <stdlib.h>

int darray_qsort(DArray *array, darray_compare cmp){
    qsort(array->contents, array->end, sizeof(void *), cmp);
    return 0;
}

// gcc的标准库压根就没有heapsort和mergesort！！！难道是clang独占？

int darray_heapsort(DArray *array, darray_compare cmp){
    qsort(array->contents, array->end, sizeof(void *), cmp);
    return 0;
}

int darray_mergesort(DArray *array, darray_compare cmp){
    qsort(array->contents, array->end, sizeof(void *), cmp);
    return 0;
}