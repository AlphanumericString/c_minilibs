#include <bits/time.h>
#include <bits/types/struct_timeval.h>
#include <string.h>
#include <time.h>

#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

// search
// -> hit -> set as head
void *cache_get_lru(t_lruc *c, const void *key, size_t key_len) {
	t_lruc_keynode *node;
	size_t          pos;

	if (!c || !c->MRU || !c->nb) return (NULL);
	node = cache_search_keyinhash(c, key, key_len, &pos);
	if (!node || memcmp(node->kv.key, key, key_len) != 0) {
		c->stats.misses++;
		LOG(C_MINI__LRU_LOG_MISS_LRU, c->stats.misses, key, key_len);
		return (NULL);
	}
	cache_unlink_node(c, node);
	node->lru_next = c->MRU;
	c->stats.hit++;
	c->MRU = node;
	LOG(C_MINI__LRU_LOG_HIT, c->stats.hit, key, key_len);
	return (c->h_arr[pos]->kv.value);
}
void *cache_get(t_lruc *c, const void *key, size_t key_len) {
	struct timespec now;
	t_lruc_keynode *node;
	size_t          pos;

	if (!c || !c->MRU || !c->nb) return (NULL);
	clock_gettime(CLOCK_MONOTONIC, &now);
	node = cache_search_keyinhash(c, key, key_len, &pos);
	if (!node || memcmp(node->kv.key, key, key_len) != 0) {
		c->stats.misses++;
		LOG(C_MINI__LRU_LOG_MISS_LRU, c->stats.misses, key, key_len);
		return (NULL);
	}
	if (distance_ms(now, node->time) >= c->ttl_glob) {
		c->stats.misses++;
		LOG(C_MINI__LRU_LOG_MISS_TTL, c->stats.misses, key, key_len,
		    distance_ms(now, node->time));
		return (NULL);
	}
	cache_unlink_node(c, node);
	node->lru_next = c->MRU;
	c->stats.hit++;
	c->MRU = node;
	LOG(C_MINI__LRU_LOG_HIT, c->stats.hit, key, key_len);
	return (c->h_arr[pos]->kv.value);
}
