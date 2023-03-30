#include "minunit.h"
#include <lcthw/bstrlib.h>
#include <string.h>

char *text = "to hello world or not, that is a question";

char *test_copy(){
    bstring dest;

    dest = bfromcstr(text);
    mu_assert((unsigned)blength(dest) == strlen(text), "Incorrect length of destination(bfromcstr)");

    dest = blk2bstr(dest, 12);
    mu_assert(blength(dest) == 12, "Incorrect length of destination(blk2bstr)");

    return NULL;
}


char *all_tests(){
    mu_suite_start();

    mu_run_test(test_copy);

    return NULL;
}

RUN_TESTS(all_tests);