#include "dstr_test.h"

#define DSTR_IMPLEMENTATION
#include "../dstr.h"

#include "runit.h"

static void dstr_tests();

int dstr_test()
{
    RUNIT_RUN(dstr_tests);
    
    return runit_fail == 0;
}

static void print_dstr(const dstr* s);
static void print_dstr_ln(const dstr* s);

static void dstr_constructor_test();
static void dstr_reserve_test();
static void dstr_append_test();
static void dstr_append_from_test();

static void dstr_shrink_to_fit_test();
static void dstr_insert_test();
static void dstr_erase_test();
static void dstr_resize_test();

static void dstr_replace_test();
static void dstr_assign_f_test();
static void dstr_local_buffer_test();
static void dstr_misc_capacity_test();

static void dstr_tests()
{
    dstr_constructor_test();
    dstr_reserve_test();
    dstr_append_test();
    dstr_append_from_test();

    dstr_shrink_to_fit_test();
    dstr_insert_test();
    dstr_erase_test();
    dstr_resize_test();

    dstr_replace_test();
    dstr_assign_f_test();
    dstr_local_buffer_test();
    dstr_misc_capacity_test();
}

static void dstr_constructor_test() {
    enum {
        SIZE = 5
    };

    dstr dummy = dstr_make_from_str("Hello World!");

    dstr str[SIZE] = {
        dstr_make(),
        dstr_make_reserve(64),
        dstr_make_from_dstr(&dummy),
        dstr_make_from_str("Hello Everybody!"),
        dstr_make_from_nchar(6, 'd'),
    };

    const char* cstr[SIZE] = {
        "",
        "",
        "Hello World!",
        "Hello Everybody!",
        "dddddd",
    };

    for (size_t i = 0; i < SIZE; ++i) {
        RUNIT_ASSERT(str[i].size == strlen(cstr[i]));
        RUNIT_ASSERT(dstr_compare_str(&str[i], cstr[i]) == 0);
    }

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_destroy(&str[i]);
    }

    for (size_t i = 0; i < SIZE; ++i) {
        RUNIT_ASSERT(dstr_size(&str[i]) == 0);
    }

    /* With local buffer */
    {
        dstr32 str32;
        dstr32_init(&str32);
        dstr_assign_str(&str32.dstr, "Hello World!");
        RUNIT_ASSERT(dstr_compare_str(&str32.dstr, "Hello World!") == 0);
    }
}

static void dstr_reserve_test() {
    enum {
        SIZE = 9
    };

    dstr str[SIZE] = {
        dstr_make(),
        dstr_make(),
        dstr_make_reserve(32),
        dstr_make_reserve(32),
        dstr_make_reserve(32),
        dstr_make_reserve(32),
        dstr_make_from_str("abcdefg"),
        dstr_make_from_str("abcdefg"),
        dstr_make_from_str("abcdefg")
    };

    /* Reserve 0 from 0 capacity dstr. */
    {
        dstr_reserve(&str[0], 0);

        RUNIT_ASSERT(str[0].capacity == 0);
        RUNIT_ASSERT(dstr_compare_str(&str[1], "") == 0);
    }

    /* Reserve 32 from 0 capacity dstr. */
    {
        dstr_reserve(&str[1], 32);

        RUNIT_ASSERT(str[1].capacity == 32);
        RUNIT_ASSERT(dstr_compare_str(&str[1], "") == 0);
    }

    /* Reserve 64 from 32 capacity empty dstr. */
    {
        dstr_reserve(&str[3], 64);

        RUNIT_ASSERT(str[3].capacity == 64);
        RUNIT_ASSERT(dstr_compare_str(&str[3], "") == 0);
    }
    
    /* reserve 16 from 32 capacity empty dstr. */
    {
        dstr_reserve(&str[4], 16);

        RUNIT_ASSERT(str[4].capacity == 32);
        RUNIT_ASSERT(dstr_compare_str(&str[4], "") == 0);
    }

    /* Reserve 32 from 32 capacity empty dstr. */
    {
        dstr_reserve(&str[5], 32);

        RUNIT_ASSERT(str[5].capacity == 32);
        RUNIT_ASSERT(dstr_compare_str(&str[5], "") == 0);
    }

    /* Reserve 16 from 8 size dstr. */
    {
        dstr_reserve(&str[6], 16);

        RUNIT_ASSERT(str[6].capacity == 16);
        RUNIT_ASSERT(dstr_compare_str(&str[6], "abcdefg") == 0);
    }

    /* Reserve 8 from 16 size dstr. */
    {
        dstr_reserve(&str[7], 8);

        RUNIT_ASSERT(str[7].capacity == 8);
        RUNIT_ASSERT(dstr_compare_str(&str[7], "abcdefg") == 0);
    }

    /* Reserve 8 from 8 size dstr. */
    {
        dstr_reserve(&str[8], 8);

        RUNIT_ASSERT(str[8].capacity == 8);
        RUNIT_ASSERT(dstr_compare_str(&str[8], "abcdefg") == 0);
    }

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_destroy(&str[i]);
    }
}

static void dstr_append_test() {
    enum {
        SIZE = 6
    };
    dstr str[SIZE] = {0};

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_init(&str[i]);
    }

    const char* cstr[SIZE] = {
        "",
        "a",
        "abcd",
        "\0",
        "\0a",
        "a\0"
    };

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_append_str(&str[i], cstr[i]);
    }

    for (size_t i = 0; i < SIZE; ++i) {
        RUNIT_ASSERT(str[i].size == strlen(cstr[i]));
        RUNIT_ASSERT(dstr_compare_str(&str[i], cstr[i]) == 0);
    }

    dstr_append_str(&str[0], "a long string");
    dstr_append_str(&str[1], "nother long string");

    RUNIT_ASSERT(str[0].size == strlen("a long string"));
    RUNIT_ASSERT(dstr_compare_str(&str[0], "a long string") == 0);
    RUNIT_ASSERT(str[1].size == strlen("another long string"));
    RUNIT_ASSERT(dstr_compare_str(&str[1], "another long string") == 0);

    dstr_append_char(&str[2], 'e');
    dstr_append_char(&str[2], 'f');
    dstr_append_char(&str[2], 'g');
    dstr_append_char(&str[2], 'h');

    RUNIT_ASSERT(dstr_compare_str(&str[2], "abcdefgh") == 0);

    dstr xyz = dstr_make_from_str("xyz");

    dstr_append_dstr(&str[2], &xyz);

    RUNIT_ASSERT(dstr_compare_str(&str[2], "abcdefghxyz") == 0);

    dstr_append_nchar(&str[2], 3, '0');

    RUNIT_ASSERT(dstr_compare_str(&str[2], "abcdefghxyz000") == 0);

    char array[] = "0123456789";
    dstr_append(&str[2], strv_make_from(array + 4, 3));

    RUNIT_ASSERT(dstr_compare_str(&str[2], "abcdefghxyz000456") == 0);

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_destroy(&str[i]);
    }

    for (size_t i = 0; i < SIZE; ++i) {
        RUNIT_ASSERT(dstr_size(&str[i]) == 0);
    }
}

static void dstr_append_from_test()
{
    dstr dstr1;
    dstr_init(&dstr1);
    dstr_append_str(&dstr1, "World Hello!");
    dstr_append_str_from(&dstr1, 0, "Hello World");
    
    RUNIT_ASSERT(dstr_equals_str(&dstr1, "Hello World"));

    dstr_append_char(&dstr1, '.');
    RUNIT_ASSERT(dstr_equals_str(&dstr1, "Hello World."));

    dstr_destroy(&dstr1);
}

static void dstr_shrink_to_fit_test() {
    enum {
        SIZE = 4
    };

    dstr str[SIZE] = {
        dstr_make(),
        dstr_make_reserve(64),
        dstr_make(),
        dstr_make()
    };

    for (size_t i = 2; i < SIZE; ++i) {
        dstr_append_nchar(&str[i], 10, 'a');
    }

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_shrink_to_fit(&str[i]);
    }

    RUNIT_ASSERT(str[0].size == 0);
    RUNIT_ASSERT(str[0].capacity == 0);

    RUNIT_ASSERT(str[1].size == 0);
    RUNIT_ASSERT(str[1].capacity == 0);

    for (size_t i = 2; i < SIZE; ++i) {
        RUNIT_ASSERT(str[i].size == 10);
        RUNIT_ASSERT((str[i].size) == str[i].capacity);
    }

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_destroy(&str[i]);
    }
}

static void dstr_insert_test()
{
    enum {
        SIZE = 4
    };

    dstr str[SIZE] = {
        dstr_make(),
        dstr_make_reserve(64),
        dstr_make_from_str("llo Worl"),
        dstr_make_from_str("World!")
    };

    dstr_insert_char(&str[2], dstr_begin(&str[2]), 'e');
    dstr_insert_char(&str[2], dstr_begin(&str[2]), 'H');
    dstr_insert_char(&str[2], dstr_end(&str[2]), 'd');
    dstr_insert_char(&str[2], dstr_end(&str[2]), '!');
    
    dstr_insert_str(&str[3], dstr_begin(&str[3]), "Hello ");

    RUNIT_ASSERT(dstr_compare_str(&str[2], "Hello World!") == 0 );
    RUNIT_ASSERT(dstr_compare_str(&str[3], "Hello World!") == 0 );

    char hello[] = {"Hello World!"};

    dstr_insert(&str[0], dstr_begin(&str[0]), strv_make_from_str(hello));
    dstr_insert(&str[1], dstr_begin(&str[1]), strv_make_from_str(hello));

    RUNIT_ASSERT(dstr_compare_str(&str[0], "Hello World!") == 0);
    RUNIT_ASSERT(dstr_compare_str(&str[1], "Hello World!") == 0);

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_destroy(&str[i]);
    }    
}

static void dstr_erase_test()
{
    enum {
        SIZE = 4
    };

    dstr str[SIZE] = {
        dstr_make(),
        dstr_make_from_str("Hello World!"),
        dstr_make_from_str("Hello World!"),
        dstr_make_from_str("Hello World!")
    };

    for (size_t i = 0; i < 6; ++i) {
        dstr_erase_value(&str[1], dstr_begin(&str[1]));
    }
    RUNIT_ASSERT(dstr_compare_str(&str[1], "World!") == 0 );

    dstr_erase(&str[2], str[2].data + 5, 7);
    RUNIT_ASSERT(dstr_compare_str(&str[2], "Hello") == 0 );

    dstr_erase(&str[2], str[2].data, str[2].size);

    RUNIT_ASSERT(dstr_compare_str(&str[2], "") == 0 );

    dstr_erase(&str[3], str[3].data, str[3].size);

    RUNIT_ASSERT(dstr_compare_str(&str[3], "") == 0 );

    dstr_erase(&str[0], str[0].data, str[0].size);

    RUNIT_ASSERT(dstr_compare_str(&str[0], "") == 0 );

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_destroy(&str[i]);
    }
}

static void dstr_resize_test()
{
    enum {
        SIZE = 12
    };

    dstr str[SIZE] = { 0 };

    for (size_t i = 0; i < SIZE; ++i) {
        str[i] = dstr_make_reserve(8);
    }

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_append_str(&str[i], "aaaaaa");
    }

    dstr_resize(&str[0], 0);  /* resize to 0 */
    dstr_resize(&str[1], 8);  /* resize at capacity */
    dstr_resize(&str[2], 6);  /* resize at size */
    dstr_resize(&str[3], 4);  /* resize to < size */
    dstr_resize(&str[4], 7);  /* resize between size and capacity */
    dstr_resize(&str[5], 16); /* resize beyond capacity */

    RUNIT_ASSERT(str[0].size == 0);
    RUNIT_ASSERT(str[1].size == 8);
    RUNIT_ASSERT(str[2].size == 6);
    RUNIT_ASSERT(str[3].size == 4);
    RUNIT_ASSERT(str[4].size == 7);
    RUNIT_ASSERT(str[5].size == 16);

    RUNIT_ASSERT(str[0].capacity == 8);
    RUNIT_ASSERT(str[1].capacity == 8);
    RUNIT_ASSERT(str[2].capacity == 8);
    RUNIT_ASSERT(str[3].capacity == 8);
    RUNIT_ASSERT(str[4].capacity == 8);
    RUNIT_ASSERT(str[5].capacity == 16);

    dstr_resize_fill(&str[6],  0, 'b');  /* resize to 0 */
    dstr_resize_fill(&str[7],  8, 'b');  /* resize at capacity */
    dstr_resize_fill(&str[8],  6, 'b');  /* resize at size */
    dstr_resize_fill(&str[9],  4, 'b');  /* resize to < size */
    dstr_resize_fill(&str[10], 7, 'b');  /* resize between size and capacity */
    dstr_resize_fill(&str[11], 16, 'b'); /* resize beyond capacity */

    RUNIT_ASSERT(dstr_compare_str(&str[6], "") == 0);
    RUNIT_ASSERT(dstr_compare_str(&str[7], "aaaaaabb") == 0);
    RUNIT_ASSERT(dstr_compare_str(&str[8], "aaaaaa") == 0);
    RUNIT_ASSERT(dstr_compare_str(&str[9], "aaaa") == 0);
    RUNIT_ASSERT(dstr_compare_str(&str[10], "aaaaaab") == 0);
    RUNIT_ASSERT(dstr_compare_str(&str[11], "aaaaaabbbbbbbbbb") == 0);

    for (size_t i = 0; i < SIZE; ++i) {
        dstr_destroy(&str[i]);
    }

    dstr16 local_str;
    
    dstr16_init(&local_str);

    RUNIT_ASSERT(local_str.dstr.size == 0);
    RUNIT_ASSERT(local_str.dstr.capacity == 15);

    dstr_resize(&local_str.dstr, 0);

    RUNIT_ASSERT(local_str.dstr.size == 0);
    RUNIT_ASSERT(local_str.dstr.capacity == 15);

    dstr_resize(&local_str.dstr, 8);

    RUNIT_ASSERT(local_str.dstr.size == 8);
    RUNIT_ASSERT(local_str.dstr.capacity == 15);

    dstr_resize(&local_str.dstr, 16); /* Resize beyond capacity. */

    RUNIT_ASSERT(local_str.dstr.size == 16);
    RUNIT_ASSERT(local_str.dstr.capacity > 15);

    dstr_resize(&local_str.dstr, 32); /* Resize beyond capacity */

    RUNIT_ASSERT(local_str.dstr.size == 32); 
    RUNIT_ASSERT(local_str.dstr.capacity > 16); /* Not reinitialized to 16. */

    dstr_size_t capacity = local_str.dstr.capacity;

    dstr_resize(&local_str.dstr, 0);

    RUNIT_ASSERT(local_str.dstr.size == 0);
    RUNIT_ASSERT(local_str.dstr.capacity == capacity);

    dstr_destroy(&local_str.dstr);
}

static void dstr_replace_test()
{
    {
        enum {
            SIZE = 10
        };

        dstr str[SIZE] = {

            dstr_make_from_str("aaaaaaaa"),
            dstr_make_from_str("aaaaaaaa"),
            dstr_make_from_str("aaaaaaaa"),

            dstr_make_from_str("aaaaaaaa"),
            dstr_make_from_str("aaaaaaaa"),
            dstr_make_from_str("aaaaaaaa"),

            dstr_make(),
            dstr_make_reserve(64),
            dstr_make(),
            dstr_make_reserve(64),
        };

        strv replacing_equal_len = strv_make_from_str("bbbbbbbb");
        strv replacing_less_len = strv_make_from_str("bbbb");
        strv replacing_greater_len = strv_make_from_str("bbbbbbbbbbbb");

        dstr_replace_range(&str[0], (dstr_size_t)0, str[0].size, replacing_equal_len);
        dstr_replace_range(&str[1], (dstr_size_t)0, str[1].size, replacing_less_len);
        dstr_replace_range(&str[2], (dstr_size_t)0, str[2].size, replacing_greater_len);

        dstr_replace_range(&str[3], (dstr_size_t)0, 4, strv_make_from_str("bbbb"));
        dstr_replace_range(&str[4], (dstr_size_t)4, 4, strv_make_from_str("bbbb"));
        dstr_replace_range(&str[5], (dstr_size_t)2, 4, strv_make_from_str("bbbb"));

        dstr_replace_range(&str[6], (dstr_size_t)0, 0, strv_make_from_str(""));
        dstr_replace_range(&str[7], (dstr_size_t)0, 0, strv_make_from_str(""));

        dstr_replace_range(&str[8], (dstr_size_t)0, 0, strv_make_from_str("abcdefgh"));
        dstr_replace_range(&str[9], (dstr_size_t)0, 0, strv_make_from_str("abcdefgh"));

        RUNIT_ASSERT(dstr_compare_str(&str[0], "bbbbbbbb") == 0, "01");
        RUNIT_ASSERT(dstr_compare_str(&str[1], "bbbb") == 0, "02");
        RUNIT_ASSERT(dstr_compare_str(&str[2], "bbbbbbbbbbbb") == 0, "03");

        RUNIT_ASSERT(dstr_compare_str(&str[3], "bbbbaaaa") == 0);
        RUNIT_ASSERT(dstr_compare_str(&str[4], "aaaabbbb") == 0);
        RUNIT_ASSERT(dstr_compare_str(&str[5], "aabbbbaa") == 0);

        for (size_t i = 0; i < SIZE; ++i) {
            dstr_destroy(&str[i]);
        }
    }
    {
        enum {
            SIZE = 7
        };

        dstr str[SIZE] = {
            dstr_make(),
            dstr_make_reserve(64),
            dstr_make_from_str(""),
            dstr_make_from_str("a"),
            dstr_make_from_str("aa"),
            dstr_make_from_str("aaa"),
            dstr_make_from_str("aaaa"),

        };

        for (size_t i = 0; i < SIZE; ++i) {
            dstr_replace_str(&str[i], "aa", "bb");
        }

        RUNIT_ASSERT(dstr_compare_str(&str[0], "") == 0);

        RUNIT_ASSERT(dstr_compare_str(&str[1], "") == 0);

        RUNIT_ASSERT(dstr_compare_str(&str[2], "") == 0);
        RUNIT_ASSERT(dstr_compare_str(&str[3], "a") == 0);
        RUNIT_ASSERT(dstr_compare_str(&str[4], "bb") == 0);
        RUNIT_ASSERT(dstr_compare_str(&str[5], "bba") == 0);
        RUNIT_ASSERT(dstr_compare_str(&str[6], "bbbb") == 0);

        for (size_t i = 0; i < SIZE; ++i) {
            dstr_destroy(&str[i]);
        }

        /* Replace nothing from 0 capacity string. */
        {
            dstr d0 = dstr_make_reserve(0);
            /* Find nothing and replaced by nothing. */
            dstr_replace_str(&d0, "", "");
            /* find nothing and replaced by test. */
            dstr_replace_str(&d0, "", "test");

            RUNIT_ASSERT(dstr_compare_str(&d0, "") == 0);
        }

        /* Replace nothing from 64 capacity empty string. */
        {
            dstr d0 = dstr_make_reserve(64);
            /* Find nothing and replaced by nothing. */
            dstr_replace_str(&d0, "", "");
            /* Find nothing and replaced by test. */
            dstr_replace_str(&d0, "", "test");

            RUNIT_ASSERT(dstr_compare_str(&d0, "") == 0);
        }

        /* Replace nothing from "aaaa" string */
        {
            dstr d1 = dstr_make_from_str("aaaa");

            dstr_replace_str(&d1, "", "");
            dstr_replace_str(&d1, "", "test");

            RUNIT_ASSERT(dstr_compare_str(&d1, "aaaa") == 0);

            dstr_destroy(&d1);
        }

        /* From "aaaa" replace "aaaa" to "". */
        {
            dstr d0 = dstr_make_from_str("aaaa");

            dstr_replace_str(&d0, "aaaa", "");

            RUNIT_ASSERT(dstr_compare_str(&d0, "") == 0);

            dstr_destroy(&d0);
        }

        /* From "aaaa" replace "a" to "". */
        {
            dstr d0 = dstr_make_from_str("aaaa");
            dstr_replace_str(&d0, "a", "");
            RUNIT_ASSERT(dstr_compare_str(&d0, "") == 0);
            dstr_destroy(&d0);
        }

        /* From "aaaa" replace "a" by "bbb". */
        {
            dstr d0 = dstr_make_from_str("aaaa");

            dstr_replace_str(&d0, "a", "bbb");

            RUNIT_ASSERT(dstr_compare_str(&d0, "bbbbbbbbbbbb") == 0);

            dstr_destroy(&d0);
        }
    }
}

static void dstr_assign_f_test()
{
    {
        dstr str = dstr_make_from_str("012345");

        dstr_assign_f(&str, "%s %s", "test1", "test2test3test4test5test6");

        RUNIT_ASSERT(dstr_compare_str(&str, "test1 test2test3test4test5test6") == 0);
        RUNIT_ASSERT(str.size == strlen(str.data));

        dstr_destroy(&str);
        RUNIT_ASSERT(str.size == strlen(str.data));
    }
    {
        dstr16 str;
        dstr16_init(&str);
        dstr16_assign_f(&str, "%s", "012345");
        dstr16_assign_f(&str, "%s", "test1 test2test3test4test5test6");

        RUNIT_ASSERT(dstr_compare_str(&str.dstr, "test1 test2test3test4test5test6") == 0);
        RUNIT_ASSERT(str.dstr.size == strlen(str.dstr.data));

        dstr16_destroy(&str);
        RUNIT_ASSERT(str.dstr.size == strlen(str.dstr.data));
    }
}

static void print_dstr(const dstr* s) {

    printf("[\"%s\"]", s->data);

    printf("[s:%zu]", s->size);
    printf("[c:%zu]", s->capacity);
}

static void print_dstr_ln(const dstr* s) {

    print_dstr(s);
    printf("\n");
}

static void dstr_local_buffer_test() {
    dstr16 str;

    dstr16_init(&str);

    RUNIT_ASSERT(str.dstr.capacity == 15);
    RUNIT_ASSERT(str.dstr.size == 0);

    dstr_assign_str(&str.dstr, "15---characters");

    RUNIT_ASSERT(str.dstr.capacity == 15);
    RUNIT_ASSERT(str.dstr.size == 15);

    dstr_assign_str(&str.dstr, "16----characters");

    RUNIT_ASSERT(str.dstr.capacity >= 16);
    RUNIT_ASSERT(str.dstr.size == 16);

    dstr16_destroy(&str);
}

static void dstr_misc_capacity_test() {
    dstr str;
    dstr_init(&str);

    RUNIT_ASSERT(str.capacity == 0);

    dstr_append(&str, strv_make_from_str("012"));

    RUNIT_ASSERT(str.capacity == 8);

    dstr_append_f(&str, "%d%d%d%d%d%d%d%d", 3, 4, 5, 6, 7, 8, 9);

    RUNIT_ASSERT(str.capacity > 8);
}