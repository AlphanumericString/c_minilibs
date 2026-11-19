#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"

/**
 * \brief map every element of the dynamic array through a function and store
 * the results in a new dynamic array
 * \param da pointer to the dynamic array
 * \param fn function transforming an element, must return a pointer readable
 * for out_elem_size bytes (NULL aborts the mapping)
 * \param out_elem_size size of one element of the resulting array
 * \return the new dynamic array, or a zeroed one on failure
 * \note the returned array owns its memory, destroy it with da_destroy
 * \ingroup da
**/
t_cm_da da_map(const t_cm_da *da, t_cm_mapper fn, size_t out_elem_size) {
	t_cm_da r;

	if (!da_is_valid(da) || !fn || !out_elem_size) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return ((t_cm_da){0});
	}
	r = da_nmap(da, (size_t)-1, fn, out_elem_size);
	if (r.array) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	} else {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
	}
	return (r);
}
/**
 * \brief map the first n elements of the dynamic array through a function and
 * store the results in a new dynamic array
 * \param da pointer to the dynamic array
 * \param n number of elements to map
 * \param fn function transforming an element, must return a pointer readable
 * for out_elem_size bytes (NULL aborts the mapping)
 * \param out_elem_size size of one element of the resulting array
 * \return the new dynamic array, or a zeroed one on failure
 * \note the returned array owns its memory, destroy it with da_destroy
 * \ingroup da
**/
t_cm_da da_nmap(const t_cm_da *da, size_t n, t_cm_mapper fn,
                size_t out_elem_size) {
	t_cm_da ret = {.array = NULL, .nb_e = 0, .cap_e = 0, .sz_e = 0};
	void   *mapped;
	size_t  i;

	if (!da_is_valid(da) || !fn || !out_elem_size || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return ((t_cm_da){0});
	}
	if (n > da->nb_e) n = da->nb_e;
	if (!da_ninit(&ret, n ? n : C_MINI__DA_NB_ELEM_DFLT, out_elem_size))
		return ((t_cm_da){0});
	for (i = 0; i < n; i++) {
		mapped = fn(da_at(da, i));
		if (!mapped) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
			da_destroy(&ret);
			return ((t_cm_da){0});
		}
		memcpy((char *)ret.array + (i * out_elem_size), mapped, out_elem_size);
	}
	ret.nb_e = n;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (ret);
}
