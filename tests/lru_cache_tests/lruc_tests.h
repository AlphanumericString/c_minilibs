#pragma once

#include "lru_cache_tests.h"

#define C_MINI__LRUC_INTERNAL
#include "lru_cache/lruc/least_recently_used_cache.h"

void test_cm_lruc_create(void);
void test_cm_lruc_init(void);
void test_cm_lruc_destroy(void);
void test_cm_lruc_free(void);
void test_cm_lruc_reserve(void);

void test_cm_lruc_put_kv(void);
void test_cm_lruc_put(void);
void test_cm_lruc_put_evict(void);
void test_cm_lruc_put_evict_bucket(void);

void test_cm_lruc_try_get(void);
void test_cm_lruc_get(void);

void test_cm_lruc_clear(void);
void test_cm_lruc_clear_reserved(void);
void test_cm_lruc_clear_to_reserved(void);
void test_cm_lruc_remove(void);

void test_cm_lruc_has(void);

void test_cm_lruc_size(void);
void test_cm_lruc_hits(void);
void test_cm_lruc_misses(void);
void test_cm_lruc_expired(void);
void test_cm_lruc_stats(void);
void test_cm_lruc_is_valid(void);

void test_cm_lruc_reserved_nb(void);
void test_cm_lruc_has_reserved(void);
void test_cm_lruc_max_reserve(void);

void test_cm_lruc_foreach(void);
void test_cm_lruc_foreach_collisions(void);
void test_cm_lruc_foreach_ru_ordered(void);

void test_cm_lruc_iter(void);
void test_cm_lruc_iter_promote_long(void);

void test_cm_lruci_lru_push_pop(void);
void test_cm_lruci_hsh_pop(void);
void test_cm_lruci_rsvrd_push_pop(void);
void test_cm_lruci_keyinhash(void);
void test_cm_lruci_keyinlist(void);
void test_cm_lruci_node_eq(void);
void test_cm_lruci_unlink_node(void);
void test_cm_lruci_validate(void);
