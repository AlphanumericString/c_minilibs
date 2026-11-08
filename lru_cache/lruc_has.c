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
bool lruc_has(const t_cm_lruc *c, const void *key, size_t key_len) {
	size_t                   pos;
	const t_cm_lruc_keynode *kv;

	if (!lruc_is_valid(c) || !c->node_nb) return (NULL);
	kv = lruci_keyinhash(c, key, key_len, &pos);
	if (!kv) return (false);
	return (true);
}

/**
 * \brief Check if a key exists and is not expired in the cache
 * \param c cache pointer
 * \param key key to check
 * \param key_len key length
 * \return true if the key exists and is not expired, false otherwise
 * \ingroup lruc
**/
bool lruc_has_ttl(const t_cm_lruc *c, const void *key, size_t key_len) {
	const t_cm_lruc_keynode *node;
	uint64_t                 now;

	if (!lruc_is_valid(c) || !c->node_nb) return (false);
	node = lruci_keyinhash(c, key, key_len, NULL);
	if (!node) return (false);
	if (node->ns_gap == C_MINI__LRUC_TTL_DFLT) return (true);
	now = lruci_timenow();
	if (now == (uint64_t)-1) return (false);
	return (!lruci_expired(node, now));
}
