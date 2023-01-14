// 双链表数据类型
#ifndef _lcthw_list_h
#define _lcthw_list_h

#include <stdlib.h>

struct ListNode;

typedef struct ListNode{
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
} ListNode;

typedef struct List{
    int count;
    ListNode *first;
    ListNode *last;
} List;

List *list_create();
void list_destroy(List *list);
void list_clear(List *list);
void list_clear_destroy(List *list);

#define list_count(A) ((A)->count)
#define list_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define list_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

void list_push(List *list, void *value);
void *list_pop(List *list);

void list_unshift(List *list, void *value);
void *list_shift(List *list);

void *list_remove(List *list, ListNode *node);

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL; \
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif