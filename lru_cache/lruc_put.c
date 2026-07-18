#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

// Insert a key-value pair into the cache in lru and ttl internals
static int insert_with_ttl_regards(t_lruc_keynode kv, t_lruc_keynode *node,
                                   t_lruc *c, size_t h_pos) {
	*node = kv;
	lruci_lru_push(c, node);
	lruci_hsh_push(c, node, h_pos);
	c->node_in_use--;
	lruci_hp_push(c, node);
	return (0);
}
// Put a key-value pair into the cache (constructor + node choice)
static int cache_put_internal(t_lruc *c, t_lruc_keynode to_add) {
	const uint64_t  now  = lruci_timenow();
	t_lruc_keynode *node = NULL;
	size_t          hash_pos;

	if (!c || !to_add.kv.key || to_add.kv.k_len <= 0 ||
	    to_add.kv.k_len == (size_t)-1) {
		LOG(C_MINI__LRUC_LOG_EARG);
		return (-1);
	}
	node = lruci_keyinhash(c, to_add.kv.key, to_add.kv.k_len, &hash_pos);
	if (node != NULL && lruci_key_eq(node->kv.key, node->kv.k_len,
	                                 to_add.kv.key, to_add.kv.k_len)) {
		// update value
		LOG(C_MINI__LRUC_LOG_UPDATEVAL, to_add.kv.key, to_add.kv.k_len);
		lruci_unlink_node(c, node, hash_pos);
	} else if (!node && c->node_in_use < c->buckets_nb) { // new node
		LOG(C_MINI__LRUC_LOG_CREATEVAL, to_add.kv.key, to_add.kv.k_len);
		node = calloc(sizeof(t_lruc_keynode), 1);
		if (!node) {
			LOG(C_MINI__LRUC_LOG_EALLOC);
			return (-1);
		}
		c->node_in_use++;
	} else { // re-use cache tail node (LRU) since cachce is full
		if (now == (uint64_t)-1 || (c->minHeap[0]->time + c->ttl_glob) >= now) {
			node = c->LRU;
			LOG(C_MINI__LRUC_LOG_NODE_LRU);
		} else {
			node = c->minHeap[0];
			LOG(C_MINI__LRUC_LOG_NODE_MINH);
		}
		LOG(C_MINI__LRUC_LOG_CREATEVAL, to_add.kv.key, to_add.kv.k_len);
		LOG(C_MINI__LRUC_LOG_DELETEVAL, node->kv.key, node->kv.k_len);
		if (c->dtor) c->dtor(node->kv.value);
		lruci_unlink_node(c, node, hash_pos);
	}
	return (insert_with_ttl_regards(to_add, node, c, hash_pos));
}
/**
 * \brief Put a key-value pair into the cache
 * \param c cache pointer
 * \param kv key-value pair
 * \return 0 on success, -1 on failure
 * \ingroup lruc
**/
int lruc_put_kv(t_lruc *c, t_keyvalue kv) {
	uint64_t now;

	now = lruci_timenow();
	if (now == (uint64_t)-1) {
		LOG(C_MINI__LRUC_LOG_ECLOCK);
		return (-1);
	}
	return (cache_put_internal(c, (t_lruc_keynode){
	                                  .kv         = kv,
	                                  .time       = now,
	                                  .lru_next   = 0,
	                                  .lru_prev   = 0,
	                                  .heap_index = 0,
	                                  .hash_next  = NULL,
	                              }));
}

/**
 * \brief Put a key-value pair into the cache
 * \param cache cache pointer
 * \param key key to put
 * \param key_len key length
 * \param value value to put
 * \return 0 on success, -1 on failure
 * \ingroup lruc
**/
int lruc_put(t_lruc *cache, const void *key, size_t key_len,
             const void *value) {
	uint64_t now;

	now = lruci_timenow();
	if (now == (uint64_t)-1) {
		LOG(C_MINI__LRUC_LOG_ECLOCK);
		return (-1);
	}
	return (cache_put_internal(cache,
	                           (t_lruc_keynode){
	                               .kv       = (t_keyvalue){.key   = (void *)key,
	                                                        .k_len = key_len,
	                                                        .value = (void *)value},
	                               .time     = now,
	                               .lru_next = NULL,
	                               .lru_prev = NULL,
	                               .heap_index = 0,
	                               .hash_next  = NULL,
    }));
}

/**
 * \brief Put a key-value pair into the cache with a time to live
 * \param cache cache pointer
 * \param key key to put
 * \param key_len key length
 * \param value value to put
 * \param ttl_ms time to live in milliseconds
 * \return 0 on success, -1 on failure
 * \ingroup lruc
**/
int lruc_put_ttl(t_lruc *cache, const void *key, size_t key_len,
                 const void *value, uint64_t ttl_ms) {
	uint64_t now;

	now = lruci_timenow();
	if (now == (uint64_t)-1) {
		LOG(C_MINI__LRUC_LOG_ECLOCK);
		return (-1);
	}
	return (cache_put_internal(cache,
	                           (t_lruc_keynode){
	                               .kv       = (t_keyvalue){.key   = (void *)key,
	                                                        .k_len = key_len,
	                                                        .value = (void *)value},
	                               .time     = now + ttl_ms,
	                               .lru_next = NULL,
	                               .lru_prev = NULL,
	                               .heap_index = 0,
	                               .hash_next  = NULL,
    }));
}
