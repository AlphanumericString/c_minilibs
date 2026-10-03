#include <string.h>

#include "dynamic_array.h"

/**
 * \brief set element at offset pos respecting the size of elements
 * \param vec pointer to the dynamic array
 * \param pos index where the element should be set
 * \param elem pointer to the value of element to set
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_set(t_da *da, size_t pos, void *elem) {
	if (!da_is_valid(da) || !elem || pos >= da->nb_e) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	memcpy(da->array + da->sz_e * pos, elem, da->sz_e);
	LOG(C_MINI__DA_LOG_SET, pos, 1UL, elem);
	return (true);
}
/**
 * \brief set element at offset pos respecting the size of elements
 * \param da pointer to the dynamic array
 * \param n number of elements to update
 * \param pos index where the element should be set
 * \param elems pointer to the value of elements to set
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_nset(t_da *da, size_t n, size_t pos, void *elems) {
	if (!da_is_valid(da) || (pos + n) >= da->nb_e || !elems) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	if (!n) {
		LOG(C_MINI__DA_LOG_SUCCESS);
		return (true);
	}
	memcpy(da_at(da, pos), elems, da->sz_e * n);
	LOG(C_MINI__DA_LOG_SET, pos, n, elems);
	return (true);
}
