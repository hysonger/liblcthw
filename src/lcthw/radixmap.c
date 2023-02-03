#include <lcthw/radixmap.h>


RadixMap *radixmap_create(size_t max){
    RadixMap *map = calloc(1, sizeof(RadixMap));
    check_mem(map); // 确保每次alloc都成功执行了才能继续;

    map->contents = calloc(max, sizeof(RMElement));
    check_mem(map->contents);

    map->temp = calloc(max, sizeof(RMElement));
    check_mem(map->temp);

    map->max = max;
    map->end = 0;
    
    return map;
error:
    if(map) free(map);

    if(map->contents) free(map->contents);

    return NULL;
}

// 释放contents temp以及map本身
void radixmap_destroy(RadixMap *map){
    if(map){
        free(map->contents);
        free(map->temp);
        free(map);
    }

}


// 以第offset位上数为基数位，对source中的各项进桶重排，结果进入dest
// 此为基数排序的核心代码，trick很多，有一定理解难度
static inline void __radixmap_sort(short offset, uint64_t max, uint64_t *source, uint64_t *dest){
    uint64_t count[256] = {0}; // 统计该位上256种数字情况出现次数的桶
    uint64_t *cp, *sp; // 索引数组的指针(count source)
    uint64_t *end = source + max; // source的结尾，后用
    uint64_t co = 0, co_sum = 0; // *cp=>co, co_sum += co

    // 计出count数组内容
    for(sp = source; sp < end; sp++){
        count[ByteOf(sp, offset)]++;
    }

    // 算出source每项应被放到的真正的位置(偏移量)
    for(cp = count; cp - count < 256; cp++){
        co = *cp;
        *cp = co_sum; // count数组记录的是“排在前面的数有多少个”
        co_sum += co;
    }

    for(sp = source; sp < end; sp++){
        cp = count + ByteOf(sp, offset); // ??? 这里似乎只能用+语法，用[]语法会导致段错误
        dest[*cp] = *sp;
        ++(*cp); // 这是为了让相同offset位的下一个数放到下一个位置上
    }
}


void radixmap_sort(RadixMap *map){
    uint64_t *c = &map->contents[0].raw;
    uint64_t *t = &map->temp[0].raw;

    // 不用整体互换data和temp两个数组，只需要“左手倒右手”
    __radixmap_sort(0, map->end, c, t);
    __radixmap_sort(1, map->end, t, c);
    __radixmap_sort(2, map->end, c, t);
    __radixmap_sort(3, map->end, t, c);

}


// 这里采用二分搜索，搜索对象（比较目标）是数的高32位(key)，这里就看出为什么要用union了
RMElement *radixmap_find(RadixMap *map, uint32_t key){
    size_t low = 0, high = map->end - 1, mid;

    while(low <= high){
        mid = (low + high) / 2;
        if(map->contents[mid].data.key < key){
            low = mid + 1;
        }else if(map->contents[mid].data.key > key){
            high = mid == 0 ? mid : mid - 1; // 防下溢
        }else{
            return &map->contents[mid]; // contents不是指针的数组而是直值数组，返回取一下指针避免复制一次数据
        }
    }

    return NULL;
}


int radixmap_add(RadixMap *map, uint32_t key, uint32_t value){
    check(key < UINT32_MAX, "Invaild key, out of range"); // key为UINT32_MAX是无效的

    // 创建element
    RMElement element = {.data = {.key = key, .value = value}};

    // 放进去
    check(map->end < map->max, "RadixMap is full."); // ?
    map->contents[map->end++] = element;
    
    radixmap_sort(map); // 每次加入新项，都进行一次基数排序

    return 0;
error: return -1;
}


int radixmap_delete(RadixMap *map, RMElement *elem){
    check(map->end > 0, "Nothing in map");
    check(elem, "provided a NULL for delete");

    elem->data.key = UINT32_MAX; // 置最大值，以使此项沉到最底便于删除
    
    if(map->end > 1){ // 只有一项的map不需要排序
        radixmap_sort(map);
    }

    map->end--;
    

    return 0;
error: return -1;
}

