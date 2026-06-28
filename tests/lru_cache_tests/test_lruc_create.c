#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache/least_recently_used_cache.h"
#include "lru_cache_tests.h"

void test_lruc_create(void) {
	t_lruc            *lru;
	const t_lruc_stats cmp = (t_lruc_stats){0};

	lru = cache_create(3, (void *)0xDEADBEEF);
	assert(lru != NULL);
	assert(memcmp(&lru->stats, &cmp, sizeof(t_lruc_stats)) == 0);
	assert(lru->MRU == NULL);
	assert(lru->LRU == NULL);
	assert(lru->cap == 3);
	assert(lru->dtor == (void *)0xDEADBEEF);
	assert(lru->h_arr != NULL);
	cache_destroy(lru);
	free(lru);
}
void test_lruc_init(void) {
	t_lruc             lru;
	const t_lruc_stats cmp = (t_lruc_stats){0};

	cache_init(&lru, 3, (void *)0xDEADBEEF);
	assert(memcmp(&lru.stats, &cmp, sizeof(t_lruc_stats)) == 0);
	assert(lru.MRU == NULL);
	assert(lru.LRU == NULL);
	assert(lru.cap == 3);
	assert(lru.dtor == (void *)0xDEADBEEF);
	assert(lru.h_arr != NULL);
	cache_destroy(&lru);
}
void test_lruc_destroy(void) {
	t_lruc lru;

	cache_init(&lru, 3, (void *)0xDEADBEEF);
	assert(lru.cap == 3);
	assert(lru.dtor == (void *)0xDEADBEEF);
	assert(lru.h_arr != NULL);
	cache_destroy(&lru);
	assert(lru.h_arr == NULL);
	assert(lru.cap == 0);
	assert(lru.cap == 0);
}
