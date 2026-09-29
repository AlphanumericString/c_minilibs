#include <stdio.h>
#include <string.h>

#include "dynamic_array.h"

/**
 * \brief find an element matching the condition
 * \param da pointer to the dynamic array
 * \param c callback to test elements
 * \param data user data for callback
 * \return pointer to the found element, or NULL if not found
 * \ingroup da
**/
void *da_find(t_da *da, t_comparator c, void *data) {
	if (!da_is_valid(da) || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	for (size_t i = 0; i < da->nb_e; i++)
		if (c(da_at(da, i), data) == 0) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
			return (da_at(da, i));
		}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_FINDFAIL);
	return (NULL);
}
/**
 * \brief find an element matching the condition within the first n elements
 * \param da pointer to the dynamic array
 * \param n number of elements to search
 * \param c callback to test elements
 * \param data user data for callback
 * \return pointer to the found element, or NULL if not found
 * \ingroup da
**/
void *da_nfind(t_da *da, size_t n, t_comparator c, void *data) {
	if (!da_is_valid(da) || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	if (n > da->nb_e) n = da->nb_e;
	for (size_t i = 0; i < n; i++)
		if (c(da_at(da, i), data) == 0) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
			return (da_at(da, i));
		}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_FINDFAIL);
	return (NULL);
}
/**
 * \brief find an element matching the condition and copy it to a buffer
 * \param da pointer to the dynamic array
 * \param c callback to test elements
 * \param data user data for callback
 * \param out_buffer buffer to copy the found element to
 * \return true if found and copied, false otherwise
 * \ingroup da
**/
bool da_find_get(t_da *da, t_comparator c, void *data, void *out_buffer) {
	void *elem;

	if (!da_is_valid(da) || !c || !out_buffer) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return (false);
	}
	elem = da_find(da, c, data);
	if (elem) {
		memcpy(out_buffer, elem, da->sz_e);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
		return (true);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_GETFAIL);
	return (false);
}
