#include <stdlib.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

static inline void lruci_hsh_push(t_cm_lruc *c, t_cm_lruc_keynode *n,
                                  size_t hash) {
	hash             = hash % c->buckets_nb;
	n->hash_next     = c->buckets[hash];
	c->buckets[hash] = n;
}
/**
 * \brief [INTERNAL] fill a fresh node and link it in every structure
 * \note owns the node_nb increment
**/
static bool insert_to_structs(t_cm_lruc_keynode kv, t_cm_lruc_keynode *node,
                              t_cm_lruc *c, size_t h_pos) {
	*node = kv;
	lruci_lru_push(c, node);
	lruci_hsh_push(c, node, h_pos);
	c->node_nb++;
	return (true);
}
static t_cm_lruc_keynode *get_node_to_update(t_cm_lruc         *c,
                                             t_cm_lruc_keynode *node) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_UPDATEVAL, node->kv.key,
	       node->kv.k_len)
	lruci_unlink_node(c, node);
	if (c->dtor) c->dtor(node->kv.value);
	return (node);
}
static t_cm_lruc_keynode *create_node(void) {
	// node_nb is owned by the insertion, creating a node does not count it yet
	t_cm_lruc_keynode *node = calloc(1, sizeof(*node));

	if (!node) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_EALLOC)
		return (NULL);
	}
	return (node);
}
/**
 * \brief [INTERNAL] pick the least recently used node, destroy its value and
 * unlink it so the memory can be reused
 * \return the recycled node, ready to be filled
**/
static t_cm_lruc_keynode *evict_lru(t_cm_lruc *c) {
	t_cm_lruc_keynode *node = c->LRU;

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_EVICTION, node->kv.key,
	       node->kv.k_len)
	if (c->dtor) c->dtor(node->kv.value);
	lruci_unlink_node(c, node);
	c->stats.expired++;
	return (node);
}
static t_cm_lruc_keynode *get_node(t_cm_lruc *c, t_cm_lruc_keynode add_val,
                                   size_t *hash_pos) {
	t_cm_lruc_keynode *node;

	node = lruci_keyinhash(c, add_val.kv.key, add_val.kv.k_len, hash_pos);
	if (node != NULL) {
		node = get_node_to_update(c, node);
	} else if (c->reserved_nodes) {
		node = lruci_rsrvd_pop(c);
	} else if (c->node_nb < c->buckets_nb) {
		node = create_node();
	} else {
		node = evict_lru(c);
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
 * \return true on success, false on failure
 * \note when the cache is full the least recently used entry is evicted and
 * counted in the `expired` stats. Updating an existing key reuses its node and
 * counts nothing.
 * \ingroup lruc
**/
bool lruc_put_kv(t_cm_lruc *cache, t_cm_keyvalue kv) {
	return (cache_put_internal(cache, (t_cm_lruc_keynode){
	                                      .kv        = kv,
	                                      .lru_next  = NULL,
	                                      .lru_prev  = NULL,
	                                      .hash_next = NULL,
	                                  }));
}
/**
 * \brief Put a key-value pair into the cache
 * \param cache cache pointer
 * \param key key to put
 * \param key_len key length
 * \param value value to put
 * \return true on success, false on failure
 * \ingroup lruc
**/
bool lruc_put(t_cm_lruc *cache, const void *key, size_t key_len,
              const void *value) {
	return (cache_put_internal(
	    cache, (t_cm_lruc_keynode){
	               .kv        = (t_cm_keyvalue){.key   = (void *)key,
	                                            .k_len = key_len,
	                                            .value = (void *)value},
	               .lru_next  = NULL,
	               .lru_prev  = NULL,
	               .hash_next = NULL,
    }));
}
