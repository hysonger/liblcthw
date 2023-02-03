#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

// 当#define NDEBUG时，debug不会打印出错信息，用于区分调试和正式版
#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n",\
    __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr,\
    "[ERROR] (%s:%d: errno: %s) " M "\n",\
    __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr,\
    "[WARN] (%s:%d: errno: %s) " M "\n",\
    __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr,\
    "[INFO] (%s:%d) " M "\n",\
    __FILE__, __LINE__, ##__VA_ARGS__)

// 检查条件，为假则报错并跳到error
#define check(A, M, ...) if(!(A)) {\
    log_err(M, ##__VA_ARGS__); errno = 0; goto error; }

// 表示“不该运行”，运行到此即报错并跳到error
#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__);\
    errno = 0; goto error; }

// 检查空指针，主要用于malloc后检查是否成功分配
#define check_mem(A) check((A), "Out of memory.")

// check和debug的组合
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__);\
    errno = 0; goto error; }

#endif