#include <stdlib.h>

#include "compact_disjointed_set_union.h"

/**
 * \brief frees internal memory
 * \param ds the cdsu to destroy
 * \ingroup cdsu
**/
void cdsu_destroy(t_cm_cdsu *ds) {
	if (!ds) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return;
	}
	free(ds->parent);
	if (!ds->rp_single_alloc) free(ds->rank);
	*ds = (t_cm_cdsu){0};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
}
/**
 * \brief reset structure to a clean init state
 * \param cdsu the cdsu to clear
 * \ingroup cdsu
**/
bool cdsu_clr(t_cm_cdsu *cdsu) {
	size_t i;

	if (!cdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return (false);
	}
	if (cdsu->components == cdsu->size) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
		return (true);
	}
	i = 0;
	while (i < cdsu->size) {
		cdsu->parent[i] = i;
		cdsu->rank[i++] = 0;
	}
	CM_LOG(C_MINI__LEVEL_INFO, "Clear (components:%zu -> %zu)",
	       cdsu->components, cdsu->size)
	cdsu->components = cdsu->size;
	return (true);
}
/**
 * \brief cleans up the structurre and then free's it
 * \param ds the cdsu to clean
 * \ingroup cdsu
**/
void cdsu_free(t_cm_cdsu **ds) {
	if (!ds || !*ds) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return;
	}
	cdsu_destroy(*ds);
	free(*ds);
	*ds = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
}
