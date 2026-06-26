#include <assert.h>
#include <lru_cache/least_recently_used_cache.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache_tests.h"

// get
void test_lruc_get(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_lruc c;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");

	cache_init(&c, 2, free);
	cache_put(&c, k1, strlen(k1), v1);
	cache_put(&c, k2, strlen(k2), v2);
	cache_put(&c, k3, strlen(k3), v3);
	cache_put(&c, k4, strlen(k4), v4);
	assert(cache_get(&c, k1, strlen(k1)) == NULL);
	assert(cache_get(&c, k2, strlen(k2)) == NULL);
	assert(cache_get(&c, k3, strlen(k3)) != NULL);
	assert(cache_get(&c, k4, strlen(k4)) != NULL);
	assert(cache_get(&c, k3, strlen(k3)) == v3);
	assert(cache_get(&c, k4, strlen(k4)) == v4);

	cache_destroy(&c);
	return;
}
