#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

void cache_clear(t_lruc *c) {
	t_lruc_keynode *nxt;

	if (!c || !c->nb || !c->cap) return;
	if ((c->cap / c->nb) >= C_MINI__LRU_HASH_SKIP) { // less than 10% filled
		LOG(C_MINI__LRU_LOG_LFREE);
		for (t_lruc_keynode *node = c->MRU; node; node = nxt) {
			if (c->dtor) c->dtor(node->kv.value);
			LOG(C_MINI__LRU_LOG_DELETEVAL, node->kv.key, node->kv.k_len);
			nxt = node->lru_next;
			free(node);
		}
	} else {
		LOG(C_MINI__LRU_LOG_HFREE);
		for (size_t i = 0; i < c->cap; i++) {
			if (!c->h_arr[i]) continue;
			if (c->dtor) c->dtor(c->h_arr[i]->kv.value);
			LOG(C_MINI__LRU_LOG_DELETEVAL, c->h_arr[i]->kv.key,
			    c->h_arr[i]->kv.k_len);
			free(c->h_arr[i]);
		}
	}
	bzero(c->h_arr, sizeof(typeof(*c->h_arr)) * c->cap);
	c->LRU     = NULL;
	c->MRU     = NULL;
	c->TTL_Min = NULL;
	c->TTL_Max = NULL;
	c->nb      = 0;
	c->stats   = (t_lruc_stats){0};
}
int cache_remove(t_lruc *c, const void *key, size_t key_len) {
	t_lruc_keynode *node;
	size_t          pos;

	if (!c || !c->MRU || !c->nb) return (-1);
	node = cache_search_keyinhash(c, key, key_len, &pos);
	if (!node || memcmp(node->kv.key, key, key_len) != 0) return (-1);
	cache_unlink_node(c, node);
	if (c->dtor) c->dtor(node->kv.value);
	LOG(C_MINI__LRU_LOG_DELETEVAL, node->kv.key, node->kv.k_len);
	free(node);
	c->h_arr[pos] = NULL;
	c->nb--;
	return (0);
}
