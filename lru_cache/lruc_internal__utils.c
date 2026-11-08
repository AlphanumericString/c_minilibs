#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

// is node in list of keys of lruc
// [very slow] [used for debug] [not production code]
t_cm_lruc_keynode *lruci_keyinlist(const t_cm_lruc *c, const void *key,
                                   size_t key_len) {
	t_cm_lruc_keynode *node;

	for (node = c->MRU; node; node = node->lru_next)
		if (node->kv.k_len == key_len &&
		    memcmp(key, node->kv.key, key_len) == 0)
			return (node);
	return (NULL);
}
// _out -> bucket offset
t_cm_lruc_keynode *lruci_keyinhash(const t_cm_lruc *c, const void *key,
                                   size_t key_len, size_t *_out) {
	size_t             hash;
	t_cm_lruc_keynode *node;

	if (!_out) _out = &hash;
	hash = lruci_hash_djb2(key, key_len) % c->buckets_nb;
	node = c->buckets[hash];
	for (; node; node = node->hash_next)
		if (lruci_key_eq(node->kv.key, node->kv.k_len, key, key_len))
			return (*_out = hash, node);
	return (*_out = hash, NULL);
}

void lruci_unlink_node(t_cm_lruc *c, t_cm_lruc_keynode *n, size_t h) {
	lruci_lru_pop(c, n);
	lruci_hsh_pop(c, n, h);
	if (c->minHeap[0] == n) {
		lruci_hp_pop(c);
	} else {
		lruci_hp_pop_nth(c, n->heap_index);
	}
	c->node_nb++;
}
size_t lruci_hash_djb2(const void *key, size_t size) {
	const unsigned char *str;
	size_t               hash;

	hash = 5381;
	str  = (const unsigned char *)key;
	while (size--)
		hash = (hash << 5) + hash + str[size];
	return (hash);
}
uint64_t lruci_timeto64(struct timespec t) {
	return (t.tv_sec * 1000000000ULL + t.tv_nsec);
}
uint64_t lruci_expiry(const t_cm_lruc_keynode *node) {
	if (node->ns_gap == (size_t)-1) return ((uint64_t)-1);
	if ((uint64_t)node->ns_gap > (uint64_t)-1 - node->time_emitted)
		return ((uint64_t)-1);
	return (node->time_emitted + (uint64_t)node->ns_gap);
}
bool lruci_expired(const t_cm_lruc_keynode *node, uint64_t now) {
	if (node->ns_gap == (size_t)-1) return (false);
	return (now > node->time_emitted &&
	        (now - node->time_emitted) > (uint64_t)node->ns_gap);
}
bool lruci_node_eq(const t_cm_lruc_keynode *node_1,
                   const t_cm_lruc_keynode *node_2) {
	return (node_1 == node_2 || lruci_key_eq(node_1->kv.key, node_1->kv.k_len,
	                                         node_2->kv.key, node_2->kv.k_len));
}
bool lruci_key_eq(const void *k1, size_t k1_len, const void *k2,
                  size_t k2_len) {
	return (k1_len == k2_len && memcmp(k1, k2, k2_len) == 0);
}
uint64_t lruci_timenow(void) {
	struct timespec t;

	if (clock_gettime(CLOCK_MONOTONIC, &t) != 0) return (-1);
	return (lruci_timeto64(t));
}
