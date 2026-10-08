#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache_tests.h"

void test_lruc_create(void) {
	t_lruc                  *lru;
	const typeof(lru->stats) cmp = (typeof(lru->stats)){0};

	lru = lruc_create(3, (void *)0xDEADBEEF);
	assert(lru != NULL);
	assert(memcmp(&lru->stats, &cmp, sizeof(lru->stats)) == 0);
	assert(lru->MRU == NULL);
	assert(lru->LRU == NULL);
	assert(lru->buckets_nb == 3);
	assert(lru->dtor == (void *)0xDEADBEEF);
	assert(lru->buckets != NULL);
	lruc_destroy(lru);
	assert(lruc_create(0, (void *)0xDEADBEEF) == NULL);
	free(lru);
}
void test_lruc_init(void) {
	t_lruc                  lru;
	const typeof(lru.stats) cmp = (typeof(lru.stats)){0};

	assert(lruc_init(&lru, 3, (void *)0xDEADBEEF) == &lru);
	assert(memcmp(&lru.stats, &cmp, sizeof(lru.stats)) == 0);
	assert(lru.MRU == NULL);
	assert(lru.LRU == NULL);
	assert(lru.buckets_nb == 3);
	assert(lru.dtor == (void *)0xDEADBEEF);
	assert(lru.buckets != NULL);
	assert(lru.ttl_glob == (typeof(lru.ttl_glob))C_MINI__LRUC_TTL_DFLT);
	lruc_destroy(&lru);
	assert(lruc_init(NULL, 3, (void *)0xDEADBEEF) == NULL);
	assert(lruc_init(&lru, 0, (void *)0xDEADBEEF) == NULL);
}
void test_lruc_destroy(void) {
	t_lruc lru;

	lruc_init(&lru, 3, (void *)0xDEADBEEF);
	assert(lru.buckets_nb == 3);
	assert(lru.dtor == (void *)0xDEADBEEF);
	assert(lru.buckets != NULL);
	assert(lru.minHeap != NULL);
	lruc_destroy(&lru);
	assert(lru.minHeap == NULL);
	assert(lru.buckets == NULL);
	assert(lru.buckets_nb == 0);
	lruc_destroy(NULL);
}
void test_lruc_free(void) {
	t_lruc *lru;

	lru = lruc_create(3, (void *)0xDEADBEEF);
	assert(lru != NULL);
	lruc_free(&lru);
	assert(lru == NULL);
	lruc_free(&lru);
	lruc_free(NULL);
}
