#include <stdint.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Look a key up without computing anything on a miss
 * \param c cache pointer
 * \param key key to get
 * \param key_len key length
 * \return pointer to the value, NULL on miss
 * \ingroup lruc
**/
void *lruc_try_get(t_cm_lruc *c, const void *key, size_t key_len) {
	t_cm_lruc_keynode *node;

	if (!lruc_is_valid(c) || !c->node_nb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (NULL);
	}
	node = lruci_keyinhash(c, key, key_len, NULL);
	if (!node) {
		c->stats.misses++;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_MISS, c->stats.misses, key,
		       key_len)
		return (NULL);
	}
	lruci_lru_push(c, lruci_lru_pop(c, node));
	c->stats.hit++;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_HIT, c->stats.hit, key, key_len)
	return (node->kv.value);
}
/**
 * \brief Get a key from the cache, computing and storing it on a miss
 * \param c cache pointer
 * \param key key to get
 * \param key_len key length
 * \param fun work called with arg on a miss, its return value is stored in the
 * cache and returned
 * \param arg argument passed "as is" to fun
 * \return pointer to the value, NULL on failure
 * \ingroup lruc
**/
void *lruc_get(t_cm_lruc *c, const void *key, size_t key_len, t_cm_work fun,
               void *arg) {
	void *v;

	if (!lruc_is_valid(c)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (NULL);
	}
	v = lruc_try_get(c, key, key_len);
	if (v) return (v);
	if (!fun) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (NULL);
	}
	v = fun(arg);
	if (!v) return (NULL);
	if (!lruc_put(c, key, key_len, v)) return (NULL);
	return (v);
}
