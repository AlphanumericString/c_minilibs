#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lrutc_tests.h"

NO_COV void test_cm_lrutc_put_kv(void) {
	t_cm_lrutc          c;
	t_cm_lrutc_keynode *kv;

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
		lrutc_init(&c, 2, NULL);

		assert(lrutc_put_kv(&c, kv1_ok) == true);
		assert(c.LRU == c.MRU && c.LRU != NULL);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		kv = c.LRU;
		assert(kv->kv.key == key1 && kv->kv.k_len == strlen(key1) &&
		       kv->kv.value == val1);

		assert(lrutc_put_kv(&c, kv2_ok) == true);
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

		lrutc_destroy(&c);
	}
	{ // update val
		lrutc_init(&c, 2, NULL);

		assert(lrutc_put_kv(&c, kv1_ok) == true);
		assert(c.LRU == c.MRU && c.LRU != NULL);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		kv = c.LRU;
		assert(kv->kv.key == key1 && kv->kv.k_len == strlen(key1) &&
		       kv->kv.value == val1);

		kv1_ok.value = val2;
		assert(lrutc_put_kv(&c, kv1_ok) == true);
		assert(c.LRU == c.MRU && c.LRU != NULL);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		kv = c.LRU;
		assert(kv->kv.key == key1 && kv->kv.k_len == strlen(key1) &&
		       kv->kv.value == val2);

		lrutc_destroy(&c);
	}
	{ // overfill
		lrutc_init(&c, 2, NULL);

		assert(lrutc_put_kv(&c, kv1_ok) == true);
		assert(lrutc_put_kv(&c, kv2_ok) == true);

		assert(lrutc_put_kv(&c, kv3_ok) == true);
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

		lrutc_destroy(&c);
	}
	{ // re-use nodes
		lrutc_init(&c, 2, NULL);

		assert(lrutc_put_kv(&c, kv3_ok) == true);
		assert(lrutc_put_kv(&c, kv2_ok) == true);
		assert(c.node_nb == 2);

		lrutc_remove(&c, key2, strlen(key2));
		assert(c.node_nb == 1);
		assert(c.reserved_nodes_nb == 1);
		assert(c.reserved_nodes != NULL);

		assert(lrutc_put_kv(&c, kv2_ok) == true);
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

		lrutc_destroy(&c);
	}
	{ // errors
		lrutc_init(&c, 2, NULL);

		// EAGRS
		assert(lrutc_put_kv(NULL, kv1_ok) == false);
		assert(lrutc_put_kv(&c, kv4_no_key) == false);
		assert(lrutc_put_kv(&c, kv5_no_klen) == false);
		assert(c.LRU == NULL && c.MRU == NULL);
		assert(c.node_nb == 0);

		lrutc_destroy(&c); // reset
		lrutc_init(&c, 2, NULL);

		// EALLOC
		alloc_set_call_limit(0);
		assert(lrutc_put_kv(&c, kv1_ok) == false);
		alloc_set_call_limit(ALLOC_DFT_LIMIT);
		lrutc_put_kv(&c, kv1_ok);
		lrutc_remove(&c, kv1_ok.key, kv1_ok.k_len);
		assert(c.node_nb == 0 && c.reserved_nodes_nb == 1);
		assert(c.reserved_nodes != NULL);
		alloc_set_call_limit(0);
		// success since we use reserved nodes
		assert(lrutc_put_kv(&c, kv1_ok) == true);
		alloc_set_call_limit(ALLOC_DFT_LIMIT);
		assert(c.node_nb == 1 && c.reserved_nodes_nb == 0);
		assert(c.reserved_nodes == NULL);

		lrutc_destroy(&c); // reset
		lrutc_init(&c, 2, NULL);

		// ECLOCK
		clock_set_call_limit(0);
		lrutc_setttl(&c, C_MINI__LRUTC_TTL_DFLT);
		// true, ttl not on, so clock fail is ignored
		assert(lrutc_put_kv(&c, kv1_ok) == true);
		lrutc_setttl(&c, LRUC_MS_TO_NS(300));
		// false clock fail not ignored
		assert(lrutc_put_kv(&c, kv1_ok) == false);
		clock_set_call_limit(CLOCK_DFT_LIMIT);
		// true clock call successful
		assert(lrutc_put_kv(&c, kv1_ok) == true);

		lrutc_destroy(&c);
	}
	return;
}
NO_COV void test_cm_lrutc_put(void) {
	t_cm_lrutc   c;
	const char *key1   = "toto";
	const char *key2   = "titi";
	const char *key3   = "tutu";
	void       *value1 = "1";
	void       *value2 = "2";
	void       *value3 = "3";

	{ // mru/lru checks
		lrutc_init(&c, 2, NULL);

		assert(lrutc_put(&c, key1, strlen(key1), value1) == true);
		assert(c.LRU == c.MRU && c.LRU->kv.key == key1);
		assert(c.node_nb == 1);
		assert(lrutc_put(&c, key2, strlen(key2), value2) == true);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);
		assert(lrutc_put(&c, key3, strlen(key3), value3) == true); // < re-use
		assert(c.LRU->kv.key == key2);
		assert(c.MRU->kv.key == key3);
		assert(c.node_nb == 2);
		assert(lrutc_has(&c, key1, strlen(key1)) == false);
		assert(lrutc_has(&c, key2, strlen(key2)) == true);
		assert(lrutc_has(&c, key3, strlen(key3)) == true);

		lrutc_destroy(&c);
	}
	{ // update val with single elem
		lrutc_init(&c, 2, NULL);
		lrutc_put(&c, key1, strlen(key1), value1);

		assert(c.LRU == c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		assert(lrutc_put(&c, key1, strlen(key1), (void *)0xDEAD) == true);
		assert(c.LRU == c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 1);
		assert(c.MRU->kv.value == (void *)0xDEAD);

		lrutc_destroy(&c);
	}
	{ // update val with 2 vals
		lrutc_init(&c, 2, NULL);
		lrutc_put(&c, key1, strlen(key1), value1);
		lrutc_put(&c, key2, strlen(key2), value2);

		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);
		assert(lrutc_put(&c, key1, strlen(key1), (void *)0xDEAD) == true);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key2);
		assert(c.MRU->kv.key == key1);
		assert(c.node_nb == 2);
		assert(c.MRU->kv.value == (void *)0xDEAD);

		lrutc_destroy(&c);
	}
	{ // errors
	  // setup
		lrutc_init(&c, 2, NULL);
		assert(lrutc_put(&c, key1, strlen(key1), value1) == true);
		assert(lrutc_put(&c, key2, strlen(key2), value2) == true);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		// eargs
		assert(lrutc_put(NULL, key1, strlen(key1), value1) == false);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		assert(lrutc_put(&c, NULL, strlen(key1), value1) == false);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		assert(lrutc_put(&c, key1, 0, value1) == false);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		assert(lrutc_put(&c, key1, -1, value1) == false);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		lrutc_clear(&c); // reset cache
		lrutc_clear_reserved(&c);
		// set-up
		assert(lrutc_put(&c, key1, strlen(key1), value1) == true);
		assert(lrutc_put(&c, key2, strlen(key2), value2) == true);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		// eclock
		clock_reset_counter();
		clock_set_call_limit(0);
		assert(lrutc_put(&c, key3, strlen(key3), value3) == true);
		clock_set_call_limit(CLOCK_DFT_LIMIT);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key2);
		assert(c.MRU->kv.key == key3);
		assert(c.node_nb == 2);
		// key 3 was pushed, clock not called since no ttl set

		lrutc_clear(&c); // reset cache
		lrutc_clear_reserved(&c);
		// set-up
		lrutc_put(&c, key1, strlen(key1), value1);
		lrutc_put(&c, key2, strlen(key2), value2);
		lrutc_setttl(&c, LRUC_MS_TO_NS(300));

		clock_reset_counter();
		clock_set_call_limit(0);

		assert(lrutc_put(&c, key3, strlen(key3), value3) == false);
		clock_set_call_limit(CLOCK_DFT_LIMIT);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);
		// k3 not pushed, ttl set so clock call needed, so fail

		lrutc_clear(&c); // reset cache
		lrutc_clear_reserved(&c);
		// set-up
		lrutc_setttl(&c, LRUC_MS_TO_NS(300));
		lrutc_put(&c, key1, strlen(key1), value1);
		lrutc_put(&c, key2, strlen(key2), value2);

		clock_set_call_limit(1);
		clock_reset_counter();
		assert(lrutc_put(&c, key3, strlen(key3), value3) == false);
		// 2nd clock call fails since ttl is set must take from minHeap, fail
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key1);
		assert(c.MRU->kv.key == key2);
		assert(c.node_nb == 2);

		clock_set_call_limit(1);
		clock_reset_counter();
		lrutc_setttl(&c, C_MINI__LRUTC_TTL_DFLT); // reset ttl

		assert(lrutc_put(&c, key3, strlen(key3), value3) == true);
		clock_set_call_limit(CLOCK_DFT_LIMIT);
		assert(c.LRU != c.MRU);
		assert(c.LRU->kv.key == key2);
		assert(c.MRU->kv.key == key3);
		assert(c.node_nb == 2);

		lrutc_destroy(&c);
	}
	return;
}
NO_COV void test_cm_lrutc_put_ttl(void) {
	t_cm_lrutc   c;
	const char *k1 = "luigi au pay des shtroumpfs";
	const char *k2 = "squalala";
	const char *k3 = "on attend toujours wtc 38";

	lrutc_init(&c, 2, free);
	{ // MRU/LRU and minHeap check
		assert(lrutc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(1000)) == true);
		assert(c.MRU->kv.key == k2);
		assert(c.LRU->kv.key == k2);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.node_nb == 1);
		assert(lrutc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(2000)) == true);
		assert(c.MRU->kv.key == k1);
		assert(c.LRU->kv.key == k2);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.minHeap[1]->kv.key == k1);
		assert(c.node_nb == 2);
		lrutc_clear(&c);
		assert(lrutc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(2000)) == true);
		assert(lrutc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(1000)) == true);
		assert(c.MRU->kv.key == k2);
		assert(c.LRU->kv.key == k1);
		assert(c.minHeap[0]->kv.key == k2);
		assert(c.minHeap[1]->kv.key == k1);
		lrutc_clear(&c);
	}
	{
		lrutc_setttl(&c, LRUC_MS_TO_NS(10));
		assert(lrutc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(2000)) == true);
		assert(lrutc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(1000)) == true);
		uint64_t now_int = lrutci_timenow();
		assert(now_int != (uint64_t)-1);
		assert(lrutci_expiry(c.minHeap[0]) - now_int >= LRUC_MS_TO_NS(900));
		assert(lrutci_expiry(c.minHeap[c.node_nb - 1]) - now_int >=
		       LRUC_MS_TO_NS(1900));
		lrutc_clear(&c);
	}
	{
		lrutc_setttl(&c, 0);
		assert(lrutc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(1900)) == true);
		assert(lrutc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(900)) == true);
		assert(lrutc_isexpired(&c, k1, strlen(k1)) == false);
		assert(lrutc_isexpired(&c, k2, strlen(k2)) == false);
		lruc_test_sleep_ms(1000);
		assert(lrutc_isexpired(&c, k1, strlen(k1)) == false);
		assert(lrutc_isexpired(&c, k2, strlen(k2)) == true);
		lruc_test_sleep_ms(1000);
		assert(lrutc_isexpired(&c, k1, strlen(k1)) == true);
		assert(lrutc_isexpired(&c, k2, strlen(k2)) == true);
		lrutc_clear(&c);
	}
	{
		lrutc_setttl(&c, LRUC_MS_TO_NS(2000));
		assert(lrutc_put_ttl(&c, k1, strlen(k1), strdup("result"),
		                    LRUC_MS_TO_NS(1900)) == true);
		assert(lrutc_put_ttl(&c, k2, strlen(k2), strdup("other"),
		                    LRUC_MS_TO_NS(900)) == true);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);
		assert(c.node_nb == 2);

		// fail, can't set time on node init
		clock_set_call_limit(0);
		assert(lrutc_put_ttl(&c, k3, strlen(k3), (void *)0xDEAD,
		                    LRUC_MS_TO_NS(9000)) == false);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);

		clock_set_call_limit(1);
		clock_reset_counter();
		// 2nd clock call fails and instead taking of ttl/minHeap takes from LRU
		// doesnt change anything on simple put but changes stuff on ttl side
		assert(lrutc_put_ttl(&c, k3, strlen(k3), (void *)0xDEAD,
		                    LRUC_MS_TO_NS(9000)) == false);
		assert(c.LRU->kv.key == k1);
		assert(c.MRU->kv.key == k2);

		clock_set_call_limit(1);
		clock_reset_counter();
		lrutc_setttl(&c, C_MINI__LRUTC_TTL_DFLT);
		// fell back on lru since ttl wasnt set at global and just discarded
		// the validity of the minheap
		assert(lrutc_put_ttl(&c, k3, strlen(k3), strdup("works"),
		                    LRUC_MS_TO_NS(9000)) == true);
		assert(c.LRU->kv.key == k2);
		assert(c.MRU->kv.key == k3);

		clock_set_call_limit(CLOCK_DFT_LIMIT);
	}
	lrutc_destroy(&c);
	return;
}

// number of nodes reachable through the hash buckets
static size_t bucket_chain_nodes_nb(const t_cm_lrutc *c) {
	t_cm_lrutc_keynode *n;
	size_t             nb = 0;

	for (size_t i = 0; i < c->buckets_nb; i++)
		for (n = c->buckets[i]; n; n = n->hash_next)
			nb++;
	return (nb);
}
// number of nodes reachable through the recency list
static size_t recency_chain_nodes_nb(const t_cm_lrutc *c) {
	t_cm_lrutc_keynode *n;
	size_t             nb = 0;

	for (n = c->MRU; n; n = n->lru_next)
		nb++;
	return (nb);
}
// every live heap slot must point back to its own index
static bool heap_is_consistent(const t_cm_lrutc *c) {
	for (size_t i = 0; i < c->node_nb; i++)
		if (!c->minHeap[i] || c->minHeap[i]->heap_index != i) return (false);
	return (true);
}
/**
 * \brief eviction must unlink the victim from the victim's own bucket
 * \note the inserted key and the evicted key are required to live in different
 * buckets: when they collide a wrong bucket unlink is invisible because the
 * victim is found anyway. with a full cache and a differing bucket, a wrong
 * unlink leaves the recycled node aliased in its old bucket, which shows up as
 * more nodes in the buckets than live entries (and as a double free on
 * destroy).
**/
NO_COV void test_cm_lrutc_put_evict_lru_bucket(void) {
	t_cm_lrutc   c;
	const char *keys[] = {"a", "b", "c", "d", "o"};

	lrutc_init(&c, 4, NULL);
	for (size_t i = 0; i < 4; i++)
		assert(lrutc_put(&c, keys[i], 1, (void *)keys[i]) == true);
	assert(lrutci_hash_djb2(keys[0], 1) % c.buckets_nb !=
	       lrutci_hash_djb2(keys[4], 1) % c.buckets_nb);
	assert(c.LRU->kv.key == keys[0]);
	assert(lrutc_put(&c, keys[4], 1, (void *)keys[4]) == true);
	assert(c.node_nb == 4);
	assert(lrutc_has(&c, keys[0], 1) == false);
	for (size_t i = 1; i < 5; i++)
		assert(lrutc_has(&c, keys[i], 1) == true);
	assert(bucket_chain_nodes_nb(&c) == c.node_nb);
	assert(recency_chain_nodes_nb(&c) == c.node_nb);
	assert(heap_is_consistent(&c));
	lrutc_destroy(&c);
	return;
}
/**
 * \brief same guarantee when eviction picks the expired node of the min-heap
 * (the victim is not the LRU and still lives in another bucket than the key
 * that triggers the eviction)
**/
NO_COV void test_cm_lrutc_put_evict_ttl_bucket(void) {
	t_cm_lrutc   c;
	const char *expired = "a"; // bucket 0 for a capacity of 2
	const char *alive   = "b"; // bucket 1
	const char *added   = "d"; // bucket 1, same as alive

	lrutc_init(&c, 2, NULL);
	lrutc_setttl(&c, LRUC_MS_TO_NS(1000)); // keeps the min-heap eviction path on
	assert(lrutci_hash_djb2(expired, 1) % c.buckets_nb !=
	       lrutci_hash_djb2(added, 1) % c.buckets_nb);
	assert(lrutc_put_ttl(&c, expired, 1, (void *)expired, LRUC_MS_TO_NS(1)) ==
	       true);
	assert(lrutc_put(&c, alive, 1, (void *)alive) == true);
	lruc_test_sleep_ms(10);
	assert(c.minHeap[0]->kv.key == expired);
	assert(lrutc_put(&c, added, 1, (void *)added) == true);
	assert(c.node_nb == 2);
	assert(lrutc_has(&c, expired, 1) == false);
	assert(lrutc_has(&c, alive, 1) == true);
	assert(lrutc_has(&c, added, 1) == true);
	assert(bucket_chain_nodes_nb(&c) == c.node_nb);
	assert(recency_chain_nodes_nb(&c) == c.node_nb);
	assert(heap_is_consistent(&c));
	lrutc_destroy(&c);
	return;
}
