#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define C_MINI__LRU_UTILS
#include "least_recently_used_cache.h"

void cache_clear(t_lruc *c) {
	t_keyCache *nxt;

	if (!c || !c->nb || !c->cap) return;
	if ((c->cap / c->nb) >= 10) // less than 10% filled
		for (t_keyCache *node = c->head; node; node = nxt) {
			if (c->dtor) c->dtor(node->value);
			nxt = node->next;
			free(node);
		}
	else {
		for (size_t i = 0; i < c->cap; i++) {
			if (!c->h_arr[i]) continue;
			if (c->dtor) c->dtor(c->h_arr[i]->value);
			free(c->h_arr[i]);
		}
	}
	bzero(c->h_arr, sizeof(typeof(*c->h_arr)) * c->cap);
	c->tail  = NULL;
	c->head  = NULL;
	c->nb    = 0;
	c->stats = (cstats_t){0};
}
int cache_remove(t_lruc *c, const void *key, size_t key_len) {
	t_keyCache *node;
	size_t      pos;

	if (!c || !c->head || !c->nb) return (-1);
	pos = hash_djb2(key, key_len);
	for (; pos < c->nb && c->h_arr[pos]; pos = (pos + 1) % c->nb) {
		if (!memcmp(c->h_arr[pos]->key, key,
		            MAX(c->h_arr[pos]->k_len, key_len)))
			break;
	}
	if (!c->h_arr[pos] || memcmp(c->h_arr[pos]->key, key, key_len) != 0)
		return (-1);
	node = c->h_arr[pos];
	cache_unlink_node(c, node);
	if (c->dtor) c->dtor(node);
	free(node);
	c->h_arr[pos] = NULL;
	c->nb--;
	return (true);
}
