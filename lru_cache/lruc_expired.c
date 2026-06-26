#include <assert.h>
#include <bits/types/struct_timeval.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "least_recently_used_cache.h"

static inline long distance_ms(struct timeval a, struct timeval b)
{
	return ((a.tv_sec - b.tv_sec) * 1000 + (a.tv_usec - b.tv_usec) / 1000);
}

// expired
// expired-> is key in the cache and expired?
bool cache_isexpired(t_lruc *c, const void *key, size_t key_len) {
	size_t         offset;
	t_keyCache    *kv;
	struct timeval now;

	if (c->ttl_glob == (long)-1) return (false);

	offset = hash_djb2(key, key_len);
	for (; offset < c->nb && c->h_arr[offset]; offset = (offset + 1) % c->nb) {
		kv = c->h_arr[offset];
		if (!memcmp(kv->key, key, MAX(kv->k_len, key_len))) break;
	}
	if (!kv || memcmp(kv->key, key, MAX(kv->k_len, key_len)) != 0)
		return (false);
	gettimeofday(&now, NULL);
	return (distance_ms(kv->time, now) >= c->ttl_glob);
}

// cleanup -> call dtor on all expired elements
void cache_cleanup(t_lruc *c) {
	t_keyCache *node; // aliasses node - c->h_arr[pos]
	t_keyCache *t;    // aliasses t - c->tail
	t_keyCache *prev;
	size_t      pos;
	struct timeval now;

	gettimeofday(&now, NULL);
	while (c->tail && distance_ms(c->tail->time, now) >= c->ttl_glob) {
		t = c->tail;
		if (c->dtor) c->dtor(t->value);

		pos  = hash_djb2(t->key, t->k_len);
		node = c->h_arr[pos];
		for (; node &&
		       (node->k_len != t->k_len || memcmp(node->key, t->key, t->k_len));
		     node = c->h_arr[pos])
			pos = (pos + 1) % c->nb;

#ifdef C_MINI__TESTS
		assert(node);
		assert(node->k_len == t->k_len);
		assert(memcmp(node->key, t->key, node->k_len) == 0);
		assert(c->h_arr[pos] == node);
#endif

		c->h_arr[pos] = NULL;

		prev = t->prev;
		free(t);
		c->tail = prev;

		c->stats.expired++;
	}
}
void cache_setttl(t_lruc *cache, long ttl) {
	cache->ttl_glob = ttl;
}
