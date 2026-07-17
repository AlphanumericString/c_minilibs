#include "compact_disjointed_set_union.h"

/**
 * \brief check if 2 node have the same ancestor (uses find so compression)
 * \param ds the cdsu structure
 * \param x first element
 * \param y second element
 * \return true if related, false otherwise
 **/
bool cdsu_is_related(t_cdsu *ds, size_t x, size_t y) {
	return (cdsu_find(ds, x) == cdsu_find(ds, y));
}
/**
 * \brief check if 2 node have the same ancestor (uses strct_find so no
 * compression)
 * \param ds the cdsu structure
 * \param x first element
 * \param y second element
 * \return true if related, false otherwise
 **/
bool cdsu_strict_is_related(t_cdsu ds, size_t x, size_t y) {
	return (cdsu_strict_find(ds, x) == cdsu_strict_find(ds, y));
}
/**
 * \brief check if cdsu is valid and w/out errors
 * \param cdsu the cdsu structure
 * \return true if valid, false otherwise
 **/
bool cdsu_validate(t_cdsu cdsu) {
	size_t       i;
	size_t       size_div_2;
	unsigned int x;
	size_t       steps;

	if (!cdsu.size || !cdsu.rank || !cdsu.parent) {
		LOG(C_MINI__CDSU_LOG_EARG);
		return (false);
	}
	size_div_2 = cdsu.size / 2;
	i          = 0;
	steps      = 0;
	while (i < cdsu.size) {
		x = i;
		while (x != cdsu.parent[x]) {
			if (cdsu.parent[i] > cdsu.size || cdsu.rank[i] > size_div_2) {
				LOG("Invalid DSU (rank/parent for %zu)", i);
				return (false);
			}
			x = cdsu.parent[x];
			steps++;
			if (steps > cdsu.size) {
				LOG("Invalid DSU (infinite chain)");
				return (false);
			}
		}
		i++;
	}
	LOG(C_MINI__CDSU_LOG_SUCCESS);
	return (true);
}
