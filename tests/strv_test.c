#include "strv_test.h"

#include "string.h"
#include "assert.h"

#include "runit.h"

#define STRV_IMPLEMENTATION
#include "../strv.h"
#include "../strv_extensions.h"

static void strv_tests();

int strv_test()
{
    RUNIT_RUN(strv_tests);
    
    return runit_fail == 0;
}

static void strv_begin_and_end_test();
static void strv_compare_test();
static void strv_find_test();
static void strv_substr_test();

static void strv_starts_with_test();
static void strv_ends_with_test();
static void strv_find_first_of_chars_test();
static void strv_find_last_of_char_test();

static void strv_trimmed_test();
static void strv_remove_left_and_right_test();
static void strv_contains_char_test();

static void strv_extension_tok_test();
static void strv_extension_line_at_test();
static void strv_extension_pop_line_test();
static void strv_extension_get_surrounding_lines_test();
static void strv_extension_is_email_test();
void strv_extension_equals_new_line_insensitive_test();

static void strv_tests()
{
    strv_begin_and_end_test();
    strv_compare_test();
    strv_find_test();
    strv_substr_test();

    strv_starts_with_test();
    strv_ends_with_test();
    strv_find_first_of_chars_test();
    strv_find_last_of_char_test();

    strv_trimmed_test();
    strv_remove_left_and_right_test();
    strv_contains_char_test();

    strv_extension_tok_test();
    strv_extension_line_at_test();
    strv_extension_pop_line_test();
    strv_extension_get_surrounding_lines_test();
    strv_extension_is_email_test();
    strv_extension_equals_new_line_insensitive_test();
}

static void strv_compare_test()
{
    enum {
        SIZE = 2
    };

    strv str[SIZE] = {0};

    str[0] = strv_make_from_str("aaa");
    str[1] =  strv_make_from_str("zzzzzz");

    RUNIT_ASSERT(strv_compare(str[0], str[1]) != 0);
    RUNIT_ASSERT(strv_compare(str[1], str[0]) != 0);
}

static void strv_begin_and_end_test()
{
    struct strv text = strv_make_from_str("Hello World");
    struct strv sub = strv_substr(text, strv_begin(text), text.size);
    RUNIT_ASSERT(strv_equals_str(sub, "Hello World"));

    sub = strv_substr(text, strv_begin(text), text.size);
    RUNIT_ASSERT(strv_equals_str(sub, "Hello World"));

    sub = strv_substr(text, strv_begin(text), 5);
    RUNIT_ASSERT(strv_equals_str(sub, "Hello"));
}

static void strv_trimmed_test() {
    enum {
        SIZE = 4
    };

    strv str[SIZE] = { 0 };
    strv trimmed[SIZE] = { 0 };

    const char* cstr[SIZE] = {
        "a",
        "a",
        "abcde",
        ""
    };

    str[0] = strv_make_from_str("\t\t\t\ta");
    str[1] = strv_make_from_str("a\t\t\t\t");
    str[2] = strv_make_from_str("\t\tabcde\t\t");
    str[3] = strv_make_from_str("\t\t\t\t\t");

    for (size_t i = 0; i < SIZE; ++i) {
        trimmed[i] = strv_trimmed(str[i]);
    }
    for (size_t i = 0; i < SIZE; ++i) {
        RUNIT_ASSERT(strv_compare_str(trimmed[i], cstr[i]) == 0);
    }
}

/* Shorter alias. */
static strv strv_from_str(const char* str)
{
    return strv_make_from_str(str);
}

static void strv_remove_left_and_right_test()
{
    strv sv = strv_from_str("abcde");
    
    RUNIT_ASSERT(strv_equals_str(strv_remove_left(sv, 0), "abcde"));
    RUNIT_ASSERT(strv_equals_str(strv_remove_left(sv, 1),"bcde"));
    RUNIT_ASSERT(strv_equals_str(strv_remove_left(sv, 2), "cde"));
    RUNIT_ASSERT(strv_equals_str(strv_remove_left(sv, 5), ""));
    RUNIT_ASSERT(strv_equals_str(strv_remove_left(sv, sv.size + 10), ""));

    RUNIT_ASSERT(strv_equals_str(strv_remove_right(sv, 0), "abcde"));
    RUNIT_ASSERT(strv_equals_str(strv_remove_right(sv, 1), "abcd"));
    RUNIT_ASSERT(strv_equals_str(strv_remove_right(sv, 2), "abc"));
    RUNIT_ASSERT(strv_equals_str(strv_remove_right(sv, 5), ""));
    RUNIT_ASSERT(strv_equals_str(strv_remove_right(sv, sv.size + 10), ""));
}

static void strv_contains_char_test()
{
    strv sv = strv_from_str("abcde");
    RUNIT_ASSERT(strv_contains_char(sv, 'a'));
    RUNIT_ASSERT(strv_contains_char(sv, 'e'));
    RUNIT_ASSERT(strv_contains_char(sv, 'c'));
    RUNIT_ASSERT( ! strv_contains_char(sv, 'z'));

    RUNIT_ASSERT(strv_contains_chars(sv, strv_from_str("abcde")));
    RUNIT_ASSERT(strv_contains_chars(sv, strv_from_str("edcba")));
    RUNIT_ASSERT( ! strv_contains_chars(sv, strv_from_str("fghij")));

    RUNIT_ASSERT( ! strv_contains_chars(sv, strv_from_str("")));
    RUNIT_ASSERT( ! strv_contains_chars(strv_from_str(""), strv_from_str("abcde")));
    RUNIT_ASSERT( ! strv_contains_chars(strv_from_str(""), strv_from_str("")));
}

static void strv_find_test()
{
    strv sv =  strv_make_from_str("Hello World!");

    RUNIT_ASSERT(strv_find_str(sv, "Hello World!") == 0);
    RUNIT_ASSERT(strv_find_str(sv, "Hello") == 0);
    RUNIT_ASSERT(strv_find_str(sv, "World!") == 6);
    RUNIT_ASSERT(strv_find_str(sv, "Nothing") == STRV_NPOS);

    RUNIT_ASSERT(strv_find_str(sv, "") == STRV_NPOS);
}

static void strv_starts_with_test()
{
    strv sv = strv_make_from_str("1.2.3");

    RUNIT_ASSERT(strv_starts_with_str(sv, "1"));
    RUNIT_ASSERT(strv_starts_with_str(sv, "1.2"));
    RUNIT_ASSERT(strv_starts_with_str(sv, "1.2.3"));
    RUNIT_ASSERT(strv_starts_with_str(sv, ""));
    RUNIT_ASSERT(!strv_starts_with_str(sv, "1.2.3.4"));
}

static void strv_ends_with_test()
{
    strv sv = strv_make_from_str("1.2.3");

    RUNIT_ASSERT(strv_ends_with_str(sv, "3"));
    RUNIT_ASSERT(strv_ends_with_str(sv, "2.3"));
    RUNIT_ASSERT(strv_ends_with_str(sv, "1.2.3"));
    RUNIT_ASSERT(strv_ends_with_str(sv, ""));
    RUNIT_ASSERT(!strv_ends_with_str(sv, "1.2.3.4"));
}

static void strv_find_first_of_chars_test()
{
    strv sv = strv_from_str("abcde");


    RUNIT_ASSERT(strv_find_first_of_chars(sv, strv_from_str("abcde")) == 0);
    RUNIT_ASSERT(strv_find_first_of_chars(sv, strv_from_str("edcba")) == 0);
    RUNIT_ASSERT(strv_find_first_of_chars(sv, strv_from_str("fghij")) == STRV_NPOS);

    RUNIT_ASSERT(strv_find_first_of_chars(sv, strv_from_str("")) == STRV_NPOS);
    RUNIT_ASSERT(strv_find_first_of_chars(strv_from_str(""), strv_from_str("abcde")) == STRV_NPOS);
    RUNIT_ASSERT(strv_find_first_of_chars(strv_from_str(""), strv_from_str("")) == STRV_NPOS);
}

static void strv_find_last_of_char_test()
{
    strv sv = strv_make_from_str("1.2.3.2.1");

    RUNIT_ASSERT(strv_find_last_of_char(sv, '1') == 8);
    RUNIT_ASSERT(strv_find_last_of_char(sv, '2') == 6);
    RUNIT_ASSERT(strv_find_last_of_char(sv, '3') == 4);
    RUNIT_ASSERT(strv_find_last_of_char(sv, '4') == STRV_NPOS);

    RUNIT_ASSERT(strv_find_last_of_chars(sv, strv_make_from_str("23")) == 6);
} 

static void strv_substr_test()
{
    strv sv = strv_make_from_str("0123456789abcdefghij");
    {
        strv sub = strv_substr_from(sv, 0, 20);
        RUNIT_ASSERT(strv_equals_str(sub, "0123456789abcdefghij"));

        sub = strv_substr_from(sv, 10, 10);
        RUNIT_ASSERT(strv_equals_str(sub, "abcdefghij"));

        sub = strv_substr_from(sv, 5, 3);
        RUNIT_ASSERT(strv_equals_str(sub, "567"));

        sub = strv_substr_from(sv, 12, 8);
        RUNIT_ASSERT(strv_equals_str(sub, "cdefghij"));

        sub = strv_substr_from(sv, sv.size - 3, 3);
        RUNIT_ASSERT(strv_equals_str(sub, "hij"));
    }
}

static void strv_extension_tok_test()
{
    strv sv = strv_make_from_str("one + two * (three - four)!");
    strv delim = strv_make_from_str("! +- (*)");
    
    int i = 0;
    strv result;
    while ((result = strv_tok(sv, delim)).size)
    {
        sv = strv_remove_left(sv, strv_end(result)  - sv.data);
        switch (i)
        {
        case 0: { RUNIT_ASSERT(strv_equals_str(result, "one")) } break;
        case 1: { RUNIT_ASSERT(strv_equals_str(result, "two")) } break;
        case 2: { RUNIT_ASSERT(strv_equals_str(result, "three")) } break;
        case 3: { RUNIT_ASSERT(strv_equals_str(result, "four")) } break;
        default: RUNIT_ASSERT(0);
        }
        i++;
    }
}

static void strv_extension_line_at_test()
{
    strv text = strv_make_from_str("line1\n" "line2\n" "line3\n" "line4\n");

    strv line;

    line = strv_line_at(text, 0);
    RUNIT_ASSERT(strv_equals_str(line, "line1\n"));
    line = strv_line_at(text, 5);
    RUNIT_ASSERT(strv_equals_str(line, "line1\n"));
    line = strv_line_at(text, 6);
    RUNIT_ASSERT(strv_equals_str(line, "line2\n"));
    line = strv_line_at(text, text.size - 1);
    RUNIT_ASSERT(strv_equals_str(line, "line4\n"));

    text = strv_make_from_str("a");

    line = strv_line_at(text, 0);
    RUNIT_ASSERT(strv_equals_str(line, "a"));

    text = strv_make_from_str("aaa");

    line = strv_line_at(text, 0);
    RUNIT_ASSERT(strv_equals_str(line, "aaa"));
    line = strv_line_at(text, 1);
    RUNIT_ASSERT(strv_equals_str(line, "aaa"));
    line = strv_line_at(text, 2);
    RUNIT_ASSERT(strv_equals_str(line, "aaa"));

    /* \n */

    {
        text = strv_make_from_str("\n");

        line = strv_line_at(text, 0);
        RUNIT_ASSERT(strv_equals_str(line, "\n"));

        text = strv_make_from_str("\n\n");

        line = strv_line_at(text, 0);
        RUNIT_ASSERT(strv_equals_str(line, "\n"));
        line = strv_line_at(text, 1);
        RUNIT_ASSERT(strv_equals_str(line, "\n"));

        text = strv_make_from_str("\n\n\n");

        line = strv_line_at(text, 0);
        RUNIT_ASSERT(strv_equals_str(line, "\n"));
        line = strv_line_at(text, 1);
        RUNIT_ASSERT(strv_equals_str(line, "\n"));
        line = strv_line_at(text, 2);
        RUNIT_ASSERT(strv_equals_str(line, "\n"));
    }

    /* \r */
    {
        text = strv_make_from_str("\r");

        line = strv_line_at(text, 0);
        RUNIT_ASSERT(strv_equals_str(line, "\r"));

        text = strv_make_from_str("\r\r");

        line = strv_line_at(text, 0);
        RUNIT_ASSERT(strv_equals_str(line, "\r"));
        line = strv_line_at(text, 1);
        RUNIT_ASSERT(strv_equals_str(line, "\r"));

        text = strv_make_from_str("\r\r\r");

        line = strv_line_at(text, 0);
        RUNIT_ASSERT(strv_equals_str(line, "\r"));
        line = strv_line_at(text, 1);
        RUNIT_ASSERT(strv_equals_str(line, "\r"));
        line = strv_line_at(text, 2);
        RUNIT_ASSERT(strv_equals_str(line, "\r"));
    }

    /* \r\n */
    {
        text = strv_make_from_str("\r\n");

        line = strv_line_at(text, 0);
        RUNIT_ASSERT(strv_equals_str(line, "\r\n"));
        line = strv_line_at(text, 1);
        RUNIT_ASSERT(strv_equals_str(line, "\r\n"));

        text = strv_make_from_str("\r\n\r");

        line = strv_line_at(text, 0);
        RUNIT_ASSERT(strv_equals_str(line, "\r\n"));
        line = strv_line_at(text, 1);
        RUNIT_ASSERT(strv_equals_str(line, "\r\n"));
        line = strv_line_at(text, 2);
        RUNIT_ASSERT(strv_equals_str(line, "\r"));

        text = strv_make_from_str("\r\r\n\n");

        line = strv_line_at(text, 0);
        RUNIT_ASSERT(strv_equals_str(line, "\r"));
        line = strv_line_at(text, 1);
        RUNIT_ASSERT(strv_equals_str(line, "\r\n"));
        line = strv_line_at(text, 2);
        RUNIT_ASSERT(strv_equals_str(line, "\r\n"));
        line = strv_line_at(text, 3);
        RUNIT_ASSERT(strv_equals_str(line, "\n"));
    }

    text = strv_make_from_str("");
    line = strv_line_at(text, 0);
    RUNIT_ASSERT(strv_equals_str(line, ""));
}

static void strv_extension_pop_line_test()
{
    strv lines = strv_make_from_str("\n\n\n\n\n");
    strv line;
    while ((line = strv_pop_line(&lines)).size)
    {
        RUNIT_ASSERT(strv_equals_str(line, "\n"));
    }

    lines = strv_make_from_str("line1\r\nline2\nline3\rline4\r\n");

    RUNIT_ASSERT(strv_equals_str(strv_pop_line(&lines), "line1\r\n"));
    RUNIT_ASSERT(strv_equals_str(lines, "line2\nline3\rline4\r\n"));
    RUNIT_ASSERT(strv_equals_str(strv_pop_line(&lines), "line2\n"));
    RUNIT_ASSERT(strv_equals_str(lines, "line3\rline4\r\n"));
    RUNIT_ASSERT(strv_equals_str(strv_pop_line(&lines), "line3\r"));
    RUNIT_ASSERT(strv_equals_str(lines, "line4\r\n"));
    RUNIT_ASSERT(strv_equals_str(strv_pop_line(&lines), "line4\r\n"));
}

static void strv_extension_get_surrounding_lines_test()
{
    size_t previous_line_count;
    size_t next_line_count;
    strv sv;
    strv result;
    
    /* Empty */
    {
        sv = strv_make_from_str("");

        result = strv_get_surrounding_lines(sv, 0, 0, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, ""));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);

        result = strv_get_surrounding_lines(sv, 0, 10, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, ""));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);
    }

    /* Word */
    {
        sv = strv_make_from_str("Hello");

        result = strv_get_surrounding_lines(sv, 0, 0, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "Hello"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);

        result = strv_get_surrounding_lines(sv, 5, 0, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "Hello"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);

        result = strv_get_surrounding_lines(sv, 0, 10, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "Hello"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);
    }
    /* Line */
    {
        sv = strv_make_from_str("Hello\r\n");

        result = strv_get_surrounding_lines(sv, 0, 0, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "Hello\r\n"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);

        result = strv_get_surrounding_lines(sv, 5, 0, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "Hello\r\n"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);

        result = strv_get_surrounding_lines(sv, 6, 0, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "Hello\r\n"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);

        result = strv_get_surrounding_lines(sv, 0, 10, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "Hello\r\n"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);
    }
    /* Lines */
    {
        sv = strv_make_from_str("Hello\nWorld\r!\r\n");

        result = strv_get_surrounding_lines(sv, 0, 1, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "Hello\nWorld\r"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 1);

        result = strv_get_surrounding_lines(sv, 6, 0, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "World\r"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);

        result = strv_get_surrounding_lines(sv, 6, 1, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "Hello\nWorld\r!\r\n"));
        RUNIT_ASSERT(previous_line_count == 1 && next_line_count == 1);
    }

    {
        sv = strv_make_from_str("a\nb\rc\r\nd\ne");

        result = strv_get_surrounding_lines(sv, 4, 0, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "c\r\n"));
        RUNIT_ASSERT(previous_line_count == 0 && next_line_count == 0);

        result = strv_get_surrounding_lines(sv, 4, 2, &previous_line_count, &next_line_count);
        RUNIT_ASSERT(strv_equals_str(result, "a\nb\rc\r\nd\ne"));
        RUNIT_ASSERT(previous_line_count == 2 && next_line_count == 2);
    }
}

static void strv_extension_is_email_test()
{
    RUNIT_ASSERT(strv_is_email(strv_from_str("test.test@test.test")));
    RUNIT_ASSERT(strv_is_email(strv_from_str("a@a.a")));
    RUNIT_ASSERT(strv_is_email(strv_from_str("a.a@a.a")));
    RUNIT_ASSERT(strv_is_email(strv_from_str("a.a@a.a.a")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("a.a@a..a.a")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("a.a@a.a..a")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str(" a@a.a")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("a@a.a ")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("a")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("aaa")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("aa@a")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("aa@.")));

    RUNIT_ASSERT(!strv_is_email(strv_from_str("aa@.a")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("aa@a.")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("a.aa@")));
    RUNIT_ASSERT(!strv_is_email(strv_from_str("a.a@a")));
}

void strv_extension_equals_new_line_insensitive_test()
{
    struct pair {
        bool equals;
        strv left;
        strv right;
    } pairs[] = {
        { true, STRV(""), STRV("") },
        { true, STRV("a"), STRV("a") },
        { true, STRV("aa"), STRV("aa") },
        
        { false, STRV("\na"), STRV("a") }, 
        { false, STRV("\ra"), STRV("a") },
        { false, STRV("\r\na"), STRV("a") },
        
        { false, STRV("a\n"), STRV("a") },
        { false, STRV("a\r"), STRV("a") },
        { false, STRV("a\r\n"), STRV("a") },
        
        { false, STRV("a"), STRV("a\n") },
        { false, STRV("a"), STRV("a\r") },
        { false, STRV("a"), STRV("a\r\n") },
        
        { false, STRV("a\n"), STRV("a\n\n") },
        { false, STRV("a\r"), STRV("a\n\n") },
        { false, STRV("a\r\n"), STRV("a\r\r") },
        
        { false, STRV("a\r\n"), STRV("a\r\n\r\n") },
        { false, STRV("a\r\nb"), STRV("a\r\nb\r\n") },
        { false, STRV("a\r\rb"), STRV("a  b") },
       
        { false, STRV("a\n"), STRV("a\r\r") },
        { false, STRV("a\r"), STRV("a\r\r") },
        { false, STRV("a\r\n"), STRV("a\r\r") },
        
        { false, STRV("a\n"), STRV("a\n\r") },
        { false, STRV("a\r"), STRV("a\n\r") },
        { false, STRV("a\r\n"), STRV("a\n\r") },
        
        { true, STRV("\na"), STRV("\na") },
        { true, STRV("\ra"), STRV("\na") },
        { true, STRV("\r\na"), STRV("\na") },
        
        { true, STRV("\na"), STRV("\ra") },
        { true, STRV("\ra"), STRV("\ra") },
        { true, STRV("\r\na"), STRV("\ra") },
        
        { true, STRV("\na"), STRV("\r\na") },
        { true, STRV("\ra"), STRV("\r\na") },
        { true, STRV("\r\na"), STRV("\r\na") },
        
        { true, STRV("a\n"), STRV("a\n") },
        { true, STRV("a\r"), STRV("a\n") },
        { true, STRV("a\r\n"), STRV("a\n") },
        
        { true, STRV("a\n"), STRV("a\r") },
        { true, STRV("b\r"), STRV("b\r") },
        { true, STRV("c\r\n"), STRV("c\r") },
        
        { true, STRV("a\n"), STRV("a\r\n") },
        { true, STRV("b\r"), STRV("b\r\n") },
        { true, STRV("c\r\n"), STRV("c\r\n") },
        
        { true, STRV("\r\na"), STRV("\r\na") },
        { false, STRV("\r\ra"), STRV("\r\na") },
        
        { false, STRV("\na"), STRV("\n\ra") },
        { false, STRV("\ra"), STRV("\n\ra") },
        { false, STRV("\r\na"), STRV("\n\ra") },
        
        { false, STRV("a    \r\nb"), STRV("a    b") },
        { false, STRV("a ^\r\n"), STRV("a\n ^\n")},
        
        { false, {0,0}, {0,0}}
    };

    struct pair* p = pairs;
    while(p->left.data) {
        RUNIT_ASSERT(p->equals == strv_equals_newline_insensitive(p->left, p->right));
        p += 1;
    }
}
