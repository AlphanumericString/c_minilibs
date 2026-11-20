#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

NO_COV void test_cm_lruc_size(void) {
	t_cm_lruc   c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	lruc_init(&c, 4, free);
	assert(lruc_size(&c) == 0);
	lruc_put(&c, k1, strlen(k1), strdup("un"));
	assert(lruc_size(&c) == 1);
	lruc_put(&c, k2, strlen(k2), strdup("deux"));
	assert(lruc_size(&c) == 2);
	lruc_put(&c, k3, strlen(k3), strdup("trois"));
	assert(lruc_size(&c) == 3);
	lruc_remove(&c, k2, strlen(k2));
	assert(lruc_size(&c) == 2);
	lruc_clear(&c);
	assert(lruc_size(&c) == 0);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_hits(void) {
	t_cm_lruc c;

	lruc_init(&c, 4, NULL);
	lruc_put(&c, "a", 1, "A");
	assert(lruc_hits(&c) == 0);
	assert(lruc_try_get(&c, "a", 1) != NULL);
	assert(lruc_hits(&c) == 1);
	assert(lruc_try_get(&c, "a", 1) != NULL);
	assert(lruc_hits(&c) == 2);
	assert(lruc_try_get(&c, "nope", 4) == NULL);
	assert(lruc_hits(&c) == 2);
	lruc_clear(&c);
	assert(lruc_hits(&c) == 0);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_misses(void) {
	t_cm_lruc c;

	lruc_init(&c, 4, NULL);
	lruc_put(&c, "a", 1, "A");
	assert(lruc_misses(&c) == 0);
	assert(lruc_try_get(&c, "nope", 4) == NULL);
	assert(lruc_misses(&c) == 1);
	assert(lruc_try_get(&c, "a", 1) != NULL);
	assert(lruc_misses(&c) == 1);
	lruc_clear(&c);
	assert(lruc_misses(&c) == 0);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_expired(void) {
	t_cm_lruc c;

	lruc_init(&c, 2, NULL);
	assert(lruc_expired(&c) == 0);
	lruc_put(&c, "a", 1, "A");
	lruc_put(&c, "b", 1, "B");
	assert(lruc_expired(&c) == 0);
	// full cache: the new entry evicts the least recently used one
	lruc_put(&c, "c", 1, "C");
	assert(lruc_expired(&c) == 1);
	lruc_put(&c, "d", 1, "D");
	assert(lruc_expired(&c) == 2);
	// an update is not an eviction
	lruc_put(&c, "c", 1, "C2");
	assert(lruc_expired(&c) == 2);
	lruc_clear(&c);
	assert(lruc_expired(&c) == 0);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_stats(void) {
	t_cm_lruc       c;
	t_cm_lruc_stats st;

	lruc_init(&c, 2, NULL);
	st = lruc_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);
	lruc_put(&c, "a", 1, "A");
	lruc_put(&c, "b", 1, "B");
	assert(lruc_try_get(&c, "a", 1) != NULL);
	assert(lruc_try_get(&c, "nope", 4) == NULL);
	lruc_put(&c, "c", 1, "C"); // evicts b
	st = lruc_stats(&c);
	assert(st.hit == 1 && st.misses == 1 && st.expired == 1);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_is_valid(void) {
	t_cm_lruc c, intact;

	lruc_init(&c, 4, NULL);
	assert(lruc_is_valid(&c) == true);
	lruc_put(&c, "a", 1, "A");
	lruc_put(&c, "b", 1, "B");
	assert(lruc_is_valid(&c) == true);
	intact = c;

	assert(lruc_is_valid(NULL) == false);

	c.buckets = NULL;
	assert(lruc_is_valid(&c) == false);
	c = intact;

	c.buckets_nb = 0;
	assert(lruc_is_valid(&c) == false);
	c = intact;

	c.node_nb = c.buckets_nb + 1;
	assert(lruc_is_valid(&c) == false);
	c = intact;

	c.MRU = NULL;
	assert(lruc_is_valid(&c) == false);
	c = intact;

	c.LRU = NULL;
	assert(lruc_is_valid(&c) == false);
	c = intact;

	lruc_clear(&c);
	assert(lruc_is_valid(&c) == true);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_reserved_nb(void) {
	t_cm_lruc c;

	lruc_init(&c, 4, NULL);
	assert(lruc_reserved_nb(&c) == 0);
	lruc_put(&c, "a", 1, "A");
	assert(lruc_reserved_nb(&c) == 0);
	lruc_remove(&c, "a", 1);
	assert(lruc_reserved_nb(&c) == 1);
	assert(lruc_reserve(&c, 2) == 2);
	assert(lruc_reserved_nb(&c) == 3);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_has_reserved(void) {
	t_cm_lruc c;

	lruc_init(&c, 4, NULL);
	assert(lruc_has_reserved(&c) == false);
	lruc_put(&c, "a", 1, "A");
	assert(lruc_has_reserved(&c) == false);
	lruc_remove(&c, "a", 1);
	assert(lruc_has_reserved(&c) == true);
	lruc_clear_reserved(&c);
	assert(lruc_has_reserved(&c) == false);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_max_reserve(void) {
	t_cm_lruc c;

	lruc_init(&c, 4, NULL);
	assert(lruc_max_reserve(&c) == 4);
	lruc_put(&c, "a", 1, "A");
	assert(lruc_max_reserve(&c) == 3);
	assert(lruc_reserve(&c, 2) == 2);
	assert(lruc_max_reserve(&c) == 1);
	assert(lruc_remove(&c, "a", 1) == 0);
	assert(lruc_max_reserve(&c) == 1);
	lruc_clear_reserved(&c);
	assert(lruc_max_reserve(&c) == 4);
	lruc_destroy(&c);
	return;
}
