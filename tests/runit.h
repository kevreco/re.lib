// runit.h - v0.1 - kevreco - CC0 1.0 Licence (public domain)

/*

NOTES:
=====

- Minimalistic Unit Test framework with two function (assert & run).


CHANGES (DD/MM/YYYY):
====================

- 19/11/2016: First implementation

TODO:
====

- nothing yet

DOCUMENTATION:
==============

- Only use RUNIT_ASSERT anywhere and use RUNIT_RUN at upper level
- See the example at the end of file.

*/


#ifndef RE_RUNIT_H
#define RE_RUNIT_H

#include "stdio.h"  /* stderrm stdout */
#include <stdarg.h> /* va_start */
#ifndef RE_UNIT_API
#ifdef RE_UNIT_STATIC
#define RE_UNIT_API static
#else
#define RE_UNIT_API extern
#endif
#endif

/// Very simple unit testing framework for C/C++
#ifdef __cplusplus
extern "C" {
#endif

static int runit_count        = 0; // Number of test
static int runit_fail         = 0; // Number of failed test
static const char* runit_name = 0; // Optional name to identify where we are durring the test
static void re_unit_fprintf(FILE* const file, const char* const fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    vfprintf(file, fmt, args);

    va_end(args);

} // dstr_make_from_fmt

#ifdef __cplusplus
} /* extern "C" */
#endif

#define RE_UNIT_RESET_COUNT { runit_count = runit_fail = 0; } {runit_name = 0; }
#define __RE_UNIT_ASSERT(X, format_, msg_) \
    if (!(X)) {                                     \
        re_unit_fprintf(stderr, "runit test fail (%s:%d)  [[",  __FILE__, __LINE__); \
        if (runit_name)                             \
            re_unit_fprintf(stderr, "[%s]", runit_name);   \
        re_unit_fprintf(stderr, format_, msg_);               \
        re_unit_fprintf(stderr,"]]\n");                       \
        ++runit_fail; \
    } \
    ++runit_count;

#define _RE_UNIT_ASSERT1(X_)              __RE_UNIT_ASSERT(X_, "%s", "")
#define _RE_UNIT_ASSERT2(X_, msg_)         __RE_UNIT_ASSERT(X_, "%s", msg_)
#define _RE_UNIT_ASSERT3(X_, format_, msg_) __RE_UNIT_ASSERT(X_, format_, msg_)

#define RE_UNIT_GET_ASSERT(arg1_, arg2_, arg3_, arg4_, ...) arg4_
#define RE_UNIT_ASSERT_MACRO_CHOOSER(...) RE_UNIT_GET_ASSERT(__VA_ARGS__, \
    _RE_UNIT_ASSERT3, \
    _RE_UNIT_ASSERT2, \
    _RE_UNIT_ASSERT1)

#define RE_UNIT_ASSERT(...) RE_UNIT_ASSERT_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define RE_UNIT_RUN(X_) \
    RE_UNIT_RESET_COUNT \
    re_unit_fprintf(stdout, "===== runit %s Run =====\n", #X_); \
    X_(); \
    re_unit_fprintf(stdout, "===== runit %s Result =====\n", #X_); \
    if (runit_fail && runit_count) { \
        re_unit_fprintf(stderr, "\t%d/%d failed.\n", runit_fail, runit_count); \
    } else if (!runit_fail && !runit_count) { \
        re_unit_fprintf(stderr, "\tNo test performed.\n" ); \
    } else {\
        re_unit_fprintf(stdout,"\tOK: %d/%d successul.\n", runit_count, runit_count); \
    }
#define __RE_UNIT_ASSERT(X, format_, msg_) \
    if (!(X)) {                                     \
        re_unit_fprintf(stderr, "runit test fail (%s:%d)  [[",  __FILE__, __LINE__); \
        if (runit_name)                             \
            re_unit_fprintf(stderr, "[%s]", runit_name);   \
        re_unit_fprintf(stderr, format_, msg_);               \
        re_unit_fprintf(stderr,"]]\n");                       \
        ++runit_fail; \
    } \
    ++runit_count;

// X must be const char *
#define RE_UNIT_SET_NAME(X) runit_name = X
#define RUNIT_ASSERT RE_UNIT_ASSERT
#define RUNIT_RUN RE_UNIT_RUN

/*

Example 1:
=========

int one = 1;
int two = 3;

static void unit_test_all_in_one() {
    RUNIT_ASSERT(one == 1, "error, one != 1");
    RUNIT_ASSERT(two == 2, "error, two != 2");
}

int main(int argc, char **argv) {

    RUNIT_RUN(unit_test_all_in_one);
}

Example 2:
=========

int one = 1;
int two = 3;

static void unit_test_one() {
    RUNIT_ASSERT(one == 1, "error, one != 1");
    // many others ...
}

static void unit_test_two() {
    RUNIT_ASSERT(two == 2, "error, two != 2");
     // many others ...
}

static void unit_test_all() {
    RUNIT_RUN(unit_test_one);
    RUNIT_RUN(unit_test_two);
}


int main(int argc, char **argv) {

    unit_test_all();
}

*/

#endif // RE_RUNIT_H
