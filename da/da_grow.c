#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"

/**
 * \brief grow the capacity of the dynamic array
 * \param da pointer to the dynamic array
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_grow(t_da *da) {
	size_t new_size;
	void  *tmp;

	if (!da) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	new_size = da->cap_e * C_MINI_DA__GROWTH;
	if (!new_size) new_size = C_MINI_DA__NB_ELEM_DFLT;
	tmp = malloc(da->sz_e * new_size);
	if (!tmp) {
		LOG(C_MINI__DA_LOG_EALLOC);
		return (false);
	}
	if (da->array) {
		memcpy(tmp, da->array, da->sz_e * da->nb_e);
		free(da->array);
	}
	LOG(C_MINI__DA_LOG_GROWTH, da->array, tmp, da->cap_e, new_size);
	da->array = tmp;
	da->cap_e = new_size;
	return (true);
}

/**
 * \brief grow the capacity of the dynamic array to ensure a minimum number of
 * free elements
 * \param da pointer to the dynamic array
 * \param minimal_free_elems minimum number of free elements to guarantee
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_grow_for_size(t_da *da, size_t minimal_free_elems) {
	size_t new_size;
	void  *tmp;

	if (!da) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	new_size = da->cap_e;
	if (!new_size) new_size = C_MINI_DA__NB_ELEM_DFLT;
	while ((new_size - da->nb_e) < minimal_free_elems)
		new_size *= C_MINI_DA__GROWTH;
	tmp = malloc(da->sz_e * new_size);
	if (!tmp) {
		LOG(C_MINI__DA_LOG_EALLOC);
		return (false);
	}
	if (da->array) {
		memcpy(tmp, da->array, da->nb_e * da->sz_e);
		free(da->array);
	}
	LOG(C_MINI__DA_LOG_GROWTH, da->array, tmp, da->cap_e, new_size);
	da->array = tmp;
	da->cap_e = new_size;
	return (true);
}
