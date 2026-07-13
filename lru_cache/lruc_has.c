#include <string.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * @brief Check if a key exists in the cache
 * @param c cache pointer
 * @param key key to check
 * @param key_len key length
 * @return true if key exists, false otherwise
 **/
bool lruc_has(const t_lruc *c, const void *key, size_t key_len) {
	size_t          pos;
	t_lruc_keynode *kv;

	if (!c || !c->MRU || !c->node_in_use) return (NULL);
	kv = lruc_i_search_keyinhash(c, key, key_len, &pos);
	if (!kv ||
	    lruc_i_is_key_eq(kv->kv.key, kv->kv.k_len, key, key_len) == false)
		return (false);
	return (true);
}
