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
		LOG(C_MINI__LRU_LOG_EARG);
		return (cache);
	}
	bzero(cache, sizeof(*cache));
	cache->buckets = calloc(sizeof(t_lruc_keynode *), cap);
	if (!cache->buckets) {
		LOG(C_MINI__LRU_LOG_EALLOC);
		return (NULL);
	}
	cache->buckets_nb      = cap;
	cache->ttl_glob = LONG_MAX, cache->stats = (t_lruc_stats){0};
	cache->dtor = dtor;
	LOG(C_MINI__LRU_LOG_SUCCESS);
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
		LOG(C_MINI__LRU_LOG_EALLOC);
		return (NULL);
	}
	if (cache_init(ret, cap, dtor) == NULL) {
		LOG(C_MINI__LRU_LOG_EALLOC);
		free(ret);
		ret = NULL;
	}
	LOG(C_MINI__LRU_LOG_SUCCESS);
	return (ret);
}
/**
 * @brief Destroy a cache
 * @param cache pointer to the cache
 **/
void cache_destroy(t_lruc *cache) {
	if (!cache) {
		LOG(C_MINI__LRU_LOG_EARG);
		return;
	}
	cache_clear(cache);
	free(cache->buckets);
	cache->buckets_nb   = 0;
	cache->buckets = NULL;
	LOG(C_MINI__LRU_LOG_SUCCESS);
}
/**
 * @brief Destroy internal and cache itself is freed
 * @param cache the cache to free.
**/
void cache_free(t_lruc *cache) {
	if (!cache) {
		LOG(C_MINI__LRU_LOG_EARG);
		return;
	}
	cache_destroy(cache);
	free(cache);
	LOG(C_MINI__LRU_LOG_SUCCESS);
}
