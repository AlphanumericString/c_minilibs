#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

NO_COV void test_cm_lruc_clear_reserved(void) {
	t_cm_lruc c;

	lruc_init(&c, 4, free);
	lruc_put(&c, "a", 1, strdup("A"));
	lruc_put(&c, "b", 1, strdup("B"));
	lruc_clear_to_reserved(&c);
	assert(lruc_reserved_nb(&c) == 2);
	assert(lruc_clear_reserved(&c) == true);
	assert(lruc_reserved_nb(&c) == 0);
	assert(c.reserved_nodes == NULL);
	assert(lruc_clear_reserved(&c) == true);
	assert(lruc_clear_reserved(NULL) == false);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_clear(void) {
	t_cm_lruc c;

	lruc_init(&c, 40, free);
	lruc_put(&c, "a", 1, strdup("A"));
	lruc_put(&c, "b", 1, strdup("B"));
	lruc_clear(&c);
	assert(lruc_size(&c) == 0);
	assert(c.MRU == NULL && c.LRU == NULL);
	assert(lruc_hits(&c) == 0 && lruc_misses(&c) == 0);
	// cleared nodes are freed, not reserved
	assert(lruc_reserved_nb(&c) == 0);
	assert(lruci_validate(&c) == true);

	// many buckets for few nodes: the recency walk is used
	lruc_clear(&c);
	lruc_put(&c, "a", 1, strdup("A"));
	lruc_put(&c, "b", 1, strdup("B"));
	lruc_clear(&c);
	assert(lruc_size(&c) == 0);
	assert(lruc_reserved_nb(&c) == 0);

	lruc_clear(&c); // empty cache
	lruc_clear(NULL);
	c.node_nb = 1; // incoherent state
	lruc_clear(&c);
	c.node_nb = 0;
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_clear_to_reserved(void) {
	t_cm_lruc c;

	// few nodes over few buckets: the hash walk is used
	lruc_init(&c, 2, NULL);
	lruc_put(&c, "a", 1, "A");
	lruc_put(&c, "b", 1, "B");
	lruc_clear_to_reserved(&c);
	assert(lruc_size(&c) == 0);
	assert(lruc_reserved_nb(&c) == 2);
	assert(lruci_validate(&c) == true);
	lruc_destroy(&c);

	// many buckets for few nodes: the recency walk is used
	lruc_init(&c, 40, NULL);
	lruc_put(&c, "a", 1, "A");
	lruc_put(&c, "b", 1, "B");
	lruc_clear_to_reserved(&c);
	assert(lruc_size(&c) == 0);
	assert(lruc_reserved_nb(&c) == 2);
	assert(lruci_validate(&c) == true);

	lruc_clear_to_reserved(&c); // empty cache
	lruc_clear_to_reserved(NULL);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_remove(void) {
	t_cm_lruc c;

	lruc_init(&c, 3, free);
	lruc_put(&c, "a", 1, strdup("A"));
	lruc_put(&c, "b", 1, strdup("B"));
	lruc_put(&c, "c", 1, strdup("C"));

	assert(lruc_remove(&c, "b", 1) == 0);
	assert(lruc_size(&c) == 2);
	assert(lruc_has(&c, "b", 1) == false);
	assert(lruc_reserved_nb(&c) == 1);
	assert(lruci_validate(&c) == true);

	assert(lruc_remove(&c, "b", 1) == -1); // already gone
	assert(lruc_remove(NULL, "b", 1) == -1);
	lruc_clear(&c);
	assert(lruc_remove(&c, "a", 1) == -1); // empty cache
	lruc_destroy(&c);
	return;
}
