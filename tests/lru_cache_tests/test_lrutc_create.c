#include <assert.h>
#include <common_base/fptr.h>
#include <limits.h>
#include <string.h>

#include "lrutc_tests.h"

static void free_function(void *_) {
	return;
}

NO_COV void test_cm_lrutc_create(void) {
	t_cm_lrutc            *lru;
	const t_cm_lrutc_stats cmp = (t_cm_lrutc_stats){0};
	t_cm_dtor              dt  = free_function;

	lru = lrutc_create(3, dt);
	assert(lru != NULL);
	assert(memcmp(&lru->stats, &cmp, sizeof(lru->stats)) == 0);
	assert(lru->MRU == NULL);
	assert(lru->LRU == NULL);
	assert(lru->buckets_nb == 3);
	assert(lru->dtor == dt);
	assert(lru->buckets != NULL);
	lrutc_free(&lru);
	assert(lrutc_create(0, dt) == NULL);
	alloc_set_call_limit(0);
	assert(lrutc_create(3, dt) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(lrutc_create(3, dt) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
}
NO_COV void test_cm_lrutc_init(void) {
	t_cm_lrutc             lru;
	const t_cm_lrutc_stats cmp = (t_cm_lrutc_stats){0};
	t_cm_dtor              dt  = free_function;

	assert(lrutc_init(&lru, 3, dt) == &lru);
	assert(memcmp(&lru.stats, &cmp, sizeof(lru.stats)) == 0);
	assert(lru.MRU == NULL);
	assert(lru.LRU == NULL);
	assert(lru.buckets_nb == 3);
	assert(lru.dtor == dt);
	assert(lru.buckets != NULL);
	assert(lru.ns_gap == C_MINI__LRUTC_TTL_DFLT);
	lrutc_destroy(&lru);
	alloc_set_call_limit(0);
	assert(lrutc_init(&lru, 3, dt) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(lrutc_init(NULL, 3, dt) == NULL);
	assert(lrutc_init(&lru, 0, dt) == NULL);
}
NO_COV void test_cm_lrutc_destroy(void) {
	t_cm_lrutc lru;
	t_cm_dtor  dt = free_function;

	lrutc_init(&lru, 3, dt);
	assert(lru.buckets_nb == 3);
	assert(lru.dtor == dt);
	assert(lru.buckets != NULL);
	assert(lru.minHeap != NULL);
	lrutc_destroy(&lru);
	assert(lru.minHeap == NULL);
	assert(lru.buckets == NULL);
	assert(lru.buckets_nb == 0);
	lrutc_destroy(NULL);
}
NO_COV void test_cm_lrutc_free(void) {
	t_cm_lrutc *lru;
	t_cm_dtor   dt = free_function;

	lru = lrutc_create(3, dt);
	assert(lru != NULL);
	lrutc_free(&lru);
	assert(lru == NULL);
	lrutc_free(&lru);
	lrutc_free(NULL);
}

NO_COV void test_cm_lrutc_reserve(void) {
	t_cm_lrutc c, intact_copy;

	{ // simple reservation
		lrutc_init(&c, 4, NULL);
		assert(lrutc_reserve(&c, 0) == 0);
		assert(c.reserved_nodes_nb == 0);
		assert(lrutc_reserve(&c, 2) == 2);
		assert(c.reserved_nodes_nb == 2);
		assert(c.reserved_nodes != NULL);
		assert(c.reserved_nodes->lru_next != NULL);
		assert(lrutc_reserve(&c, 1) == 1);
		assert(c.reserved_nodes_nb == 3);
		lrutc_destroy(&c);
	}
	{ // capped at capacity
		lrutc_init(&c, 2, NULL);
		assert(lrutc_max_reserve(&c) == 2);
		assert(lrutc_reserve(&c, 5) == 2);
		assert(c.reserved_nodes_nb == 2);
		assert(lrutc_max_reserve(&c) == 0);
		assert(lrutc_reserve(&c, 3) == 0);
		assert(c.reserved_nodes_nb == 2);
		lrutc_destroy(&c);
	}
	{ // reserved nodes consumed on insertion
		lrutc_init(&c, 3, NULL);
		assert(lrutc_reserve(&c, 2) == 2);
		assert(lrutc_put(&c, "a", 1, "va") == true);
		assert(c.node_nb == 1);
		assert(c.reserved_nodes_nb == 1);
		assert(lrutc_put(&c, "b", 1, "vb") == true);
		assert(c.node_nb == 2);
		assert(c.reserved_nodes_nb == 0);
		assert(lrutc_max_reserve(&c) == 1);
		lrutc_destroy(&c);
	}
	{ // allocation failures
		lrutc_init(&c, 4, NULL);
		alloc_set_call_limit(0);
		assert(lrutc_reserve(&c, 2) == 0);
		assert(c.reserved_nodes_nb == 0);
		alloc_reset_counter();
		alloc_set_call_limit(1);
		assert(lrutc_reserve(&c, 2) == 1);
		assert(c.reserved_nodes_nb == 1);
		alloc_set_call_limit(ALLOC_DFT_LIMIT);
		lrutc_destroy(&c);
	}
	{ // errors
		lrutc_init(&c, 4, NULL);
		intact_copy = c;
		c.buckets   = NULL;
		assert(lrutc_reserve(&c, 1) == 0);
		c = intact_copy;
		assert(lrutc_reserve(NULL, 1) == 0);
		lrutc_destroy(&c);
	}
	return;
}
