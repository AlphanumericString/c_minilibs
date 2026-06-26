#include <limits.h>
#include <stdlib.h>

#include "least_recently_used_cache.h"

t_lruc *cache_init(t_lruc *cache, size_t cap, void (*dtor)(void *)) {
	cache->h_arr = malloc(sizeof(t_keyCache *) * cap);
	if (!cache->h_arr) return (NULL);
	cache->cap      = cap;
	cache->nb       = 0;
	cache->ttl_glob = LONG_MAX, cache->stats = (cstats_t){0};
	cache->dtor = dtor;
	cache->head = NULL;
	cache->tail = NULL;
	return (cache);
}

t_lruc *cache_create(size_t cap, void (*dtor)(void *)) {
	t_lruc *ret;

	ret = malloc(sizeof(*ret));
	if (!ret) return (NULL);
	if (cache_init(ret, cap, dtor) == NULL) {
		free(ret);
		ret = NULL;
	}
	return (ret);
}
void cache_destroy(t_lruc *cache) {
	if (!cache) return;
	cache_clear(cache);
	free(cache->h_arr);
	cache->h_arr = NULL;
}
