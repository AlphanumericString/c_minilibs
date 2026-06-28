#include <string.h>

#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

bool cache_has(const t_lruc *c, const void *key, size_t key_len) {
	size_t          pos;
	t_lruc_keynode *kv;

	if (!c || !c->MRU || !c->nb) return (NULL);
	if ((c->cap / c->nb) >= C_MINI__LRU_HASH_SKIP) { // less than 10% filled
		for (kv = c->MRU; kv; kv = kv->lru_next)
			if (kv->kv.k_len == key_len && !memcmp(kv->kv.key, key, key_len))
				return (true);
		return (false);
	} else {
		kv = cache_search_keyinhash(c, key, key_len, &pos);
		if (!kv || memcmp(c->h_arr[pos]->kv.key, key, key_len) != 0)
			return (false);
		return (true);
	}
}
