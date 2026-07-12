#include <stdlib.h>

#include "disjointed_set_union.h"

/**
 * \brief frees internal memory
 * \param ds the dsu to destroy
 **/
void dsu_destroy(t_dsu *ds) {
	if (!ds) {
		LOG(C_MINI__DSU_LOG_EARG);
		return;
	}
	free(ds->parent);
	if (!ds->rp_single_alloc) free(ds->rank);
	*ds = (t_dsu){0};
	LOG(C_MINI__DSU_LOG_SUCCESS);
}
/**
 * \brief reset structure to a clean init state
 * \param dsu the dsu to clear
 **/
void dsu_clr(t_dsu *dsu) {
	size_t i;

	if (!dsu) {
		LOG(C_MINI__DSU_LOG_EARG);
		return;
	}
	i = 0;
	while (i < dsu->size) {
		dsu->parent[i] = i;
		dsu->rank[i++] = 0;
	}
	LOG("Clear (components:%zu -> %zu)", dsu->components, dsu->size);
	dsu->components = dsu->size;
}
/**
 * \brief cleans up the structurre and then free's it
 * \param ds the dsu to clean
 **/
void dsu_free(t_dsu *ds) {
	if (!ds) {
		LOG(C_MINI__DSU_LOG_EARG);
		return;
	}
	dsu_destroy(ds);
	free(ds);
	LOG(C_MINI__DSU_LOG_SUCCESS);
}
