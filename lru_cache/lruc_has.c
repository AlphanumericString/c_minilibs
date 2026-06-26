#include <string.h>
#include "least_recently_used_cache.h"

bool cache_has(t_lruc *c, const void *key, size_t key_len)
{
	size_t	pos;

	if (!c || !c->head || !c->nb) return (NULL);
	pos = hash_djb2(key, key_len);
	for (; pos < c->nb && c->h_arr[pos]; pos = (pos + 1) % c->nb) {
		if (!memcmp(c->h_arr[pos]->key, key,
		            MAX(c->h_arr[pos]->k_len, key_len)))
			break;
	}
	if (!c->h_arr[pos] || memcmp(c->h_arr[pos]->key, key, key_len) != 0)
		return (false);
	return (true);
}
