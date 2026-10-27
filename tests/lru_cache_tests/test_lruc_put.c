#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

NO_COV void test_lruc_put_kv(void) {
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

	lruc_init(&c, 2, NULL);

	alloc_set_call_limit(0);
	assert(lruc_put_kv(&c, kv1) == -1);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	clock_set_call_limit(0);
	assert(lruc_put_kv(&c, kv1) == -1);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	lruc_destroy(&c);

	return;
}
NO_COV void test_lruc_put(void) {
	t_lruc      c;
	const char *key1       = "toto";
	const char *key2       = "titi";
	const char *key3       = "tutu";
	void       *value1     = "1";
	void       *value2     = "2";
	void       *value3     = "3";
	void       *value2_alt = "2_alt";

	lruc_init(&c, 2, NULL);
	assert(lruc_put(&c, key1, strlen(key1), value1) == 0);
	assert(lruc_put(&c, key2, strlen(key2), value2) == 0);
	assert(lruc_put(&c, key3, strlen(key3), value3) == 0); // < re-use node
	assert(lruc_has(&c, key1, strlen(key1)) == false);
	assert(lruc_has(&c, key2, strlen(key2)) == true);
	assert(lruc_has(&c, key3, strlen(key3)) == true);
	assert(c.MRU->kv.key == key3), assert(c.LRU->kv.key == key2);
	assert(lruc_put(&c, key1, strlen(key1), (void *)0xDEAD) == 0);
	assert(c.MRU->kv.key == key1), assert(c.LRU->kv.key == key3);
	assert(lruc_get(&c, key3, strlen(key3)) == value3);
	assert(lruc_put(&c, key2, strlen(key2), value2) == 0);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	assert(lruc_get(&c, key2, strlen(key2)) == value2);
	assert(lruc_put(&c, key2, strlen(key2), value2_alt) == 0);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	assert(lruc_get(&c, key2, strlen(key2)) == value2_alt);
	// errors
	// status
	assert(lruc_has(&c, key2, strlen(key2)) == true);
	assert(lruc_has(&c, key3, strlen(key3)) == true);
	// eargs
	assert(lruc_put(NULL, key1, strlen(key1), value1) == -1);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	assert(lruc_put(&c, NULL, strlen(key1), value1) == -1);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	assert(lruc_put(&c, key1, 0, value1) == -1);
	assert(lruc_put(&c, key1, -1, value1) == -1);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	// eclock
	clock_set_call_limit(0);
	clock_reset_counter();
	assert(lruc_put(&c, key2, strlen(key2), value3) == -1);
	clock_set_call_limit(1);
	clock_reset_counter();
	// 2nd clock call fails and instead taking of ttl/minHeap takes from LRU
	// doesnt change anything on simple put but changes stuff on ttl side
	// TODO: remove call to clock when ttl isn't set on cache, useless
	assert(lruc_put(&c, key1, strlen(key1), value1) == 0);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	// cleanup
	lruc_destroy(&c);
	return;
}
NO_COV void test_lruc_put_ttl(void) {
	t_lruc      c;
	const char *k1 = "luigi au pay des shtroumpfs";
	const char *k2 = "squalala";
	const char *k3 = "on attend toujours wtc 38";

	lruc_init(&c, 2, free);
	{ // MRU/LRU and minHeap check
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 1000) == 0);
		assert(c.MRU->kv.key == k2);
		assert(c.LRU->kv.key == k2);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.node_nb == 1);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);
		assert(c.MRU->kv.key == k1);
		assert(c.LRU->kv.key == k2);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.minHeap[1]->kv.key == k1);
		assert(c.node_nb == 2);
		lruc_clear(&c);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 1000) == 0);
		assert(c.MRU->kv.key == k2);
		assert(c.LRU->kv.key == k1);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.minHeap[1]->kv.key == k1);
		lruc_clear(&c);
	}
	{
		lruc_setttl(&c, 10);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) == 0);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 1000) == 0);
		uint64_t now_int = lruci_timenow();
		assert(now_int != (uint64_t)-1);
		assert(c.minHeap[0]->time - now_int >= 900);
		assert(c.minHeap[c.node_nb - 1]->time - now_int >= 1900);
		lruc_clear(&c);
	}
	{
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
	{
		lruc_setttl(&c, 2000);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 1900) == 0);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 900) == 0);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);
		assert(c.node_nb == 2);

		// fail, can't set time on node init
		clock_set_call_limit(0);
		assert(lruc_put_ttl(&c, k3, strlen(k3), (void *)0xDEAD, 9000) == -1);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);

		clock_set_call_limit(1);
		clock_reset_counter();
		// 2nd clock call fails and instead taking of ttl/minHeap takes from LRU
		// doesnt change anything on simple put but changes stuff on ttl side
		assert(lruc_put_ttl(&c, k3, strlen(k3), (void *)0xDEAD, 9000) == -1);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);

		clock_set_call_limit(1);
		clock_reset_counter();
		lruc_setttl(&c, C_MINI__LRUC_TTL_DFLT);
		// fell back on lru since ttl wasnt set at global and just discarded
		// the validity of the minheap
		assert(lruc_put_ttl(&c, k3, strlen(k3), strdup("works"), 9000) == 0);
		assert(c.LRU->kv.key == k2);
		assert(c.MRU->kv.key == k3);

		clock_set_call_limit(CLOCK_DFT_LIMIT);
	}
	lruc_destroy(&c);
	return;
}
