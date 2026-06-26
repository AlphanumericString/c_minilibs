#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define C_MINI__LRU_UTILS
#include "least_recently_used_cache.h"

#define MAX(a, b) ((a) > (b)) ? (a) : (b)

// Very fast, but not very good
// see http://www.cse.yorku.ca/~oz/hash.html
// See ft_hash_sdbm or ft_hash_fnv1a for less collisions prone hash functions
size_t hash_djb2(const void *key, size_t size) {
	unsigned char *str;
	size_t         hash;

	hash = 5381;
	str  = (unsigned char *)key;
	while (size--)
		hash = (hash << 5) + hash + str[size];
	return (hash);
}

int cache_put_kv(t_lruc *c, t_keyCache kv) {
	t_keyCache    *node = NULL;
	struct timeval tmp;
	size_t         pos;

	pos = hash_djb2(node->key, node->k_len);
	for (; pos < c->nb && c->h_arr[pos]; pos = (pos + 1) % c->nb) {
		if (!memcmp(c->h_arr[pos]->key, kv.key,
		            MAX(c->h_arr[pos]->k_len, kv.k_len)))
			break;
	}
	if (c->h_arr[pos] && !memcmp(c->h_arr[pos]->key, kv.key,
	                             MAX(c->h_arr[pos]->k_len,
	                                 kv.k_len))) { // update value
		node = c->h_arr[pos];
		cache_unlink_node(c, node);
	} else if (c->nb < c->cap) {
		node = calloc(sizeof(t_keyCache), 1);
		if (!node) return (-1);
		c->h_arr[pos] = node;
		c->nb++;
	} else { // re-use cache tail node (LRU) since cachce is full
		node = c->tail;
		cache_unlink_node(c, node);
	}
	tmp = kv.time;
	gettimeofday(&kv.time, NULL);
	kv.time.tv_sec += tmp.tv_sec + ((kv.time.tv_usec + tmp.tv_usec) / 1000000);
	kv.time.tv_usec = (kv.time.tv_usec + tmp.tv_usec) % 1000000;
	*node           = kv;
	cache_link_node(c, node);
	return (0);
}
int cache_put(t_lruc *cache, const void *key, size_t key_len, void *value) {
	return (cache_put_kv(cache, (t_keyCache){.key   = (void *)key,
	                                         .k_len = key_len,
	                                         .value = value,
	                                         .time  = (struct timeval){0},
	                                         .next  = NULL,
	                                         .prev  = NULL}));
}

// puts a key in the cache and add ttl_ms more ms to the cache validity period
int cache_put_ttl(t_lruc *cache, const void *key, size_t key_len, void *value,
                  uint64_t ttl_ms) {
	return (cache_put_kv(
	    cache,
	    (t_keyCache){
	        .key   = (void *)key,
	        .k_len = key_len,
	        .value = value,
	        .time  = (struct timeval){.tv_sec = 0, .tv_usec = ttl_ms * 1000},
	        .next  = NULL,
	        .prev  = NULL
    }));
}
