#include <string.h>

#define C_MINI__LRU_UTILS
#include "least_recently_used_cache.h"

bool cache_has(const t_lruc *c, const void *key, size_t key_len)
{
	size_t	pos;
	t_keyCache *kv;

	if (!c || !c->head || !c->nb) return (NULL);
	kv = cache_search_keyinhash(c, key, key_len, &pos);
	if (!kv || memcmp(c->h_arr[pos]->key, key, key_len) != 0)
		return (false);
	return (true);
}
