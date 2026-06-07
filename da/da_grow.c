#include "dynamic_array.h"

bool da_grow(t_da *da) {
	size_t new_size;
	void  *tmp;

	if (!da) return (false);
	new_size = da->cap_e * 1.5;
	if (!new_size) new_size = PVEC_NB_ELEM_DFLT;
	tmp = malloc(da->sz_e * new_size);
	if (!tmp) return (false);
	if (da->array) {
		memcpy(tmp, da->array, da->sz_e * new_size);
		free(da->array);
	}
	da->array = tmp;
	da->cap_e = new_size;
	return (true);
}

bool da_grow_for_size(t_da *da, size_t minimal_free_elems) {
	size_t new_size;
	void  *tmp;

	if (!da) return (false);
	new_size = da->cap_e;
	if (!new_size) new_size = PVEC_NB_ELEM_DFLT;
	while ((new_size - da->nb_e) < minimal_free_elems)
		new_size *= 1.5;
	tmp = malloc(da->sz_e * new_size);
	if (!tmp) return (false);
	if (da->array) {
		memcpy(tmp, da->array, da->nb_e * da->sz_e);
		free(da->array);
	}
	da->array  = tmp;
	da->cap_e = new_size;
	return (true);
}
