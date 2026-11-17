#include "compact_disjointed_set_union.h"

/**
 * \brief returns the max depth of the trees (the longest chain of parents from
 * any element up to its root)
 * \param cdsu the cdsu to query
 * \return the max depth, -1 on bad arguments
 * \ingroup cdsu
**/
int cdsu_max_depth(const t_cm_cdsu *cdsu) {
	unsigned int max = 0;

	if (!cdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return (-1);
	}
	for (size_t i = 0; i < cdsu->size; i++) {
		unsigned int depth = 0;

		for (size_t x = i; cdsu->parent[x] != x; x = cdsu->parent[x])
			depth++;
		if (depth > max) max = depth;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (max);
}
/**
 * \brief returns the number of elems in the tree
 * \param cdsu the cdsu to query
 * \return the number of elems, 0 on bad arguments
 * \ingroup cdsu
**/
size_t cdsu_size(const t_cm_cdsu *cdsu) {
	if (!cdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (cdsu->size);
}
/**
 * \brief return the number of distinct separate sets in the cdsu
 * \param cdsu the cdsu to query
 * \return the number of components, 0 on bad arguments
 * \ingroup cdsu
**/
size_t cdsu_nb_components(const t_cm_cdsu *cdsu) {
	if (!cdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (cdsu->components);
}
