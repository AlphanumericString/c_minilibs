#include <string.h>

#include "dynamic_array.h"

/**
 * \brief get element at offset pos respecting the size of elements and copies
 * it at out_elem
 * \param da pointer to the dynamic array
 * \param pos index of the element
 * \param out_elem pointer where the element will be copied
 * \return 1 on success, -1 on failure
 * \ingroup da
**/
int da_get(const t_da *da, size_t pos, void *out_elem) {
	if (!da_is_valid(da) || pos >= da->nb_e || !out_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (-1);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (memcpy(out_elem, da_at(da, pos), da->sz_e), 1);
}
/**
 * \brief get up to n elements starting at offset pos respecting the size of
 * elements and copies them at out_buffer
 * \param da pointer to the dynamic array
 * \param n number of elements to copy
 * \param pos index of the first element
 * \param out_buffer pointer where the elements will be copied
 * \return the number of elements copied, or -1 on failure
 * \ingroup da
**/
int da_nget(const t_da *da, size_t n, size_t pos, void *out_buffer) {
	if (!da_is_valid(da) || pos >= da->nb_e || !out_buffer) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (-1);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	if (!n) return (0);
	if (pos + n > da->nb_e) n = da->nb_e - pos;
	return (memcpy(out_buffer, da_at(da, pos), da->sz_e * n), n);
}

/**
 * \brief get the raw inner array of the da
 * \param da pointer to the dynamic array
 * \return pointer to the raw inner array or null on failure
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
