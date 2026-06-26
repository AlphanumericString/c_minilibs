#include <stddef.h>
#include <string.h>

#define C_MINI__LRU_UTILS
#include "least_recently_used_cache.h"

// search
// -> hit -> set as head
void *cache_get(t_lruc *c, const void *key, size_t key_len) {
	t_keyCache *node;
	size_t      pos;

	if (!c || !c->head || !c->nb) return (NULL);
	pos = hash_djb2(key, key_len);
	for (; pos < c->nb && c->h_arr[pos]; pos = (pos + 1) % c->nb) {
		if (!memcmp(c->h_arr[pos]->key, key,
		            MAX(c->h_arr[pos]->k_len, key_len)))
			break;
	}
	if (!c->h_arr[pos] || memcmp(c->h_arr[pos]->key, key, key_len) != 0) {
		c->stats.misses++;
		return (NULL);
	}
	node = c->h_arr[pos];

	cache_unlink_node(c, node);

	node->next = c->head;
	c->stats.hit++;
	c->head = node;
	return (c->h_arr[pos]->value);
}
