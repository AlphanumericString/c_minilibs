#include "disjointed_set_union.h"

/**
 * \brief frees internal memory
 * \param ds the dsu to destroy
**/
void dsu_destroy(t_dsu *ds) {
	if (!ds) return;
	free(ds->parent);
	if (!ds->rp_single_alloc) free(ds->rank);
	*ds = (t_dsu){0};
}
/**
 * \brief reset structure to a clean init state
 * \param dsu the dsu to clear
**/
void dsu_clr(t_dsu *dsu) {
	size_t i;

	if (!dsu) return;
	i = 0;
	while (i < dsu->size) {
		dsu->parent[i] = i;
		dsu->rank[i++] = 0;
	}
	dsu->components = dsu->size;
}
