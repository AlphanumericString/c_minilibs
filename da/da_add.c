#include <string.h>

#include "dynamic_array.h"

/**
 * \brief add an element, if needed grow the da
 * \param da pointer to the dynamic array
 * \param elem pointer to the element to add
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_add(t_da *da, void *elem) {
	if (!elem || !da_is_not_null(da)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if ((da->nb_e + 1) > da->cap_e) da_grow(da);
	da->nb_e++;
	memcpy(da_at(da, da->nb_e - 1), elem, da->sz_e);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
/**
 * \brief add nb_e elements of size sz_e from the array arr, if needed grows the
 * da
 * \param da pointer to the dynamic array
 * \param arr pointer to the array to concatenate
 * \param nb_e number of elements to add
 * \param sz_e size of the elements
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_cat_array(t_da *da, void *arr, size_t nb_e, size_t sz_e) {
	if (!arr || !da_is_not_null(da) || sz_e != da->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if ((da->nb_e + nb_e) > da->cap_e) da_grow_for_size(da, nb_e);
	da->nb_e += nb_e;
	memcpy(da_at(da, da->nb_e - nb_e), arr, da->sz_e * nb_e);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
