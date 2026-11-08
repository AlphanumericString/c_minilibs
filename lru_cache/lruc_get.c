#include <bits/time.h>
#include <bits/types/struct_timeval.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Get a key from the cache without checks for ttl
 * \param c cache pointer
 * \param key key to get
 * \param key_len key length
 * \return pointer to the value
 * \ingroup lruc
**/
void *lruc_get_lru(t_cm_lruc *c, const void *key, size_t key_len) {
	t_cm_lruc_keynode *node;
	size_t             pos;

	if (!lruc_is_valid(c) || !c->node_nb) return (NULL);
	node = lruci_keyinhash(c, key, key_len, &pos);
	if (!node) {
		c->stats.misses++;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_MISS_LRU, c->stats.misses,
		       key, key_len)
		return (NULL);
	}
	lruci_lru_push(c, lruci_lru_pop(c, node));
	c->stats.hit++;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_HIT, c->stats.hit, key, key_len)
	return (node->kv.value);
}
/**
 * \brief Try to get a key from the cache, ttl aware (returns NULL on miss or
 * expiry)
 * \param c cache pointer
 * \param key key to get
 * \param len key length
 * \return pointer to the value, NULL on miss or expiry
 * \ingroup lruc
**/
void *lruc_try_get(t_cm_lruc *c, const void *key, size_t len) {
	uint64_t           now;
	t_cm_lruc_keynode *node;

	if (!lruc_is_valid(c) || !c->node_nb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (NULL);
	}
	node = lruci_keyinhash(c, key, len, NULL);
	if (!node) {
		c->stats.misses++;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_MISS_LRU, c->stats.misses,
		       key, len)
		return (NULL);
	}
	if (node->ns_gap != C_MINI__LRUC_TTL_DFLT) {
		now = lruci_timenow();
		if (now == (uint64_t)-1) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_ECLOCK)
			return (NULL);
		}
		if (lruci_expired(node, now)) {
			c->stats.misses++;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_MISS_TTL,
			       c->stats.misses, key, len, now - node->time_emitted)
			return (NULL);
		}
	}
	lruci_lru_push(c, lruci_lru_pop(c, node));
	c->stats.hit++;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_HIT, c->stats.hit, key, len)
	return (node->kv.value);
}
/**
 * \brief Get a key from the cache, computing and storing it on a miss
 * \param c cache pointer
 * \param key key to get
 * \param len key length
 * \param fun work called with arg on a miss, its return value is stored in the
 * cache and returned
 * \param arg argument passed "as is" to fun
 * \return pointer to the value, NULL on failure
 * \ingroup lruc
**/
void *lruc_get(t_cm_lruc *c, const void *key, size_t len, t_cm_work fun,
               void *arg) {
	void *v;

	if (!lruc_is_valid(c)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (NULL);
	}
	v = lruc_try_get(c, key, len);
	if (v) return (v);
	if (!fun) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (NULL);
	}
	v = fun(arg);
	if (!v) return (NULL);
	if (!lruc_put(c, key, len, v)) return (NULL);
	return (v);
}
/**
 * \brief Get a key from the cache with a per-call ttl, computing and storing it
 * on a miss
 * \param c cache pointer
 * \param key key to get
 * \param len key length
 * \param fun work called with arg on a miss, its return value is stored in the
 * cache and returned
 * \param arg argument passed "as is" to fun
 * \param ttl_ns ttl in nanoseconds to apply to the entry, 0 delegates to
 * lruc_get
 * \return pointer to the value, NULL on failure
 * \ingroup lruc
**/
void *lruc_getttl(t_cm_lruc *c, const void *key, size_t len, t_cm_work fun,
                  void *arg, size_t ttl_ns) {
	t_cm_lruc_keynode *node;
	uint64_t           now;
	void              *v;

	if (!ttl_ns) return (lruc_get(c, key, len, fun, arg));
	if (!lruc_is_valid(c)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (NULL);
	}
	node = lruci_keyinhash(c, key, len, NULL);
	if (node && node->ns_gap != C_MINI__LRUC_TTL_DFLT) {
		now = lruci_timenow();
		if (now == (uint64_t)-1) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LRUC_LOG_ECLOCK)
			return (NULL);
		}
		if (!lruci_expired(node, now)) {
			lruci_lru_push(c, lruci_lru_pop(c, node));
			c->stats.hit++;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_HIT, c->stats.hit, key,
			       len)
			return (node->kv.value);
		}
	}
	if (!fun) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return (NULL);
	}
	v = fun(arg);
	if (!v) return (NULL);
	c->stats.misses++;
	if (!lruc_put_ttl(c, key, len, v, ttl_ns)) return (NULL);
	return (v);
}
