#include "least_recently_used_cache.h"

size_t cache_size(t_lruc *cache) {
	return (cache->nb);
}
uint64_t cache_hits(t_lruc *cache) {
	return (cache->stats.hit);
}
uint64_t cache_misses(t_lruc *cache) {
	return (cache->stats.misses);
}
uint64_t cache_expired(t_lruc *cache) {
	return (cache->stats.expired);
}
cstats_t cache_stats(t_lruc *cache) {
	return (cache->stats);
}
