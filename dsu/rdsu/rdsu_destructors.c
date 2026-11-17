#include <stdlib.h>

#include "rewindable_disjointed_set_union.h"

/**
 * \brief frees internal memory
 * \param ds the rdsu to destroy
 * \ingroup rdsu
**/
void rdsu_destroy(t_cm_rdsu *ds) {
	if (!ds) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return;
	}
	if (ds->parent) free(ds->parent);
	if (!ds->rp_single_alloc && ds->rank) free(ds->rank);
	if (ds->change_log || ds->reserved_logs) rdsu_rewind_clear(ds);
	*ds = (t_cm_rdsu){0};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
}
/**
 * \brief reset structure to a clean init state
 * \param rdsu the rdsu to clear
 * \return true if the rdsu was cleared, false on bad arguments
 * \note the arrays are always reset: the constructors rely on this to
 * initialize freshly malloc'd buffers
 * \ingroup rdsu
**/
bool rdsu_clr(t_cm_rdsu *rdsu) {
	size_t i;

	if (!rdsu || !rdsu->rank || !rdsu->parent) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (false);
	}
	i = 0;
	while (i < rdsu->size) {
		rdsu->parent[i] = i;
		rdsu->rank[i++] = 0;
	}
	rdsu_rewind_clear(rdsu);
	CM_LOG(C_MINI__LEVEL_INFO, "Clear (components:%zu -> %zu)",
	       rdsu->components, rdsu->size)
	rdsu->components = rdsu->size;
	return (true);
}
/**
 * \brief cleans up the structure and then frees it
 * \param ds the rdsu to clean
 * \ingroup rdsu
**/
void rdsu_free(t_cm_rdsu **ds) {
	if (!ds) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return;
	}
	rdsu_destroy(*ds);
	free(*ds);
	*ds = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
}
