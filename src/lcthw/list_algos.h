#ifndef _lcthw_list_algos_h
#define _lcthe_list_algos_h

#include <lcthw/list.h>

typedef int (*list_compare) (const void *a, const void *b); // 指向函数的指针

int list_bubble_sort(List *list, list_compare cmp); // 第二参数需要提供用于判定两项相等的函数名

List *list_merge_sort(List *list, list_compare cmp);

#endif
