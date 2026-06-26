#include <assert.h>
#include <bits/types/struct_timeval.h>
#include <lru_cache/least_recently_used_cache.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

void test_lruc_isexpired(void) {
	t_lruc      cache;
	const char *key1 = "zod";
	void       *v1;

	cache_init(&cache, 4, free);
	assert(cache_isexpired(&cache, key1, strlen(key1)) == false);
	cache_setttl(&cache, 200);
	assert(cache_isexpired(&cache, key1, strlen(key1)) == false);
	v1 = calloc(42, 1);
	cache_put(&cache, key1, strlen(key1), v1);
	assert(cache_has(&cache, key1, strlen(key1)) == true);
	assert(cache_isexpired(&cache, key1, strlen(key1)) == false);
	sleep(1);
	assert(cache_isexpired(&cache, key1, strlen(key1)) == true);
	cache_destroy(&cache);

	return;
}
void test_lruc_cleanup(void) {
	t_lruc      c;
	const char *key1 = "abc", *key2 = "def", *key3 = "ghi", *key4 = "jkl";
	void       *v1, *v2, *v3, *v4;

	v1 = calloc(42, 1);
	v2 = calloc(42, 1);
	v3 = calloc(42, 1);
	v4 = calloc(42, 1);
	cache_init(&c, 4, free);
	cache_setttl(&c, 200);
	cache_put(&c, key1, strlen(key1), v1);
	cache_put(&c, key2, strlen(key2), v2);
	cache_put(&c, key3, strlen(key3), v3);
	cache_put(&c, key4, strlen(key4), v4);
	sleep(1);
	cache_cleanup(&c);
	assert(cache_has(&c, key1, strlen(key1)) == false);
	assert(cache_has(&c, key2, strlen(key2)) == false);
	assert(cache_has(&c, key3, strlen(key3)) == false);
	assert(cache_has(&c, key4, strlen(key4)) == false);
	cache_destroy(&c);
	return;
}
void test_lruc_setttl(void) {
	t_lruc cache;

	cache_init(&cache, 4, free);
	cache_setttl(&cache, 200);
	assert(cache.ttl_glob == 200);
	cache_setttl(&cache, 3000);
	assert(cache.ttl_glob == 3000);
	cache_setttl(&cache, 300000);
	assert(cache.ttl_glob == 300000);
	cache_destroy(&cache);
	return;
}
