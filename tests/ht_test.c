#include "ht_test.h"

#include "runit.h"

#define HT_IMPLEMENTATION
#include "../ht.h"

static void ht_tests();

int ht_test()
{
    RUNIT_RUN(ht_tests);
    
    return runit_fail == 0;
}

struct item {
    char key;
    float value;
};

static ht_bool items_are_same(struct item* left, struct item* right)
{
    return left->key == right->key;
}

static void swap_items(struct item* left, struct item* right)
{
    struct item tmp = *left;
    *left = *right;
    *right = tmp;
}

/* Dummy hash method to similate collision */
static ht_hash_t hash(struct item* i)
{
    switch(i->key)
    {
    case 'a':
    case 'b':
        return 0;
    case 'c':
        return 1;
    default: 
        return 99;
    }
}

static void ht_tests()
{
    ht ht;
    ht_init(&ht,
        sizeof(struct item),
        (ht_hash_function_t)hash,
        (ht_predicate_t)items_are_same,
        (ht_swap_function_t)swap_items,
    0);

    struct item data[] = {
        {'a', 3.14f},
        {'b', 6.28f},
        {'c', -3.14f}
    };

    for (int i = 0; i < 3; ++i)
    {
        ht_insert(&ht, &data[i]);
    }

    for (int i = 0; i < 3; ++i)
    {
        struct item expect = data[i];
        struct item res = { 0 };
        RUNIT_ASSERT(ht_contains(&ht, &expect));
        RUNIT_ASSERT(ht_get(&ht, &expect, &res));
        RUNIT_ASSERT(res.value == expect.value);
    }

    RUNIT_ASSERT(!ht_is_empty(&ht));
    RUNIT_ASSERT(ht_size(&ht) == 3);

    struct item not_expected = { 0, 0 };
    struct item res = { 0, 0 };
    RUNIT_ASSERT(!ht_contains(&ht, &not_expected));
    RUNIT_ASSERT(!ht_get(&ht, &not_expected, &res));

    /* Erase one items */
    {
        struct item expected = { 'a' };
        RUNIT_ASSERT(ht_erase(&ht, &expected));
        not_expected.key = 'a'; /* Not expected since it has been removed */
        RUNIT_ASSERT(!ht_erase(&ht, &not_expected));
        RUNIT_ASSERT(!ht_is_empty(&ht));
        RUNIT_ASSERT(ht_size(&ht) == 2);
    }

    /* Remove all items. */
    for (int i = 0; i < 3; ++i)
    {
        ht_erase(&ht, &data[i]);
    }
    
    not_expected.key = 'a'; /* Not expected since it has been removed */
    RUNIT_ASSERT(!ht_contains(&ht, &not_expected));
    not_expected.key = 'b';
    RUNIT_ASSERT(!ht_contains(&ht, &not_expected));
    not_expected.key = 'c';
    RUNIT_ASSERT(!ht_contains(&ht, &not_expected));
    RUNIT_ASSERT(ht_size(&ht) == 0);

    /* Clear items */
    for (int i = 0; i < 3; ++i)
    {
        ht_insert(&ht, &data[i]);
    }
    ht_clear(&ht);
    RUNIT_ASSERT(ht_size(&ht) == 0);
}
