#include <stddef.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * \brief prepare an iterator handle for a cache
 * \param it iterator handle, caller owned
 * \param c cache to walk
 * \param promote_to_mru true to promote every yielded entry (forward only)
 * \ingroup lruc
**/
void lruc_iter_init(t_cm_lruc_iter *it, t_cm_lruc *c, bool promote_to_mru) {
	if (!it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return;
	}
	*it = (t_cm_lruc_iter){
	    .cache          = c,
	    .cur            = NULL,
	    .promote_to_mru = promote_to_mru,
	    .done           = false,
	};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_SUCCESS)
}
/**
 * \brief yield the next entry from most recently used to least recently used
 * \param it iterator handle initialized by lruc_iter_init
 * \param kv set to the yielded key-value pair, NULL once the walk is done
 * \return true while an entry is available, false once the walk is done or on
 * invalid arguments
 * \ingroup lruc
**/
bool lruc_iter(t_cm_lruc_iter *it, t_cm_keyvalue **kv) {
	t_cm_lruc_keynode *next;

	if (!it || !kv || !lruc_is_valid(it->cache)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		if (kv) *kv = NULL;
		return (false);
	}
	if (it->done || !it->cache->node_nb) {
		*kv = NULL;
		return (false);
	}
	if (!it->cur) {
		it->cur = it->cache->MRU;
	} else {
		// read the successor before promoting: promoting rewires the links of
		// the current node
		next = it->cur->lru_next;
		if (it->promote_to_mru)
			lruci_lru_push(it->cache, lruci_lru_pop(it->cache, it->cur));
		it->cur = next;
	}
	if (!it->cur) {
		it->done = true;
		*kv      = NULL;
		return (false);
	}
	*kv = &it->cur->kv;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_SUCCESS)
	return (true);
}
/**
 * \brief yield the next entry from least recently used to most recently used
 * \param it iterator handle initialized by lruc_iter_init
 * \param kv set to the yielded key-value pair, NULL once the walk is done
 * \return true while an entry is available, false once the walk is done or on
 * invalid arguments
 * \note never promotes, even when the handle was created with promote_to_mru
 * \ingroup lruc
**/
bool lruc_iter_rev(t_cm_lruc_iter *it, t_cm_keyvalue **kv) {
	if (!it || !kv || !lruc_is_valid(it->cache)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		if (kv) *kv = NULL;
		return (false);
	}
	if (it->done || !it->cache->node_nb) {
		*kv = NULL;
		return (false);
	}
	if (!it->cur) it->cur = it->cache->LRU;
	else it->cur = it->cur->lru_prev;
	if (!it->cur) {
		it->done = true;
		*kv      = NULL;
		return (false);
	}
	*kv = &it->cur->kv;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_SUCCESS)
	return (true);
}
/**
 * \brief rewind an iterator handle, the next call yields the first entry of the
 * walked direction again
 * \param it iterator handle
 * \ingroup lruc
**/
void lruc_iter_reset(t_cm_lruc_iter *it) {
	if (!it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LRUC_LOG_EARG)
		return;
	}
	it->cur  = NULL;
	it->done = false;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LRUC_LOG_SUCCESS)
}
