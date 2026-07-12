#include <limits.h>
#include <stdlib.h>
#include <strings.h>

#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

/**
 * @brief Initialize a cache
 * @param cache pointer to the cache
 * @param cap capacity of the cache
 * @param dtor destructor function
 * @return pointer to the cache
 **/
t_lruc *cache_init(t_lruc *cache, size_t cap, void (*dtor)(void *)) {
	if (!cache) {
		LOG("Fail : No Cache given");
		return (cache);
	}
	bzero(cache, sizeof(*cache));
	cache->h_arr = calloc(sizeof(t_lruc_keynode *), cap);
	if (!cache->h_arr) {
		LOG("Allocation error");
		return (NULL);
	}
	cache->cap      = cap;
	cache->ttl_glob = LONG_MAX, cache->stats = (t_lruc_stats){0};
	cache->dtor = dtor;
	LOG("Success");
	return (cache);
}

/**
 * @brief Create a new cache
 * @param cap capacity of the cache
 * @param dtor destructor function
 * @return pointer to the new cache
 **/
t_lruc *cache_create(size_t cap, void (*dtor)(void *)) {
	t_lruc *ret;

	ret = malloc(sizeof(*ret));
	if (!ret) {
		LOG("allocation error (main)");
		return (NULL);
	}
	if (cache_init(ret, cap, dtor) == NULL) {
		LOG("allocation error (hash)");
		free(ret);
		ret = NULL;
	}
	LOG("Success");
	return (ret);
}
/**
 * @brief Destroy a cache
 * @param cache pointer to the cache
 **/
void cache_destroy(t_lruc *cache) {
	if (!cache) return;
	cache_clear(cache);
	free(cache->h_arr);
	cache->cap   = 0;
	cache->h_arr = NULL;
	LOG("Success");
}
