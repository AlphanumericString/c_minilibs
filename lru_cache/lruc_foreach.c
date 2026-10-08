#include <stddef.h>

#define C_MINI__INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Iterate over the cache
 * \param c cache pointer
 * \param fn callback function
 * \param user_data user data
 * \ingroup lruc
**/
void lruc_foreach(const t_lruc *c, t_kvcallback fn, void *user_data) {
	if (!c || !fn) {
		LOG(C_MINI__LRUC_LOG_EARG);
		return;
	}
	for (size_t i = 0; i < c->buckets_nb; i++)
		for (t_lruc_keynode *n = c->buckets[i]; n; n = n->hash_next)
			fn(&c->buckets[i]->kv, user_data);
}
/**
 * \brief Iterate over the cache in LRU order
 * \param c cache pointer
 * \param fn callback function
 * \param user_data user data
 * \ingroup lruc
**/
void lruc_foreach_ru_ordered(const t_lruc *c, t_kvcallback fn,
                             void *user_data) {
	if (!c || !fn) {
		LOG(C_MINI__LRUC_LOG_EARG);
		return;
	}
	for (t_lruc_keynode *it = c->MRU; it; it = it->lru_next)
		fn(&it->kv, user_data);
}
/**
 * \brief Iterate over the cache in TTL order
 * \param c cache pointer
 * \param fn callback function
 * \param user_data user data
 * \ingroup lruc
**/
void lruc_foreach_ttl_ordered(const t_lruc *c, t_kvcallback fn,
                              void *user_data) {
	size_t i;
	if (!c || !fn) {
		LOG(C_MINI__LRUC_LOG_EARG);
		return;
	}
	i = 0;
	for (i = 0; i < c->node_nb; i++)
		fn(&(c->minHeap[i]->kv), user_data);
}
