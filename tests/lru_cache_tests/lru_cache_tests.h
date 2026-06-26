#pragma once

#include "lru_cache/least_recently_used_cache.h"

// create
void test_lruc_create(void);
void test_lruc_init(void);
void test_lruc_destroy(void);

// put
void test_lruc_put_kv(void);
void test_lruc_put(void);
void test_lruc_put_ttl(void);

// get
void test_lruc_get(void);

// rms
void test_lruc_clear(void);
void test_lruc_remove(void);

// has
void test_lruc_has(void);

// expired
void test_lruc_isexpired(void);
void test_lruc_cleanup(void);
void test_lruc_setttl(void);

// infos
void test_lruc_size(void);
void test_lruc_hits(void);
void test_lruc_misses(void);
void test_lruc_expired(void);
void test_lruc_stats(void);

// foreach
void test_lruc_foreach(void);
