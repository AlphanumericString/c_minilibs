#include <string.h>

#include "heap.h"

/**
 * \brief Duplicate a heap, including its capacity and current elements
 * \param src pointer to the source heap
 * \return the duplicated heap or NULL on failure
 * \note the returned heap owns its storage, free it with hp_free
 * \ingroup hp
**/
t_cm_hp *hp_dup(const t_cm_hp *src) {
	t_cm_hp *dst;

	if (!hp_is_valid(src) || !src->capacity) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	dst = hp_create(src->capacity, src->sz_e, src->cmp);
	if (!dst) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__HP_LOG_EALLOC)
		return (NULL);
	}
	if (src->nb) memcpy(dst->elems, src->elems, src->nb * src->sz_e);
	dst->nb = src->nb;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (dst);
}

/**
 * \brief Duplicate the first n elements of a heap
 * \param src pointer to the source heap
 * \param n number of elements to duplicate
 * \return the duplicated heap or NULL on failure
 * \note n is clamped to the source element count; a prefix of a heap is still
 * a valid heap. The returned heap owns its storage, free it with hp_free.
 * \ingroup hp
**/
t_cm_hp *hp_ndup(const t_cm_hp *src, size_t n) {
	t_cm_hp *dst;

	if (!hp_is_valid(src) || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	if (n > src->nb) n = src->nb;
	if (!n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	dst = hp_create(n, src->sz_e, src->cmp);
	if (!dst) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__HP_LOG_EALLOC)
		return (NULL);
	}
	memcpy(dst->elems, src->elems, n * src->sz_e);
	dst->nb = n;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (dst);
}
