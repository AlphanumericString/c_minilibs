#include <assert.h>
#include <bits/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "lru_cache_tests.h"

int main(void);

bool dmp_kv(t_keyvalue *kv, void *u) {
	(void)u;
	// LOG("kv(%p) (%p %p)", kv, kv->key, kv->value);
	(void)kv;
	return (true);
}

// put
void test_lruc_put_kv(void) {
	t_lruc          c;
	t_keyvalue      kv1;
	t_lruc_keynode *kv_in_cache;
	char           *key = "toto";

	cache_init(&c, 2, NULL);
	kv1 = (t_keyvalue){.key = NULL, .k_len = -1, .value = (void *)0xDEADBEEF};
	assert(cache_put_kv(&c, kv1) == -1);
	kv1.key = key;
	assert(cache_put_kv(&c, kv1) == 0);
	kv_in_cache = cache_search_keyinhash(&c, key, strlen(key), NULL);
	assert(kv_in_cache);
	assert(kv_in_cache->kv.k_len == strlen(key));

	cache_destroy(&c);
	return;
}
void test_lruc_put(void) {
	t_lruc      c;
	const char *key1 = "toto";
	const char *key2 = "titi";
	const char *key3 = "tutu";

	cache_init(&c, 2, NULL);
	assert(cache_put(&c, key1, strlen(key1), (void *)0xDEAD) == 0);
	cache_foreach_ru_ordered(&c, dmp_kv, NULL);
	assert(cache_put(&c, key2, strlen(key2), (void *)0xCADE) == 0);
	cache_foreach_ru_ordered(&c, dmp_kv, NULL);
	assert(cache_put(&c, key3, strlen(key3), (void *)0xCAFE) == 0);
	cache_foreach_ru_ordered(&c, dmp_kv, NULL);
	assert(cache_has(&c, key1, strlen(key1)) == false);
	assert(cache_has(&c, key2, strlen(key2)) == true);
	assert(cache_has(&c, key3, strlen(key3)) == true);
	assert(c.LRU->kv.key == key2);
	assert(c.MRU->kv.key == key3);
	assert(cache_put(&c, key1, strlen(key1), (void *)0xDEAD) == 0);
	assert(c.LRU->kv.key == key3);
	assert(c.MRU->kv.key == key1);

	cache_destroy(&c);
	return;
}
void test_lruc_put_ttl(void) {
	t_lruc          c;
	const char     *k1 = "luigi au pay des shtroumpfs";
	const char     *k2 = "squalala";
	struct timespec now;

	cache_init(&c, 2, free);
	{ // order
		assert(cache_put_ttl(&c, k2, strlen(k2), strdup("result-prime"),
		                     1000) == 0);
		assert(cache_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);

		assert(c.MRU->kv.key == k1);
		assert(c.LRU->kv.key == k2);
		cache_clear(&c);

		assert(cache_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);
		assert(cache_put_ttl(&c, k2, strlen(k2), strdup("result-prime"),
		                     1000) == 0);
		assert(c.MRU->kv.key == k2); // lru ordering
		assert(c.LRU->kv.key == k1);
		assert(c.TTL_Max->kv.key == k1);
		assert(c.TTL_Min->kv.key == k2);

		cache_clear(&c);
	}
	{ // distances
		cache_setttl(&c, 10);
		assert(cache_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);
		assert(cache_put_ttl(&c, k2, strlen(k2), strdup("result-prime"),
		                     1000) == 0);
		assert(distance_ms(c.TTL_Max->time, c.TTL_Min->time) > 0);
		clock_gettime(CLOCK_MONOTONIC, &now);
		assert(distance_ms(now, c.TTL_Max->time) == -2000);
		assert(distance_ms(now, c.TTL_Min->time) == -1000);
		cache_clear(&c);
	}
	{
		cache_setttl(&c, 0);
		assert(cache_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);
		assert(cache_put_ttl(&c, k2, strlen(k2), strdup("result-prime"),
		                     1000) == 0);
		assert(cache_isexpired(&c, k1, strlen(k1)) == false);
		assert(cache_isexpired(&c, k2, strlen(k2)) == false);
		sleep(1);
		assert(cache_isexpired(&c, k1, strlen(k1)) == false);
		assert(cache_isexpired(&c, k2, strlen(k2)) == true);
		sleep(1);
		assert(cache_isexpired(&c, k1, strlen(k1)) == true);
		assert(cache_isexpired(&c, k2, strlen(k2)) == true);
		cache_clear(&c);
	}
	cache_destroy(&c);
	return;
}
