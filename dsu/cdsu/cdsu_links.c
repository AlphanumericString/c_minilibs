#include "compact_disjointed_set_union.h"

/**
 * \brief links 2 elements
 * \param ds the cdsu structure
 * \param x first element to link
 * \param y second element to link
 * \ingroup cdsu
**/
bool cdsu_union(t_cdsu *ds, size_t x, size_t y) {
	size_t root_x;
	size_t root_y;

	if (!ds || !ds->rank || !ds->parent || !ds->size || y == x ||
	    x >= ds->size || y >= ds->size) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return (false);
	}
	root_x = cdsu_find(ds, x);
	root_y = cdsu_find(ds, y);
	if (root_x == root_y) {
		CM_LOG(C_MINI__LEVEL_INFO,
		       "Link (r1:%zu, r2:%zu, components:%zu -> %zu)", x, y,
		       ds->components, ds->components)
		return (true);
	}
	ds->components--;
	if (ds->rank[root_x] < ds->rank[root_y]) {
		ds->parent[root_x] = root_y;
	} else if (ds->rank[root_x] > ds->rank[root_y]) {
		ds->parent[root_y] = root_x;
	} else {
		if (x < y) {
			ds->parent[root_y] = root_x;
			ds->rank[root_x]++;
		} else {
			ds->parent[root_x] = root_y;
			ds->rank[root_y]++;
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, "Link (r1:%zu, r2:%zu, components:%zu -> %zu)",
	       x, y, ds->components + 1, ds->components)
	return (true);
}
/**
 * \brief search parent of x and compress parent
 * \param ds the cdsu structure
 * \param x the element to find
 * \return the parent of x
 * \ingroup cdsu
**/
size_t cdsu_find(t_cdsu *ds, size_t x) {
	if (!ds || !ds->size || !ds->parent || x > ds->size) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return (-1);
	}
	while (ds->parent[x] != x) {
		ds->parent[x] = ds->parent[ds->parent[x]];
		x             = ds->parent[x];
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (x);
}
/**
 * \brief ONLY search, no compression
 * \param ds the cdsu structure
 * \param x the element to find
 * \return the parent of x
 * \ingroup cdsu
**/
size_t cdsu_strict_find(const t_cdsu *ds, size_t x) {
	if (!ds->size || !ds->parent || x > ds->size) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return (-1);
	}
	while (ds->parent[x] != x) {
		x = ds->parent[x];
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (x);
}
/**
 * \brief try to compress all elem sequentially
 * \param ds the cdsu structure
 * \return the number of elements processed
 * \ingroup cdsu
**/
size_t cdsu_compress_all(t_cdsu *ds) {
	size_t x;
	size_t y;

	if (!ds || !ds->size || !ds->parent) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
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
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
	return (ds->size);
}
