#include <string.h>

#include "dynamic_array.h"

/**
 * \brief get element at offset pos respecting the size of elements and copies
 * it at elem
 * \param da pointer to the dynamic array
 * \param pos index of the element
 * \param out_elem pointer where the element will be copied
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_get(const t_da *da, size_t pos, void *out_elem) {
	if (!da_is_valid(da) || pos >= da->nb_e || !out_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (memcpy(out_elem, da_at(da, pos), da->sz_e), true);
}
/**
 * \brief get element at offset pos respecting the size of elements and copies
 * it at elem
 * \param da pointer to the dynamic array
 * \param n number of copied elements
 * \param pos index of the element
 * \param out_buffer pointer where the element will be copied
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_nget(const t_da *da, size_t n, size_t pos, void *out_buffer) {
	if (!da_is_valid(da) || pos >= da->nb_e || !out_buffer) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	if (!n) return (true);
	if (pos + n > da->nb_e) {
		CM_DBG("n updated: n (%zu -> %zu)", n, (size_t)(da->nb_e - pos))
		n = da->nb_e - pos;
	}
	return (memcpy(out_buffer, da_at(da, pos), da->sz_e * n), true);
}

/**
 * \brief get the raw inner array of the da
 * \param da pointer to the dynamic array
 * \return pointer to the raw inner array or null on faillure
 * \ingroup da
**/
void *da_array(const t_da *da) {
	if (!da) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (da->array);
}
