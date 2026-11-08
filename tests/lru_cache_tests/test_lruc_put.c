#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

NO_COV void test_cm_lruc_put_kv(void) {
	t_cm_lruc          c;
	t_cm_lruc_keynode *kv;

	char *key1 = "toto";
	char *key2 = "titi";
	char *key3 = "loooooooooooooong key";
	char *key5 = "tata";
	void *val1 = (void *)0xDEADBEEF;
	void *val2 = (void *)0xCAFEBEAD;
	void *val3 = (void *)0xB00B5;
	void *val4 = (void *)0x69420;
	void *val5 = (void *)0x3141592;

	t_cm_keyvalue kv1_ok      = {.key = key1, .value = val1, strlen(key1)};
	t_cm_keyvalue kv2_ok      = {.key = key2, .value = val2, strlen(key2)};
	t_cm_keyvalue kv3_ok      = {.key = key3, .value = val3, strlen(key3)};
	t_cm_keyvalue kv4_no_key  = {.key = NULL, .value = val4, 12};
	t_cm_keyvalue kv5_no_klen = {.key = key5, .value = val5, 0};

	{ // simple MRU/LRU checks
		lruc_init(&c, 2, NULL);

		assert(lruc_put_kv(&c, kv1_ok) == true);
		assert(c.LRU == c.MRU && c.LRU != NULL);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		kv = c.LRU;
		assert(kv->kv.key == key1 && kv->kv.k_len == strlen(key1) &&
		       kv->kv.value == val1);

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

		lruc_destroy(&c);
	}
	{ // update val
		lruc_init(&c, 2, NULL);

		assert(lruc_put_kv(&c, kv1_ok) == true);
		assert(c.LRU == c.MRU && c.LRU != NULL);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		kv = c.LRU;
		assert(kv->kv.key == key1 && kv->kv.k_len == strlen(key1) &&
		       kv->kv.value == val1);

		kv1_ok.value = val2;
		assert(lruc_put_kv(&c, kv1_ok) == true);
		assert(c.LRU == c.MRU && c.LRU != NULL);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		kv = c.LRU;
		assert(kv->kv.key == key1 && kv->kv.k_len == strlen(key1) &&
		       kv->kv.value == val2);

		lruc_destroy(&c);
	}
	{ // overfill
		lruc_init(&c, 2, NULL);

		assert(lruc_put_kv(&c, kv1_ok) == true);
		assert(lruc_put_kv(&c, kv2_ok) == true);

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

		lruc_destroy(&c);
	}
	{ // re-use nodes
		lruc_init(&c, 2, NULL);

		assert(lruc_put_kv(&c, kv3_ok) == true);
		assert(lruc_put_kv(&c, kv2_ok) == true);
		assert(c.node_nb == 2);

		lruc_remove(&c, key2, strlen(key2));
		assert(c.node_nb == 1);
		assert(c.reserved_nodes_nb == 1);
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
	}
	{ // errors
		lruc_init(&c, 2, NULL);

		// EAGRS
		assert(lruc_put_kv(NULL, kv1_ok) == false);
		assert(lruc_put_kv(&c, kv4_no_key) == false);
		assert(lruc_put_kv(&c, kv5_no_klen) == false);
		assert(c.LRU == NULL && c.MRU == NULL);
		assert(c.node_nb == 0);

		lruc_destroy(&c); // reset
		lruc_init(&c, 2, NULL);

		// EALLOC
		alloc_set_call_limit(0);
		assert(lruc_put_kv(&c, kv1_ok) == false);
		alloc_set_call_limit(ALLOC_DFT_LIMIT);
		lruc_put_kv(&c, kv1_ok);
		lruc_remove(&c, kv1_ok.key, kv1_ok.k_len);
		assert(c.node_nb == 0 && c.reserved_nodes_nb == 1);
		assert(c.reserved_nodes != NULL);
		alloc_set_call_limit(0);
		// success since we use reserved nodes
		assert(lruc_put_kv(&c, kv1_ok) == true);
		alloc_set_call_limit(ALLOC_DFT_LIMIT);
		assert(c.node_nb == 1 && c.reserved_nodes_nb == 0);
		assert(c.reserved_nodes == NULL);

		lruc_destroy(&c); // reset
		lruc_init(&c, 2, NULL);

		// ECLOCK
		clock_set_call_limit(0);
		lruc_setttl(&c, C_MINI__LRUC_TTL_DFLT);
		// true, ttl not on, so clock fail is ignored
		assert(lruc_put_kv(&c, kv1_ok) == true);
		lruc_setttl(&c, LRUC_MS_TO_NS(300));
		// false clock fail not ignored
		assert(lruc_put_kv(&c, kv1_ok) == false);
		clock_set_call_limit(CLOCK_DFT_LIMIT);
		// true clock call successful
		assert(lruc_put_kv(&c, kv1_ok) == true);

		lruc_destroy(&c);
	}
	return;
}
NO_COV void test_cm_lruc_put(void) {
	t_cm_lruc   c;
	const char *key1   = "toto";
	const char *key2   = "titi";
	const char *key3   = "tutu";
	void       *value1 = "1";
	void       *value2 = "2";
	void       *value3 = "3";

	{ // mru/lru checks
		lruc_init(&c, 2, NULL);

		assert(lruc_put(&c, key1, strlen(key1), value1) == true);
		assert(c.LRU == c.MRU && c.LRU->kv.key == key1);
		assert(c.node_nb == 1);
		assert(lruc_put(&c, key2, strlen(key2), value2) == true);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);
		assert(lruc_put(&c, key3, strlen(key3), value3) == true); // < re-use
		assert(c.LRU->kv.key == key2);
		assert(c.MRU->kv.key == key3);
		assert(c.node_nb == 2);
		assert(lruc_has(&c, key1, strlen(key1)) == false);
		assert(lruc_has(&c, key2, strlen(key2)) == true);
		assert(lruc_has(&c, key3, strlen(key3)) == true);

		lruc_destroy(&c);
	}
	{ // update val with single elem
		lruc_init(&c, 2, NULL);
		lruc_put(&c, key1, strlen(key1), value1);

		assert(c.LRU == c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		assert(lruc_put(&c, key1, strlen(key1), (void *)0xDEAD) == true);
		assert(c.LRU == c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		assert(c.MRU->kv.value == (void *)0xDEAD);

		lruc_destroy(&c);
	}
	{ // update val with 2 vals
		lruc_init(&c, 2, NULL);
		lruc_put(&c, key1, strlen(key1), value1);
		lruc_put(&c, key2, strlen(key2), value2);

		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);
		assert(lruc_put(&c, key1, strlen(key1), (void *)0xDEAD) == true);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key2);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 2);
		assert(c.MRU->kv.value == (void *)0xDEAD);

		lruc_destroy(&c);
	}
	{ // errors
	  // setup
		lruc_init(&c, 2, NULL);
		assert(lruc_put(&c, key1, strlen(key1), value1) == true);
		assert(lruc_put(&c, key2, strlen(key2), value2) == true);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		// eargs
		assert(lruc_put(NULL, key1, strlen(key1), value1) == false);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		assert(lruc_put(&c, NULL, strlen(key1), value1) == false);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		assert(lruc_put(&c, key1, 0, value1) == false);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		assert(lruc_put(&c, key1, -1, value1) == false);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		lruc_clear(&c); // reset cache
		lruc_clear_reserved(&c);
		// set-up
		assert(lruc_put(&c, key1, strlen(key1), value1) == true);
		assert(lruc_put(&c, key2, strlen(key2), value2) == true);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		// eclock
		clock_reset_counter();
		clock_set_call_limit(0);
		assert(lruc_put(&c, key3, strlen(key3), value3) == true);
		clock_set_call_limit(CLOCK_DFT_LIMIT);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key2);
		assert(c.MRU->kv.key == key3);
		assert(c.node_nb == 2);
		// key 3 was pushed, clock not called since no ttl set

		lruc_clear(&c); // reset cache
		lruc_clear_reserved(&c);
		// set-up
		lruc_put(&c, key1, strlen(key1), value1);
		lruc_put(&c, key2, strlen(key2), value2);
		lruc_setttl(&c, LRUC_MS_TO_NS(300));

		clock_reset_counter();
		clock_set_call_limit(0);

		assert(lruc_put(&c, key3, strlen(key3), value3) == false);
		clock_set_call_limit(CLOCK_DFT_LIMIT);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);
		// k3 not pushed, ttl set so clock call needed, so fail

		lruc_clear(&c); // reset cache
		lruc_clear_reserved(&c);
		// set-up
		lruc_setttl(&c, LRUC_MS_TO_NS(300));
		lruc_put(&c, key1, strlen(key1), value1);
		lruc_put(&c, key2, strlen(key2), value2);

		clock_set_call_limit(1);
		clock_reset_counter();
		assert(lruc_put(&c, key3, strlen(key3), value3) == false);
		// 2nd clock call fails since ttl is set must take from minHeap, fail
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		clock_set_call_limit(1);
		clock_reset_counter();
		lruc_setttl(&c, C_MINI__LRUC_TTL_DFLT); // reset ttl

		assert(lruc_put(&c, key3, strlen(key3), value3) == true);
		clock_set_call_limit(CLOCK_DFT_LIMIT);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key2);
		assert(c.MRU->kv.key == key3);
		assert(c.node_nb == 2);

		lruc_destroy(&c);
	}
	return;
}
NO_COV void test_cm_lruc_put_ttl(void) {
	t_cm_lruc   c;
	const char *k1 = "luigi au pay des shtroumpfs";
	const char *k2 = "squalala";
	const char *k3 = "on attend toujours wtc 38";

	lruc_init(&c, 2, free);
	{ // MRU/LRU and minHeap check
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(1000)) == true);
		assert(c.MRU->kv.key == k2);
		assert(c.LRU->kv.key == k2);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.node_nb == 1);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(2000)) == true);
		assert(c.MRU->kv.key == k1);
		assert(c.LRU->kv.key == k2);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.minHeap[1]->kv.key == k1);
		assert(c.node_nb == 2);
		lruc_clear(&c);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(2000)) == true);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(1000)) == true);
		assert(c.MRU->kv.key == k2);
		assert(c.LRU->kv.key == k1);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.minHeap[1]->kv.key == k1);
		lruc_clear(&c);
	}
	{
		lruc_setttl(&c, LRUC_MS_TO_NS(10));
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(2000)) == true);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(1000)) == true);
		uint64_t now_int = lruci_timenow();
		assert(now_int != (uint64_t)-1);
		assert(lruci_expiry(c.minHeap[0]) - now_int >= LRUC_MS_TO_NS(900));
		assert(lruci_expiry(c.minHeap[c.node_nb - 1]) - now_int >=
		       LRUC_MS_TO_NS(1900));
		lruc_clear(&c);
	}
	{
		lruc_setttl(&c, 0);
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(1900)) == true);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(900)) == true);
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
		lruc_setttl(&c, LRUC_MS_TO_NS(2000));
		assert(lruc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(1900)) == true);
		assert(lruc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(900)) == true);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);
		assert(c.node_nb == 2);

		// fail, can't set time on node init
		clock_set_call_limit(0);
		assert(lruc_put_ttl(&c, k3, strlen(k3), (void *)0xDEAD,
		                    LRUC_MS_TO_NS(9000)) == false);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);

		clock_set_call_limit(1);
		clock_reset_counter();
		// 2nd clock call fails and instead taking of ttl/minHeap takes from LRU
		// doesnt change anything on simple put but changes stuff on ttl side
		assert(lruc_put_ttl(&c, k3, strlen(k3), (void *)0xDEAD,
		                    LRUC_MS_TO_NS(9000)) == false);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);

		clock_set_call_limit(1);
		clock_reset_counter();
		lruc_setttl(&c, C_MINI__LRUC_TTL_DFLT);
		// fell back on lru since ttl wasnt set at global and just discarded
		// the validity of the minheap
		assert(lruc_put_ttl(&c, k3, strlen(k3), strdup("works"),
		                    LRUC_MS_TO_NS(9000)) == true);
		assert(c.LRU->kv.key == k2);
		assert(c.MRU->kv.key == k3);

		clock_set_call_limit(CLOCK_DFT_LIMIT);
	}
	lruc_destroy(&c);
	return;
}
