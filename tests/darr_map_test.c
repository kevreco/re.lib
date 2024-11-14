#include "darr_map_test.h"

#include "runit.h"

#define RE_DARR_MAP_IMPLEMENTATION
#include "../darr_map.h"

static void darr_map_tests();

int darr_map_test()
{
    RUNIT_RUN(darr_map_tests);
    
    return runit_fail == 0;
}

static void darr_map_insert_and_erase_test();

static void darr_map_tests() {

    darr_map_insert_and_erase_test();
}

typedef struct map_item map_item;
struct map_item {
    int key;
    int value;
};

static int map_item_less(const map_item* left, const map_item* right)
{
    return (left->key < right->key);
}

static void darr_map_insert_and_erase_test()
{
    darr_map map;
    darr_map_init(&map, sizeof(map_item), (darr_predicate_t)map_item_less);

    for (int i = 0; i < 10; i += 2)
    {
        map_item item = { i, i + 1 };
        RUNIT_ASSERT(darr_map_set(&map, &item));
        /* Replace value with the same value, it should not return true. */
        RUNIT_ASSERT(!darr_map_set(&map, &item)); 
    }

    RUNIT_ASSERT(darr_map_size(&map) == 5);

    /* Check existence of all items. */
    for (int i = 0; i < 10; i += 2)
    {
        map_item item = { i, 0 };
        RUNIT_ASSERT(darr_map_contains(&map, &item));
    }

    {
        map_item item;

        item.key = 8;
        RUNIT_ASSERT(darr_map_remove(&map, &item)); 

        RUNIT_ASSERT(darr_map_size(&map) == 4);
        item.key = -1;
        
        /* Returns false, no items was removed. */
        RUNIT_ASSERT(!darr_map_remove(&map, &item)); 
        RUNIT_ASSERT(darr_map_size(&map) == 4);
    }

    /* Add 5 more items. */
    for (int i = 10; i < 20; i += 2)
    {
        map_item item = { i, i + 1};

        RUNIT_ASSERT(darr_map_set(&map, &item));
        /* replace value with the same value, it should not return true. */
        RUNIT_ASSERT(!darr_map_set(&map, &item)); 
        RUNIT_ASSERT(darr_map_contains(&map, &item));
    };

    RUNIT_ASSERT(darr_map_size(&map) == 9);

    darr_map_destroy(&map);
}
