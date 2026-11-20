#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define C_MINI__LRUTC_INTERNAL
#include "least_recently_used_timed_cache.h"

// is node in list of keys of lruc
// [very slow] [used for debug] [not production code]
t_cm_lrutc_keynode *lrutci_keyinlist(const t_cm_lrutc *c, const void *key,
                                   size_t key_len) {
	t_cm_lrutc_keynode *node;

	for (node = c->MRU; node; node = node->lru_next)
		if (node->kv.k_len == key_len &&
		    memcmp(key, node->kv.key, key_len) == 0)
			return (node);
	return (NULL);
}
// _out -> bucket offset
t_cm_lrutc_keynode *lrutci_keyinhash(const t_cm_lrutc *c, const void *key,
                                   size_t key_len, size_t *_out) {
	size_t             hash;
	t_cm_lrutc_keynode *node;

	if (!_out) _out = &hash;
	hash = lrutci_hash_djb2(key, key_len) % c->buckets_nb;
	node = c->buckets[hash];
	for (; node; node = node->hash_next)
		if (lrutci_key_eq(node->kv.key, node->kv.k_len, key, key_len))
			return (*_out = hash, node);
	return (*_out = hash, NULL);
}

void lrutci_unlink_node(t_cm_lrutc *c, t_cm_lrutc_keynode *n) {
	// the bucket is derived from the node's own key: callers may hold the hash
	// of another key (an insertion evicting a node hashes the key it is about
	// to store), unlinking from that bucket would leave the node aliased in
	// its real bucket.
	lrutci_lru_pop(c, n);
	lrutci_hsh_pop(c, n, lrutci_hash_djb2(n->kv.key, n->kv.k_len));
	// removing from the heap is the step that owns the node_nb decrement, the
	// index is only trusted when it points back to this node: a stale index
	// would make pop_nth evict an unrelated entry from the expiry order.
	if (c->minHeap[0] == n) {
		lrutci_hp_pop(c);
	} else if (n->heap_index < c->node_nb && c->minHeap[n->heap_index] == n) {
		lrutci_hp_pop_nth(c, n->heap_index);
	} else {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUTC_LOG_EHEAP)
	}
}
/**
 * \brief [INTERNAL] [MAY CHANGE] check that a node is tracked by the heap at
 * its advertised slot
 * \param c cache pointer
 * \param n node to check
 * \return true when heap_index is in range and minHeap[heap_index] points back
 * to n
 * \ingroup lrutc
**/
static bool lrutci_node_in_heap(const t_cm_lrutc *c, const t_cm_lrutc_keynode *n) {
	return (n->heap_index < c->node_nb && c->minHeap[n->heap_index] == n);
}
/**
 * \brief [INTERNAL] [MAY CHANGE] walk every internal structure and check the
 * cache invariants
 * \param c cache pointer
 * \return true when every structure holds exactly node_nb entries, every heap
 * slot points back to its own heap_index, and reserved nodes are marked as out
 * of the heap
 * \note debug helper, O(n^2) in the number of live entries
 * \ingroup lrutc
**/
bool lrutci_validate(const t_cm_lrutc *c) {
	const t_cm_lrutc_keynode *n, *last;
	size_t                   seen, i;

	if (!lrutc_is_valid(c)) return (false);
	// recency list: bounded walk catches cycles, links are checked in both
	// directions and every listed node must be heap tracked
	seen = 0;
	last = NULL;
	for (n = c->MRU; n; n = n->lru_next) {
		if (++seen > c->node_nb) return (false);
		if (n->lru_next && n->lru_next->lru_prev != n) return (false);
		if (!lrutci_node_in_heap(c, n)) return (false);
		last = n;
	}
	if (seen != c->node_nb) return (false);
	if (c->node_nb && (c->MRU->lru_prev || last != c->LRU)) return (false);
	// the heap needs no separate walk: a live node is only accepted when
	// minHeap[heap_index] points back to it, and node_nb such distinct in-range
	// slots hold exactly node_nb live entries
	// buckets: same number of nodes, all heap tracked
	seen = 0;
	for (i = 0; i < c->buckets_nb; i++) {
		for (n = c->buckets[i]; n; n = n->hash_next) {
			if (++seen > c->node_nb) return (false);
			if (!lrutci_node_in_heap(c, n)) return (false);
		}
	}
	if (seen != c->node_nb) return (false);
	// reserved nodes are not live entries and must not claim a heap slot
	for (n = c->reserved_nodes; n; n = n->lru_next)
		if (n->heap_index != C_MINI__LRUTC_NO_HEAP_SLOT) return (false);
	return (true);
}
size_t lrutci_hash_djb2(const void *key, size_t size) {
	const unsigned char *str;
	size_t               hash;

	hash = 5381;
	str  = (const unsigned char *)key;
	while (size--)
		hash = (hash << 5) + hash + str[size];
	return (hash);
}
uint64_t lrutci_timeto64(struct timespec t) {
	return (t.tv_sec * 1000000000ULL + t.tv_nsec);
}
uint64_t lrutci_expiry(const t_cm_lrutc_keynode *node) {
	if (node->ns_gap == (size_t)-1) return ((uint64_t)-1);
	if ((uint64_t)node->ns_gap > (uint64_t)-1 - node->time_emitted)
		return ((uint64_t)-1);
	return (node->time_emitted + (uint64_t)node->ns_gap);
}
bool lrutci_expired(const t_cm_lrutc_keynode *node, uint64_t now) {
	if (node->ns_gap == (size_t)-1) return (false);
	return (now > node->time_emitted &&
	        (now - node->time_emitted) > (uint64_t)node->ns_gap);
}
bool lrutci_node_eq(const t_cm_lrutc_keynode *node_1,
                   const t_cm_lrutc_keynode *node_2) {
	return (node_1 == node_2 || lrutci_key_eq(node_1->kv.key, node_1->kv.k_len,
	                                         node_2->kv.key, node_2->kv.k_len));
}
bool lrutci_key_eq(const void *k1, size_t k1_len, const void *k2,
                  size_t k2_len) {
	return (k1_len == k2_len && memcmp(k1, k2, k2_len) == 0);
}
uint64_t lrutci_timenow(void) {
	struct timespec t;

	if (clock_gettime(CLOCK_MONOTONIC, &t) != 0) return (-1);
	return (lrutci_timeto64(t));
}
