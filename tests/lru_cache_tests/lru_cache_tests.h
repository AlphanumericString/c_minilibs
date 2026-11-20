#pragma once

#define C_MINI__LRUC_INTERNAL

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include <stdint.h>
#include <unistd.h>

#include "common_base/tests.h"
#include "lru_cache/least_recently_used_cache.h"

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
static inline void lruc_test_sleep_ms(unsigned ms) {
	usleep((useconds_t)((uint64_t)ms * 1000ULL / LRUC_TIME_SCALE));
}

void test_cm_lruc_create(void);
void test_cm_lruc_init(void);
void test_cm_lruc_destroy(void);
void test_cm_lruc_free(void);

void test_cm_lruc_put_kv(void);
void test_cm_lruc_put(void);
void test_cm_lruc_put_ttl(void);

void test_cm_lruc_get(void);
void test_cm_lruc_getttl(void);
void test_cm_lruc_try_get(void);
void test_cm_lruc_get_lru(void);

void test_cm_lruc_clear_reserved(void);
void test_cm_lruc_clear(void);
void test_cm_lruc_clear_to_reserved(void);
void test_cm_lruc_remove(void);
void test_cm_lruc_reserve(void);

void test_cm_lruc_has(void);
void test_cm_lruc_has_ttl(void);

void test_cm_lruc_isexpired(void);
void test_cm_lruc_cleanup(void);
void test_cm_lruc_setttl(void);

void test_cm_lruc_size(void);
void test_cm_lruc_hits(void);
void test_cm_lruc_misses(void);
void test_cm_lruc_expired(void);
void test_cm_lruc_stats(void);
void test_cm_lruc_is_valid(void);

void test_cm_lruc_reserved_nb(void);
void test_cm_lruc_has_reserved(void);
void test_cm_lruc_max_reserve(void);

void test_cm_lruci_hp_pop(void);
void test_cm_lruci_hp_pop_sift_right(void);
void test_cm_lruci_hp_pop_nth(void);
void test_cm_lruci_hp_rebuild(void);
void test_cm_lruci_keyinhash(void);
void test_cm_lruci_keyinlist(void);
void test_cm_lruci_node_eq(void);
void test_cm_lruci_expiry(void);
void test_cm_lruci_expired(void);
void test_cm_lruci_rsvrd_push(void);
void test_cm_lruci_rsvrd_pop(void);
void test_cm_lruci_hsh_pop(void);
// void test_cm_lruci_hsh_push(void);// moved to inline

void test_cm_lruc_foreach(void);
void test_cm_lruc_foreach_ru_ordered(void);
void test_cm_lruc_foreach_ttl_ordered(void);
