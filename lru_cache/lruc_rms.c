#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Clear the cache
 * \param c cache pointer
 * \ingroup lruc
 **/
void lruc_clear(t_lruc *c) {
	t_lruc_keynode *nxt;

	if (!c || !c->node_in_use || !c->buckets_nb) return;
	if ((c->buckets_nb / c->node_in_use) >=
	    C_MINI__LRUC_HASH_SKIP) { // less than 10% filled
		LOG(C_MINI__LRUC_LOG_LFREE);
		for (t_lruc_keynode *node = c->MRU; node; node = nxt) {
			if (c->dtor) c->dtor(node->kv.value);
			LOG(C_MINI__LRUC_LOG_DELETEVAL, node->kv.key, node->kv.k_len);
			nxt = node->lru_next;
			free(node);
		}
	} else {
		LOG(C_MINI__LRUC_LOG_HFREE);
		for (size_t i = 0; i < c->buckets_nb; i++) {
			if (!c->buckets[i]) continue;
			for (t_lruc_keynode *node = c->buckets[i]; node; node = nxt) {
				if (c->dtor) c->dtor(node->kv.value);
				LOG(C_MINI__LRUC_LOG_DELETEVAL, node->kv.key, node->kv.k_len);
				nxt = node->hash_next;
				free(node);
			}
		}
	}
	bzero(c->buckets, sizeof(typeof(*c->buckets)) * c->buckets_nb);
	bzero(c->minHeap, sizeof(typeof(*c->minHeap)) * c->buckets_nb);
	c->LRU         = NULL;
	c->MRU         = NULL;
	c->node_in_use = 0;
	c->stats       = (t_lruc_stats){0};
}
/**
 * \brief Remove a key from the cache
 * \param c cache pointer
 * \param key key to remove
 * \param key_len key length
 * \return 0 on success, -1 on failure
 * \ingroup lruc
 **/
int lruc_remove(t_lruc *c, const void *key, size_t key_len) {
	t_lruc_keynode *node;
	size_t          pos;

	if (!c || !c->MRU || !c->node_in_use) return (-1);
	node = lruci_keyinhash(c, key, key_len, &pos);
	if (!node ||
	    lruci_key_eq(node->kv.key, node->kv.k_len, key, key_len) == false)
		return (-1);
	lruci_unlink_node(c, node, pos);
	if (c->dtor) c->dtor(node->kv.value);
	LOG(C_MINI__LRUC_LOG_DELETEVAL, node->kv.key, node->kv.k_len);
	free(node);
	c->node_in_use--;
	return (0);
}
