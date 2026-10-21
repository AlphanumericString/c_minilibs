#include "compact_disjointed_set_union.h"

/**
 * \brief check if 2 node have the same ancestor (uses find so compression)
 * \param ds the cdsu structure
 * \param x first element
 * \param y second element
 * \return true if related, false otherwise
 * \ingroup cdsu
**/
bool cdsu_is_related(t_cdsu *ds, size_t x, size_t y) {
	if (!ds || (x > y ? x : y) > ds->size) return (false);
	return (cdsu_find(ds, x) == cdsu_find(ds, y));
}
/**
 * \brief check if 2 node have the same ancestor (uses strct_find so no
 * compression)
 * \param ds the cdsu structure
 * \param x first element
 * \param y second element
 * \return true if related, false otherwise
 * \ingroup cdsu
**/
bool cdsu_strict_is_related(t_cdsu ds, size_t x, size_t y) {
	if ((x > y ? x : y) > ds.size) return (false);
	return (cdsu_strict_find(ds, x) == cdsu_strict_find(ds, y));
}
/**
 * \brief check if cdsu is valid and w/out errors
 * \param cdsu the cdsu structure
 * \return true if valid, false otherwise
 * \ingroup cdsu
**/
bool cdsu_validate(t_cdsu cdsu) {
	if (!cdsu.size || !cdsu.rank || !cdsu.parent) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return (false);
	}
	for (size_t i = 0; i < cdsu.size; i++) {
		if (cdsu.parent[i] >= cdsu.size || cdsu.rank[i] >= cdsu.size) {
			CM_LOG(C_MINI__LEVEL_WARN, "Invalid DSU (rank/parent for %zu)", i)
			return (false);
		}
		for (size_t x = i, steps = 0; x != cdsu.parent[x]; x = cdsu.parent[x]) {
			steps++;
			if (cdsu.parent[x] >= cdsu.size) {
				CM_LOG(C_MINI__LEVEL_WARN, "Invalid DSU (rank/parent for %zu)",
				       x)
				return (false);
			}
			if (steps > cdsu.size + 1) {
				CM_LOG(C_MINI__LEVEL_WARN, "Invalid DSU (infinite chain)")
				return (false);
			}
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (true);
}
