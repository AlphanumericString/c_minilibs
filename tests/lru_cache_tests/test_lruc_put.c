#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

// number of nodes reachable through the hash buckets
static size_t bucket_chain_nodes_nb(const t_cm_lruc *c) {
	t_cm_lruc_keynode *n;
	size_t             nb = 0;

	for (size_t i = 0; i < c->buckets_nb; i++)
		for (n = c->buckets[i]; n; n = n->hash_next)
			nb++;
	return (nb);
}
// number of nodes reachable through the recency list
static size_t recency_chain_nodes_nb(const t_cm_lruc *c) {
	t_cm_lruc_keynode *n;
	size_t             nb = 0;

	for (n = c->MRU; n; n = n->lru_next)
		nb++;
	return (nb);
}

NO_COV void test_cm_lruc_put_kv(void) {
	t_cm_lruc     c;
	t_cm_keyvalue kv = {.key = "k", .value = (void *)0xDEAD, .k_len = 1};

	lruc_init(&c, 2, NULL);
	assert(lruc_put_kv(&c, kv) == true);
	assert(c.node_nb == 1);
	assert(c.MRU == c.LRU && c.MRU->kv.value == (void *)0xDEAD);
	assert(lruci_validate(&c) == true);

	// bad arguments
	assert(lruc_put_kv(NULL, kv) == false);
	assert(lruc_put_kv(&c, (t_cm_keyvalue){.key = NULL, .k_len = 1}) == false);
	assert(lruc_put_kv(&c, (t_cm_keyvalue){.key = "k", .k_len = 0}) == false);
	assert(lruc_put_kv(&c, (t_cm_keyvalue){.key = "k", .k_len = (size_t)-1}) ==
	       false);
	assert(c.node_nb == 1);

	// allocation failure on a new node
	lruc_clear(&c);
	alloc_set_call_limit(0);
	assert(lruc_put_kv(&c, kv) == false);
	assert(c.node_nb == 0);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_put(void) {
	t_cm_lruc   c;
	const char *k1 = "toto", *k2 = "titi", *k3 = "tutu";

	lruc_init(&c, 2, NULL);
	assert(lruc_put(&c, k1, strlen(k1), "1") == true);
	assert(c.MRU == c.LRU && c.MRU->kv.key == k1);
	assert(lruc_put(&c, k2, strlen(k2), "2") == true);
	assert(c.MRU->kv.key == k2 && c.LRU->kv.key == k1);

	// existing key: value and recency updated in place, no eviction
	assert(lruc_put(&c, k1, strlen(k1), (void *)0xDEAD) == true);
	assert(c.node_nb == 2);
	assert(c.stats.expired == 0);
	assert(c.MRU->kv.key == k1 && c.MRU->kv.value == (void *)0xDEAD);
	assert(c.LRU->kv.key == k2);
	assert(lruci_validate(&c) == true);

	// eviction: k2 is the least recently used
	assert(lruc_put(&c, k3, strlen(k3), "3") == true);
	assert(c.node_nb == 2);
	assert(c.stats.expired == 1);
	assert(lruc_has(&c, k1, strlen(k1)) == true);
	assert(lruc_has(&c, k2, strlen(k2)) == false);
	assert(lruc_has(&c, k3, strlen(k3)) == true);
	assert(c.MRU->kv.key == k3 && c.LRU->kv.key == k1);
	assert(lruci_validate(&c) == true);

	// reserved node reuse on a full cache: no eviction
	lruc_remove(&c, k3, strlen(k3));
	assert(c.reserved_nodes_nb == 1);
	assert(lruc_put(&c, "quatre", 6, "4") == true);
	assert(c.stats.expired == 1);
	assert(c.reserved_nodes_nb == 0);
	assert(c.node_nb == 2);
	assert(lruci_validate(&c) == true);

	// bad arguments
	assert(lruc_put(NULL, k1, strlen(k1), "1") == false);
	assert(lruc_put(&c, NULL, strlen(k1), "1") == false);
	assert(lruc_put(&c, k1, 0, "1") == false);
	lruc_destroy(&c);
	return;
}
/**
 * \brief a full cache evicts its least recently used entry, counts it and keeps
 * the node around for the new entry
**/
NO_COV void test_cm_lruc_put_evict(void) {
	t_cm_lruc   c;
	const char *ka = "a", *kb = "b";

	lruc_init(&c, 3, free);
	assert(lruc_put(&c, ka, 1, strdup("A")) == true);
	assert(lruc_put(&c, kb, 1, strdup("B")) == true);
	assert(lruc_put(&c, "c", 1, strdup("C")) == true);
	// the value of the evicted entry is destroyed
	assert(lruc_put(&c, "d", 1, strdup("D")) == true);
	assert(c.node_nb == 3);
	assert(c.stats.expired == 1);
	// the evicted node was recycled, not reallocated
	assert(lruc_has(&c, ka, 1) == false);
	assert(lruc_has(&c, kb, 1) && lruc_has(&c, "c", 1) && lruc_has(&c, "d", 1));
	assert(c.LRU->kv.key == kb);
	assert(bucket_chain_nodes_nb(&c) == 3);
	assert(recency_chain_nodes_nb(&c) == 3);
	assert(lruci_validate(&c) == true);
	lruc_destroy(&c);
	return;
}
/**
 * \brief eviction must unlink the victim from the victim's own bucket: a wrong
 * bucket leaves the recycled node aliased in its old bucket
**/
NO_COV void test_cm_lruc_put_evict_bucket(void) {
	t_cm_lruc   c;
	const char *keys[] = {"a", "b", "c", "d", "o"};

	lruc_init(&c, 4, NULL);
	for (size_t i = 0; i < 4; i++)
		assert(lruc_put(&c, keys[i], 1, (void *)keys[i]) == true);
	// premise: victim and inserted key must live in different buckets,
	// otherwise a wrong bucket unlink is invisible
	assert(lruci_hash_djb2(keys[0], 1) % c.buckets_nb !=
	       lruci_hash_djb2(keys[4], 1) % c.buckets_nb);
	assert(c.LRU->kv.key == keys[0]);
	assert(lruc_put(&c, keys[4], 1, (void *)keys[4]) == true);
	assert(c.node_nb == 4);
	assert(c.stats.expired == 1);
	assert(lruc_has(&c, keys[0], 1) == false);
	for (size_t i = 1; i < 5; i++)
		assert(lruc_has(&c, keys[i], 1) == true);
	assert(bucket_chain_nodes_nb(&c) == c.node_nb);
	assert(recency_chain_nodes_nb(&c) == c.node_nb);
	assert(lruci_validate(&c) == true);
	lruc_destroy(&c);
	return;
}
