#include <string.h>
#define C_MINI__LRUTC_INTERNAL
#include "least_recently_used_timed_cache.h"

void lrutci_lru_push(t_cm_lrutc *c, t_cm_lrutc_keynode *node) {
	node->lru_next = c->MRU;
	if (c->MRU) c->MRU->lru_prev = node;
	if (!c->LRU) c->LRU = node;
	c->MRU         = node;
	node->lru_prev = NULL;
}
t_cm_lrutc_keynode *lrutci_lru_pop(t_cm_lrutc *c, t_cm_lrutc_keynode *n) {
	if (n == c->MRU) c->MRU = c->MRU->lru_next;
	if (n == c->LRU) c->LRU = c->LRU->lru_prev;
	if (n->lru_prev) n->lru_prev->lru_next = n->lru_next;
	if (n->lru_next) n->lru_next->lru_prev = n->lru_prev;
	n->lru_prev = NULL;
	n->lru_next = NULL;
	return (n);
}
t_cm_lrutc_keynode *lrutci_hsh_pop(t_cm_lrutc *c, t_cm_lrutc_keynode *n,
                                 size_t hash) {
	t_cm_lrutc_keynode *prev = NULL;
	t_cm_lrutc_keynode *it;

	hash = hash % c->buckets_nb;
	for (it = c->buckets[hash]; it; prev = it, it = it->hash_next)
		if (lrutci_key_eq(it->kv.key, it->kv.k_len, n->kv.key, n->kv.k_len) ==
		    true)
			break;
	if (!it) return (NULL);
	if (prev) prev->hash_next = it->hash_next;
	if (c->buckets[hash] == n) c->buckets[hash] = n->hash_next;
	n->hash_next = NULL;
	return (it);
}
t_cm_lrutc_keynode *lrutci_rsrvd_pop(t_cm_lrutc *c) {
	t_cm_lrutc_keynode *node = NULL;

	if (c->reserved_nodes) {
		node              = c->reserved_nodes;
		c->reserved_nodes = node->lru_next;
		c->reserved_nodes_nb--;
	}
	return (node);
}
bool lrutci_rsrvd_push(t_cm_lrutc *c, t_cm_lrutc_keynode *n) {
	*n                = (t_cm_lrutc_keynode){0};
	n->heap_index     = C_MINI__LRUTC_NO_HEAP_SLOT; // not in the heap
	n->lru_next       = c->reserved_nodes;
	c->reserved_nodes = n;
	c->reserved_nodes_nb++;
	return (true);
}
