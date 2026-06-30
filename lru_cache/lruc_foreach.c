#include <stddef.h>

#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

/**
 * @brief Iterate over the cache
 * @param c cache pointer
 * @param fn callback function
 * @param user_data user data
 **/
void cache_foreach(const t_lruc *c, t_kvcallback fn, void *user_data) {
	size_t total;

	total = 0;
	if (!c || !fn) return;
	for (size_t i = 0; total++ < c->cap; i = (i + 1) % c->cap)
		if (c->h_arr[i]) fn(&c->h_arr[i]->kv, user_data);
}
/**
 * @brief Iterate over the cache in LRU order
 * @param c cache pointer
 * @param fn callback function
 * @param user_data user data
 **/
void cache_foreach_ru_ordered(const t_lruc *c, t_kvcallback fn, void *user_data) {
	for (t_lruc_keynode *it = c->MRU; it; it = it->lru_next)
		fn(&it->kv, user_data);
}
/**
 * @brief Iterate over the cache in TTL order
 * @param c cache pointer
 * @param fn callback function
 * @param user_data user data
 **/
void cache_foreach_ttl_ordered(const t_lruc *c, t_kvcallback fn, void *user_data) {
	for (t_lruc_keynode *it = c->TTL_Max; it; it = it->ttl_next)
		fn(&it->kv, user_data);
}
