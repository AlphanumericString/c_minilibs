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
/**
 * \brief [INTERNAL] check that a node is reachable from its own bucket
 * \param c cache pointer
 * \param n node to check
 * \return true when n is in the chain of the bucket its key hashes to
 * \ingroup lruc
**/
static bool lruci_node_in_buckets(const t_cm_lruc *c,
                                  const t_cm_lruc_keynode *n) {
	const t_cm_lruc_keynode *it;
	size_t                   hash;

	hash = lruci_hash_djb2(n->kv.key, n->kv.k_len) % c->buckets_nb;
	for (it = c->buckets[hash]; it; it = it->hash_next)
		if (it == n) return (true);
	return (false);
}
/**
 * \brief [INTERNAL] [MAY CHANGE] walk every internal structure and check the
 * cache invariants
 * \param c cache pointer
 * \return true when the recency list holds node_nb nodes, links match in both
 * directions, every listed node is reachable from its own bucket, neither chain
 * cycles, and reserved nodes carry no live links
 * \note O(n) in the number of live entries. the bucket walk only guards against
 * cycles and aliasing: every list node being in its bucket already implies that
 * the buckets hold at least node_nb nodes, so any drift is caught by the
 * overrun guard.
 * \ingroup lruc
**/
bool lruci_validate(const t_cm_lruc *c) {
	const t_cm_lruc_keynode *n, *last;
	size_t                   seen, i;

	if (!lruc_is_valid(c)) return (false);
	// recency list: bounded walk catches cycles, links are checked in both
	// directions and every listed node must live in a bucket
	seen = 0;
	last = NULL;
	for (n = c->MRU; n; n = n->lru_next) {
		if (++seen > c->node_nb) return (false);
		if (n->lru_next && n->lru_next->lru_prev != n) return (false);
		if (!lruci_node_in_buckets(c, n)) return (false);
		last = n;
	}
	if (seen != c->node_nb) return (false);
	if (c->node_nb && (c->MRU->lru_prev || last != c->LRU)) return (false);
	// buckets: a chain longer than node_nb means a cycle or a node reachable
	// twice, both forbidden
	seen = 0;
	for (i = 0; i < c->buckets_nb; i++)
		for (n = c->buckets[i]; n; n = n->hash_next)
			if (++seen > c->node_nb) return (false);
	// reserved nodes are not live entries and must not carry live links
	for (n = c->reserved_nodes; n; n = n->lru_next)
		if (n->hash_next || n->lru_prev) return (false);
	return (true);
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
bool lruci_node_eq(const t_cm_lruc_keynode *node_1,
                   const t_cm_lruc_keynode *node_2) {
	return (node_1 == node_2 || lruci_key_eq(node_1->kv.key, node_1->kv.k_len,
	                                         node_2->kv.key, node_2->kv.k_len));
}
bool lruci_key_eq(const void *k1, size_t k1_len, const void *k2,
                  size_t k2_len) {
	return (k1_len == k2_len && memcmp(k1, k2, k2_len) == 0);
}
