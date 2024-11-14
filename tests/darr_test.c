#include "darr_test.h"

#include "string.h"
#include "assert.h"

#include "runit.h"

#define DARR_IMPLEMENTATION
#include "../darr.h"
#include "../darrT.h"

static void darr_tests();

int darr_test()
{
    RE_UNIT_RUN(darr_tests);
    
    return runit_fail == 0;
}

static darr_bool darr_test_less_int(const void* left, const void* right);

static void darr_assign_test()
{
    darr arr;
    darr_init(&arr, sizeof(int));

    int values[] = { 0,1,2,3,4 };

    darr_assign(&arr, values, 5);

    RUNIT_ASSERT(arr.size == 5);

    for (int i = 0; i < 5; ++i) {  
        int value;
        darr_get_value(&arr, i, &value);
        RUNIT_ASSERT(value == values[i]);
    }

    darr_destroy(&arr);

}

static void darr_insert_and_erase_test()
{
    {
        darr arr;
        darr_init(&arr, sizeof(int));

        for (int i = 0; i < 10; ++i)
        {
            darr_append_value(&arr, &i);
        }

        for (int i = 0; i < 10; ++i)
        {
            int value;
            darr_get_value(&arr, i, &value);
            RUNIT_ASSERT(value == i);
        }
        
        //darr_test_print(&arr); // should print "[0,1,2,3,4,5,6,7,8,9]"

        // erase & begin
        for (int i = 0; i < 5; ++i) {
            darr_erase_one(&arr, 0);
        }

        // darr_test_print(&arr); // should print "[5,6,7,8,9]"

        int test_array[] = { 5,6,7,8,9 };
        for (int i = 0; i < 5; ++i)
        {
            int value;
            darr_get_value(&arr, i, &value);
            RUNIT_ASSERT(value == test_array[i]);
        }

        for (int i = 0; i < 2; ++i)
        {
            darr_erase_one(&arr, arr.size - 1);
        }
        
        //darr_test_print(&arr); // should print "[5,6,7]"

        int test_array2[] = { 5,6,7 };
        for (int i = 0; i < 3; ++i)
        {
            int value;
            darr_get_value(&arr, i, &value);
            RUNIT_ASSERT(value == test_array2[i]);
        }
        
        // insert & begin
        for (int i = 4; i >= 0; --i) {
            darr_insert_one(&arr, 0, &i);
        }

        for (int i = 8; i <= 9; ++i) {
            darr_insert_one(&arr, arr.size, &i);
        }

        //darr_test_print(&arr); // should print "[0,1,2,3,4,5,6,7,8,9]"

        int test_values[] = { 0,1,2,3,4,5,6,7,8,9 };
        for (int i = 0; i < 10; ++i)
        {
            int value;
            darr_get_value(&arr, i, &value);
            RUNIT_ASSERT(value == test_values[i]);
        }

        darr_destroy(&arr);
    }
}

static void darr_pop_back_test()
{
    darr arr;
    darr_init(&arr, sizeof(int));

    for (int i = 0; i < 5; ++i) {
        darr_push_back(&arr, i);
    }
    
    RUNIT_ASSERT(arr.size == 5);

    for (int i = 4; i >= 0; --i) {
        darr_pop_back(&arr);
        RUNIT_ASSERT(arr.size == (darr_size_t)i);
        if (arr.size > 0)
        {
            int value;
            darr_get_value(&arr, i -1, &value);
            RUNIT_ASSERT(value == i - 1);
        }
    }
    RUNIT_ASSERT(arr.size == 0);

    darr_destroy(&arr);
}

static void darr_sorted_array_test()
{
    // sorted dynamic array
    {
        darr arr;
        darr_init(&arr, sizeof(int));

        for (int i = 9; i >= 0; --i) {
            darr_insert_one_sorted(&arr, &i, darr_test_less_int);
        }

        //darr_test_print(&arr);

        for (int i = 9; i >= 0; --i) {
            darr_insert_one_sorted(&arr, &i, darr_test_less_int);
        }

        //darr_test_print(&arr);

        for (int i = 0; i < 20; ++i)
        {
            int value;
            darr_get_value(&arr, i, &value);
            //printf("%d == %d\n", value, ((i / 2) + (i % 1)));
            RUNIT_ASSERT(value == ((i / 2) + (i % 1)));
        }


        darr_destroy(&arr);
    }
}

struct darr_dummy_t {
    int a;
    int b;
};

static struct darr_dummy_t darr__dummy_make(int a, int b)
{
    struct darr_dummy_t d;
    d.a = a;
    d.b = b;
    return d;
}

static darr_bool darr__dummy_are_same(struct darr_dummy_t a, struct darr_dummy_t b)
{
    return a.a == b.a && a.b == b.b;
}

static darr_bool darr_test_less_dummy(struct darr_dummy_t* a, struct darr_dummy_t* b)
{
    return (a->b < b->b);
}

static void darr_struct_test()
{
    struct darr_dummy_t d[] = {
        darr__dummy_make(0, 1),
        darr__dummy_make(4, 5),
        darr__dummy_make(2, 3),
        darr__dummy_make(8, 9),
        darr__dummy_make(6, 7),
    };
    struct darr_dummy_t sorted[] = {
        darr__dummy_make(0, 1),
        darr__dummy_make(2, 3),
        darr__dummy_make(4, 5),
        darr__dummy_make(6, 7),
        darr__dummy_make(8, 9),
    };

    {
        darr arr;
        darr_init(&arr, sizeof(d[0]));

        // append 
        for (int i = 0; i < 5; ++i)
        {
            darr_append_value(&arr, &d[i]);
        }

        for (int i = 0; i < 5; ++i)
        {
            struct darr_dummy_t value;
            darr_get_value(&arr, i, &value);
            RUNIT_ASSERT(darr__dummy_are_same(value, d[i]));
        }


        // erase & begin
        for (int i = 0; i < 5; ++i) {
            darr_erase_one(&arr, 0);
        }

        RUNIT_ASSERT(arr.size == 0);

        for (int i = 0; i < 5; ++i)
        {
            darr_insert_one_sorted(&arr, &d[i], (darr_predicate_t)darr_test_less_dummy);
        }

        for (int i = 0; i < 5; ++i)
        {
            struct darr_dummy_t value;
            darr_get_value(&arr, i, &value);
            RUNIT_ASSERT(darr__dummy_are_same(value, sorted[i]));
        }

        // pop_back

        for (int i = 0; i < 5; ++i)
        {
            darr_pop_back(&arr);
        }

        RUNIT_ASSERT(arr.size == 0);

        darr_destroy(&arr);
    }
}

static void darrT_test()
{
    darrT(int) values;

    darrT_init(&values);

    for (int i = 0; i < 5; ++i)
    {
        darrT_push_back(&values, i);
    }
    
    for (int i = 0; i < 5; ++i)
    {
        int* ptr = darrT_ptr(&values, i);
        RUNIT_ASSERT(*ptr == i);
    }

    for (int i = 0; i < 5; ++i)
    {
        darrT_set(&values, i, 0);
    }

    for (int i = 0; i < 5; ++i)
    {
        int* ptr = darrT_ptr(&values, i);
        RUNIT_ASSERT(*ptr == 0);
    }

    darrT_destroy(&values);
}

static void darr_test_print(const darr* array) {
    darr_size_t size =  array->size;
    fprintf(stdout, "[");
    for (size_t i = 0; i < size; ++i) {
        int* value = (int*)darr_ptr(array, i);

        fprintf(stdout, "%d", *value);
        if (i != size - 1) {
            printf(",");
        }
    }
    fprintf(stdout, "]\n");
}

static darr_bool darr_test_less_int(const void* left, const void* right)
{
    const int* l = (const int*)left;
    const int* r = (const int*)right;
    return (*l < *r);
}


static void darr_tests() {

    darr_assign_test();
    darr_insert_and_erase_test();
    darr_pop_back_test();
    darr_sorted_array_test();
    darr_struct_test();
    darrT_test();
}