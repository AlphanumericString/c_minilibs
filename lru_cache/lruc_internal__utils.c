#include <string.h>
#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

t_lruc_keynode *cache_search_keyinlist(const t_lruc *c, const void *key,
                                       size_t key_len) {
	t_lruc_keynode *node;

	for (node = c->MRU; node; node = node->lru_next)
		if (node->kv.k_len == key_len &&
		    memcmp(key, node->kv.key, key_len) == 0)
			return (node);
	return (NULL);
}

t_lruc_keynode *cache_search_keyinhash(const t_lruc *c, const void *key,
                                       size_t key_len, size_t *_out) {
	size_t i;
	size_t it;

	if (!_out) _out = &it;
	it = hash_djb2(key, key_len) % c->cap;
	for (i = 0; i < c->cap && c->h_arr[it]; it = (it + 1) % c->cap, i++) {
		if (c->h_arr[it]->kv.k_len == key_len &&
		    !memcmp(c->h_arr[it]->kv.key, key, key_len)) {
			return (*_out = it, c->h_arr[it]);
		}
	}
	return (*_out = it, NULL);
}

void cache_unlink_node(t_lruc *c, t_lruc_keynode *n) {
	pop_lruc(c, n);
	pop_ttl(c, n);
}

long distance_ms(struct timespec a, struct timespec b) {
	time_t sec  = a.tv_sec - b.tv_sec;
	long   nsec = a.tv_nsec - b.tv_nsec;

	if (nsec < 0) {
		sec--;
		nsec += 1000000000L;
	}
	return sec * 1000 + nsec / 1000000;
}
