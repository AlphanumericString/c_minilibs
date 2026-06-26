#include "least_recently_used_cache.h"

size_t cache_size(const t_lruc *cache) {
	return (cache->nb);
}
uint64_t cache_hits(const t_lruc *cache) {
	return (cache->stats.hit);
}
uint64_t cache_misses(const t_lruc *cache) {
	return (cache->stats.misses);
}
uint64_t cache_expired(const t_lruc *cache) {
	return (cache->stats.expired);
}
cstats_t cache_stats(const t_lruc *cache) {
	return (cache->stats);
}
#ifdef C_MINI__TESTS
void	cache_setvalue_dump(t_lruc *c, void (*value_dumper)(void *)){
	c->value_printer = value_dumper;
}
#endif
