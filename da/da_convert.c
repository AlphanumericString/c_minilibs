#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"

/**
 * \brief convert to array (doesn't free da)
 * \param da pointer to the dynamic array
 * \return newly allocated array with elements, or NULL on failure
 * \note Warning, the data array is NOT the array of the da itself but a shallow
 * copy. *YOU* need to free it.
 * \ingroup da
**/
void *da_to_array(const t_da *da) {
	void *data;

	if (!da_is_valid(da)) {
		LOG(C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	data = malloc(da->sz_e * da->nb_e);
	if (!data) {
		LOG(C_MINI__DA_LOG_EALLOC);
		return (NULL);
	}
	da_to_loc_array(da, data);
	LOG(C_MINI__DA_LOG_CONV, data);
	return (data);
}
/**
 * \brief convert to a given array (doesn't allocate array) (doesn't free da)
 * \param da pointer to the dynamic array
 * \param arr target array where elements will be copied
 * \return pointer to the target array (arr), or NULL on failure
 * \ingroup da
**/
void *da_to_loc_array(const t_da *da, void *arr) {
	if (!da_is_valid(da) || !arr) {
		LOG(C_MINI__DA_LOG_EARG);
		return (NULL);
	};
	memcpy(arr, da->array, da->sz_e * da->nb_e);
	LOG(C_MINI__DA_LOG_CONV, arr);
	return (arr);
}
