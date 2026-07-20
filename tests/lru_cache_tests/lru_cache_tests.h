#pragma once

#define C_MINI__INTERNAL

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "lru_cache/least_recently_used_cache.h"

// TODO: test_lruc_free
// TODO: test_lruc_get_lru
// TODO: better test_lruc_get -> time invalidation and all the usual jazz

void test_lruc_create(void);
void test_lruc_init(void);
void test_lruc_destroy(void);

void test_lruc_put_kv(void);
void test_lruc_put(void);
void test_lruc_put_ttl(void);

void test_lruc_get(void);

void test_lruc_clear(void);
void test_lruc_remove(void);

void test_lruc_has(void);

void test_lruc_isexpired(void);
void test_lruc_cleanup(void);
void test_lruc_setttl(void);

void test_lruc_size(void);
void test_lruc_hits(void);
void test_lruc_misses(void);
void test_lruc_expired(void);
void test_lruc_stats(void);

void test_lruc_foreach(void);
void test_lruc_foreach_ru_ordered(void);
void test_lruc_foreach_ttl_ordered(void);
