#include "disjointed_set_union.h"

/**
 * \brief returns the max depth of the tree
 * \param dsu the dsu to query
 * \return the max depth
**/
int dsu_max_depth(t_dsu dsu) {
	unsigned int max = 0;
	size_t       i   = 0;

	while (i < dsu.size) {
		if (dsu.rank[i++] > max) max = dsu.rank[i - 1];
	}
	return (max);
}
/**
 * \brief returns the number of elems in the tree
 * \param dsu the dsu to query
 * \return the number of elems
**/
size_t dsu_size(t_dsu dsu) {
	return (dsu.size);
}
/**
 * \brief return the number of distinc separate sets in the dsu
 * \param dsu the dsu to query
 * \return the number of components
**/
size_t dsu_nb_components(t_dsu dsu) {
	return (dsu.components);
}
