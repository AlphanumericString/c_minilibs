#include "disjointed_set_union.h"

/**
 * \brief links 2 elements
 * \param ds the dsu structure
 * \param x first element to link
 * \param y second element to link
 */
void dsu_union(t_dsu *ds, size_t x, size_t y) {
	size_t root_x;
	size_t root_y;

	if (!ds || y == x) return;
	if (x > ds->size || y > ds->size) return;
	root_x = dsu_find(ds, x);
	root_y = dsu_find(ds, y);
	if (root_x == root_y || root_x == (size_t)-1 || root_y == (size_t)-1)
		return;
	ds->components--;
	if (ds->rank[root_x] < ds->rank[root_y]) {
		ds->parent[x] = root_y;
	} else if (ds->rank[root_x] > ds->rank[root_y]) {
		ds->parent[y] = root_x;
	} else {
		if (x < y) {
			ds->parent[y] = root_x;
			ds->rank[root_x]++;
		} else {
			ds->parent[x] = root_y;
			ds->rank[root_y]++;
		}
	}
	return;
}
/**
 * \brief search parent of x and compress parent
 * \param ds the dsu structure
 * \param x the element to find
 * \return the parent of x
 */
size_t dsu_find(t_dsu *ds, size_t x) {
	if (!ds) return (-1);
	if (x > ds->size) return (-1);
	while (ds->parent[x] != x) {
		ds->parent[x] = ds->parent[ds->parent[x]];
		x             = ds->parent[x];
	}
	return (x);
}
/**
 * \brief ONLY search, no compression
 * \param ds the dsu structure
 * \param x the element to find
 * \return the parent of x
 */
size_t dsu_strict_find(t_dsu ds, size_t x) {
	if (!ds.size || !ds.parent || !ds.rank) return (-1);
	if (x > ds.size) return (-1);
	while (ds.parent[x] != x) {
		x = ds.parent[x];
	}
	return (x);
}
/**
 * \brief try to compress all elem sequentially
 * \param ds the dsu structure
 * \return the number of elements processed
 */
size_t dsu_compress_all(t_dsu *ds) {
	size_t x;
	size_t y;

	if (!ds || !ds->size || !ds->parent) return (0);

	y = 0;
	while (y < ds->size) {
		x = y;
		while (ds->parent[x] != x) {
			ds->parent[x] = ds->parent[ds->parent[x]];
			x             = ds->parent[x];
		}
		y++;
	}
	return (ds->size);
}
