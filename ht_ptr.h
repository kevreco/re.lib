/*

SUMMARY:

    Specialized hash table to hold pointers.
    It's using ht.h
    
    The point is to have a more convenient API than ht.h if we are going to place pointers.

NOTES:

    ht.h must be used.
    There is no smart hashing done on the pointer and the default hash is definitly not the best for pointer use.
*/

#ifndef RE_HT_PTR_H
#define RE_HT_PTR_H


#include "ht.h"

typedef struct ht_ptr_handle ht_ptr_handle;
struct ht_ptr_handle {
    void* ptr;
};

HT_API void ht_ptr_init(ht* h, ht_hash_function_t hash, ht_predicate_t items_are_same);

HT_API void ht_ptr_destroy(ht* h);

/* Insert new pointer. */
HT_API void ht_ptr_insert(ht* h, void* ptr);

/* Return item from a key. NULL is returned if nothing was found. */
HT_API void* ht_ptr_get(ht* h, void* key_ptr);

/* Remove pointer. Return true if it was removed. */
HT_API bool ht_ptr_remove(ht* h, void* ptr);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* RE_HT_PTR_H */

#if defined(HT_IMPLEMENTATION)

static void swap_ptrs(ht_ptr_handle* left, ht_ptr_handle* right)
{
    ht_ptr_handle tmp = *left;
    *left = *right;
    *right = tmp;
}

HT_API void
ht_ptr_init(ht* h, ht_hash_function_t hash, ht_predicate_t items_are_same)
{
    ht_init(h,
        sizeof(ht_ptr_handle),
        hash,
        items_are_same,
        (ht_swap_function_t)swap_ptrs,
        0);
}

HT_API void
ht_ptr_destroy(ht* h)
{
    ht_destroy(h);
}

HT_API void
ht_ptr_insert(ht* h, void* ptr)
{
    ht_ptr_handle handle = { ptr };
    ht_insert(h, &handle);
}

HT_API void*
ht_ptr_get(ht* h, void* key_ptr)
{
    ht_ptr_handle handle = { key_ptr };
    ht_ptr_handle* result = ht_get_item(h, &handle);

    return result
        ? result->ptr
        : NULL;
}

HT_API bool
ht_ptr_remove(ht* h, void* ptr)
{
    ht_ptr_handle handle = { ptr };
    return ht_erase(h, &handle);
}

#endif /* defined(HT_IMPLEMENTATION) */

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE 1 - The MIT License (MIT)

Copyright (c) 2024 kevreco

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE 2 - Public Domain (www.unlicense.org)

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>
------------------------------------------------------------------------------
*/