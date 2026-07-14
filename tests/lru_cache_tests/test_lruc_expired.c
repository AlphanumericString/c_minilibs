#include <assert.h>
#include <bits/types/struct_timeval.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

void test_lruc_isexpired(void) {
	t_lruc      c;
	const char *k1 = "zod";
	void       *v1;

	lruc_init(&c, 4, free);
	lruc_setttl(&c, 200);
	assert(lruc_has(&c, k1, strlen(k1)) == false);
	assert(lruc_isexpired(&c, k1, strlen(k1)) == false);
	v1 = strdup("should not leak");
	lruc_put(&c, k1, strlen(k1), v1);
	assert(lruc_has(&c, k1, strlen(k1)) == true);
	assert(lruc_isexpired(&c, k1, strlen(k1)) == false);
	sleep(1);
	assert(lruc_has(&c, k1, strlen(k1)) == true);
	assert(lruc_isexpired(&c, k1, strlen(k1)) == true);
	lruc_destroy(&c);
	return;
}
void test_lruc_cleanup(void) {
	t_lruc      c;
	const char *key1 = "abc", *key2 = "def", *key3 = "ghi", *key4 = "jkl";
	void       *v1, *v2, *v3, *v4;

	v1 = strdup("value_1");
	v2 = strdup("value_2");
	v3 = strdup("value_3");
	v4 = strdup("value_4");
	lruc_init(&c, 4, free);
	lruc_setttl(&c, 200);
	lruc_put(&c, key1, strlen(key1), v1);
	lruc_put(&c, key2, strlen(key2), v2);
	lruc_put(&c, key3, strlen(key3), v3);
	lruc_put(&c, key4, strlen(key4), v4);
	sleep(1);
	assert(lruc_has(&c, key1, strlen(key1)) == true);
	assert(lruc_has(&c, key2, strlen(key2)) == true);
	assert(lruc_has(&c, key3, strlen(key3)) == true);
	assert(lruc_has(&c, key4, strlen(key4)) == true);
	assert(lruc_isexpired(&c, key1, strlen(key1)) == true);
	assert(lruc_isexpired(&c, key2, strlen(key2)) == true);
	assert(lruc_isexpired(&c, key3, strlen(key3)) == true);
	assert(lruc_isexpired(&c, key4, strlen(key4)) == true);
	lruc_cleanup(&c);
	assert(lruc_has(&c, key1, strlen(key1)) == false);
	assert(lruc_has(&c, key2, strlen(key2)) == false);
	assert(lruc_has(&c, key3, strlen(key3)) == false);
	assert(lruc_has(&c, key4, strlen(key4)) == false);
	assert(lruc_expired(&c) == 4);
	lruc_destroy(&c);
	return;
}
void test_lruc_setttl(void) {
	t_lruc cache;

	lruc_init(&cache, 4, free);
	assert(cache.ttl_glob == LONG_MAX);
	lruc_setttl(&cache, 200);
	assert(cache.ttl_glob == 200);
	lruc_setttl(&cache, 3000);
	assert(cache.ttl_glob == 3000);
	lruc_setttl(&cache, 300000);
	assert(cache.ttl_glob == 300000);
	lruc_destroy(&cache);
	return;
}
