#include "dynamic_array.h"

/**
 * \brief get element at offset pos respecting the size of elements
 * \param vec pointer to the dynamic array
 * \param pos index of the element
 * \return pointer to the element at the specified position
 * \ingroup da
**/
void *da_at(t_da *vec, size_t pos) {
	if (!vec || pos >= vec->nb_e) {
		LOG(C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	//	LOG(C_MINI__DA_LOG_SUCCESS); too spammy
	return ((char *)vec->array + vec->sz_e * pos);
}
