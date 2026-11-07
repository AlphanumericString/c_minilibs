#include "compact_disjointed_set_union.h"

/**
 * \brief returns the max depth of the tree
 * \param cdsu the cdsu to query
 * \return the max depth
 * \ingroup cdsu
**/
int cdsu_max_depth(const t_cm_cdsu *cdsu) {
	unsigned int max = 0;
	size_t       i   = 0;

	while (i < cdsu->size) {
		if (cdsu->rank[i++] > max) max = cdsu->rank[i - 1];
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (max);
}
/**
 * \brief returns the number of elems in the tree
 * \param cdsu the cdsu to query
 * \return the number of elems
 * \ingroup cdsu
**/
size_t cdsu_size(const t_cm_cdsu *cdsu) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (cdsu->size);
}
/**
 * \brief return the number of distinct separate sets in the cdsu
 * \param cdsu the cdsu to query
 * \return the number of components
 * \ingroup cdsu
**/
size_t cdsu_nb_components(const t_cm_cdsu *cdsu) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (cdsu->components);
}
