#include "least_recently_used_cache.h"

/**
 * \brief Get the size of the cache
 * \param cache cache pointer
 * \return size of the cache
 * \ingroup lruc
**/
size_t lruc_size(const t_lruc *cache) {
	return (cache->node_in_use);
}
/**
 * \brief Get the number of hits
 * \param cache cache pointer
 * \return number of hits
 * \ingroup lruc
**/
uint64_t lruc_hits(const t_lruc *cache) {
	return (cache->stats.hit);
}
/**
 * \brief Get the number of misses
 * \param cache cache pointer
 * \return number of misses
 * \ingroup lruc
**/
uint64_t lruc_misses(const t_lruc *cache) {
	return (cache->stats.misses);
}
/**
 * \brief Get the number of expired elements
 * \param cache cache pointer
 * \return number of expired elements
 * \ingroup lruc
**/
uint64_t lruc_expired(const t_lruc *cache) {
	return (cache->stats.expired);
}
/**
 * \brief Get the statistics of the cache
 * \param cache cache pointer
 * \return statistics of the cache
 * \ingroup lruc
**/
t_lruc_stats lruc_stats(const t_lruc *cache) {
	return (cache->stats);
}
