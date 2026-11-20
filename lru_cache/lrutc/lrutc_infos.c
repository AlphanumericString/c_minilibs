#include "least_recently_used_timed_cache.h"

/**
 * \brief Check if a cache is valid and usable
 * \param cache cache pointer
 * \return true if the cache is valid, false otherwise
 * \ingroup lrutc
**/
bool lrutc_is_valid(const t_cm_lrutc *cache) {
	return (cache && cache->buckets && cache->minHeap && cache->buckets_nb &&
	        cache->node_nb <= cache->buckets_nb &&
	        (!cache->node_nb || (cache->MRU && cache->LRU)));
}
/**
 * \brief Get the size of the cache
 * \param cache cache pointer
 * \return size of the cache
 * \ingroup lrutc
**/
size_t lrutc_size(const t_cm_lrutc *cache) {
	return (cache->node_nb);
}
/**
 * \brief Get the number of hits
 * \param cache cache pointer
 * \return number of hits
 * \ingroup lrutc
**/
uint64_t lrutc_hits(const t_cm_lrutc *cache) {
	return (cache->stats.hit);
}
/**
 * \brief Get the number of misses
 * \param cache cache pointer
 * \return number of misses
 * \ingroup lrutc
**/
uint64_t lrutc_misses(const t_cm_lrutc *cache) {
	return (cache->stats.misses);
}
/**
 * \brief Get the number of expired elements
 * \param cache cache pointer
 * \return number of expired elements
 * \ingroup lrutc
**/
uint64_t lrutc_expired(const t_cm_lrutc *cache) {
	return (cache->stats.expired);
}
/**
 * \brief Get the statistics of the cache
 * \param cache cache pointer
 * \return statistics of the cache
 * \ingroup lrutc
**/
t_cm_lrutc_stats lrutc_stats(const t_cm_lrutc *cache) {
	return (cache->stats);
}
/**
 * \brief Get the number of reserved nodes
 * \param cache cache pointer
 * \return number of reserved (pre-allocated) nodes
 * \ingroup lrutc
**/
size_t lrutc_reserved_nb(const t_cm_lrutc *cache) {
	return (cache->reserved_nodes_nb);
}
/**
 * \brief Check whether the cache has reserved nodes
 * \param cache cache pointer
 * \return true if at least one reserved node is available, false otherwise
 * \ingroup lrutc
**/
bool lrutc_has_reserved(const t_cm_lrutc *cache) {
	return (cache->reserved_nodes_nb > 0);
}
/**
 * \brief Get the maximum number of nodes that can still be reserved
 * \param cache cache pointer
 * \return maximum number of reservable nodes
 * \ingroup lrutc
**/
size_t lrutc_max_reserve(const t_cm_lrutc *cache) {
	return (cache->buckets_nb - (cache->node_nb + cache->reserved_nodes_nb));
}
