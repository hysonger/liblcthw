// 单元测试宏

#undef NDEBUG // 不需要调试信息时，把这句注释掉
#ifndef _minunit_h
#define _minunit_h

#include <stdio.h>
#include <lcthw/dbg.h>
#include <stdlib.h>

// 只是简单的开始语句
#define mu_suite_start() char *message = NULL

// 用于单元测试的断言宏
#define mu_assert(test, message) if(!(test)){\
    log_err(message); return message;}

// 运行一个测试并回显测试运行进度
#define mu_run_test(test) debug("\n-----%s", " " #test); \
    message = test(); tests_run++; if(message) return message;

// 单元测试文件的main函数宏
#define RUN_TESTS(name) int main(int argc, char *argv[]) {\
    if(argc == 1){\
    debug("----- RUNNING: %s", argv[0]);\
    printf("----\nRUNNING: %s\n", argv[0]);\
    char *result = name();\
    if(result != 0){\
        printf("FAILED: %s\n", result);\
    }else{\
        printf("ALL TESTS PASSED\n");\
    }\
    printf("Tests run: %d\n", tests_run);\
    exit(result != 0);\
    }\
}

int tests_run;

#endif