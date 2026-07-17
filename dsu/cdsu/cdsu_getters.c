#include "compact_disjointed_set_union.h"

/**
 * \brief returns the max depth of the tree
 * \param cdsu the cdsu to query
 * \return the max depth
 **/
int cdsu_max_depth(t_cdsu cdsu) {
	unsigned int max = 0;
	size_t       i   = 0;

	while (i < cdsu.size) {
		if (cdsu.rank[i++] > max) max = cdsu.rank[i - 1];
	}
	LOG(C_MINI__CDSU_LOG_SUCCESS);
	return (max);
}
/**
 * \brief returns the number of elems in the tree
 * \param cdsu the cdsu to query
 * \return the number of elems
 **/
size_t cdsu_size(t_cdsu cdsu) {
	LOG(C_MINI__CDSU_LOG_SUCCESS);
	return (cdsu.size);
}
/**
 * \brief return the number of distinc separate sets in the cdsu
 * \param cdsu the cdsu to query
 * \return the number of components
 **/
size_t cdsu_nb_components(t_cdsu cdsu) {
	LOG(C_MINI__CDSU_LOG_SUCCESS);
	return (cdsu.components);
}
