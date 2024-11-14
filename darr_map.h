/* darr_map.h - v0.0 - kevreco - CC0 1.0 Licence (public domain) */

/* This library requires darr.h */

/*

NOTES:
======

-

CHANGES (DD/MM/YYYY):
=====================

-

TODO:
=====

-

*/

#ifndef RE_DARR_MAP_H
#define RE_DARR_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct darr_map darr_map;
struct darr_map {
    darr arr;
    darr_size_t sizeof_item;
    darr_predicate_t comp;
};

/*-------------------------------------------------------------------------*/
/* darr_map - API */
/*-------------------------------------------------------------------------*/

DARR_API void darr_map_init(darr_map* m, darr_size_t sizeof_item, darr_predicate_t comp);
DARR_API void darr_map_destroy(darr_map* m);

DARR_API darr_bool darr_map_get(darr_map* m, const void* item, void* result);

/* set value, returns true if the value didn't exist yet, false if the value was replaced */
DARR_API darr_bool darr_map_set(darr_map* m, void* item);
/* remove value at key, return true of a value was indeed remove, false if there was nothing to remove. */
DARR_API darr_bool darr_map_remove(darr_map* m, const void* item);
DARR_API darr_bool darr_map_contains(const darr_map* m, const void* item);
DARR_API darr_size_t darr_map_size(const darr_map* m);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* RE_DARR_MAP_H */

#ifdef RE_DARR_MAP_IMPLEMENTATION

DARR_API void
darr_map_init(darr_map* m, darr_size_t sizeof_item, darr_predicate_t comp)
{
	DARR_ASSERT(m);
	DARR_ASSERT(comp);

	darr_init(&m->arr, sizeof_item);
	m->sizeof_item = sizeof_item;
	m->comp = comp;
}

DARR_API void
darr_map_destroy(darr_map* m)
{
	darr_destroy(&m->arr);
	m->sizeof_item = 0;
	m->comp = 0;
}

DARR_API darr_bool
darr_map_get(darr_map* m, const void* item, void* result)
{
	darr_size_t index = darr_binary_find_predicate(&m->arr, item, m->comp);
	if (index != m->arr.size)
	{
		DARR_MEMCPY(result, darr_ptr(&m->arr, index), m->sizeof_item);
		return (darr_bool)1;
	}

	return (darr_bool)0;
}

DARR_API darr_bool
darr_map_set(darr_map* m, void* item)
{
	darr_size_t index = darr_binary_find_predicate(&m->arr, item, m->comp);
	if (index != m->arr.size)
	{
		DARR_MEMCPY(darr_ptr(&m->arr, index), item, m->sizeof_item);
		return (darr_bool)0;
	}
	else
	{
		darr_insert_one_sorted(&m->arr, item, m->comp);
		return (darr_bool)1;
	}
}

DARR_API darr_bool
darr_map_remove(darr_map* m, const void* item)
{
	darr_size_t index = darr_binary_find_predicate(&m->arr, item, m->comp);
	if (index != m->arr.size)
	{
		darr_erase_one(&m->arr, index);
		return (darr_bool)1;
	}

	return (darr_bool)0;
}

DARR_API darr_bool
darr_map_contains(const darr_map* m, const void* item)
{
	darr_size_t index = darr_binary_find_predicate(&m->arr, item, m->comp);
	return index != m->arr.size;
}

DARR_API darr_size_t
darr_map_size(const darr_map* m)
{
	return darr_size(&m->arr);
}

#endif /* RE_DARR_MAP_IMPLEMENTATION */
