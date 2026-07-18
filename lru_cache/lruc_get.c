#include <bits/time.h>
#include <bits/types/struct_timeval.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

// search
// -> hit -> set as head
/**
 * \brief Get a key from the cache
 * \param c cache pointer
 * \param key key to get
 * \param key_len key length
 * \return pointer to the value
 * \ingroup lruc
 **/
void *lruc_get_lru(t_lruc *c, const void *key, size_t key_len) {
	t_lruc_keynode *node;
	size_t          pos;

	if (!c || !c->MRU || !c->node_in_use) return (NULL);
	node = lruci_keyinhash(c, key, key_len, &pos);
	if (!node ||
	    lruci_key_eq(node->kv.key, node->kv.k_len, key, key_len) == false) {
		c->stats.misses++;
		LOG(C_MINI__LRUC_LOG_MISS_LRU, c->stats.misses, key, key_len);
		return (NULL);
	}
	lruci_lru_pop(c, node);
	node->lru_next = c->MRU;
	c->stats.hit++;
	c->MRU = node;
	LOG(C_MINI__LRUC_LOG_HIT, c->stats.hit, key, key_len);
	return (node->kv.value);
}
/**
 * \brief Get a key from the cache with TTL
 * \param c cache pointer
 * \param key key to get
 * \param key_len key length
 * \return pointer to the value
 * \ingroup lruc
 **/
void *lruc_get(t_lruc *c, const void *key, size_t key_len) {
	struct timespec now;
	uint64_t        now_uint;
	t_lruc_keynode *node;
	size_t          pos;

	if (!c || !c->MRU || !c->node_in_use) {
		LOG(C_MINI__LRUC_LOG_EARG);
		return (NULL);
	}
	if (c->ttl_glob == (uint64_t)C_MINI__LRUC_TTL_DFLT)
		return (lruc_get_lru(c, key, key_len));
	if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
		LOG(C_MINI__LRUC_LOG_ECLOCK);
		return (NULL);
	}
	now_uint = now.tv_sec * 1000ULL + now.tv_nsec / 1000000ULL;
	node     = lruci_keyinhash(c, key, key_len, &pos);
	if (!node ||
	    lruci_key_eq(node->kv.key, node->kv.k_len, key, key_len) == false) {
		c->stats.misses++;
		LOG(C_MINI__LRUC_LOG_MISS_LRU, c->stats.misses, key, key_len);
		return (NULL);
	}
	if ((node->time + c->ttl_glob) < now_uint) {
		c->stats.misses++;
		LOG(C_MINI__LRUC_LOG_MISS_TTL, c->stats.misses, key, key_len,
		    now_uint + node->time);
		return (NULL);
	}
	lruci_lru_pop(c, node);
	node->lru_next = c->MRU;
	c->stats.hit++;
	c->MRU = node;
	LOG(C_MINI__LRUC_LOG_HIT, c->stats.hit, key, key_len);
	return (node->kv.value);
}
