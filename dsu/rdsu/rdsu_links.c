#define C_MINI__RDSU_INTERNAL
#include "rewindable_disjointed_set_union.h"

/**
 * \brief links 2 elements
 * \param ds the rdsu structure
 * \param x first element to link
 * \param y second element to link
 * \ingroup rdsu
 **/
void rdsu_union(t_rdsu *ds, size_t x, size_t y) {
	uint16_t root_x;
	uint16_t root_y;
	bool     dbg;

	if (!ds || y == x || x > ds->size || y > ds->size) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return;
	}
	root_x = rdsu_find(ds, x);
	root_y = rdsu_find(ds, y);
	if (root_x == root_y || root_x == (uint16_t)-1 || root_y == (uint16_t)-1) {
		LOG("Link (r1:%zu, r2:%zu, components:%zu -> %zu)", x, y,
		    ds->components, ds->components);
		return;
	}
	ds->components--;
	if (ds->rank[root_x] < ds->rank[root_y]) {
		dbg = rdsui_push_rewind(ds, root_x, ds->parent[root_x], (uint16_t)0,
		                        RDSU_LOG_PARENT);
		ds->parent[root_x] = root_y;
	} else if (ds->rank[root_x] > ds->rank[root_y]) {
		dbg = rdsui_push_rewind(ds, root_y, ds->parent[root_y], (uint16_t)0,
		                        RDSU_LOG_PARENT);
		ds->parent[root_y] = root_x;
	} else {
		if (x < y) {
			dbg = rdsui_push_rewind(ds, root_y, ds->parent[root_y],
			                        (uint16_t)root_x, RDSU_LOG_RANK);
			ds->parent[root_y] = root_x;
			ds->rank[root_x]++;
		} else {
			dbg = rdsui_push_rewind(ds, root_x, ds->parent[root_x],
			                        (uint16_t)root_y, RDSU_LOG_RANK);
			ds->parent[root_x] = root_y;
			ds->rank[root_y]++;
		}
	}
	DBG("rdsui_push : %s", dbg ? "true" : "false");
	LOG("Link (r1:%zu, r2:%zu, components:%zu -> %zu)", x, y,
	    ds->components + 1, ds->components);
	return;
}
/**
 * \brief search parent of x
 * \param ds the rdsu structure
 * \param x the element to find
 * \return the parent of x
 * \ingroup rdsu
 **/
uint16_t rdsu_find(t_rdsu *ds, size_t x) {
	if (!ds->size || !ds->parent || !ds->rank || x > ds->size) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (-1);
	}
	while (ds->parent[x] != x) {
		x = ds->parent[x];
	}
	LOG(C_MINI__RDSU_LOG_SUCCESS);
	return (x);
}
/**
 * \brief try to compress all elem sequentially
 * \param ds the rdsu structure
 * \return the number of elements processed
 * \note invalidates all rdsu log entries
 * \ingroup rdsu
 **/
size_t rdsu_compress_all(t_rdsu *ds) {
	size_t x;
	size_t y;

	if (!ds || !ds->size || !ds->parent) {
		LOG(C_MINI__RDSU_LOG_EARG);
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
	rdsu_rewind_clear(ds);
	LOG(C_MINI__RDSU_LOG_SUCCESS);
	return (ds->size);
}
