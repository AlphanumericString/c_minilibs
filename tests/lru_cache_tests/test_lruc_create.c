#include <assert.h>
#include <common_base/fptr.h>
#include <limits.h>
#include <string.h>

#include "lru_cache_tests.h"

static void free_function(void *_) {
	return;
}

NO_COV void test_lruc_create(void) {
	t_lruc            *lru;
	const t_lruc_stats cmp = (t_lruc_stats){0};
	t_cm_dtor          dt  = free_function;

	lru = lruc_create(3, dt);
	assert(lru != NULL);
	assert(memcmp(&lru->stats, &cmp, sizeof(lru->stats)) == 0);
	assert(lru->MRU == NULL);
	assert(lru->LRU == NULL);
	assert(lru->buckets_nb == 3);
	assert(lru->dtor == dt);
	assert(lru->buckets != NULL);
	lruc_free(&lru);
	assert(lruc_create(0, dt) == NULL);
	alloc_set_call_limit(0);
	assert(lruc_create(3, dt) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(lruc_create(3, dt) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
}
NO_COV void test_lruc_init(void) {
	t_lruc             lru;
	const t_lruc_stats cmp = (t_lruc_stats){0};
	t_cm_dtor          dt  = free_function;

	assert(lruc_init(&lru, 3, dt) == &lru);
	assert(memcmp(&lru.stats, &cmp, sizeof(lru.stats)) == 0);
	assert(lru.MRU == NULL);
	assert(lru.LRU == NULL);
	assert(lru.buckets_nb == 3);
	assert(lru.dtor == dt);
	assert(lru.buckets != NULL);
	assert(lru.ttl_glob == (uint64_t)C_MINI__LRUC_TTL_DFLT);
	lruc_destroy(&lru);
	alloc_set_call_limit(0);
	assert(lruc_init(&lru, 3, dt) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(lruc_init(NULL, 3, dt) == NULL);
	assert(lruc_init(&lru, 0, dt) == NULL);
}
NO_COV void test_lruc_destroy(void) {
	t_lruc    lru;
	t_cm_dtor dt = free_function;

	lruc_init(&lru, 3, dt);
	assert(lru.buckets_nb == 3);
	assert(lru.dtor == dt);
	assert(lru.buckets != NULL);
	assert(lru.minHeap != NULL);
	lruc_destroy(&lru);
	assert(lru.minHeap == NULL);
	assert(lru.buckets == NULL);
	assert(lru.buckets_nb == 0);
	lruc_destroy(NULL);
}
NO_COV void test_lruc_free(void) {
	t_lruc   *lru;
	t_cm_dtor dt = free_function;

	lru = lruc_create(3, dt);
	assert(lru != NULL);
	lruc_free(&lru);
	assert(lru == NULL);
	lruc_free(&lru);
	lruc_free(NULL);
}

NO_COV void test_lruc_reserve(void) {
	t_lruc c, intact_copy;

	{ // simple reservation
		lruc_init(&c, 4, NULL);
		assert(lruc_reserve(&c, 0) == 0);
		assert(c.reserved_nodes_nb == 0);
		assert(lruc_reserve(&c, 2) == 2);
		assert(c.reserved_nodes_nb == 2);
		assert(c.reserved_nodes != NULL);
		assert(c.reserved_nodes->lru_next != NULL);
		assert(lruc_reserve(&c, 1) == 1);
		assert(c.reserved_nodes_nb == 3);
		lruc_destroy(&c);
	}
	{ // capped at capacity
		lruc_init(&c, 2, NULL);
		assert(lruc_max_reserve(&c) == 2);
		assert(lruc_reserve(&c, 5) == 2);
		assert(c.reserved_nodes_nb == 2);
		assert(lruc_max_reserve(&c) == 0);
		assert(lruc_reserve(&c, 3) == 0);
		assert(c.reserved_nodes_nb == 2);
		lruc_destroy(&c);
	}
	{ // reserved nodes consumed on insertion
		lruc_init(&c, 3, NULL);
		assert(lruc_reserve(&c, 2) == 2);
		assert(lruc_put(&c, "a", 1, "va") == true);
		assert(c.node_nb == 1);
		assert(c.reserved_nodes_nb == 1);
		assert(lruc_put(&c, "b", 1, "vb") == true);
		assert(c.node_nb == 2);
		assert(c.reserved_nodes_nb == 0);
		assert(lruc_max_reserve(&c) == 1);
		lruc_destroy(&c);
	}
	{ // allocation failures
		lruc_init(&c, 4, NULL);
		alloc_set_call_limit(0);
		assert(lruc_reserve(&c, 2) == 0);
		assert(c.reserved_nodes_nb == 0);
		alloc_reset_counter();
		alloc_set_call_limit(1);
		assert(lruc_reserve(&c, 2) == 1);
		assert(c.reserved_nodes_nb == 1);
		alloc_set_call_limit(ALLOC_DFT_LIMIT);
		lruc_destroy(&c);
	}
	{ // errors
		lruc_init(&c, 4, NULL);
		intact_copy = c;
		c.buckets   = NULL;
		assert(lruc_reserve(&c, 1) == 0);
		c = intact_copy;
		assert(lruc_reserve(NULL, 1) == 0);
		lruc_destroy(&c);
	}
	return;
}
