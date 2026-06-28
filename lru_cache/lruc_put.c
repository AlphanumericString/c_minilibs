#include <assert.h>
#include <bits/time.h>
#include <bits/types/struct_timeval.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define C_MINI__LRU_INTERNAL
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

static struct timespec add_times(struct timespec a, struct timespec b) {
	a = (struct timespec){
	    .tv_sec  = a.tv_sec + b.tv_sec,
	    .tv_nsec = a.tv_nsec + b.tv_nsec,
	};

	if (a.tv_nsec >= 1000000000L) {
		++a.tv_sec;
		a.tv_nsec -= 1000000000L;
	}
	return (a);
}
static int insert_with_ttl_regards(t_lruc_keynode kv, t_lruc_keynode *node,
                                   t_lruc *c) {
	struct timespec tmp;
	t_lruc_keynode *it = NULL;

	clock_gettime(CLOCK_MONOTONIC, &tmp);
	kv.time = add_times(kv.time, tmp);
	*node   = kv;
	push_lruc(c, node);
	if (c->nb == 1) {
		c->TTL_Max = node;
		c->TTL_Min = node;
	} else if (distance_ms(kv.time, c->TTL_Min->time) <= 0) {
		c->TTL_Min->ttl_next = node;
		node->ttl_prev       = c->TTL_Min;
		c->TTL_Min           = node;
	} else if (distance_ms(kv.time, c->TTL_Max->time) >= 0) {
		c->TTL_Max->ttl_prev = node;
		node->ttl_next       = c->TTL_Max;
		c->TTL_Max           = node;
	} else {
		it = c->TTL_Max;
		while (distance_ms(kv.time, it->time) < 0)
			it = it->ttl_next;
		assert(it != NULL);
		if (it->ttl_prev) it->ttl_prev->ttl_next = node;
		node->ttl_prev = it->ttl_prev;
		it->ttl_prev   = node;
		node->ttl_next = it;
	}
	return (0);
}
static int cache_put_internal(t_lruc *c, t_lruc_keynode to_add) {
	struct timespec now;
	t_lruc_keynode *node = NULL;
	size_t          pos;

	if (!to_add.kv.key) return (-1);
	if (to_add.kv.k_len == (size_t)-1) to_add.kv.k_len = strlen(to_add.kv.key);
	node = cache_search_keyinhash(c, to_add.kv.key, to_add.kv.k_len, &pos);
	if (node != NULL && node->kv.k_len == to_add.kv.k_len &&
	    !memcmp(node->kv.key, to_add.kv.key, to_add.kv.k_len)) { // update value
		LOG(C_MINI__LRU_LOG_UPDATEVAL, to_add.kv.key, to_add.kv.k_len);
		cache_unlink_node(c, node);
	} else if (!node && c->nb < c->cap) { // new node
		LOG(C_MINI__LRU_LOG_CREATEVAL, to_add.kv.key, to_add.kv.k_len);
		node = calloc(sizeof(t_lruc_keynode), 1);
		if (!node) return (-1);
		c->h_arr[pos] = node;
		c->nb++;
	} else { // re-use cache tail node (LRU) since cachce is full
		clock_gettime(CLOCK_MONOTONIC, &now);
		node = (distance_ms(now, c->TTL_Min->time) >= c->ttl_glob) ? c->TTL_Min
		                                                           : c->LRU;
		LOG(C_MINI__LRU_LOG_CREATEVAL, to_add.kv.key, to_add.kv.k_len);
		LOG(C_MINI__LRU_LOG_DELETEVAL, node->kv.key, node->kv.k_len);
		if (c->dtor) c->dtor(c->LRU->kv.value);
		cache_unlink_node(c, node);
	}
	return (insert_with_ttl_regards(to_add, node, c));
}
int cache_put_kv(t_lruc *c, t_keyvalue kv) {
	return (cache_put_internal(c, (t_lruc_keynode){.kv   = kv,
	                                               .time = (struct timespec){0},
	                                               .lru_next = 0,
	                                               .ttl_next = 0,
	                                               .ttl_prev = 0,
	                                               .lru_prev = 0}));
}
int cache_put(t_lruc *cache, const void *key, size_t key_len, void *value) {
	return (cache_put_internal(cache, (t_lruc_keynode){
	                                      .kv = (t_keyvalue){.key = (void *)key,
	                                                         .k_len = key_len,
	                                                         .value = value},
	                                      .time     = (struct timespec){0},
	                                      .lru_next = NULL,
	                                      .lru_prev = NULL,
	                                      .ttl_next = NULL,
	                                      .ttl_prev = NULL
    }));
}

// puts a key in the cache and add ttl_ms more ms to the cache validity period
int cache_put_ttl(t_lruc *cache, const void *key, size_t key_len, void *value,
                  uint64_t ttl_ms) {
	return (cache_put_internal(
	    cache,
	    (t_lruc_keynode){
	        .kv       = (t_keyvalue){.key   = (void *)key,
	                                 .k_len = key_len,
	                                 .value = value},
	        .time     = (struct timespec){.tv_sec  = ttl_ms / 1000,
                                     .tv_nsec = (ttl_ms % 1000) * 1000000},
	        .lru_next = NULL,
	        .lru_prev = NULL,
	        .ttl_next = NULL,
	        .ttl_prev = NULL
    }));
}
