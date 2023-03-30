// 双链表算法

#include <lcthw/list_algos.h>
#include <lcthw/dbg.h>

inline void exchange_listnode(ListNode *a, ListNode *b){
    void *buffer = a->value;

    check(a->next == b, "a is not followed by b in exchanging listnode.");

    a->value = b->value;
    b->value = buffer;
error:
    return;
}

// 对链表的冒泡排序
int list_bubble_sort(List *list, list_compare cmp){
    int i, j; // 加计数，作为双保险
    ListNode *p, *cur;
    short flag = 0; // 记录遍历一轮中是否发生过交换

    // 从前向后冒泡，有序部分集合在尾部
    for(i = 0, p = list->last; i < list_count(list) - 1 && p != list->first; ++i, p = p->prev){
        for(j = 0, cur = list->first; j < list_count(list) - i - 1 && cur != p; ++j, cur = cur->next){
            if(cur->next){
                if(cmp(cur->value, cur->next->value) > 0){
                    exchange_listnode(cur, cur->next);
                    flag = 1;
                }
            }else{break;}
        }
        if(flag == 0){ // 如果未发生交换，说明表已有序，可提前结束
            return 0;
        }else{
            flag = 0;
        }
    }
    return 0;
}

// 两个子列的合并操作
List *list_merge(List *a, List *b, list_compare cmp){
    List *result = list_create();

    while(a->first || b->first){
        if(a->first && b->first){ // 二者比大小
            if(cmp(a->first->value, b->first->value) < 0){
                list_push(result, list_shift(a));
            }else{
                list_push(result, list_shift(b));
            }
        }else{ // 只有一列有项，直出
            if(a->first){
                list_push(result, list_shift(a));
            }else{
                list_push(result, list_shift(b));
            }
        }
    }
    list_destroy(a); list_destroy(b); // 第一个内存回收注意点
    return result;
}

// 对链表进行归并排序
// 递归过程：对左子列归并排序，对右子列归并排序，合并
List *list_merge_sort(List *list, list_compare cmp){
    int middle = list_count(list) / 2; // 分割点
    List *up = list_create(), *down = list_create();
    ListNode *p = list->first;

    // 只有一项的子列无需排序，注意这里是递归必需的终止条件！！！
    if(list_count(list) <= 1){
        return list;
    }

    for(int i = 0; i < list_count(list) && p; ++i){
        if(i < middle) {
            list_push(up, p->value);
        }else{
            list_push(down, p->value);
        }
        p = p->next;
    }

    // 这里按书上，其实该把原始的up和down销毁（只要不是count=1），这是第二个内存回收注意点
    return list_merge(list_merge_sort(up, cmp), list_merge_sort(down, cmp), cmp); 
}

