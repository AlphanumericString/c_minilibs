#include <stddef.h>
#include "least_recently_used_cache.h"

void cache_foreach(t_lruc *c, cache_iter_fn fn, void *user_data) {
	if (!c || !fn) return ;
	for (size_t i = 0; i < c->nb; fn(*c->h_arr[i++], user_data));
}
