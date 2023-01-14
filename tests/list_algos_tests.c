#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>

char *values[] = {"3", "1", "5", "2", "4"};

#define NUM_VALUES 5

List *create_words(){
    int i = 0;
    List *words = list_create();

    for(i = 0; i < NUM_VALUES; ++i){
        list_push(words, values[i]);
    }

    return words;
}

int is_sorted(List *words){
    LIST_FOREACH(words, first, next, cur){
        if(cur->next && strcmp(cur->value, cur->next->value) > 0){
            debug("%s %s", (char *)cur->value, (char *)cur->next->value);
            return 0;
        }
    }

    return 1;
}

char *test_bubble_sort(){
    List *words = create_words();

    int rc = list_bubble_sort(words, (list_compare) strcmp);
    mu_assert(rc == 0, "Bubble sort failed.");
    mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

    rc = list_bubble_sort(words, (list_compare) strcmp);
    mu_assert(rc == 0, "Bubble sort of already sorted failed.");
    mu_assert(is_sorted(words), "Words should be sorted if already bubble sorted.");

    list_destroy(words);

    words = list_create(words);
    rc = list_bubble_sort(words, (list_compare) strcmp);
    mu_assert(rc == 0, "Bubble sort failed on empty list.");
    mu_assert(is_sorted(words), "Words should be sorted if empty.");

    list_destroy(words);

    return NULL;
}

char *test_merge_sort(){
    List *words = create_words();

    List *res = list_merge_sort(words, (list_compare) strcmp);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

    List *res2 = list_merge_sort(res, (list_compare) strcmp);
    mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
    list_destroy(res2); list_destroy(res);

    list_destroy(words);
    return NULL;
}

char *all_tests(){
    mu_suite_start();
    
    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);

    return NULL;
}

RUN_TESTS(all_tests);