#include <assert.h>
#include <bits/types/struct_timeval.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define C_MINI__INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Check if a key is expired
 * \param c cache pointer
 * \param key key to check
 * \param key_len key length
 * \return true if key is expired, false otherwise
 * \ingroup lruc
**/
bool lruc_isexpired(t_lruc *c, const void *key, size_t key_len) {
	size_t          offset;
	t_lruc_keynode *node;
	struct timespec now;
	uint64_t        now_uint;

	if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
		LOG(C_MINI__LRUC_LOG_ECLOCK);
		return (false);
	}
	now_uint = lruci_timeto64(now);
	if (c->ttl_glob == LONG_MAX) {
		LOG(C_MINI__LRUC_LOG_SUCCESS);
		return (false);
	}
	node = lruci_keyinhash(c, key, key_len, &offset);
	if (!node) {
		LOG(C_MINI__LRUC_LOG_ENODEU);
		return (false);
	}
	LOG(C_MINI__LRUC_LOG_SUCCESS);
	return ((node->time + c->ttl_glob) < (now_uint));
}

/**
 * \brief Clean up expired elements
 * \param c cache pointer
 * \ingroup lruc
**/
void lruc_cleanup(t_lruc *c) {
	struct timespec now;
	uint64_t        now_uint;

	if (!c || !c->minHeap) {
		LOG(C_MINI__LRUC_LOG_EARG);
		return;
	}
	if (!c->node_nb) {
		LOG(C_MINI__LRUC_LOG_SUCCESS);
		return;
	}
	if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
		LOG(C_MINI__LRUC_LOG_ECLOCK);
		return;
	}
	now_uint = now.tv_sec * 1000ULL + now.tv_nsec / 1000000ULL;
	while (c->minHeap[0] && (c->minHeap[0]->time + c->ttl_glob) < now_uint) {
		if (c->dtor) c->dtor(c->minHeap[0]->kv.value);
		lruci_lru_pop(c, c->minHeap[0]);
		lruci_hsh_pop(
		    c, c->minHeap[0],
		    lruci_hash_djb2(c->minHeap[0]->kv.key, c->minHeap[0]->kv.k_len));
		free(lruci_hp_pop(c));
		c->stats.expired++;
	}
}
/**
 * \brief Set the time to live default for the cache
 * \param cache cache pointer
 * \param ttl time to live (in ms)
 * \ingroup lruc
**/
void lruc_setttl(t_lruc *cache, long ttl) {
	LOG(C_MINI__LRUC_LOG_UPDATE, cache->ttl_glob, ttl);
	cache->ttl_glob = ttl;
}
