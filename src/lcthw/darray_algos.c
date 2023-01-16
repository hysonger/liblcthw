// 动态数组算法

#include <lcthw/darray_algos.h>

// 负责执行递归任务的内部函数体
static int __darray_qsort(DArray *array, unsigned int low, unsigned int high, darray_compare cmp){
    void *pivot = darray_get(array, low);
    void *pi = darray_get(array, low), *pj = darray_get(array, high);
    unsigned int i = low, j = high;

    // 交换和划分操作
    if(low < high){
        while(i < j){
            // 右边的值都应>=pivot，若有<，则停下，赋值
            while(i < j && cmp(&pj, &pivot) >= 0){
                j--;
                pj = darray_get(array, j);
            }
            darray_set(array, i, pj);

            // 左边的值都应<=pivot，若有>，则停下，赋值
            pi = darray_get(array, i);
            while(i < j && cmp(&pi, &pivot) <= 0){
                i++;
                pi = darray_get(array, i);
            }
            darray_set(array, j, pi);
        }
        // 最后要把pivot归位
        darray_set(array, i, pivot);

        // 注意这里有关于i和j的判断！
        // i的判断是必需的，因为是无符号数，否则i = 0时i - 1会发生下溢！
        // j的判断不是必需的，但上面变量初始化在递归终止条件前，边界条件下会报错
        return __darray_qsort(array, low, i == 0 ? 0 : i - 1, cmp) \
        || __darray_qsort(array, j < high ? j + 1 : j, high, cmp);
    }else{
        return 0;
    }
}

// 调整堆顶使其符合堆要求。head从1开始，建立大根堆
static void __darray_adjust_head(DArray *array, unsigned int head, unsigned int len, darray_compare cmp){
    void *root, *left, *right, *cur;
    unsigned int i = head, curpos;

    root = darray_get(array, head - 1);
    
    // 难点：这里直到最后才把原堆顶覆盖过去，在循环中，我们假设它“如果被覆盖到i位置，会是什么情况”
    while(i * 2 <= len){
        if(i * 2 < len){
            left = darray_get(array, i * 2 - 1);
            right = darray_get(array, i * 2);
            curpos = cmp(&left, &right) > 0 ? (i * 2) : (i * 2 + 1); // 大小根堆逻辑，选大的
        }else{
            curpos = i * 2; // 只有左子树，则只能选它
        }

        cur = darray_get(array, curpos - 1);
        if(cmp(&root, &cur) < 0){ // 大小根堆逻辑
            // 比左右节点的大者更小，则该状态仍不满足堆要求，继续向下
            darray_set(array, head - 1, cur); // 将较大值调整到堆顶上
 
            // 下一轮假设原堆顶被“交换到大者处”
            i = curpos;                                           
        }else{ // 如果比左右节点的大者更大，则可以完成交换，停止循环跳出
            break;
        }
    }

    darray_set(array, i - 1, root); // 循环中止后，将原堆顶覆盖至i处，完成交换

    //print_darray(array);
}


// 对动态数组进行快速排序
// 需要通过一个内部函数体执行递归操作
int darray_qsort(DArray *array, darray_compare cmp){
    //qsort(array->contents, array->end, sizeof(void *), cmp);
    return __darray_qsort(array, 0, array->end - 1, cmp);
    //return 0;
}

// gcc的标准库压根就没有heapsort和mergesort！！！难道是clang独占？


// 对动态数组进行堆排序，最大堆首尾交换法
int darray_heapsort(DArray *array, darray_compare cmp){
    // qsort(array->contents, array->end, sizeof(void *), cmp);
    // return 0;

    // 这里计数变量从1开始，方便实现教科书，取值设值时需-1
    unsigned int i;
    void *buff;

    // 建立堆
    for(i = array->end / 2; i >= 1; --i){ 
        // 0 ~ n / 2为非叶子节点，从后往前调整“以每个非叶子节点为根”的堆的堆顶
        __darray_adjust_head(array, i, array->end, cmp);
    }

    // 堆顶交换到后面
    for(i = array->end - 1; i > 0; --i){
        buff = darray_first(array);
        darray_set(array, 0, darray_get(array, i));
        darray_set(array, i, buff);

        // 堆长-1，再次向下调整堆
        __darray_adjust_head(array, 1, i, cmp);
    }

    return 0;
}

// 对动态数组进行归并排序
int darray_mergesort(DArray *array, darray_compare cmp){
    qsort(array->contents, array->end, sizeof(void *), cmp);
    return 0;
}