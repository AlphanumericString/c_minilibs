#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Clear node in reserved node list
 * \param cache cache pointer
 * \ingroup lruc
**/
bool lruc_clear_reserved(t_lruc *cache) {
	if (!cache) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (false);
	}
	for (; cache->reserved_nodes;)
		free(lruci_rsrvd_pop(cache));
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_SUCCESS);
	return (true);
}

static inline void clear_node(t_lruc_keynode *node, t_lruc_keynode **nxt,
                              void (*dtor)(void *), bool lru) {
	if (dtor) dtor(node->kv.value);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_DELETEVAL, node->kv.key,
	       node->kv.k_len)
	if (lru) *nxt = node->lru_next;
	else *nxt = node->hash_next;
}

/**
 * \brief Clear the cache and put cleared node in reserved node list
 * \param c cache pointer
 * \ingroup lruc
**/
void lruc_clear_to_reserved(t_lruc *c) {
	t_lruc_keynode *nxt;

	if (!lruc_is_valid(c) || !c->node_nb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return;
	}
	if ((c->buckets_nb / c->node_nb) >= C_MINI__LRUC_HASH_SKIP) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_LFREE)
		for (t_lruc_keynode *node = c->MRU; node; node = nxt) {
			clear_node(node, &nxt, c->dtor, true);
			c->node_nb--;
			lruci_rsrvd_push(c, node);
		}
	} else {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_HFREE)
		for (size_t i = 0; i < c->buckets_nb; i++) {
			if (!c->buckets[i]) continue;
			for (t_lruc_keynode *node = c->buckets[i]; node; node = nxt) {
				clear_node(node, &nxt, c->dtor, false);
				c->node_nb--;
				lruci_rsrvd_push(c, node);
			}
		}
	}
	bzero(c->buckets, sizeof(*c->buckets) * c->buckets_nb);
	bzero(c->minHeap, sizeof(*c->minHeap) * c->buckets_nb);
	c->LRU     = NULL;
	c->MRU     = NULL;
	c->node_nb = 0;
	c->stats   = (t_lruc_stats){0};
}
/**
 * \brief Clear the cache (cleared nodes are not push into reserved nodes)
 * \param c cache pointer
 * \ingroup lruc
**/
void lruc_clear(t_lruc *c) {
	t_lruc_keynode *nxt;

	if (!lruc_is_valid(c) || !c->node_nb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return;
	}
	if ((c->buckets_nb / c->node_nb) >= C_MINI__LRUC_HASH_SKIP) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_LFREE)
		for (t_lruc_keynode *node = c->MRU; node; node = nxt) {
			clear_node(node, &nxt, c->dtor, true);
			c->node_nb--;
			free(node);
		}
	} else {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_HFREE)
		for (size_t i = 0; i < c->buckets_nb; i++) {
			if (!c->buckets[i]) continue;
			for (t_lruc_keynode *node = c->buckets[i]; node; node = nxt) {
				clear_node(node, &nxt, c->dtor, false);
				c->node_nb--;
				free(node);
			}
		}
	}
	bzero(c->buckets, sizeof(*c->buckets) * c->buckets_nb);
	bzero(c->minHeap, sizeof(*c->minHeap) * c->buckets_nb);
	c->LRU     = NULL;
	c->MRU     = NULL;
	c->node_nb = 0;
	c->stats   = (t_lruc_stats){0};
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

	if (!lruc_is_valid(c) || !c->node_nb) return (-1);
	node = lruci_keyinhash(c, key, key_len, &pos);
	if (!node) return (-1);
	lruci_unlink_node(c, node, pos);
	if (c->dtor) c->dtor(node->kv.value);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_DELETEVAL, node->kv.key,
	       node->kv.k_len)
	c->node_nb--;
	lruci_rsrvd_push(c, node);
	return (0);
}
