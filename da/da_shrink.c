#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"

/**
 * \brief shrink the capacity of the dynamic array to fit its elements exactly
 * \param da pointer to the dynamic array
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_shrink(t_da *da) {
	bool r;

	if (!da_is_valid(da)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return (false);
	}
	r = da_shrink_min(da, 0);
	if (!r) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
	return (r);
}
/**
 * \brief shrink the capacity of the dynamic array trying to keep a minimum
 * number of free elements
 * \param da pointer to the dynamic array
 * \param keep_n minimum number of extra capacity elements to keep (0 for a
 * 'fit to data')
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_shrink_min(t_da *da, size_t keep_n) {
	void *tmp;

	if (!da_is_valid(da)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return (false);
	}
	if ((da->nb_e + keep_n) >= da->cap_e) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
		return (true);
	}
	tmp = calloc(da->sz_e, (da->nb_e + keep_n));
	if (!tmp) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC);
		return (false);
	}
	memcpy(tmp, da->array, da->nb_e * da->sz_e);
	free(da->array);
	da->array = tmp;
	da->cap_e = da->nb_e + keep_n;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
	return (true);
}
