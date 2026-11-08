#include <assert.h>
#include <bits/types/struct_timeval.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Check if a key is expired
 * \param c cache pointer
 * \param key key to check
 * \param key_len key length
 * \return true if key is expired, false otherwise
 * \ingroup lruc
**/
bool lruc_isexpired(const t_cm_lruc *c, const void *key, size_t key_len) {
	const t_cm_lruc_keynode *node;
	uint64_t                 now_uint;

	if (!lruc_is_valid(c)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (false);
	}
	node = lruci_keyinhash(c, key, key_len, NULL);
	if (!node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_ENODEU)
		return (false);
	}
	if (node->ns_gap == C_MINI__LRUC_TTL_DFLT) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_SUCCESS)
		return (false);
	}
	now_uint = lruci_timenow();
	if (now_uint == (uint64_t)-1) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_ECLOCK)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_SUCCESS)
	return (lruci_expired(node, now_uint));
}

/**
 * \brief Clean up expired elements
 * \param c cache pointer
 * \ingroup lruc
**/
void lruc_cleanup(t_cm_lruc *c) {
	uint64_t now_uint;

	if (!lruc_is_valid(c)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return;
	}
	if (!c->node_nb) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_SUCCESS)
		return;
	}
	now_uint = lruci_timenow();
	if (now_uint == (uint64_t)-1) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_ECLOCK)
		return;
	}
	while (c->minHeap[0] && lruci_expired(c->minHeap[0], now_uint)) {
		if (c->dtor) c->dtor(c->minHeap[0]->kv.value);
		lruci_lru_pop(c, c->minHeap[0]);
		lruci_hsh_pop(
		    c, c->minHeap[0],
		    lruci_hash_djb2(c->minHeap[0]->kv.key, c->minHeap[0]->kv.k_len));
		lruci_rsrvd_push(c, lruci_hp_pop(c));
		c->stats.expired++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_SUCCESS);
}
/**
 * \brief Set the time to live default for the cache, retroactively applies it
 * to every stored entry
 * \param cache cache pointer
 * \param ttl_ns time to live (in ns), (size_t)-1 disables ttl
 * \ingroup lruc
**/
void lruc_setttl(t_cm_lruc *cache, size_t ttl_ns) {
	t_cm_lruc_keynode *node;

	if (!cache) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_UPDATE, cache->ns_gap, ttl_ns)
	cache->ns_gap = ttl_ns;
	for (node = cache->MRU; node; node = node->lru_next)
		node->ns_gap = ttl_ns;
	lruci_hp_rebuild(cache);
}
