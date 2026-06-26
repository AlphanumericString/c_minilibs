#include <bits/types/struct_timeval.h>
#include <stddef.h>
#include <stdio.h>
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

static int insert_with_ttl_regards(t_keyCache kv, t_keyCache *node, t_lruc *c) {
	struct timespec tmp;
	t_keyCache     *it = NULL;

	clock_gettime(CLOCK_MONOTONIC, &tmp);

	// ttl was specifed - we check where to insert in linkedlist
	kv.time = (struct timespec){
	    .tv_sec  = kv.time.tv_sec + tmp.tv_sec,
	    .tv_nsec = kv.time.tv_nsec + tmp.tv_nsec,
	};

	if (kv.time.tv_nsec >= 1000000000L) {
		++kv.time.tv_sec;
		kv.time.tv_nsec -= 1000000000L;
	}
	*node = kv;
	if (c->nb == 1) {
		cache_link_node(c, node);
	} else if (distance_ms(kv.time, c->tail->time) < 0) { // is kv.time shorter?
		DBG("shorter");
		node->prev = c->tail;
		node->next = NULL;
		c->tail    = node;
	} else if (distance_ms(kv.time, c->head->time) > 0) { // is kv.time longer?
		DBG("longer");
		node->next = c->head;
		node->prev = NULL;
		c->head    = node;
	} else {
		DBG("search");
		it = c->head;
		while (distance_ms(kv.time, it->time) < 0)
			it = it->next;
		node->prev = it->prev;
		node->next = it;
		it->prev   = node;
	}
	return (0);
}

int cache_put_kv(t_lruc *c, t_keyCache kv) {
	t_keyCache *node = NULL;
	size_t      pos;

	if (!kv.key) return (-1);
	if (kv.k_len == (size_t)-1) kv.k_len = strlen(kv.key);
	node = cache_search_keyinhash(c, kv.key, kv.k_len, &pos);
	if (node != NULL && node->k_len == kv.k_len &&
	    !memcmp(node->key, kv.key, kv.k_len)) { // update value
		LOG(LRU_LOG_UPDATEVAL, kv.key, kv.k_len);
		cache_unlink_node(c, node);
	} else if (!node && c->nb < c->cap) { // new node
		LOG(LRU_LOG_CREATEVAL, kv.key, kv.k_len);
		node = calloc(sizeof(t_keyCache), 1);
		if (!node) return (-1);
		c->h_arr[pos] = node;
		c->nb++;
	} else { // re-use cache tail node (LRU) since cachce is full
		LOG(LRU_LOG_CREATEVAL, kv.key, kv.k_len);
		LOG(LRU_LOG_DELETEVAL, c->tail->key, c->tail->k_len);
		node = c->tail;
		if (c->dtor) c->dtor(c->tail->value);
		cache_unlink_node(c, node);
	}
	return (insert_with_ttl_regards(kv, node, c));
}
int cache_put(t_lruc *cache, const void *key, size_t key_len, void *value) {
	return (cache_put_kv(cache, (t_keyCache){.key   = (void *)key,
	                                         .k_len = key_len,
	                                         .value = value,
	                                         .time  = (struct timespec){0},
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
	        .time  = (struct timespec){.tv_sec  = ttl_ms / 1000,
	                                   .tv_nsec = (ttl_ms % 1000) * 1000000},
	        .next  = NULL,
	        .prev  = NULL
    }));
}
