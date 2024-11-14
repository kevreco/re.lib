#include "assert.h"
#include "string.h"

#include "runit.h"

#define HT_IMPLEMENTATION
#include "../ht.h"

static void ht_tests();

int ht_test()
{
    RUNIT_RUN(ht_tests);
    
    return runit_fail == 0;
}

struct key {
    char i;
};

struct value {
    float f;
};

struct pair {
    struct key key;
 
    struct value value;
};

static ht_bool items_are_same(struct pair* a, struct pair* b)
{
    return a->key.i == b->key.i;
}

static void swap_items(struct pair* a, struct pair* b)
{
    struct pair tmp = *a;
    *a = *b;
    *b = tmp;
}

static ht_hash_t hash(void* key)
{
    const char* key_ptr = key;
    char k = *key_ptr;

    static struct
    {
        char k; ht_hash_t h;
    } map[] =
    {
        {'a', 0},
        {'b', 0},
        {'c', 0},
        {'d', 1},
        {'e', 1},
        {'f', 1},
        {'g', 1},
        {'h', 1},
        {'i', 1},
        {'j', 2},
        {'k', 2},
        {'l', 2}
    };

    for (int i = 0; i < 12; ++i)
    {
        if (k == map[i].k)
            return  map[i].h;
    }

    return 5;
}

static void ht_tests()
{
    ht ht;
    ht_init(&ht, sizeof(struct pair), hash, items_are_same, swap_items, 0);

    struct pair data[] = {
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
        struct pair expect = data[i];
        struct pair res = { 0 };
        RUNIT_ASSERT(ht_contains(&ht, &expect));
        RUNIT_ASSERT(ht_get(&ht, &expect, &res));
        RUNIT_ASSERT(res.value.f == expect.value.f);
    }

    RUNIT_ASSERT(!ht_is_empty(&ht));
    RUNIT_ASSERT(ht_size(&ht) == 3);

    struct pair not_expected = { 0, 0 };
    struct pair res = { 0, 0 };
    RUNIT_ASSERT(!ht_contains(&ht, &not_expected));
    RUNIT_ASSERT(!ht_get(&ht, &not_expected, &res));

    struct key expected = { 'a' };
    RUNIT_ASSERT(ht_erase(&ht, &expected));
    not_expected.key.i = 'a'; // not expected since it has been removed
    RUNIT_ASSERT(!ht_erase(&ht, &not_expected));
    RUNIT_ASSERT(!ht_is_empty(&ht));
    RUNIT_ASSERT(ht_size(&ht) == 2);

    // remove all
    for (int i = 0; i < 3; ++i)
    {
        ht_erase(&ht, &data[i]);
    }
    not_expected.key.i = 'a'; // not expected since it has been removed
    RUNIT_ASSERT(!ht_contains(&ht, &not_expected));
    not_expected.key.i = 'b';
    RUNIT_ASSERT(!ht_contains(&ht, &not_expected));
    not_expected.key.i = 'c';
    RUNIT_ASSERT(!ht_contains(&ht, &not_expected));
    RUNIT_ASSERT(ht_size(&ht) == 0);

    // clear
    {
        for (int i = 0; i < 3; ++i)
        {
            ht_insert(&ht, &data[i]);
        }
        ht_clear(&ht);
        RUNIT_ASSERT(ht_size(&ht) == 0);

    }

}
