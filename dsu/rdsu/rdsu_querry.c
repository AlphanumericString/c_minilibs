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
bool rdsu_validate(t_rdsu rdsu) {

	if (!rdsu.size || !rdsu.rank || !rdsu.parent) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (false);
	}
	for (size_t i = 0; i < rdsu.size; i++) {
		if (rdsu.parent[i] >= rdsu.size || rdsu.rank[i] >= rdsu.size) {
			LOG("Invalid RDSU (rank/parent for %zu)", i);
			return (false);
		}
		for (size_t x = i, steps = 0; x != rdsu.parent[x]; x = rdsu.parent[x]) {
			steps++;
			if (rdsu.parent[x] >= rdsu.size) {
				LOG("Invalid DSU (rank/parent for %zu)", x);
				return (false);
			}
			if (steps > rdsu.size + 1) {
				LOG("Invalid RDSU (infinite chain)");
				return (false);
			}
		}
	}
	LOG(C_MINI__RDSU_LOG_SUCCESS);
	return (true);
}
