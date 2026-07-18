#include <limits.h>
#include <stdlib.h>
#include <strings.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Initialize a cache
 * \param cache pointer to the cache
 * \param cap capacity of the cache
 * \param dtor destructor function
 * \return pointer to the cache
 * \ingroup lruc
 **/
t_lruc *lruc_init(t_lruc *cache, size_t cap, void (*dtor)(void *)) {
	if (!cache) {
		LOG(C_MINI__LRUC_LOG_EARG);
		return (cache);
	}
	bzero(cache, sizeof(*cache));
	bzero(&cache->stats, sizeof(cache->stats));
	cache->buckets = calloc(sizeof(t_lruc_keynode *), cap * 2);
	if (!cache->buckets) {
		LOG(C_MINI__LRUC_LOG_EALLOC);
		return (NULL);
	}
	cache->minHeap = cache->buckets + cap;
	bzero(cache->minHeap, sizeof(t_lruc_keynode *) * cap);
	cache->buckets_nb = cap;
	cache->ttl_glob   = (uint64_t)C_MINI__LRUC_TTL_DFLT;
	cache->stats      = (t_lruc_stats){0};
	cache->dtor       = dtor;
	LOG(C_MINI__LRUC_LOG_SUCCESS);
	return (cache);
}

/**
 * \brief Create a new cache
 * \param cap capacity of the cache
 * \param dtor destructor function
 * \return pointer to the new cache
 * \ingroup lruc
 **/
t_lruc *lruc_create(size_t cap, void (*dtor)(void *)) {
	t_lruc *ret;

	ret = malloc(sizeof(*ret));
	if (!ret) {
		LOG(C_MINI__LRUC_LOG_EALLOC);
		return (NULL);
	}
	if (lruc_init(ret, cap, dtor) == NULL) {
		LOG(C_MINI__LRUC_LOG_EALLOC);
		free(ret);
		ret = NULL;
	}
	LOG(C_MINI__LRUC_LOG_SUCCESS);
	return (ret);
}
/**
 * \brief Destroy a cache
 * \param cache pointer to the cache
 * \ingroup lruc
 **/
void lruc_destroy(t_lruc *cache) {
	if (!cache) {
		LOG(C_MINI__LRUC_LOG_EARG);
		return;
	}
	lruc_clear(cache);
	free(cache->buckets);
	cache->buckets_nb = 0;
	cache->buckets    = NULL;
	cache->minHeap    = NULL;
	LOG(C_MINI__LRUC_LOG_SUCCESS);
}
/**
 * \brief Destroy internal and cache itself is freed
 * \param cache the cache to free.
 * \ingroup lruc
 **/
void lruc_free(t_lruc *cache) {
	if (!cache) {
		LOG(C_MINI__LRUC_LOG_EARG);
		return;
	}
	lruc_destroy(cache);
	free(cache);
	LOG(C_MINI__LRUC_LOG_SUCCESS);
}
