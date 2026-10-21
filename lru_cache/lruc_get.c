#include <bits/time.h>
#include <bits/types/struct_timeval.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Get a key from the cache without checks for ttl
 * \param c cache pointer
 * \param key key to get
 * \param key_len key length
 * \return pointer to the value
 * \ingroup lruc
**/
void *lruc_get_lru(t_lruc *c, const void *key, size_t key_len) {
	t_lruc_keynode *node;
	size_t          pos;

	if (!c || !c->MRU || !c->node_nb) return (NULL);
	node = lruci_keyinhash(c, key, key_len, &pos);
	if (!node) {
		c->stats.misses++;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_MISS_LRU, c->stats.misses,
		       key, key_len)
		return (NULL);
	}
	lruci_lru_pop(c, node);
	node->lru_next = c->MRU;
	if (c->MRU) c->MRU->lru_prev = node;
	c->stats.hit++;
	c->MRU = node;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_HIT, c->stats.hit, key, key_len)
	return (node->kv.value);
}
/**
 * \brief Get a key from the cache with TTL
 * \param c cache pointer
 * \param key key to get
 * \param len key length
 * \return pointer to the value
 * \ingroup lruc
**/
void *lruc_get(t_lruc *c, const void *key, size_t len) {
	uint64_t        now;
	t_lruc_keynode *node;
	size_t          pos;

	if (!c || !c->MRU || !c->node_nb || !c->buckets) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (NULL);
	}
	if (c->ttl_glob == (uint64_t)C_MINI__LRUC_TTL_DFLT)
		return (lruc_get_lru(c, key, len));
	now = lruci_timenow();
	if (now == (uint64_t)-1) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_ECLOCK)
		return (NULL);
	}
	node = lruci_keyinhash(c, key, len, &pos);
	if (!node) {
		c->stats.misses++;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_MISS_LRU, c->stats.misses,
		       key, len)
		return (NULL);
	}
	if ((node->time + c->ttl_glob) < now) {
		c->stats.misses++;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_MISS_TTL, c->stats.misses,
		       key, len, now + node->time)
		return (NULL);
	}
	lruci_lru_pop(c, node);
	c->stats.hit++;
	node->lru_next = c->MRU;
	c->MRU         = node;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_HIT, c->stats.hit, key, len)
	return (node->kv.value);
}
