#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

NO_COV void test_lruc_size(void) {
	t_lruc      c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	lruc_init(&c, 4, free);
	assert(c.node_nb == 0);
	assert(lruc_size(&c) == 0);
	lruc_put(&c, k1, strlen(k1), strdup("pique sous"));
	assert(lruc_size(&c) == 1);
	lruc_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	assert(lruc_size(&c) == 2);
	lruc_put(&c, k3, strlen(k3), strdup("grippe sous"));
	assert(lruc_size(&c) == 3);
	lruc_remove(&c, k2, strlen(k2));
	assert(lruc_size(&c) == 2);
	lruc_clear(&c);
	assert(lruc_size(&c) == 0);
	lruc_destroy(&c);
	return;
}
NO_COV void test_lruc_hits(void) {
	t_lruc      c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	lruc_init(&c, 4, free);
	assert(c.node_nb == 0);
	assert(lruc_hits(&c) == 0);
	lruc_put(&c, k1, strlen(k1), strdup("pique sous"));
	lruc_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	lruc_put(&c, k3, strlen(k3), strdup("grippe sous"));
	assert(lruc_hits(&c) == 0);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_hits(&c) == 1);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_hits(&c) == 4);
	lruc_remove(&c, k2, strlen(k2));
	assert(lruc_get(&c, k2, strlen(k2)) == NULL);
	assert(lruc_get(&c, k2, strlen(k2)) == NULL);
	assert(lruc_hits(&c) == 4);
	lruc_clear(&c);
	assert(lruc_hits(&c) == 0);
	lruc_destroy(&c);
	return;
}
NO_COV void test_lruc_misses(void) {
	t_lruc      c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	lruc_init(&c, 4, free);
	assert(c.node_nb == 0);
	assert(lruc_hits(&c) == 0);
	lruc_put(&c, k1, strlen(k1), strdup("pique sous"));
	lruc_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	lruc_put(&c, k3, strlen(k3), strdup("grippe sous"));
	assert(lruc_misses(&c) == 0);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_misses(&c) == 0);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_misses(&c) == 0);
	lruc_remove(&c, k2, strlen(k2));
	assert(lruc_get(&c, k2, strlen(k2)) == NULL);
	assert(lruc_get(&c, k2, strlen(k2)) == NULL);
	assert(lruc_misses(&c) == 2);
	lruc_clear(&c);
	assert(lruc_misses(&c) == 0);
	lruc_destroy(&c);
	return;
}

NO_COV void test_lruc_expired(void) {
	t_lruc      c;
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;

	v1 = strdup("value 1");
	v2 = strdup("value 2");
	v3 = strdup("value 3");
	v4 = strdup("value 4");
	lruc_init(&c, 4, free);
	lruc_setttl(&c, 200);
	lruc_put(&c, k1, strlen(k1), v1);
	lruc_put(&c, k2, strlen(k2), v2);
	lruc_put_ttl(&c, k3, strlen(k3), v3, 3000);
	lruc_put(&c, k4, strlen(k4), v4);
	assert(c.MRU->kv.key == k4);
	sleep(1);
	lruc_cleanup(&c);
	assert(lruc_has(&c, k1, strlen(k1)) == false);
	assert(lruc_has(&c, k2, strlen(k2)) == false);
	assert(lruc_has(&c, k3, strlen(k3)) == true);
	assert(lruc_has(&c, k4, strlen(k4)) == false);
	assert(lruc_expired(&c) == 3);
	lruc_destroy(&c);
	return;
}

NO_COV void test_lruc_stats(void) {
	t_lruc       c;
	t_lruc_stats st;
	const char  *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	lruc_init(&c, 4, free);
	assert(c.node_nb == 0);
	st = lruc_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);
	lruc_put(&c, k1, strlen(k1), strdup("pique sous"));
	lruc_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	lruc_put(&c, k3, strlen(k3), strdup("grippe sous"));
	st = lruc_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	st = lruc_stats(&c);
	assert(st.misses == 0 && st.hit == 1 && st.expired == 0);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	assert(lruc_get(&c, k2, strlen(k2)) != NULL);
	st = lruc_stats(&c);
	assert(st.misses == 0 && st.hit == 4 && st.expired == 0);
	lruc_remove(&c, k2, strlen(k2));
	assert(lruc_get(&c, k2, strlen(k2)) == NULL);
	assert(lruc_get(&c, k2, strlen(k2)) == NULL);
	st = lruc_stats(&c);
	assert(st.misses == 2 && st.hit == 4 && st.expired == 0);
	lruc_clear(&c);
	st = lruc_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);
	lruc_destroy(&c);
	return;
}
