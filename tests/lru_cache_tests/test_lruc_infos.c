#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

// infos
void test_lruc_size(void) {
	t_lruc      c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	cache_init(&c, 4, free);
	assert(c.node_in_use == 0);
	assert(cache_size(&c) == 0);
	cache_put(&c, k1, strlen(k1), strdup("pique sous"));
	assert(cache_size(&c) == 1);
	cache_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	assert(cache_size(&c) == 2);
	cache_put(&c, k3, strlen(k3), strdup("grippe sous"));
	assert(cache_size(&c) == 3);
	cache_remove(&c, k2, strlen(k2));
	assert(cache_size(&c) == 2);
	cache_clear(&c);
	assert(cache_size(&c) == 0);

	cache_destroy(&c);
	return;
}
void test_lruc_hits(void) {
	t_lruc      c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	cache_init(&c, 4, free);
	assert(c.node_in_use == 0);
	assert(cache_hits(&c) == 0);
	cache_put(&c, k1, strlen(k1), strdup("pique sous"));
	cache_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	cache_put(&c, k3, strlen(k3), strdup("grippe sous"));
	assert(cache_hits(&c) == 0);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_hits(&c) == 1);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_hits(&c) == 4);
	cache_remove(&c, k2, strlen(k2));
	assert(cache_get(&c, k2, strlen(k2)) == NULL);
	assert(cache_get(&c, k2, strlen(k2)) == NULL);
	assert(cache_hits(&c) == 4);
	cache_clear(&c);
	assert(cache_hits(&c) == 0);
	cache_destroy(&c);
	return;
}
void test_lruc_misses(void) {
	t_lruc      c;
	const char *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	cache_init(&c, 4, free);
	assert(c.node_in_use == 0);
	assert(cache_hits(&c) == 0);
	cache_put(&c, k1, strlen(k1), strdup("pique sous"));
	cache_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	cache_put(&c, k3, strlen(k3), strdup("grippe sous"));
	assert(cache_misses(&c) == 0);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_misses(&c) == 0);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_misses(&c) == 0);
	cache_remove(&c, k2, strlen(k2));
	assert(cache_get(&c, k2, strlen(k2)) == NULL);
	assert(cache_get(&c, k2, strlen(k2)) == NULL);
	assert(cache_misses(&c) == 2);
	cache_clear(&c);
	assert(cache_misses(&c) == 0);
	cache_destroy(&c);
	return;
}

void test_lruc_expired(void) {
	t_lruc      c;
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;

	v1 = strdup("value 1");
	v2 = strdup("value 2");
	v3 = strdup("value 3");
	v4 = strdup("value 4");
	cache_init(&c, 4, free);
	cache_setttl(&c, 200);
	cache_put(&c, k1, strlen(k1), v1);
	cache_put(&c, k2, strlen(k2), v2);
	cache_put_ttl(&c, k3, strlen(k3), v3, 3000);
	cache_put(&c, k4, strlen(k4), v4);
	assert(c.MRU->kv.key == k4);
	assert(c.TTL_Max->kv.key == k3);
	sleep(1);
	cache_cleanup(&c);
	assert(cache_has(&c, k1, strlen(k1)) == false);
	assert(cache_has(&c, k2, strlen(k2)) == false);
	assert(cache_has(&c, k3, strlen(k3)) == true);
	assert(cache_has(&c, k4, strlen(k4)) == false);
	assert(cache_expired(&c) == 3);

	cache_destroy(&c);
	return;
}

void test_lruc_stats(void) {
	t_lruc        c;
	t_lruc_stats st;
	const char   *k1 = "titi", *k2 = "toto", *k3 = "tutu";

	cache_init(&c, 4, free);
	assert(c.node_in_use == 0);
	st = cache_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);
	cache_put(&c, k1, strlen(k1), strdup("pique sous"));
	cache_put(&c, k2, strlen(k2), strdup("gontrant du bonheur"));
	cache_put(&c, k3, strlen(k3), strdup("grippe sous"));
	st = cache_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	st = cache_stats(&c);
	assert(st.misses == 0 && st.hit == 1 && st.expired == 0);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	assert(cache_get(&c, k2, strlen(k2)) != NULL);
	st = cache_stats(&c);
	assert(st.misses == 0 && st.hit == 4 && st.expired == 0);
	cache_remove(&c, k2, strlen(k2));
	assert(cache_get(&c, k2, strlen(k2)) == NULL);
	assert(cache_get(&c, k2, strlen(k2)) == NULL);
	st = cache_stats(&c);
	assert(st.misses == 2 && st.hit == 4 && st.expired == 0);
	cache_clear(&c);
	st = cache_stats(&c);
	assert(st.misses == 0 && st.hit == 0 && st.expired == 0);

	cache_destroy(&c);
	return;
}
