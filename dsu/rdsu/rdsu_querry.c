#include "rewindable_disjointed_set_union.h"

/**
 * \brief check if 2 node have the same ancestor
 * \param ds the rdsu structure
 * \param x first element
 * \param y second element
 * \return true if related, false otherwise
 * \ingroup rdsu
**/
bool rdsu_is_related(t_rdsu *ds, size_t x, size_t y) {
	return (rdsu_find(ds, x) == rdsu_find(ds, y));
}
/**
 * \brief check if rdsu is valid and w/out errors
 * \param rdsu the rdsu structure
 * \return true if valid, false otherwise
 * \ingroup rdsu
**/
bool rdsu_validate(t_rdsu rdsu) {
	size_t       i;
	size_t       size_div_2;
	unsigned int x;
	size_t       steps;

	if (!rdsu.size || !rdsu.rank || !rdsu.parent) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (false);
	}
	size_div_2 = rdsu.size / 2;
	i          = 0;
	steps      = 0;
	while (i < rdsu.size) {
		x = i;
		while (x != rdsu.parent[x]) {
			if (rdsu.parent[i] > rdsu.size || rdsu.rank[i] > size_div_2) {
				LOG("Invalid DSU (rank/parent for %zu)", i);
				return (false);
			}
			x = rdsu.parent[x];
			steps++;
			if (steps > rdsu.size) {
				LOG("Invalid DSU (infinite chain)");
				return (false);
			}
		}
		i++;
	}
	LOG(C_MINI__RDSU_LOG_SUCCESS);
	return (true);
}
