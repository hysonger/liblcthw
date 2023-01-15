#include "minunit.h"
#include <lcthw/darray.h>

static DArray *array = NULL;
static int *val1 = NULL;
static int *val2 = NULL;

char *test_create(){
    array = darray_create(sizeof(int), 100);
    mu_assert(array, "DArray_create failed.");
    mu_assert(array->contents, "contents are wrong in darray");
    mu_assert(array->end == 0, "end isn't at the right spot");
    mu_assert(array->element_size == sizeof(int), "element size is wrong.");
    mu_assert(array->max == 100, "wrong max length on initial size");

    return NULL;

}

char *test_destroy(){
    darray_destroy(array);

    return NULL;
}

char *test_new(){
    val1 = darray_new(array);
    mu_assert(val1, "failed to make a new element");

    val2 = darray_new(array);
    mu_assert(val2, "failed to make a new element");

    mu_assert(sizeof(*val1) == sizeof(int) && sizeof(*val2) == sizeof(int), "new element size is not correct");

    return NULL;
}

char *test_set(){
    darray_set(array, 0, val1);
    darray_set(array, 1, val2);

    // mu_assert(array->contents[0] == val1 && array->contents[1] == val2, "set operation failed");

    return NULL;
}

char *test_get(){
    mu_assert(darray_get(array, 0) == val1, "Wrong first value");
    mu_assert(darray_get(array, 1) == val2, "Wrong second value");

    return NULL;
}

char *test_remove(){
    int *val_check = darray_remove(array, 0);
    mu_assert(val_check, "Should not get NULL");
    mu_assert(*val_check == *val1, "Should get the first value");
    mu_assert(darray_get(array, 0) == NULL, "Removed item should be NULL")
    free(val_check);

    val_check = darray_remove(array, 1);
    mu_assert(val_check, "Should not get NULL");
    mu_assert(*val_check == *val2, "Should get the second value");
    mu_assert(darray_get(array, 1) == NULL, "Removed item should be NULL")
    free(val_check);

    return NULL;
 
}

char *test_expand_contract(){
    int old_max = array->max;
    darray_expand(array);
    mu_assert(array->max == old_max + array->expand_rate, "Wrong size after expand."); 
    // 注意size_t是一个无符号数！！！
    // 无符号数和有符号数运算，有符号数会被强制转为无符号，结果为无符号数，则比较相等时，会出现有符号!=无符号

    darray_contract(array);
    mu_assert(array->max == array->expand_rate + 1, "Should stay at the expand_rate at least.");
    
    darray_contract(array);
    mu_assert(array->max == array->expand_rate + 1, "Should stay at the expand_rate at least.");

    return NULL;
}

char *test_push_pop(){
    int i = 0;
    for(i = 0; i < 1000; ++i){
        int *val = darray_new(array);
        *val = i * 333;
        darray_push(array, val);
    }

    mu_assert(array->max == 1201, "Wrong max size.");

    for(i = 999; i >= 0; --i){
        int *val = darray_pop(array);
        mu_assert(val, "Shouldn't get a NULL.");
        mu_assert(*val == i * 333, "Wrong value.");
        free(val);
    }

    return NULL;
}

char *all_tests(){
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_new);
    mu_run_test(test_set);
    mu_run_test(test_get);
    mu_run_test(test_remove);
    mu_run_test(test_expand_contract);
    mu_run_test(test_push_pop);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);