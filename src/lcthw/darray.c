#include <lcthw/darray.h>
#include <assert.h>

// 带安全地检查上界的set
int darray_set(DArray *array, unsigned int i, void *elem){
    check(i < array->max, "darray attempt to set past max");
    if(i > array->end){
        array->end = i;
    }
    array->contents[i] = elem;
    return 0;
error:
    return -1;
}

// 带安全地检查上界的get
void *darray_get(DArray *array, unsigned int i){
    check(i < array->max, "darray attempt to get past max");
    return array->contents[i];
error:
    return NULL;
}

// 带安全地检查上界的remove
void *darray_remove(DArray *array, unsigned int i){
    check(i < array->max, "darray attempt to remove past max");
    void *elem = darray_get(array, i);

    array->contents[i] = NULL;

    return elem;
error:
    return NULL;
}

// 按照darray所记录的element_size新建一个项并返回指针
void *darray_new(DArray *array){
    check(array->element_size > 0, "Can't use darray_new on 0 size darrays.");

    return calloc(1, array->element_size);
error:
    return NULL;
}

// 创建并初始化darray
DArray *darray_create(size_t element_size, size_t initial_max){
    DArray *ret = malloc(sizeof(DArray));
    check_mem(ret);

    ret->contents = calloc(initial_max, sizeof(void*));
    check_mem(ret->contents);

    ret->max = initial_max;
    ret->end = 0;
    ret->element_size = element_size;
    ret->expand_rate = DEFAULT_EXPAND_RATE;

    return ret;
error:
    if(ret) free(ret); // 分配contents失败时需要回收darray本身
    return NULL;
}

// 释放各元素的指针
void darray_clear(DArray *array){
    unsigned int i = 0;
    if(array->element_size > 0){
        for(i = 0; i < array->end; ++i){
            darray_remove(array, i);
        }
    }
}

// 释放contents指针数组本身；注意darray本身直接用free()即可
void darray_destroy(DArray *array){
    if(array->max > 0){
        if(array->contents){
            free(array->contents);
        }
    }
}

// 用于重分配darray储存大小的内部函数
static inline int darray_resize(DArray *array, unsigned int new_size){
    check(new_size > 0, "new size must be a positive number");

    void **contents = realloc(array->contents, new_size * sizeof(void*));
    check_mem(contents);

    array->contents = contents;
    array->max = new_size;

    return 0;
error:
    return -1;
}

// 按expand_rate扩展一次darray大小
int darray_expand(DArray *array){
    unsigned int old_size = array->max;

    check(darray_resize(array, array->max + array->expand_rate) == 0, "Failed in resizing darray");
    memset(array->contents + old_size, 0, array->expand_rate); // 新增部分要用0填充

    return 0;
error: return -1;
}

// 收缩darray，最小不会小于expand_rate
int darray_contract(DArray *array){
    if(array->end < array->expand_rate){
        return darray_resize(array, array->expand_rate + 1);
    }else{
        return darray_resize(array, array->end + 1);
    }
}

// 向darray尾部新增一项
int darray_push(DArray *array, void *elem){
    if(array->end >= array->max){ // 超限则需要一次扩展操作
        darray_expand(array);
    }

    //check_mem(array->contents[array->end]);
    array->end++;
    return darray_set(array, array->end - 1, elem); // 因为已经让end+1，所以不要忘记取-1
}

// 弹出darray尾部的一项
void *darray_pop(DArray *array){
    void *pop = darray_remove(array, array->end - 1);

    // check_mem(pop); 
    array->end--;

    // 判断是否收缩的逻辑(有疑问)
    if(array->end > array->expand_rate && array->end % array->expand_rate){
        check(!darray_contract(array), "Failed in contracting darray");
    }
    return pop;

error: return NULL; 
}

// 两件事一起做，彻底销毁一个darray的所有东西
void darray_clear_destroy(DArray *array){
    darray_clear(array);
    darray_destroy(array);
}

