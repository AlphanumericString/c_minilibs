#include <assert.h>
#include <bits/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "lru_cache_tests.h"

// put
void test_lruc_put_kv(void) {
	t_lruc          c;
	t_keyvalue      kv1;
	t_lruc_keynode *kv_in_cache;
	char           *key = "toto";

	lruc_init(&c, 2, NULL);

	kv1 = (t_keyvalue){.key = NULL, .k_len = 0, .value = (void *)0xDEADBEEF};
	assert(lruc_put_kv(&c, kv1) != 0);
	kv1.key = key;
	assert(lruc_put_kv(&c, kv1) != 0);
	kv1.k_len = strlen(key);
	assert(lruc_put_kv(&c, kv1) == 0);
	kv_in_cache = lruci_keyinhash(&c, key, strlen(key), NULL);
	assert(kv_in_cache);
	assert(kv_in_cache->kv.k_len == strlen(key));
	lruc_destroy(&c);
	return;
}
void test_lruc_put(void) {
	t_lruc      c;
	const char *key1   = "toto";
	const char *key2   = "titi";
	const char *key3   = "tutu";
	void       *value1 = "1";
	void       *value2 = "2";
	void       *value3 = "3";

	lruc_init(&c, 2, NULL);
	assert(lruc_put(&c, key1, strlen(key1), value1) == 0);
	assert(lruc_put(&c, key2, strlen(key2), value2) == 0);
	assert(lruc_put(&c, key3, strlen(key3), value3) == 0);
	assert(lruc_has(&c, key1, strlen(key1)) == false);
	assert(lruc_has(&c, key2, strlen(key2)) == true);
	assert(lruc_has(&c, key3, strlen(key3)) == true);
	assert(c.LRU->kv.key == key2);
	assert(c.MRU->kv.key == key3);
	assert(lruc_put(&c, key1, strlen(key1), (void *)0xDEAD) == 0);
	assert(c.LRU->kv.key == key3);
	assert(c.MRU->kv.key == key1);
	lruc_destroy(&c);
	return;
}
void test_lruc_put_ttl(void) {
	t_lruc      c;
	const char *k1 = "luigi au pay des shtroumpfs";
	const char *k2 = "squalala";

	lruc_init(&c, 2, free);
	{ // order
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 1000) == 0);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);
		assert(c.MRU->kv.key == k1);
		assert(c.LRU->kv.key == k2);
		lruc_clear(&c);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 1000) == 0);
		assert(c.MRU->kv.key == k2); // lru ordering
		assert(c.LRU->kv.key == k1);
		assert(c.minHeap[0]->kv.key == k2); // TTL ordering
		assert(c.minHeap[1]->kv.key == k1); // TTL ordering
		lruc_clear(&c);
	}
	{ // distances
		lruc_setttl(&c, 10);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 1000) == 0);
		uint64_t now_int = lruci_timenow();
		assert(now_int != (uint64_t)-1);             // time setup failed?
		assert(c.minHeap[0]->time - now_int >= 900); // 100 ms leeway
		assert(c.minHeap[c.node_in_use - 1]->time - now_int >= 1900); // 100 ms
		lruc_clear(&c);
	}
	{ // expiration check
		lruc_setttl(&c, 0);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 1900) == 0);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 900) == 0);
		assert(lruc_isexpired(&c, k1, strlen(k1)) == false);
		assert(lruc_isexpired(&c, k2, strlen(k2)) == false);
		sleep(1);
		assert(lruc_isexpired(&c, k1, strlen(k1)) == false);
		assert(lruc_isexpired(&c, k2, strlen(k2)) == true);
		sleep(1);
		assert(lruc_isexpired(&c, k1, strlen(k1)) == true);
		assert(lruc_isexpired(&c, k2, strlen(k2)) == true);
		lruc_clear(&c);
	}
	lruc_destroy(&c);
	return;
}
