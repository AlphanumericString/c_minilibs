#include <assert.h>
#include <bits/types/struct_timeval.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define C_MINI__LRU_UTILS
#include "least_recently_used_cache.h"

// expired
// expired-> is key in the cache and expired?
bool cache_isexpired(t_lruc *c, const void *key, size_t key_len) {
	size_t         offset;
	t_keyCache    *kv;
	struct timespec now;

	if (c->ttl_glob == LONG_MAX) return (false);
	kv     = cache_search_keyinhash(c, key, key_len, &offset);
	if (!kv || memcmp(kv->key, key, MAX(kv->k_len, key_len)) != 0)
		return (false);
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (distance_ms(now, kv->time) >= c->ttl_glob);
}

// cleanup -> call dtor on all expired elements
void cache_cleanup(t_lruc *c) {
	t_keyCache    *prev;
	size_t         pos;
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	while (c->tail && distance_ms(now, c->tail->time) >= c->ttl_glob) {
		if (c->dtor) c->dtor(c->tail->value);
		cache_search_keyinhash(c, c->tail->key, c->tail->k_len, &pos);
		LOG(LRU_LOG_EVICTION, c->tail->key, distance_ms(now, c->tail->time),
		    c->ttl_glob);
		prev = c->tail->prev;
		free(c->tail);
		c->h_arr[pos] = NULL;
		c->tail       = prev;
		c->nb--;
		c->stats.expired++;
	}
}
void cache_setttl(t_lruc *cache, long ttl) {
	LOG(LRU_LOG_UPDATE, cache->ttl_glob, ttl);
	cache->ttl_glob = ttl;
}
