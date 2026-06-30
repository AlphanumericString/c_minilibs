#include <assert.h>
#include <bits/types/struct_timeval.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

// expired
// expired-> is key in the cache and expired?
/**
 * @brief Check if a key is expired
 * @param c cache pointer
 * @param key key to check
 * @param key_len key length
 * @return true if key is expired, false otherwise
 **/
bool cache_isexpired(t_lruc *c, const void *key, size_t key_len) {
	size_t          offset;
	t_lruc_keynode *node;
	struct timespec now;
	uint64_t        now_uint;

	if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
		LOG(C_MINI__LRU_LOG_ECLOCK);
		return (false);
	}
	now_uint = timeto64(now);
	if (c->ttl_glob == LONG_MAX) return (false);
	node = cache_search_keyinhash(c, key, key_len, &offset);
	if (!node || node->kv.k_len != key_len ||
	    memcmp(node->kv.key, key, key_len) != 0)
		return (false);
	return ((node->time + c->ttl_glob) < (now_uint));
}

// cleanup -> call dtor on all expired elements
/**
 * @brief Clean up expired elements
 * @param c cache pointer
 **/
void cache_cleanup(t_lruc *c) {
	size_t          pos;
	struct timespec now;
	uint64_t        now_uint;

	if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
		LOG(C_MINI__LRU_LOG_ECLOCK);
		return;
	}
	now_uint = now.tv_sec * 1000ULL + now.tv_nsec / 1000000ULL;
	while (c->TTL_Min && (c->TTL_Min->time + c->ttl_glob) < now_uint) {
		if (c->dtor) c->dtor(c->TTL_Min->kv.value);
		cache_search_keyinhash(c, c->TTL_Min->kv.key, c->TTL_Min->kv.k_len,
		                       &pos);
		LOG(C_MINI__LRU_LOG_EVICTION, c->TTL_Min->kv.key,
		    c->TTL_Min->time - now_uint, c->ttl_glob);
		pop_lruc(c, c->TTL_Min);
		free(pop_ttl(c, c->TTL_Min));
		c->h_arr[pos] = NULL;
		c->nb--;
		c->stats.expired++;
	}
}

/**
 * @brief Set the time to live default for the cache
 * @param cache cache pointer
 * @param ttl time to live
 **/
void cache_setttl(t_lruc *cache, long ttl) {
	LOG(C_MINI__LRU_LOG_UPDATE, cache->ttl_glob, ttl);
	cache->ttl_glob = ttl;
}
