#include <stdlib.h>

#include "compact_disjointed_set_union.h"

/**
 * \brief frees internal memory
 * \param ds the cdsu to destroy
 **/
void cdsu_destroy(t_cdsu *ds) {
	if (!ds) {
		LOG(C_MINI__CDSU_LOG_EARG);
		return;
	}
	free(ds->parent);
	if (!ds->rp_single_alloc) free(ds->rank);
	*ds = (t_cdsu){0};
	LOG(C_MINI__CDSU_LOG_SUCCESS);
}
/**
 * \brief reset structure to a clean init state
 * \param cdsu the cdsu to clear
 **/
void cdsu_clr(t_cdsu *cdsu) {
	size_t i;

	if (!cdsu) {
		LOG(C_MINI__CDSU_LOG_EARG);
		return;
	}
	i = 0;
	while (i < cdsu->size) {
		cdsu->parent[i] = i;
		cdsu->rank[i++] = 0;
	}
	LOG("Clear (components:%zu -> %zu)", cdsu->components, cdsu->size);
	cdsu->components = cdsu->size;
}
/**
 * \brief cleans up the structurre and then free's it
 * \param ds the cdsu to clean
 **/
void cdsu_free(t_cdsu *ds) {
	if (!ds) {
		LOG(C_MINI__CDSU_LOG_EARG);
		return;
	}
	cdsu_destroy(ds);
	free(ds);
	LOG(C_MINI__CDSU_LOG_SUCCESS);
}
