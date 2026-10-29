#include "rewindable_disjointed_set_union.h"

/**
 * \brief check if 2 node have the same ancestor
 * \param ds the rdsu structure
 * \param x first element
 * \param y second element
 * \return true if related, false otherwise
 * \ingroup rdsu
**/
bool rdsu_is_related(const t_rdsu *ds, size_t x, size_t y) {
	if (!ds || (x > y ? x : y) > ds->size) return (false);
	return (rdsu_find(ds, x) == rdsu_find(ds, y));
}
/**
 * \brief check if rdsu is valid and w/out errors
 * \param rdsu the rdsu structure
 * \return true if valid, false otherwise
 * \ingroup rdsu
**/
bool rdsu_validate(const t_rdsu *rdsu) {

	if (!rdsu || !rdsu->size || !rdsu->rank || !rdsu->parent) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (false);
	}
	for (size_t i = 0; i < rdsu->size; i++) {
		if (rdsu->parent[i] >= rdsu->size || rdsu->rank[i] >= rdsu->size) {
			CM_LOG(C_MINI__LEVEL_INFO, "Invalid DSU (rank/parent for %zu)", i)
			return (false);
		}
		for (size_t x = i, steps = 0; x != rdsu->parent[x];
		     x = rdsu->parent[x]) {
			steps++;
			if (rdsu->parent[x] >= rdsu->size) {
				CM_LOG(C_MINI__LEVEL_INFO, "Invalid DSU (rank/parent for %zu)",
				       i)
				return (false);
			}
			if (steps > rdsu->size + 1) {
				CM_LOG(C_MINI__LEVEL_WARN, "Invalid DSU (infinite chain)")
				return (false);
			}
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (true);
}
