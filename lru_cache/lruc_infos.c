#include "least_recently_used_cache.h"

/**
 * @brief Get the size of the cache
 * @param cache cache pointer
 * @return size of the cache
 **/
size_t cache_size(const t_lruc *cache) {
	return (cache->nb);
}
/**
 * @brief Get the number of hits
 * @param cache cache pointer
 * @return number of hits
 **/
uint64_t cache_hits(const t_lruc *cache) {
	return (cache->stats.hit);
}
/**
 * @brief Get the number of misses
 * @param cache cache pointer
 * @return number of misses
 **/
uint64_t cache_misses(const t_lruc *cache) {
	return (cache->stats.misses);
}
/**
 * @brief Get the number of expired elements
 * @param cache cache pointer
 * @return number of expired elements
 **/
uint64_t cache_expired(const t_lruc *cache) {
	return (cache->stats.expired);
}
/**
 * @brief Get the statistics of the cache
 * @param cache cache pointer
 * @return statistics of the cache
 **/
t_lruc_stats cache_stats(const t_lruc *cache) {
	return (cache->stats);
}
