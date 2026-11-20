#include <stddef.h>

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
bool lruc_has(const t_cm_lruc *c, const void *key, size_t key_len) {
	if (!lruc_is_valid(c) || !c->node_nb) return (false);
	return (lruci_keyinhash(c, key, key_len, NULL) != NULL);
}
