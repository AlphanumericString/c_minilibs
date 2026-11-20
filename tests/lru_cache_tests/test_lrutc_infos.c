#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

NO_COV void test_cm_lrutc_size(void) {
	t_cm_lrutc   c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	lrutc_init(&c, 4, free);
	assert(c.node_nb == 0);
	assert(lrutc_size(&c) == 0);
	lrutc_put(&c, k1, strlen(k1), strdup("pique sous"));
	assert(lrutc_size(&c) == 1);
	lrutc_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	assert(lrutc_size(&c) == 2);
	lrutc_put(&c, k3, strlen(k3), strdup("grippe sous"));
	assert(lrutc_size(&c) == 3);
	lrutc_remove(&c, k2, strlen(k2));
	lrutc_clear_reserved(&c);
	assert(lrutc_size(&c) == 2);
	lrutc_clear(&c);
	assert(lrutc_size(&c) == 0);
	lrutc_destroy(&c);
	return;
}
NO_COV void test_cm_lrutc_hits(void) {
	t_cm_lrutc   c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	lrutc_init(&c, 4, free);
	assert(c.node_nb == 0);
	assert(lrutc_hits(&c) == 0);
	lrutc_put(&c, k1, strlen(k1), strdup("pique sous"));
	lrutc_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	lrutc_put(&c, k3, strlen(k3), strdup("grippe sous"));
	assert(lrutc_hits(&c) == 0);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_hits(&c) == 1);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_hits(&c) == 4);
	lrutc_remove(&c, k2, strlen(k2));
	assert(lrutc_try_get(&c, k2, strlen(k2)) == NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) == NULL);
	assert(lrutc_hits(&c) == 4);
	lrutc_clear(&c);
	assert(lrutc_hits(&c) == 0);
	lrutc_destroy(&c);
	return;
}
NO_COV void test_cm_lrutc_misses(void) {
	t_cm_lrutc   c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	lrutc_init(&c, 4, free);
	assert(c.node_nb == 0);
	assert(lrutc_hits(&c) == 0);
	lrutc_put(&c, k1, strlen(k1), strdup("pique sous"));
	lrutc_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	lrutc_put(&c, k3, strlen(k3), strdup("grippe sous"));
	assert(lrutc_misses(&c) == 0);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_misses(&c) == 0);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_misses(&c) == 0);
	lrutc_remove(&c, k2, strlen(k2));
	assert(lrutc_try_get(&c, k2, strlen(k2)) == NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) == NULL);
	assert(lrutc_misses(&c) == 2);
	lrutc_clear(&c);
	assert(lrutc_misses(&c) == 0);
	lrutc_destroy(&c);
	return;
}

NO_COV void test_cm_lrutc_expired(void) {
	t_cm_lrutc   c;
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;

	v1 = strdup("value 1");
	v2 = strdup("value 2");
	v3 = strdup("value 3");
	v4 = strdup("value 4");
	lrutc_init(&c, 4, free);
	lrutc_setttl(&c, LRUC_MS_TO_NS(200));
	lrutc_put(&c, k1, strlen(k1), v1);
	lrutc_put(&c, k2, strlen(k2), v2);
	lrutc_put_ttl(&c, k3, strlen(k3), v3, LRUC_MS_TO_NS(3000));
	lrutc_put(&c, k4, strlen(k4), v4);
	assert(c.MRU->kv.key == k4);
	lrutc_test_sleep_ms(1000);
	lrutc_cleanup(&c);
	assert(lrutc_has(&c, k1, strlen(k1)) == false);
	assert(lrutc_has(&c, k2, strlen(k2)) == false);
	assert(lrutc_has(&c, k3, strlen(k3)) == true);
	assert(lrutc_has(&c, k4, strlen(k4)) == false);
	assert(lrutc_expired(&c) == 3);
	lrutc_destroy(&c);
	return;
}

NO_COV void test_cm_lrutc_stats(void) {
	t_cm_lrutc       c;
	t_cm_lrutc_stats st;
	const char     *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	lrutc_init(&c, 4, free);
	assert(c.node_nb == 0);
	st = lrutc_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);
	lrutc_put(&c, k1, strlen(k1), strdup("pique sous"));
	lrutc_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	lrutc_put(&c, k3, strlen(k3), strdup("grippe sous"));
	st = lrutc_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	st = lrutc_stats(&c);
	assert(st.misses == 0 && st.hit == 1 && st.expired == 0);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) != NULL);
	st = lrutc_stats(&c);
	assert(st.misses == 0 && st.hit == 4 && st.expired == 0);
	lrutc_remove(&c, k2, strlen(k2));
	assert(lrutc_try_get(&c, k2, strlen(k2)) == NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) == NULL);
	st = lrutc_stats(&c);
	assert(st.misses == 2 && st.hit == 4 && st.expired == 0);
	lrutc_clear(&c);
	st = lrutc_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);
	lrutc_destroy(&c);
	return;
}

NO_COV void test_cm_lrutc_is_valid(void) {
	t_cm_lrutc   c, intact_copy;
	const char *k1 = "titi", *k2 = "toto";

	lrutc_init(&c, 4, free);
	assert(lrutc_is_valid(&c) == true);
	lrutc_put(&c, k1, strlen(k1), strdup("pique sous"));
	lrutc_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	assert(lrutc_is_valid(&c) == true);
	intact_copy = c;

	assert(lrutc_is_valid(NULL) == false);

	c.buckets = NULL;
	assert(lrutc_is_valid(&c) == false);
	c = intact_copy;

	c.minHeap = NULL;
	assert(lrutc_is_valid(&c) == false);
	c = intact_copy;

	c.buckets_nb = 0;
	assert(lrutc_is_valid(&c) == false);
	c = intact_copy;

	c.node_nb = c.buckets_nb + 1;
	assert(lrutc_is_valid(&c) == false);
	c = intact_copy;

	c.MRU = NULL;
	assert(lrutc_is_valid(&c) == false);
	c = intact_copy;

	c.LRU = NULL;
	assert(lrutc_is_valid(&c) == false);
	c = intact_copy;

	lrutc_clear(&c);
	assert(lrutc_is_valid(&c) == true);
	lrutc_destroy(&c);
	return;
}

NO_COV void test_cm_lrutc_reserved_nb(void) {
	t_cm_lrutc   c;
	const char *k1 = "titi";

	lrutc_init(&c, 4, free);
	assert(lrutc_reserved_nb(&c) == 0);
	lrutc_put(&c, k1, strlen(k1), strdup("pique sous"));
	assert(lrutc_reserved_nb(&c) == 0);
	lrutc_remove(&c, k1, strlen(k1));
	assert(lrutc_reserved_nb(&c) == 1);
	assert(lrutc_reserve(&c, 2) == 2);
	assert(lrutc_reserved_nb(&c) == 3);
	lrutc_clear_reserved(&c);
	assert(lrutc_reserved_nb(&c) == 0);
	lrutc_destroy(&c);
	return;
}

NO_COV void test_cm_lrutc_has_reserved(void) {
	t_cm_lrutc   c;
	const char *k1 = "titi";

	lrutc_init(&c, 4, free);
	assert(lrutc_has_reserved(&c) == false);
	lrutc_put(&c, k1, strlen(k1), strdup("pique sous"));
	assert(lrutc_has_reserved(&c) == false);
	lrutc_remove(&c, k1, strlen(k1));
	assert(lrutc_has_reserved(&c) == true);
	lrutc_clear_reserved(&c);
	assert(lrutc_has_reserved(&c) == false);
	lrutc_destroy(&c);
	return;
}

NO_COV void test_cm_lrutc_max_reserve(void) {
	t_cm_lrutc   c;
	const char *k1 = "titi";

	lrutc_init(&c, 4, free);
	assert(lrutc_max_reserve(&c) == 4);
	lrutc_put(&c, k1, strlen(k1), strdup("pique sous"));
	assert(lrutc_max_reserve(&c) == 3);
	assert(lrutc_reserve(&c, 2) == 2);
	assert(lrutc_max_reserve(&c) == 1);
	assert(lrutc_remove(&c, k1, strlen(k1)) == 0);
	assert(lrutc_max_reserve(&c) == 1);
	lrutc_clear_reserved(&c);
	assert(lrutc_max_reserve(&c) == 4);
	lrutc_destroy(&c);
	return;
}
