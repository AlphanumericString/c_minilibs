#include <string.h>

#include "dsu/dsu_convert/dsu_convertions.h"

/**
 * \brief builds a rewindable dsu from a compact dsu
 * \param src the compact dsu to copy infos from
 * \return a newly allocated cdsu or null if any error occured
 * \note fields: 'ranks, parents' must match types between cdsu and rdsu
 * \ingroup cdsu
**/
t_rdsu *rdsu_new_from_cdsu(const t_cdsu *src) {
	t_rdsu *ret;

	if (!src) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	ret = rdsu_new(cdsu_size(src));
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RDSU_LOG_EALLOC);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS);
	return (rdsu_init_from_cdsu(ret, src));
}
/**
 * \brief init a given rewindable dsu from a compact dsu
 * \param to the rewindable dsu to init
 * \param src the compact dsu to get infos from
 * \return the address of the rewindable dsu or null if any error occured
 * \note fields: 'ranks, parents' must match types between cdsu and rdsu
 * \ingroup cdsu
**/
t_rdsu *rdsu_init_from_cdsu(t_rdsu *to, const t_cdsu *src) {
	_Static_assert((sizeof(*to->rank) == sizeof(*src->rank) ||
	                sizeof(*to->parent) == sizeof(*src->parent)),
	               "Internal array sizes must be equal");
	if (!src || !to || to->size < src->size) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	if (to->change_log) rdsu_rewind_clear(to);
	memcpy(to->parent, src->parent, sizeof(*to->parent) * src->size);
	memcpy(to->rank, src->rank, sizeof(*to->rank) * src->size);
	to->components = src->components;
	to->size       = src->size;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS);
	return (to);
}
/**
 * \brief turns a rewindable dsu to a compact dsu
 * \param src address of the rewindable dsu to transform (set to null on
 * success)
 * \return a newly allocated compact dsu or null if any error occured
 * \note fields: 'ranks, parents' must match types between cdsu and rdsu
 * \ingroup cdsu
**/
t_cdsu *rdsu_to_cdsu(t_rdsu **src) {
	t_cdsu *ret;

	if (!src || !rdsu_validate(*src)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	ret = cdsu_new_from_rdsu(*src);
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EALLOC);
		return (NULL);
	}
	rdsu_free(src);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS);
	return (ret);
}
