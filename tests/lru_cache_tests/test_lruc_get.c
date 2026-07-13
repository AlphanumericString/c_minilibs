#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache_tests.h"

// get
void test_lruc_get(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_lruc      c;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lruc_init(&c, 2, free);
	lruc_setttl(&c, 3000);
	lruc_put(&c, k1, strlen(k1), v1);
	lruc_put(&c, k2, strlen(k2), v2);
	assert(c.LRU != NULL);
	assert(c.MRU != NULL);
	assert(c.TTL_Min != NULL);
	assert(c.TTL_Max != NULL);
	assert(c.LRU->kv.key == k1);
	assert(c.MRU->kv.key == k2);
	lruc_put(&c, k3, strlen(k3), v3); // <
	assert(c.MRU->kv.key == k3);
	assert(c.LRU->kv.key == k2);
	lruc_put(&c, k4, strlen(k4), v4);
	assert(c.MRU->kv.key == k4);
	assert(c.LRU->kv.key == k3);
	assert(c.TTL_Min->kv.key != k2);
	assert(c.TTL_Max->kv.key != k1);
	assert(lruc_get(&c, k1, strlen(k1)) == NULL);
	assert(lruc_get(&c, k2, strlen(k2)) == NULL);
	assert(lruc_get(&c, k3, strlen(k3)) != NULL);
	assert(lruc_get(&c, k4, strlen(k4)) != NULL);
	assert(lruc_get(&c, k3, strlen(k3)) == v3);
	assert(lruc_get(&c, k4, strlen(k4)) == v4);

	lruc_destroy(&c);
	return;
}
