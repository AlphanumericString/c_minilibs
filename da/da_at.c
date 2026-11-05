#include "dynamic_array.h"

/**
 * \brief get element at offset pos respecting the size of elements
 * \param da pointer to the dynamic array
 * \param pos index of the element
 * \return pointer to the element at the specified position
 * \ingroup da
**/
void *da_at(const t_cm_da *da, size_t pos) {
	if (!da_is_valid(da) || pos >= da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return ((char *)da->array + da->sz_e * pos);
}
