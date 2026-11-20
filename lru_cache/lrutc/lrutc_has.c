#include <string.h>

#define C_MINI__LRUTC_INTERNAL
#include "least_recently_used_timed_cache.h"

/**
 * \brief Check if a key exists in the cache
 * \param c cache pointer
 * \param key key to check
 * \param key_len key length
 * \return true if key exists, false otherwise
 * \ingroup lrutc
**/
bool lrutc_has(const t_cm_lrutc *c, const void *key, size_t key_len) {
	size_t                   pos;
	const t_cm_lrutc_keynode *kv;

	if (!lrutc_is_valid(c) || !c->node_nb) return (false);
	kv = lrutci_keyinhash(c, key, key_len, &pos);
	if (!kv) return (false);
	return (true);
}

/**
 * \brief Check if a key exists and is not expired in the cache
 * \param c cache pointer
 * \param key key to check
 * \param key_len key length
 * \return true if the key exists and is not expired, false otherwise
 * \ingroup lrutc
**/
bool lrutc_has_ttl(const t_cm_lrutc *c, const void *key, size_t key_len) {
	const t_cm_lrutc_keynode *node;
	uint64_t                 now;

	if (!lrutc_is_valid(c) || !c->node_nb) return (false);
	node = lrutci_keyinhash(c, key, key_len, NULL);
	if (!node) return (false);
	if (node->ns_gap == C_MINI__LRUTC_TTL_DFLT) return (true);
	now = lrutci_timenow();
	if (now == (uint64_t)-1) return (false);
	return (!lrutci_expired(node, now));
}
