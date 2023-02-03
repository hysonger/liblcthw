// 这里和书上不同，采用随机生成整数数组作为测试对象

#include "minunit.h"
#include <lcthw/darray_algos.h>
#include <time.h>

int testcmp(int **a, int **b){
    //check(*a && *b, "given NULL in testcmp()! ");
    //return strcmp(*a, *b);
//error: exit(-1); // 很暴力！！！

    return **a - **b;
}

DArray *create_words(){
    DArray *result = darray_create(0, 200000);
    //char *words[] = {"faadadva", "89acja", "ia0adkc", "c9a9ia", "3ja9aaaaaaaaaa"};
    int i = 0, *word;

    srand(time(0));
    for(i = 0; i < 200000; ++i){
        //darray_push(result, words[i]);
        word = malloc(sizeof(int));
        *word = rand() % 1000000;
        darray_push(result, word);
    }

    return result;
}

int is_sorted(DArray *array){
    unsigned i = 0;
    int *a, *b;

    for(i = 0; i < array->end - 1; ++i){
        a = darray_get(array, i);
        b = darray_get(array, i + 1);
        if(*a > *b){
            //debug("%d(%d), %d(%d) is not sorted", *a, i, *b, i + 1);
            return 0;
        }
    }

    return 1;
}

void print_words(DArray *array){
    int *a;

    for(unsigned int i = 0; i < array->end; ++i){
        a = darray_get(array, i);
        printf("%5d ", *a);
    }
    putchar('\n');
}

char *run_sort_test(int (*func) (DArray *, darray_compare), const char *name){
    clock_t a;

    DArray *words = create_words();
    mu_assert(!is_sorted(words), "Words should not be sorted.");

    // print_words(words);


    debug("--- Testing %s sorting algorithm", name);
    a = clock();
    int rc = func(words, (darray_compare) testcmp);
    printf("\t%s used %lu μs\n", name, clock() - a);

    // print_words(words);

    mu_assert(rc == 0, "sort failed");
    mu_assert(is_sorted(words), "didn't sort it");

    darray_clear_destroy(words);

    return NULL;
}

char *test_qsort(){
    return run_sort_test(darray_qsort, "qsort");
}

char *test_heapsort(){
    return run_sort_test(darray_heapsort, "heapsort");
}

char *test_mergesort(){
    return run_sort_test(darray_mergesort, "mergesort");
}

char *test_find(){
    DArray *words = create_words();
    int pos;
    int a = -10;

    darray_qsort(words, (darray_compare)testcmp); // 二分搜索只能用于排好序的对象

    srand(time(0));
    pos = rand() % words->end;

    // pos强制转换为无符号不可省略，否则结果不可预测！
    // 有符号与无符号数自动转换：1 运算：有符号变为无符号 2 比较 3 赋值 4 传参
    mu_assert(darray_find(words, darray_get(words, (unsigned)pos), (darray_compare)testcmp) == pos, "failed finding a existed item");

    //b = &a;
    mu_assert(darray_find(words, &a, (darray_compare)testcmp) == -1, "shouldn't find an unknown item");

    return NULL;
}

char *all_tests(){
    mu_suite_start();

    mu_run_test(test_qsort);
    mu_run_test(test_heapsort);
    mu_run_test(test_mergesort);
    mu_run_test(test_find);

    return NULL;
}

RUN_TESTS(all_tests);