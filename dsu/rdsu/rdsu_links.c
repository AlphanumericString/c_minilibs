#include <stdlib.h>
#define C_MINI__RDSU_INTERNAL
#include "rewindable_disjointed_set_union.h"

// -- local func

/*
 * push a log
 */
static inline bool rdsui_push_rewind(t_cm_rdsu *rdsu, t_cm_rdsu_log *entry) {
	entry->nxt       = rdsu->change_log;
	rdsu->change_log = entry;
	return (true);
}
/*
 * init a log
 */
static inline t_cm_rdsu_log rdsui_init_log(size_t elem, uint16_t value,
                                           uint16_t                rank,
                                           enum s_cm_rdsu_log_type type) {
	return ((t_cm_rdsu_log){
	    .type = type, .elem = elem, .p = value, .r = rank, .nxt = NULL});
}

/*
 * get a log from the reserved pile or create a new one
 */
static inline t_cm_rdsu_log *rdsui_get_log(t_cm_rdsu *rdsu) {
	t_cm_rdsu_log *ret;

	if (rdsu->reserved_logs) {
		ret                 = rdsu->reserved_logs;
		rdsu->reserved_logs = ret->nxt;
		ret->nxt            = NULL;
		return (ret);
	}
	return ((t_cm_rdsu_log *)calloc(sizeof(t_cm_rdsu_log), 1));
}
// ----------

/**
 * \brief links 2 elements
 * \param ds the rdsu structure
 * \param x first element to link
 * \param y second element to link
 * \ingroup rdsu
**/
bool rdsu_union(t_cm_rdsu *ds, size_t x, size_t y) {
	t_cm_rdsu_log *entry;
	uint16_t       root_x;
	uint16_t       root_y;

	if (!ds || !ds->rank || !ds->parent || !ds->size || y == x ||
	    x >= ds->size || y >= ds->size) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (false);
	}
	root_x = rdsu_find(ds, x);
	root_y = rdsu_find(ds, y);
	if (root_x == root_y) {
		// INFO: should be true(already linked) but no log emmited so no rewind
		// possible on action, so false
		CM_LOG(C_MINI__LEVEL_INFO,
		       "Link (r1:%zu, r2:%zu, components:%zu -> %zu)", x, y,
		       ds->components, ds->components)
		return (false);
	}
	entry = rdsui_get_log(ds);
	if (!entry) return (false);
	ds->components--;
	if (ds->rank[root_x] < ds->rank[root_y]) {
		*entry = rdsui_init_log(root_x, ds->parent[root_x], (uint16_t)0,
		                        RDSU_LOG_NAYRANK);
		ds->parent[root_x] = root_y;
	} else if (ds->rank[root_x] > ds->rank[root_y]) {
		*entry = rdsui_init_log(root_y, ds->parent[root_y], (uint16_t)0,
		                        RDSU_LOG_NAYRANK);
		ds->parent[root_y] = root_x;
	} else {
		if (x < y) {
			*entry             = rdsui_init_log(root_y, ds->parent[root_y],
			                                    (uint16_t)root_x, RDSU_LOG_YESRANK);
			ds->parent[root_y] = root_x;
			ds->rank[root_x]++;
		} else {
			*entry             = rdsui_init_log(root_x, ds->parent[root_x],
			                                    (uint16_t)root_y, RDSU_LOG_YESRANK);
			ds->parent[root_x] = root_y;
			ds->rank[root_y]++;
		}
	}
	rdsui_push_rewind(ds, entry);
	CM_LOG(C_MINI__LEVEL_INFO, "Link (r1:%zu, r2:%zu, components:%zu -> %zu)",
	       x, y, ds->components + 1, ds->components)
	return (true);
}
/**
 * \brief search parent of x
 * \param ds the rdsu structure
 * \param x the element to find
 * \return the parent of x
 * \ingroup rdsu
**/
uint16_t rdsu_find(const t_cm_rdsu *ds, size_t x) {
	if (!ds || !ds->size || !ds->parent || !ds->rank || x > ds->size) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (-1);
	}
	while (ds->parent[x] != x) {
		x = ds->parent[x];
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (x);
}
/**
 * \brief try to compress all elem sequentially
 * \param ds the rdsu structure
 * \return the number of elements processed
 * \note invalidates all rdsu log entries
 * \ingroup rdsu
**/
size_t rdsu_compress_all(t_cm_rdsu *ds) {

	if (!ds || !ds->size || !ds->parent) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (0);
	}
	for (size_t y = 0; y < ds->size; y++) {
		for (size_t x = y; ds->parent[x] != x;) {
			ds->parent[x] = ds->parent[ds->parent[x]];
			x             = ds->parent[x];
		}
	}
	rdsu_rewind_clear(ds);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (ds->size);
}
