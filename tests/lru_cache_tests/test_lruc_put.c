#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

NO_COV void test_lruc_put_kv(void) {
	t_lruc          c;
	t_keyvalue      kv1_ok;
	t_keyvalue      kv2_ok;
	t_keyvalue      kv3_ok;
	t_keyvalue      kv4_no_key;
	t_keyvalue      kv5_no_keylen;
	t_lruc_keynode *kv;
	char           *key1 = "toto";
	char           *key2 = "titi";
	char           *key3 = "loooooooooooooong key";
	char           *key5 = "tata";
	void           *val1 = (void *)0xDEADBEEF;
	void           *val2 = (void *)0xCAFEBEAD;
	void           *val3 = (void *)0xB00B5;
	void           *val4 = (void *)0x69420;
	void           *val5 = (void *)0x3141592;

	lruc_init(&c, 2, NULL);

	kv1_ok = (t_keyvalue){.key = key1, .k_len = strlen(key1), .value = val1};
	kv2_ok = (t_keyvalue){.key = key2, .k_len = strlen(key2), .value = val2};
	kv3_ok = (t_keyvalue){.key = key3, .k_len = strlen(key3), .value = val3};
	kv4_no_key    = (t_keyvalue){.key = NULL, .k_len = 12, .value = val4};
	kv5_no_keylen = (t_keyvalue){.key = key5, .k_len = 0, .value = val5};

	assert(lruc_put_kv(&c, kv4_no_key) == false);
	assert(lruc_put_kv(&c, kv5_no_keylen) == false);
	assert(lruc_put_kv(&c, kv1_ok) == true);
	assert(c.LRU == c.MRU && c.LRU != NULL);
	assert(c.MRU->kv.key == key1);
	assert(c.node_nb == 1);
	kv = c.LRU;
	assert(kv->kv.key == key1 && kv->kv.k_len == strlen(key1) &&
	       kv->kv.value == val1);

	// update
	kv1_ok.value = val2;
	assert(lruc_put_kv(&c, kv1_ok) == true);
	assert(c.LRU == c.MRU && c.LRU != NULL);
	assert(c.MRU->kv.key == key1);
	assert(c.node_nb == 1);
	kv = c.LRU;
	assert(kv->kv.key == key1 && kv->kv.k_len == strlen(key1) &&
	       kv->kv.value == val2);
	kv->kv.value = val1; // manual reset

	// max fill
	assert(lruc_put_kv(&c, kv2_ok) == true);
	assert(c.LRU != c.MRU && c.LRU != NULL && c.MRU != NULL);
	assert(c.MRU->kv.key == key2);
	assert(c.LRU->kv.key == key1);
	assert(c.node_nb == 2);
	kv = c.LRU;
	assert(kv->kv.key == key1 && kv->kv.k_len == strlen(key1) &&
	       kv->kv.value == val1);
	kv = c.MRU;
	assert(kv->kv.key == key2 && kv->kv.k_len == strlen(key2) &&
	       kv->kv.value == val2);

	// overfill
	assert(lruc_put_kv(&c, kv3_ok) == true);
	assert(c.LRU != c.MRU && c.LRU != NULL && c.MRU != NULL);
	assert(c.MRU->kv.key == key3);
	assert(c.LRU->kv.key == key2);
	assert(c.node_nb == 2);
	kv = c.LRU;
	assert(kv->kv.key == key2 && kv->kv.k_len == strlen(key2) &&
	       kv->kv.value == val2);
	kv = c.MRU;
	assert(kv->kv.key == key3 && kv->kv.k_len == strlen(key3) &&
	       kv->kv.value == val3);

	// reserved nodes
	lruc_remove(&c, key2, strlen(key2));
	assert(c.reserved_nodes != NULL);
	assert(lruc_put_kv(&c, kv2_ok) == true);
	assert(c.LRU != c.MRU && c.LRU != NULL && c.MRU != NULL);
	assert(c.reserved_nodes == NULL);
	assert(c.MRU->kv.key == key2);
	assert(c.LRU->kv.key == key3);
	assert(c.node_nb == 2);
	kv = c.MRU;
	assert(kv->kv.key == key2 && kv->kv.k_len == strlen(key2) &&
	       kv->kv.value == val2);
	kv = c.LRU;
	assert(kv->kv.key == key3 && kv->kv.k_len == strlen(key3) &&
	       kv->kv.value == val3);

	lruc_destroy(&c);

	lruc_init(&c, 2, NULL);

	alloc_set_call_limit(0);
	assert(lruc_put_kv(&c, kv1_ok) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	clock_set_call_limit(0);
	assert(lruc_put_kv(&c, kv1_ok) == false);
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
	assert(lruc_put(&c, key1, strlen(key1), value1) == true);
	assert(lruc_put(&c, key2, strlen(key2), value2) == true);
	assert(lruc_put(&c, key3, strlen(key3), value3) == true); // < re-use node
	assert(lruc_has(&c, key1, strlen(key1)) == false);
	assert(lruc_has(&c, key2, strlen(key2)) == true);
	assert(lruc_has(&c, key3, strlen(key3)) == true);
	assert(c.MRU->kv.key == key3), assert(c.LRU->kv.key == key2);
	assert(lruc_put(&c, key1, strlen(key1), (void *)0xDEAD) == true);
	assert(c.MRU->kv.key == key1), assert(c.LRU->kv.key == key3);
	assert(lruc_get(&c, key3, strlen(key3)) == value3);
	assert(lruc_put(&c, key2, strlen(key2), value2) == true);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	assert(lruc_get(&c, key2, strlen(key2)) == value2);
	assert(lruc_put(&c, key2, strlen(key2), value2_alt) == true);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	assert(lruc_get(&c, key2, strlen(key2)) == value2_alt);
	// errors
	// status
	assert(lruc_has(&c, key2, strlen(key2)) == true);
	assert(lruc_has(&c, key3, strlen(key3)) == true);
	// eargs
	assert(lruc_put(NULL, key1, strlen(key1), value1) == false);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	assert(lruc_put(&c, NULL, strlen(key1), value1) == false);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	assert(lruc_put(&c, key1, 0, value1) == false);
	assert(lruc_put(&c, key1, -1, value1) == false);
	assert(c.MRU->kv.key == key2), assert(c.LRU->kv.key == key3);
	// eclock
	clock_set_call_limit(0);
	clock_reset_counter();
	assert(lruc_put(&c, key2, strlen(key2), value3) == false);
	clock_set_call_limit(1);
	clock_reset_counter();
	// 2nd clock call fails and instead taking of ttl/minHeap takes from LRU
	// doesnt change anything on simple put but changes stuff on ttl side
	// TODO: remove call to clock when ttl isn't set on cache, useless
	assert(lruc_put(&c, key1, strlen(key1), value1) == true);
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
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 1000) == true);
		assert(c.MRU->kv.key == k2);
		assert(c.LRU->kv.key == k2);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.node_nb == 1);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) ==
		       true);
		assert(c.MRU->kv.key == k1);
		assert(c.LRU->kv.key == k2);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.minHeap[1]->kv.key == k1);
		assert(c.node_nb == 2);
		lruc_clear(&c);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) ==
		       true);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 1000) == true);
		assert(c.MRU->kv.key == k2);
		assert(c.LRU->kv.key == k1);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.minHeap[1]->kv.key == k1);
		lruc_clear(&c);
	}
	{
		lruc_setttl(&c, 10);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 2000) ==
		       true);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 1000) == true);
		uint64_t now_int = lruci_timenow();
		assert(now_int != (uint64_t)-1);
		assert(c.minHeap[0]->time - now_int >= 900);
		assert(c.minHeap[c.node_nb - 1]->time - now_int >= 1900);
		lruc_clear(&c);
	}
	{
		lruc_setttl(&c, 0);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 1900) ==
		       true);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 900) == true);
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
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"), 1900) ==
		       true);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"), 900) == true);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);
		assert(c.node_nb == 2);

		// fail, can't set time on node init
		clock_set_call_limit(0);
		assert(lruc_put_ttl(&c, k3, strlen(k3), (void *)0xDEAD, 9000) == false);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);

		clock_set_call_limit(1);
		clock_reset_counter();
		// 2nd clock call fails and instead taking of ttl/minHeap takes from LRU
		// doesnt change anything on simple put but changes stuff on ttl side
		assert(lruc_put_ttl(&c, k3, strlen(k3), (void *)0xDEAD, 9000) == false);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);

		clock_set_call_limit(1);
		clock_reset_counter();
		lruc_setttl(&c, C_MINI__LRUC_TTL_DFLT);
		// fell back on lru since ttl wasnt set at global and just discarded
		// the validity of the minheap
		assert(lruc_put_ttl(&c, k3, strlen(k3), strdup("works"), 9000) == true);
		assert(c.LRU->kv.key == k2);
		assert(c.MRU->kv.key == k3);

		clock_set_call_limit(CLOCK_DFT_LIMIT);
	}
	lruc_destroy(&c);
	return;
}
