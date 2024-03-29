#include "minunit.h"
#include <lcthw/radixmap.h>
#include <time.h>

static int make_map(RadixMap *map){
    size_t i = 0;

    for(i = 0; i < map->max; i++){ // ?
        uint32_t key = (uint32_t) (rand() | (rand() << 16)); // ?
        check(radixmap_add(map, key, i) == 0, "Failed to add key %u.", key);
    }

    return i;
error: return 0;
}

static int check_order(RadixMap *map){
    RMElement d1, d2;

    size_t i = 0;

    for(i = 0; map->end > 0 && i < map->end - 1; i++){
        d1 = map->contents[i];
        d2 = map->contents[i + 1];

        if(d1.data.key > d2.data.key){
            debug("FAIL: disorder, i=%lu, %u:%u > %u:%u", i, d1.data.key, d1.data.value, d2.data.key, d2.data.value);
            return 0;
        }
    }

    return 1;
}

static int test_search(RadixMap *map){
    size_t i = 0;
    RMElement *d = NULL;
    RMElement *found = NULL;

    for(i = map->end / 2; i < map->end; i++){
        d = &map->contents[i];
        found = radixmap_find(map, d->data.key);
        check(found, "Didn't find %u at %lu.", d->data.key, i);
        check(found->data.key == d->data.key, "Got the wrong result: %p:%u looking for %u at %lu", found, found->data.key, d->data.key, i);
    }

    return 1;
error: return 0;
}

static char *test_operations(){
    size_t N = 10000;
    clock_t t;

    RadixMap *map = radixmap_create(N);
    mu_assert(map != NULL, "Failed to make the map.");

    t = clock();
    mu_assert(make_map(map), "Didn't make the random fake radix map.");
    log_info("Making map used %lu", clock() - t);

    t = clock();
    radixmap_sort(map, 0);
    log_info("sorting map used %lu", clock() - t);
    mu_assert(check_order(map), "Failed to properly sort the RadixMap.");

    mu_assert(test_search(map), "Failed the search test.");
    mu_assert(check_order(map), "RadixMap didn't stay sorted after search.");

    t = clock();
    while(map->end > 0){
        RMElement *elem = radixmap_find(map, map->contents[map->end / 2].data.key);
        mu_assert(elem, "Should get a result.");

        size_t old_end = map->end;

        mu_assert(radixmap_delete(map, elem) == 0, "Didn't delete it.");
        mu_assert(old_end - 1 == map->end, "Wrong size after delete.");

        mu_assert(check_order(map), "RadixMap didn't stay sorted after delete.");
    }
    log_info("deleting map item used %lu", clock() - t);

    radixmap_destroy(map);

    return NULL;
}


char *all_tests(){
    mu_suite_start();
    srand(time(NULL));

    mu_run_test(test_operations);

    return NULL;
}

RUN_TESTS(all_tests);