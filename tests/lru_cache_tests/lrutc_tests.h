#pragma once

#include "lru_cache_tests.h"

#define C_MINI__LRUTC_INTERNAL
#include "lru_cache/lrutc/least_recently_used_timed_cache.h"

void test_cm_lrutc_create(void);
void test_cm_lrutc_init(void);
void test_cm_lrutc_destroy(void);
void test_cm_lrutc_free(void);

void test_cm_lrutc_put_kv(void);
void test_cm_lrutc_put(void);
void test_cm_lrutc_put_ttl(void);
void test_cm_lrutc_put_evict_lru_bucket(void);
void test_cm_lrutc_put_evict_ttl_bucket(void);
void test_cm_lrutc_put_update_dtor(void);

void test_cm_lrutc_get(void);
void test_cm_lrutc_getttl(void);
void test_cm_lrutc_try_get(void);
void test_cm_lrutc_get_lru(void);

void test_cm_lrutc_clear_reserved(void);
void test_cm_lrutc_clear(void);
void test_cm_lrutc_clear_to_reserved(void);
void test_cm_lrutc_remove(void);
void test_cm_lrutc_reserve(void);

void test_cm_lrutc_has(void);
void test_cm_lrutc_has_ttl(void);

void test_cm_lrutc_isexpired(void);
void test_cm_lrutc_cleanup(void);
void test_cm_lrutc_setttl(void);

void test_cm_lrutc_size(void);
void test_cm_lrutc_hits(void);
void test_cm_lrutc_misses(void);
void test_cm_lrutc_expired(void);
void test_cm_lrutc_stats(void);
void test_cm_lrutc_is_valid(void);

void test_cm_lrutc_reserved_nb(void);
void test_cm_lrutc_has_reserved(void);
void test_cm_lrutc_max_reserve(void);

void test_cm_lrutci_hp_pop(void);
void test_cm_lrutci_hp_pop_sift_right(void);
void test_cm_lrutci_hp_pop_nth(void);
void test_cm_lrutci_hp_rebuild(void);
void test_cm_lrutci_keyinhash(void);
void test_cm_lrutci_keyinlist(void);
void test_cm_lrutci_node_eq(void);
void test_cm_lrutci_expiry(void);
void test_cm_lrutci_expired(void);
void test_cm_lrutci_rsvrd_push(void);
void test_cm_lrutci_rsvrd_pop(void);
void test_cm_lrutci_hsh_pop(void);
void test_cm_lrutci_validate(void);
void test_cm_lrutci_unlink_untracked(void);

void test_cm_lrutc_foreach(void);
void test_cm_lrutc_foreach_collisions(void);
void test_cm_lrutc_foreach_ru_ordered(void);
void test_cm_lrutc_foreach_ttl_ordered(void);
