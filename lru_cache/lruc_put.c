#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

static inline void lruci_hsh_push(t_cm_lruc *c, t_cm_lruc_keynode *n,
                                  size_t hash) {
	hash             = hash % c->buckets_nb;
	n->hash_next     = NULL;
	n->hash_next     = c->buckets[hash];
	c->buckets[hash] = n;
}

static bool insert_to_structs(t_cm_lruc_keynode kv, t_cm_lruc_keynode *node,
                              t_cm_lruc *c, size_t h_pos) {
	*node = kv;
	lruci_lru_push(c, node);
	lruci_hsh_push(c, node, h_pos);
	c->node_nb--;
	lruci_hp_push(c, node);
	return (true);
}
static t_cm_lruc_keynode *
get_node_to_update(t_cm_lruc *c, t_cm_lruc_keynode *node, size_t hash_pos) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_UPDATEVAL, node->kv.key,
	       node->kv.k_len)
	lruci_unlink_node(c, node, hash_pos);
	return (node);
}
static t_cm_lruc_keynode *create_node(t_cm_lruc *c, t_cm_lruc_keynode *node) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_CREATEVAL,
	       node ? node->kv.key : NULL, node ? node->kv.k_len : -1)
	node = calloc(sizeof(t_cm_lruc_keynode), 1);
	if (!node) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_EALLOC)
		return (NULL);
	}
	c->node_nb++;
	return (node);
}
static t_cm_lruc_keynode *reuse_node(t_cm_lruc *c, size_t hash_pos) {
	const uint64_t     now = lruci_timenow();
	t_cm_lruc_keynode *node;

	if (now == (uint64_t)-1 && c->ttl_glob != C_MINI__LRUC_TTL_DFLT) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_ECLOCK);
		return (NULL);
	}
	if (c->ttl_glob == C_MINI__LRUC_TTL_DFLT ||
	    (c->minHeap[0]->time + c->ttl_glob) >= now) {
		node = c->LRU;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_NODE_LRU)
	} else {
		node = c->minHeap[0];
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_NODE_MINH)
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_DELETEVAL,
	       node ? node->kv.key : NULL, node ? node->kv.k_len : -1)
	if (c->dtor) c->dtor(node->kv.value);
	lruci_unlink_node(c, node, hash_pos);
	return (node);
}
static t_cm_lruc_keynode *get_node(t_cm_lruc *c, t_cm_lruc_keynode add_val,
                                   size_t *hash_pos) {
	t_cm_lruc_keynode *node;

	node = lruci_keyinhash(c, add_val.kv.key, add_val.kv.k_len, hash_pos);
	if (node != NULL) {
		node = get_node_to_update(c, node, *hash_pos);
	} else if (c->reserved_nodes) {
		node = lruci_rsrvd_pop(c);
		c->node_nb++;
	} else if (c->node_nb < c->buckets_nb) {
		node = create_node(c, node);
	} else {
		node = reuse_node(c, *hash_pos);
	}
	return (node);
}
static bool cache_put_internal(t_cm_lruc *c, t_cm_lruc_keynode to_add) {
	t_cm_lruc_keynode *node = NULL;
	size_t             hash_pos;

	if (!lruc_is_valid(c) || !to_add.kv.key || !to_add.kv.k_len ||
	    to_add.kv.k_len == (size_t)-1) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
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
 * \ingroup lruc
**/
bool lruc_put_kv(t_cm_lruc *cache, t_cm_keyvalue kv) {
	uint64_t now;

	if (cache && cache->ttl_glob != C_MINI__LRUC_TTL_DFLT) {
		now = lruci_timenow();
		if (now == (uint64_t)-1) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_ECLOCK)
			return (false);
		}
	} else now = 0;
	return (cache_put_internal(cache, (t_cm_lruc_keynode){
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
bool lruc_put(t_cm_lruc *cache, const void *key, size_t key_len,
              const void *value) {
	uint64_t now;

	if (cache && cache->ttl_glob != C_MINI__LRUC_TTL_DFLT) {
		now = lruci_timenow();
		if (now == (uint64_t)-1) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_ECLOCK)
			return (false);
		}
	} else now = 0;
	return (cache_put_internal(
	    cache, (t_cm_lruc_keynode){
	               .kv         = (t_cm_keyvalue){.key   = (void *)key,
	                                             .k_len = key_len,
	                                             .value = (void *)value},
	               .time       = now,
	               .lru_next   = NULL,
	               .lru_prev   = NULL,
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
bool lruc_put_ttl(t_cm_lruc *cache, const void *key, size_t key_len,
                  const void *value, uint64_t ttl_ms) {
	uint64_t now;

	now = lruci_timenow();
	if (now == (uint64_t)-1) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_ECLOCK)
		return (false);
	}
	return (cache_put_internal(
	    cache, (t_cm_lruc_keynode){
	               .kv         = (t_cm_keyvalue){.key   = (void *)key,
	                                             .k_len = key_len,
	                                             .value = (void *)value},
	               .time       = now + ttl_ms,
	               .lru_next   = NULL,
	               .lru_prev   = NULL,
	               .heap_index = 0,
	               .hash_next  = NULL,
    }));
}
