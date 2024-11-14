#include "darr_map_test.h"

#include "runit.h"

#include "string.h"
#include "assert.h"

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

typedef struct darr_map_test_pair darr_map_test_pair;
struct darr_map_test_pair {
    int key;
    int value;
};

static int darr_map_test_less_pair(const darr_map_test_pair* left, const darr_map_test_pair* right)
{
    return (left->key < right->key);
}

static void darr_map_insert_and_erase_test()
{
    typedef darr_map_test_pair pair;

    {
        darr_map map;
        darr_map_init(&map, sizeof(pair), (darr_predicate_t)darr_map_test_less_pair);

        for (int i = 0; i < 10; i += 2)
        {
            pair pair = { i, i + 1 };
            RUNIT_ASSERT(darr_map_set(&map, &pair));
            RUNIT_ASSERT(!darr_map_set(&map, &pair)); /* replace value with the same value, it should not return true */
        }

        RUNIT_ASSERT(darr_map_size(&map) == 5);

        for (int i = 0; i < 10; i += 2)
        {
            pair pair = { i, 0 };
            RUNIT_ASSERT(darr_map_contains(&map, &pair));
        }

        {
            pair item;

            item.key = 8;
            RUNIT_ASSERT(darr_map_remove(&map, &item)); 

            RUNIT_ASSERT(darr_map_size(&map) == 4);
            item.key = -1;
            RUNIT_ASSERT(!darr_map_remove(&map, &item)); /* Returns false, no items was removed. */
            RUNIT_ASSERT(darr_map_size(&map) == 4);
        }

        /* Add 5 more items. */
        for (int i = 10; i < 20; i += 2)
        {
            pair item = { i, i + 1};

            RUNIT_ASSERT(darr_map_set(&map, &item));
            RUNIT_ASSERT(!darr_map_set(&map, &item)); // replace value with the same value, it should not return true
            RUNIT_ASSERT(darr_map_contains(&map, &item));
        };

        RUNIT_ASSERT(darr_map_size(&map) == 9);

        darr_map_destroy(&map);
    }
}
