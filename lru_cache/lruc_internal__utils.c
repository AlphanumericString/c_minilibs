#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define C_MINI__INTERNAL
#include "least_recently_used_cache.h"

t_lruc_keynode *lruci_keyinlist(const t_lruc *c, const void *key,
                                size_t key_len) {
	t_lruc_keynode *node;

	for (node = c->MRU; node; node = node->lru_next)
		if (node->kv.k_len == key_len &&
		    memcmp(key, node->kv.key, key_len) == 0)
			return (node);
	return (NULL);
}
t_lruc_keynode *lruci_keyinhash(const t_lruc *c, const void *key,
                                size_t key_len, size_t *_out) {
	size_t          hash;
	t_lruc_keynode *node;

	if (!_out) _out = &hash;
	hash = lruci_hash_djb2(key, key_len) % c->buckets_nb;
	node = c->buckets[hash];
	for (; node; node = node->hash_next)
		if (lruci_key_eq(node->kv.key, node->kv.k_len, key, key_len))
			return (*_out = hash, node);
	return (*_out = hash, NULL);
}

void lruci_unlink_node(t_lruc *c, t_lruc_keynode *n, size_t h) {
	lruci_lru_pop(c, n);
	lruci_hsh_pop(c, n, h);
	if (c->minHeap[0] == n) {
		lruci_hp_pop(c);
	} else {
		lruci_hp_pop_nth(c, n->heap_index);
	}
	c->node_in_use++;
}
size_t lruci_hash_djb2(const void *key, size_t size) {
	unsigned char *str;
	size_t         hash;

	hash = 5381;
	str  = (unsigned char *)key;
	while (size--)
		hash = (hash << 5) + hash + str[size];
	return (hash);
}
uint64_t lruci_timeto64(struct timespec t) {
	return (t.tv_sec * 1000ULL + t.tv_nsec / 1000000ULL);
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
