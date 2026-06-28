#include <stddef.h>

#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

void cache_foreach(t_lruc *c, t_kvcallback fn, void *user_data) {
	size_t total;

	total = 0;
	if (!c || !fn) return;
	for (size_t i = 0; total++ < c->cap; i = (i + 1) % c->cap)
		if (c->h_arr[i]) fn(&c->h_arr[i]->kv, user_data);
}
void cache_foreach_ru_ordered(t_lruc *c, t_kvcallback fn, void *user_data) {
	for (t_lruc_keynode *it = c->MRU; it; it = it->lru_next)
		fn(&it->kv, user_data);
}
void cache_foreach_ttl_ordered(t_lruc *c, t_kvcallback fn, void *user_data) {
	for (t_lruc_keynode *it = c->TTL_Max; it; it = it->lru_next)
		fn(&it->kv, user_data);
}
