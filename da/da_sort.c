#include <stdlib.h>

#include "dynamic_array.h"

/**
 * \brief sort the elements of the dynamic array using a comparator function
 * \param da pointer to the dynamic array
 * \param c comparator function
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_sort(t_cm_da *da, t_cm_comparator c) {
	if (!da_is_valid(da) || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	da_nsort(da, -1, c);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
/**
 * \brief sort the first n elements of the dynamic array using a comparator
 * function
 * \param da pointer to the dynamic array
 * \param n number of elements to sort
 * \param c comparator function
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_nsort(t_cm_da *da, size_t n, t_cm_comparator c) {
	if (!da_is_valid(da) || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (n > da->nb_e) n = da->nb_e;
	if (n < 2) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (true);
	}
	qsort(da->array, n, da->sz_e, c);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
