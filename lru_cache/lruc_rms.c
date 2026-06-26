#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define C_MINI__LRU_UTILS
#include "least_recently_used_cache.h"

void cache_clear(t_lruc *c) {
	t_keyCache *nxt;

	if (!c || !c->nb || !c->cap) return;
	if ((c->cap / c->nb) >= 10) { // less than 10% filled
		LOG(LRU_LOG_LFREE);
		for (t_keyCache *node = c->head; node; node = nxt) {
			if (c->dtor) c->dtor(node->value);
			LOG(LRU_LOG_DELETEVAL, node->key, node->k_len);
			nxt = node->next;
			free(node);
		}
	} else {
		LOG(LRU_LOG_HFREE);
		for (size_t i = 0; i < c->cap; i++) {
			if (!c->h_arr[i]) continue;
			if (c->dtor) c->dtor(c->h_arr[i]->value);
			LOG(LRU_LOG_DELETEVAL, c->h_arr[i]->key, c->h_arr[i]->k_len);
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
	node = cache_search_keyinhash(c, key, key_len, &pos);
	if (!node || memcmp(node->key, key, key_len) != 0) return (-1);
	cache_unlink_node(c, node);
	if (c->dtor) c->dtor(node->value);
	LOG(LRU_LOG_DELETEVAL, node->key, node->k_len);
	free(node);
	c->h_arr[pos] = NULL;
	c->nb--;
	return (0);
}
