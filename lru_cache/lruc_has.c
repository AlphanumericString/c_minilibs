#include <string.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief Check if a key exists in the cache
 * \param c cache pointer
 * \param key key to check
 * \param key_len key length
 * \return true if key exists, false otherwise
 * \ingroup lruc
**/
bool lruc_has(const t_lruc *c, const void *key, size_t key_len) {
	size_t          pos;
	t_lruc_keynode *kv;

	if (!lruc_is_valid(c) || !c->node_nb) return (NULL);
	kv = lruci_keyinhash(c, key, key_len, &pos);
	if (!kv) return (false);
	return (true);
}
