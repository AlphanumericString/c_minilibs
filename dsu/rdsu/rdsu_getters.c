#include "rewindable_disjointed_set_union.h"

/**
 * \brief returns the max depth of the tree
 * \param rdsu the rdsu to query
 * \return the max depth
 * \ingroup rdsu
**/
int rdsu_max_depth(const t_rdsu *rdsu) {
	unsigned int max = 0;
	size_t       i   = 0;

	while (i < rdsu->size) {
		if (rdsu->rank[i++] > max) max = rdsu->rank[i - 1];
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (max);
}
/**
 * \brief returns the number of elems in the tree
 * \param rdsu the rdsu to query
 * \return the number of elems
 * \ingroup rdsu
**/
size_t rdsu_size(const t_rdsu *rdsu) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (rdsu->size);
}
/**
 * \brief return the number of distinc separate sets in the rdsu
 * \param rdsu the rdsu to query
 * \return the number of components
 * \ingroup rdsu
**/
size_t rdsu_nb_components(const t_rdsu *rdsu) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (rdsu->components);
}
