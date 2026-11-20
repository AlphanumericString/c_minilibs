#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

NO_COV void test_cm_lruc_create(void) {
	t_cm_lruc *c;

	c = lruc_create(4, NULL);
	assert(c != NULL);
	assert(c->buckets_nb == 4);
	assert(c->buckets != NULL);
	assert(c->node_nb == 0);
	assert(c->MRU == NULL && c->LRU == NULL);
	assert(lruc_is_valid(c) == true);
	lruc_free(&c);
	assert(c == NULL);

	assert(lruc_create(0, NULL) == NULL);

	// cache allocation fails
	alloc_set_call_limit(0);
	assert(lruc_create(4, NULL) == NULL);
	// cache allocated, bucket allocation fails
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(lruc_create(4, NULL) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	lruc_free(NULL);
	return;
}
NO_COV void test_cm_lruc_init(void) {
	t_cm_lruc c;

	assert(lruc_init(&c, 4, free) == &c);
	assert(c.buckets_nb == 4);
	assert(c.buckets != NULL);
	assert(c.dtor == free);
	assert(c.stats.hit == 0 && c.stats.misses == 0 && c.stats.expired == 0);
	assert(lruc_is_valid(&c) == true);
	lruc_destroy(&c);

	assert(lruc_init(NULL, 4, NULL) == NULL);
	assert(lruc_init(&c, 0, NULL) == NULL);
	alloc_set_call_limit(0);
	assert(lruc_init(&c, 4, NULL) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	return;
}
NO_COV void test_cm_lruc_destroy(void) {
	t_cm_lruc c;

	lruc_init(&c, 4, free);
	lruc_put(&c, "a", 1, strdup("A"));
	lruc_put(&c, "b", 1, strdup("B"));
	lruc_destroy(&c);
	assert(c.buckets == NULL);
	assert(c.buckets_nb == 0);
	assert(c.MRU == NULL && c.LRU == NULL);
	assert(c.node_nb == 0);
	lruc_destroy(NULL);
	return;
}
NO_COV void test_cm_lruc_free(void) {
	t_cm_lruc *c = lruc_create(2, free);

	lruc_put(c, "a", 1, strdup("A"));
	lruc_free(&c);
	assert(c == NULL);
	lruc_free(&c);
	lruc_free(NULL);
	return;
}
NO_COV void test_cm_lruc_reserve(void) {
	t_cm_lruc c;

	lruc_init(&c, 4, NULL);
	assert(lruc_reserve(&c, 2) == 2);
	assert(c.reserved_nodes_nb == 2);
	assert(c.reserved_nodes != NULL);
	assert(lruc_reserve(&c, 10) == 2); // capped by the capacity
	assert(c.reserved_nodes_nb == 4);
	assert(lruc_reserve(&c, 1) == 0);
	assert(lruc_reserve(NULL, 1) == 0);

	// reservation failure: nothing kept, nothing leaked
	lruc_clear_reserved(&c);
	alloc_set_call_limit(0);
	assert(lruc_reserve(&c, 2) == 0);
	assert(c.reserved_nodes_nb == 0);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	lruc_destroy(&c);
	return;
}
