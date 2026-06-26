#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define C_MINI__LRU_UTILS
#include "least_recently_used_cache.h"

t_keyCache *cache_search_keyinlist(const t_lruc *c, const void *key, size_t key_len) {
	t_keyCache *node;

	node = c->head;
	for (node = c->head; node; node = node->next)
		if (node->k_len == key_len && memcmp(key, node->key, key_len) == 0)
			return (node);
	return (NULL);
}

t_keyCache *cache_search_keyinhash(const t_lruc *c, const void *key, size_t key_len,
                            size_t *_out) {
	size_t i;
	size_t it;

	if (!_out) _out = &it;
	it = hash_djb2(key, key_len) % c->cap;
	for (i = 0; i < c->cap && c->h_arr[it]; it = (it + 1) % c->cap, i++) {
		if (c->h_arr[it]->k_len == key_len &&
		    !memcmp(c->h_arr[it]->key, key, key_len)) {
			return (*_out = it, c->h_arr[it]);
		}
	}
	return (*_out = it, NULL);
}

// search
// -> hit -> set as head
void *cache_get(t_lruc *c, const void *key, size_t key_len) {
	t_keyCache *node;
	size_t      pos;

	if (!c || !c->head || !c->nb) return (NULL);
	node = cache_search_keyinhash(c, key, key_len, &pos);
	if (!node || memcmp(node->key, key, key_len) != 0) {
		c->stats.misses++;
		LOG(LRU_LOG_MISS, c->stats.misses, key, key_len);
		return (NULL);
	}
	cache_unlink_node(c, node);
	node->next = c->head;
	c->stats.hit++;
	c->head = node;
	LOG(LRU_LOG_HIT, c->stats.hit, key, key_len);
	return (c->h_arr[pos]->value);
}
