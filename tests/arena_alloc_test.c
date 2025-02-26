#include "arena_alloc_test.h"

#include "runit.h"

#define RE_AA_IMPLEMENTATION
#include "../arena_alloc.h"

static void arena_alloc_tests();

int arena_alloc_test()
{
    RUNIT_RUN(arena_alloc_tests);
    
    return runit_fail == 0;
}

static void* arena_calloc(re_arena* a, size_t size)
{
    void* mem = re_arena_alloc(a, size);
    memset(mem, 0, size);
    return mem;
}
static void arena_alloc_tests()
{
    re_arena a;

    /* Can init */
    {
        re_arena_init(&a, 64);
    
        RUNIT_ASSERT(a.last == NULL);
        RUNIT_ASSERT(a.first == NULL);
        RUNIT_ASSERT(a.chunk_min_capacity == 64);
    
        re_arena_destroy(&a);
    }
    
    /* Can allocate tiny item */
    {
        re_arena_init(&a, 64);
    
        size_t size_to_next_bucket = 64 - sizeof(re_chunk);
        void* mem = re_arena_alloc(&a, size_to_next_bucket - 16);
        RUNIT_ASSERT(a.last != NULL);
        RUNIT_ASSERT(a.first != NULL);
        RUNIT_ASSERT(a.first == a.last);
        RUNIT_ASSERT(mem != NULL);
    
        re_arena_destroy(&a);
    }
    
    /* Can allocate big item */
    {
        re_arena_init(&a, 64);

        size_t size_to_next_bucket = 64 - sizeof(re_chunk);

        void* mem = arena_calloc(&a, size_to_next_bucket - 16);
        RUNIT_ASSERT(mem != NULL);
    
        re_chunk* first_chunk = a.last;
    
        RUNIT_ASSERT(first_chunk->size == 48);
        RUNIT_ASSERT(first_chunk->capacity == 64);
        mem = arena_calloc(&a, size_to_next_bucket * 2);
        RUNIT_ASSERT(mem != NULL);
    
        RUNIT_ASSERT(a.last != NULL);
        RUNIT_ASSERT(a.first != NULL);
        RUNIT_ASSERT(a.first != a.last);
        RUNIT_ASSERT(mem != NULL);
    
        re_arena_destroy(&a);
    }

    /* Can rollback to empty chunk from empty chunk */
    {
        re_arena_init(&a, 64);

        RUNIT_ASSERT(a.last == NULL);
        re_arena_state s = re_arena_save_state(&a);
        RUNIT_ASSERT(a.last == NULL);
        re_arena_rollback_state(&a, s);
        RUNIT_ASSERT(a.last == NULL);

        re_arena_destroy(&a);
    }
     
    /* Can rollback from allocated chunk to empty chunk */
    {
        re_arena_init(&a, 64);

        RUNIT_ASSERT(a.last == NULL);

        re_arena_state empty_state = re_arena_save_state(&a);
           
        size_t size_to_next_bucket = 64 - sizeof(re_chunk);

        void* mem = arena_calloc(&a, size_to_next_bucket);
        mem = arena_calloc(&a, size_to_next_bucket);
        mem = arena_calloc(&a, size_to_next_bucket);
        mem = arena_calloc(&a, size_to_next_bucket);

        size_t alloc_chunks = re_arena_allocated_chunk_count(&a);
        RUNIT_ASSERT(alloc_chunks == 4);
            
        re_arena_rollback_state(&a, empty_state);
        RUNIT_ASSERT(a.last != NULL);  // Allocated chunks are used but set to 0
        RUNIT_ASSERT(a.last->size == sizeof(re_chunk));

        alloc_chunks = re_arena_allocated_chunk_count(&a);
        RUNIT_ASSERT(alloc_chunks == 4);

        re_arena_destroy(&a);
    }

    /* Can rollback from allocated chunk to empty chunk */
    {
        re_arena_init(&a, 64);

        RUNIT_ASSERT(a.last == NULL);

        size_t size_to_next_bucket = 64 - sizeof(re_chunk);

        void* mem = arena_calloc(&a, size_to_next_bucket);
        mem = arena_calloc(&a, size_to_next_bucket);

        size_t alloc_chunks = re_arena_allocated_chunk_count(&a);
        RUNIT_ASSERT(alloc_chunks == 2);

        re_arena_state allocated_state = re_arena_save_state(&a);

        mem = arena_calloc(&a, size_to_next_bucket);
        mem = arena_calloc(&a, size_to_next_bucket);

        alloc_chunks = re_arena_allocated_chunk_count(&a);
        RUNIT_ASSERT(alloc_chunks == 4);

        re_arena_rollback_state(&a, allocated_state);

        RUNIT_ASSERT(a.last != NULL);  // Allocated chunks are used but set to 0
        RUNIT_ASSERT(a.last->size == 64);

        alloc_chunks = re_arena_allocated_chunk_count(&a);
        RUNIT_ASSERT(alloc_chunks == 4);

        re_arena_destroy(&a);
    }
}
