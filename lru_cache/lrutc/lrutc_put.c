#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define C_MINI__LRUTC_INTERNAL
#include "least_recently_used_timed_cache.h"

static inline void lrutci_hsh_push(t_cm_lrutc *c, t_cm_lrutc_keynode *n,
                                  size_t hash) {
	hash             = hash % c->buckets_nb;
	n->hash_next     = c->buckets[hash];
	c->buckets[hash] = n;
}

static bool insert_to_structs(t_cm_lrutc_keynode kv, t_cm_lrutc_keynode *node,
                              t_cm_lrutc *c, size_t h_pos) {
	*node = kv;
	lrutci_lru_push(c, node);
	lrutci_hsh_push(c, node, h_pos);
	lrutci_hp_push(c, node); // owns the node_nb increment
	return (true);
}
static t_cm_lrutc_keynode *get_node_to_update(t_cm_lrutc         *c,
                                             t_cm_lrutc_keynode *node) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUTC_LOG_UPDATEVAL, node->kv.key,
	       node->kv.k_len)
	lrutci_unlink_node(c, node);
	return (node);
}
static t_cm_lrutc_keynode *create_node(void) {
	t_cm_lrutc_keynode *node;

	// node_nb is owned by lrutci_hp_push, creating a node does not count it yet
	node = calloc(1, sizeof(*node));
	if (!node) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUTC_LOG_EALLOC)
		return (NULL);
	}
	return (node);
}
static t_cm_lrutc_keynode *reuse_node(t_cm_lrutc *c) {
	const uint64_t     now = lrutci_timenow();
	t_cm_lrutc_keynode *node;

	if (now == (uint64_t)-1 && c->ns_gap != C_MINI__LRUTC_TTL_DFLT) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUTC_LOG_ECLOCK);
		return (NULL);
	}
	if (c->ns_gap == C_MINI__LRUTC_TTL_DFLT ||
	    !lrutci_expired(c->minHeap[0], now)) {
		node = c->LRU;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUTC_LOG_NODE_LRU)
	} else {
		node = c->minHeap[0];
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUTC_LOG_NODE_MINH)
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUTC_LOG_DELETEVAL,
	       node ? node->kv.key : NULL, node ? node->kv.k_len : -1)
	if (c->dtor) c->dtor(node->kv.value);
	lrutci_unlink_node(c, node);
	return (node);
}
static t_cm_lrutc_keynode *get_node(t_cm_lrutc *c, t_cm_lrutc_keynode add_val,
                                   size_t *hash_pos) {
	t_cm_lrutc_keynode *node;

	node = lrutci_keyinhash(c, add_val.kv.key, add_val.kv.k_len, hash_pos);
	if (node != NULL) {
		node = get_node_to_update(c, node);
	} else if (c->reserved_nodes) {
		node = lrutci_rsrvd_pop(c);
	} else if (c->node_nb < c->buckets_nb) {
		node = create_node();
	} else {
		node = reuse_node(c);
	}
	return (node);
}
static bool cache_put_internal(t_cm_lrutc *c, t_cm_lrutc_keynode to_add) {
	t_cm_lrutc_keynode *node = NULL;
	size_t             hash_pos;

	if (!lrutc_is_valid(c) || !to_add.kv.key || !to_add.kv.k_len ||
	    to_add.kv.k_len == (size_t)-1) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUTC_LOG_EARG)
		return (false);
	}
	node = get_node(c, to_add, &hash_pos);
	if (!node) return (false);
	return (insert_to_structs(to_add, node, c, hash_pos));
}
/**
 * \brief Put a key-value pair into the cache
 * \param cache the lru cache pointer
 * \param kv key-value pair
 * \return 0 on success, -1 on failure
 * \ingroup lrutc
**/
bool lrutc_put_kv(t_cm_lrutc *cache, t_cm_keyvalue kv) {
	uint64_t now = 0;
	size_t   gap = C_MINI__LRUTC_TTL_DFLT;

	if (cache) {
		gap = cache->ns_gap;
		if (gap != C_MINI__LRUTC_TTL_DFLT) {
			now = lrutci_timenow();
			if (now == (uint64_t)-1) {
				CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUTC_LOG_ECLOCK)
				return (false);
			}
		}
	}
	return (cache_put_internal(cache, (t_cm_lrutc_keynode){
	                                      .kv           = kv,
	                                      .time_emitted = now,
	                                      .ns_gap       = gap,
	                                      .lru_next     = 0,
	                                      .lru_prev     = 0,
	                                      .heap_index   = 0,
	                                      .hash_next    = NULL,
	                                  }));
}
/**
 * \brief Put a key-value pair into the cache
 * \param cache cache pointer
 * \param key key to put
 * \param key_len key length
 * \param value value to put
 * \return 0 on success, -1 on failure
 * \ingroup lrutc
**/
bool lrutc_put(t_cm_lrutc *cache, const void *key, size_t key_len,
              const void *value) {
	uint64_t now = 0;
	size_t   gap = C_MINI__LRUTC_TTL_DFLT;

	if (cache) {
		gap = cache->ns_gap;
		if (gap != C_MINI__LRUTC_TTL_DFLT) {
			now = lrutci_timenow();
			if (now == (uint64_t)-1) {
				CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUTC_LOG_ECLOCK)
				return (false);
			}
		}
	}
	return (cache_put_internal(
	    cache, (t_cm_lrutc_keynode){
	               .kv           = (t_cm_keyvalue){.key   = (void *)key,
	                                               .k_len = key_len,
	                                               .value = (void *)value},
	               .time_emitted = now,
	               .ns_gap       = gap,
	               .lru_next     = NULL,
	               .lru_prev     = NULL,
	               .heap_index   = 0,
	               .hash_next    = NULL,
    }));
}
/**
 * \brief Put a key-value pair into the cache with a time to live
 * \param cache cache pointer
 * \param key key to put
 * \param key_len key length
 * \param value value to put
 * \param ttl_ns time to live in nanoseconds
 * \return 0 on success, -1 on failure
 * \ingroup lrutc
**/
bool lrutc_put_ttl(t_cm_lrutc *cache, const void *key, size_t key_len,
                  const void *value, size_t ttl_ns) {
	uint64_t now;

	now = lrutci_timenow();
	if (now == (uint64_t)-1) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUTC_LOG_ECLOCK)
		return (false);
	}
	return (cache_put_internal(
	    cache, (t_cm_lrutc_keynode){
	               .kv           = (t_cm_keyvalue){.key   = (void *)key,
	                                               .k_len = key_len,
	                                               .value = (void *)value},
	               .time_emitted = now,
	               .ns_gap       = ttl_ns,
	               .lru_next     = NULL,
	               .lru_prev     = NULL,
	               .heap_index   = 0,
	               .hash_next    = NULL,
    }));
}
