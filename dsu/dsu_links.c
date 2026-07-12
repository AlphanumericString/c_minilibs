#include "disjointed_set_union.h"

/**
 * \brief links 2 elements
 * \param ds the dsu structure
 * \param x first element to link
 * \param y second element to link
 **/
void dsu_union(t_dsu *ds, size_t x, size_t y) {
	size_t root_x;
	size_t root_y;

	if (!ds || y == x || x > ds->size || y > ds->size) {
		LOG(C_MINI__DSU_LOG_EARG);
		return;
	}
	root_x = dsu_find(ds, x);
	root_y = dsu_find(ds, y);
	if (root_x == root_y || root_x == (size_t)-1 || root_y == (size_t)-1) {
		LOG("Link (r1:%zu, r2:%zu, components:%zu -> %zu)", x, y,
		    ds->components, ds->components);
		return;
	}
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
	LOG("Link (r1:%zu, r2:%zu, components:%zu -> %zu)", x, y,
	    ds->components + 1, ds->components);
	return;
}
/**
 * \brief search parent of x and compress parent
 * \param ds the dsu structure
 * \param x the element to find
 * \return the parent of x
 **/
size_t dsu_find(t_dsu *ds, size_t x) {
	if (!ds || x > ds->size) {
		LOG(C_MINI__DSU_LOG_EARG);
		return (-1);
	}
	while (ds->parent[x] != x) {
		ds->parent[x] = ds->parent[ds->parent[x]];
		x             = ds->parent[x];
	}
	LOG(C_MINI__DSU_LOG_SUCCESS);
	return (x);
}
/**
 * \brief ONLY search, no compression
 * \param ds the dsu structure
 * \param x the element to find
 * \return the parent of x
 **/
size_t dsu_strict_find(t_dsu ds, size_t x) {
	if (!ds.size || !ds.parent || !ds.rank || x > ds.size) {
		LOG(C_MINI__DSU_LOG_EARG);
		return (-1);
	}
	while (ds.parent[x] != x) {
		x = ds.parent[x];
	}
	LOG(C_MINI__DSU_LOG_SUCCESS);
	return (x);
}
/**
 * \brief try to compress all elem sequentially
 * \param ds the dsu structure
 * \return the number of elements processed
 **/
size_t dsu_compress_all(t_dsu *ds) {
	size_t x;
	size_t y;

	if (!ds || !ds->size || !ds->parent) {
		LOG(C_MINI__DSU_LOG_EARG);
		return (0);
	}
	y = 0;
	while (y < ds->size) {
		x = y;
		while (ds->parent[x] != x) {
			ds->parent[x] = ds->parent[ds->parent[x]];
			x             = ds->parent[x];
		}
		y++;
	}
	LOG(C_MINI__DSU_LOG_SUCCESS);
	return (ds->size);
}
