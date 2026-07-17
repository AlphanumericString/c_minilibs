#include <stdlib.h>

#include "rewindable_disjointed_set_union.h"

/**
 * \brief frees internal memory
 * \param ds the rdsu to destroy
 **/
void rdsu_destroy(t_rdsu *ds) {
	if (!ds) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return;
	}
	free(ds->parent);
	if (!ds->rp_single_alloc) free(ds->rank);
	if (ds->change_log) rdsu_rewind_clear(ds);
	*ds = (t_rdsu){0};
	LOG(C_MINI__RDSU_LOG_SUCCESS);
}
/**
 * \brief reset structure to a clean init state
 * \param rdsu the rdsu to clear
 **/
void rdsu_clr(t_rdsu *rdsu) {
	size_t i;

	if (!rdsu) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return;
	}
	i = 0;
	while (i < rdsu->size) {
		rdsu->parent[i] = i;
		rdsu->rank[i++] = 0;
	}
	rdsu_rewind_clear(rdsu);
	LOG("Clear (components:%zu -> %zu)", rdsu->components, rdsu->size);
	rdsu->components = rdsu->size;
}
/**
 * \brief cleans up the structurre and then free's it
 * \param ds the rdsu to clean
 **/
void rdsu_free(t_rdsu *ds) {
	if (!ds) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return;
	}
	rdsu_destroy(ds);
	free(ds);
	LOG(C_MINI__RDSU_LOG_SUCCESS);
}
