#include "disjointed_set_union.h"

bool dsu_is_related(t_dsu *ds, size_t x, size_t y) {
	return (dsu_find(ds, x) == dsu_find(ds, y));
}
bool dsu_strict_is_related(t_dsu ds, size_t x, size_t y) {
	return (dsu_strict_find(ds, x) == dsu_strict_find(ds, y));
}
bool dsu_validate(t_dsu dsu) {
	size_t i;
	size_t size_div_2;
	uint   x;
	size_t steps;

	if (!dsu.size || !dsu.rank || !dsu.parent) return (false);
	size_div_2 = dsu.size / 2;
	i          = 0;
	steps      = 0;

	while (i < dsu.size) {
		x = i;
		while (x != dsu.parent[x]) {
			if (dsu.parent[i] > dsu.size || dsu.rank[i] > size_div_2) {
				return (false);
			}
			x = dsu.parent[x];
			steps++;
			if (steps > dsu.size) {
				return false;
			}
		}
		i++;
	}
	return (true);
}
