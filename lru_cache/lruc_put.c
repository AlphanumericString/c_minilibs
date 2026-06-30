#include <assert.h>
#include <bits/time.h>
#include <bits/types/struct_timeval.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

/**
 * Insert a key-value pair into the cache in lru and ttl
 * internal function
 **/
static int insert_with_ttl_regards(t_lruc_keynode kv, t_lruc_keynode *node,
                                   t_lruc *c, size_t h_pos) {
	t_lruc_keynode *it = NULL;

	*node = kv;
	push_lruc(c, node);
	push_hash(c, node, h_pos);
	if (c->node_in_use == 1) {
		c->TTL_Max = node;
		c->TTL_Min = node;
	} else if (kv.time <= c->TTL_Min->time) {
		c->TTL_Min->ttl_next = node;
		node->ttl_prev       = c->TTL_Min;
		c->TTL_Min           = node;
	} else if (kv.time >= c->TTL_Max->time) {
		c->TTL_Max->ttl_prev = node;
		node->ttl_next       = c->TTL_Max;
		c->TTL_Max           = node;
	} else {
		it = c->TTL_Max;
		while (kv.time < it->time)
			it = it->ttl_next;
		assert(it != NULL);
		if (it->ttl_prev) it->ttl_prev->ttl_next = node;
		node->ttl_prev = it->ttl_prev;
		it->ttl_prev   = node;
		node->ttl_next = it;
	}
	return (0);
}
/**
 * Put a key-value pair into the cache (constructor + node choice)
 * internal function
 **/
static int cache_put_internal(t_lruc *c, t_lruc_keynode to_add) {
	struct timespec now;
	t_lruc_keynode *node = NULL;
	size_t          hash_pos;

	if (!c || !to_add.kv.key || to_add.kv.k_len <= 0 ||
	    to_add.kv.k_len == (size_t)-1) {
		LOG(C_MINI__LRU_LOG_EARG);
		return (-1);
	}
	node = cache_search_keyinhash(c, to_add.kv.key, to_add.kv.k_len, &hash_pos);
	if (node != NULL && is_key_eq(node->kv.key, node->kv.k_len, to_add.kv.key,
	                              to_add.kv.k_len)) { // update value
		LOG(C_MINI__LRU_LOG_UPDATEVAL, to_add.kv.key, to_add.kv.k_len);
		cache_unlink_node(c, node, hash_pos);
	} else if (!node && c->node_in_use < c->buckets_nb) { // new node
		LOG(C_MINI__LRU_LOG_CREATEVAL, to_add.kv.key, to_add.kv.k_len);
		node = calloc(sizeof(t_lruc_keynode), 1);
		if (!node) {
			LOG(C_MINI__LRU_LOG_EALLOC);
			return (-1);
		}
		c->node_in_use++;
	} else { // re-use cache tail node (LRU) since cachce is full
		if (clock_gettime(CLOCK_MONOTONIC, &now) == -1 ||
		    c->TTL_Min->time <= timeto64(now) + c->ttl_glob)
			node = c->LRU;
		else node = c->TTL_Min;
		LOG(C_MINI__LRU_LOG_CREATEVAL, to_add.kv.key, to_add.kv.k_len);
		LOG(C_MINI__LRU_LOG_DELETEVAL, node->kv.key, node->kv.k_len);
		if (c->dtor) c->dtor(c->LRU->kv.value);
		cache_unlink_node(c, node, hash_pos);
	}
	return (insert_with_ttl_regards(to_add, node, c, hash_pos));
}
/**
 * @brief Put a key-value pair into the cache
 * @param c cache pointer
 * @param kv key-value pair
 * @return 0 on success, -1 on failure
 **/
int cache_put_kv(t_lruc *c, t_keyvalue kv) {
	struct timespec now;

	if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
		LOG(C_MINI__LRU_LOG_ECLOCK);
		return (-1);
	}
	return (
	    cache_put_internal(c, (t_lruc_keynode){.kv   = kv,
	                                           .time = now.tv_sec * 1000ULL +
	                                                   now.tv_nsec / 1000000ULL,
	                                           .lru_next = 0,
	                                           .ttl_next = 0,
	                                           .ttl_prev = 0,
	                                           .lru_prev = 0}));
}
/**
 * @brief Put a key-value pair into the cache
 * @param cache cache pointer
 * @param key key to put
 * @param key_len key length
 * @param value value to put
 * @return 0 on success, -1 on failure
 **/
int cache_put(t_lruc *cache, const void *key, size_t key_len,
              const void *value) {
	struct timespec now;

	if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
		LOG(C_MINI__LRU_LOG_ECLOCK);
		return (-1);
	}
	return (cache_put_internal(
	    cache, (t_lruc_keynode){
	               .kv       = (t_keyvalue){.key   = (void *)key,
	                                        .k_len = key_len,
	                                        .value = (void *)value},
	               .time     = now.tv_sec * 1000ULL + now.tv_nsec / 1000000ULL,
	               .lru_next = NULL,
	               .lru_prev = NULL,
	               .ttl_next = NULL,
	               .ttl_prev = NULL
    }));
}

/**
 * @brief Put a key-value pair into the cache with a time to live
 * @param cache cache pointer
 * @param key key to put
 * @param key_len key length
 * @param value value to put
 * @param ttl_ms time to live in milliseconds
 * @return 0 on success, -1 on failure
 **/
int cache_put_ttl(t_lruc *cache, const void *key, size_t key_len,
                  const void *value, uint64_t ttl_ms) {
	struct timespec now;

	if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
		LOG(C_MINI__LRU_LOG_ECLOCK);
		return (-1);
	}
	return (cache_put_internal(
	    cache,
	    (t_lruc_keynode){
	        .kv   = (t_keyvalue){.key   = (void *)key,
	                             .k_len = key_len,
	                             .value = (void *)value},
	        .time = now.tv_sec * 1000ULL + now.tv_nsec / 1000000ULL + ttl_ms,
	        .lru_next = NULL,
	        .lru_prev = NULL,
	        .ttl_next = NULL,
	        .ttl_prev = NULL
    }));
}
