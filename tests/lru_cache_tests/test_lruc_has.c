#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache_tests.h"

// has
void test_lruc_has(void) {
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
	assert(cache_has(&c, k1, strlen(k1)) == false);
	assert(cache_has(&c, k2, strlen(k2)) == false);
	assert(cache_has(&c, k3, strlen(k3)) != false);
	assert(cache_has(&c, k4, strlen(k4)) != false);
	assert(cache_has(&c, k3, strlen(k3)) == true);
	assert(cache_has(&c, k4, strlen(k4)) == true);

	cache_destroy(&c);
	return;
}
