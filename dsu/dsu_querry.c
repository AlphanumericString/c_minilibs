#include "disjointed_set_union.h"

/**
 * \brief check if 2 node have the same ancestor (uses find so compression)
 * \param ds the dsu structure
 * \param x first element
 * \param y second element
 * \return true if related, false otherwise
 **/
bool dsu_is_related(t_dsu *ds, size_t x, size_t y) {
	return (dsu_find(ds, x) == dsu_find(ds, y));
}
/**
 * \brief check if 2 node have the same ancestor (uses strct_find so no
 * compression)
 * \param ds the dsu structure
 * \param x first element
 * \param y second element
 * \return true if related, false otherwise
 **/
bool dsu_strict_is_related(t_dsu ds, size_t x, size_t y) {
	return (dsu_strict_find(ds, x) == dsu_strict_find(ds, y));
}
/**
 * \brief check if dsu is valid and w/out errors
 * \param dsu the dsu structure
 * \return true if valid, false otherwise
 **/
bool dsu_validate(t_dsu dsu) {
	size_t       i;
	size_t       size_div_2;
	unsigned int x;
	size_t       steps;

	if (!dsu.size || !dsu.rank || !dsu.parent) {
		LOG(C_MINI__DSU_LOG_EARG);
		return (false);
	}
	size_div_2 = dsu.size / 2;
	i          = 0;
	steps      = 0;
	while (i < dsu.size) {
		x = i;
		while (x != dsu.parent[x]) {
			if (dsu.parent[i] > dsu.size || dsu.rank[i] > size_div_2) {
				LOG("Invalid DSU (rank/parent for %zu)", i);
				return (false);
			}
			x = dsu.parent[x];
			steps++;
			if (steps > dsu.size) {
				LOG("Invalid DSU (infinite chain)");
				return (false);
			}
		}
		i++;
	}
	LOG(C_MINI__DSU_LOG_SUCCESS);
	return (true);
}
