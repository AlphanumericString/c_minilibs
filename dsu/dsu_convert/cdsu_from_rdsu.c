#include <string.h>

#include "dsu/dsu_convert/dsu_convertions.h"

/**
 * \brief builds a compact dsu from a rewindable dsu
 * \param src the rewindable dsu to copy info from
 * \return a newly allocated cdsu or null if any error occurred
 * \note fields: 'ranks, parents' must match types between cdsu and rdsu
 * \ingroup cdsu
**/
t_cm_cdsu *cdsu_new_from_rdsu(const t_cm_rdsu *src) {
	t_cm_cdsu *ret;

	if (!src) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	ret = cdsu_new(rdsu_size(src));
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RDSU_LOG_EALLOC);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS);
	return (cdsu_init_from_rdsu(ret, src));
}
/**
 * \brief init a given compact dsu from a rewindable dsu
 * \param to the compact dsu to init
 * \param src the rewindable dsu to get info from
 * \return the address of the compact dsu or null if any error occurred
 * \note fields: 'ranks, parents' must match types between cdsu and rdsu
 * \ingroup cdsu
**/
t_cm_cdsu *cdsu_init_from_rdsu(t_cm_cdsu *to, const t_cm_rdsu *src) {
	_Static_assert((sizeof(*to->rank) == sizeof(*src->rank) ||
	                sizeof(*to->parent) == sizeof(*src->parent)),
	               "Internal array sizes must be equal");
	if (!src || !to || to->size < src->size) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	memcpy(to->parent, src->parent, sizeof(*to->parent) * src->size);
	memcpy(to->rank, src->rank, sizeof(*to->rank) * src->size);
	to->components = src->components;
	to->size       = src->size;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS);
	return (to);
}
/**
 * \brief turns a compact dsu to a rewindable dsu
 * \param src address of the compact dsu to transform (set to null on success)
 * \return a newly allocated rewindable dsu or null if any error occurred
 * \note fields: 'ranks, parents' must match types between cdsu and rdsu
 * \ingroup cdsu
**/
t_cm_rdsu *cdsu_to_rdsu(t_cm_cdsu **src) {
	t_cm_rdsu *ret;

	if (!src || !cdsu_validate(*src)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	ret = rdsu_new_from_cdsu(*src);
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EALLOC);
		return (NULL);
	}
	cdsu_free(src);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS);
	return (ret);
}
