#pragma once

#define C_MINI__LRUTC_INTERNAL

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include <stdint.h>
#include <unistd.h>

#include "common_base/tests.h"
#include "lru_cache/lrutc/least_recently_used_timed_cache.h"

// ttl arguments are expressed in ms for readability, the api expects ns.
//
// LRUC_TIME_SCALE divides every duration the suite uses, ttls and sleeps alike,
// so the margins between a ttl and the sleep meant to outlive it are preserved
// while the suite stays fast (a slow machine or valgrind run sees the same
// ratios). raise it if an expiry assertion ever turns flaky, lower it to go
// faster. use a divisor of 1000000 to keep the division exact.
#define LRUC_TIME_SCALE   20ULL
#define LRUC_MS_TO_NS(ms) ((size_t)(ms) * (1000000ULL / LRUC_TIME_SCALE))

/**
 * \brief sleep for `ms` milliseconds, scaled down by LRUC_TIME_SCALE
 * \param ms wanted duration in ms (before scaling)
**/
static inline void lrutc_test_sleep_ms(unsigned ms) {
	usleep((useconds_t)((uint64_t)ms * 1000ULL / LRUC_TIME_SCALE));
}

void test_cm_lrutc_create(void);
void test_cm_lrutc_init(void);
void test_cm_lrutc_destroy(void);
void test_cm_lrutc_free(void);

void test_cm_lrutc_put_kv(void);
void test_cm_lrutc_put(void);
void test_cm_lrutc_put_ttl(void);
void test_cm_lrutc_put_evict_lru_bucket(void);
void test_cm_lrutc_put_evict_ttl_bucket(void);

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
// void test_cm_lrutci_hsh_push(void);// moved to inline

void test_cm_lrutc_foreach(void);
void test_cm_lrutc_foreach_collisions(void);
void test_cm_lrutc_foreach_ru_ordered(void);
void test_cm_lrutc_foreach_ttl_ordered(void);
