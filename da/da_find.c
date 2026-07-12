#include <stdio.h>
#include <string.h>

#include "dynamic_array.h"

/**
 * \brief find an element matching the condition
 * \param da pointer to the dynamic array
 * \param c callback to test elements
 * \param u_data user data for callback
 * \return pointer to the found element, or NULL if not found
 **/
void *da_find(t_da *da, t_comparator c, void *u_data) {
	if (!da || !c) {
		LOG(C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	for (size_t i = 0; i < da->nb_e; i++)
		if (c(da_at(da, i), u_data) == 0) {
			LOG(C_MINI__DA_LOG_FIND, i, da_at(da, i));
			return (da_at(da, i));
		}
	LOG(C_MINI__DA_LOG_FINDFAIL);
	return (NULL);
}

/**
 * \brief find an element matching the condition within the first n elements
 * \param da pointer to the dynamic array
 * \param n number of elements to search
 * \param c callback to test elements
 * \param u_data user data for callback
 * \return pointer to the found element, or NULL if not found
 **/
void *da_nfind(t_da *da, size_t n, t_comparator c, void *u_data) {
	if (!da || !c) {
		LOG(C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	if (n > da->nb_e) n = da->nb_e;
	for (size_t i = 0; i < n; i++)
		if (c(da_at(da, i), u_data) == 0) {
			LOG(C_MINI__DA_LOG_FIND, i, da_at(da, i));
			return (da_at(da, i));
		}
	LOG(C_MINI__DA_LOG_FINDFAIL);
	return (NULL);
}

/**
 * \brief find an element matching the condition and copy it to a buffer
 * \param da pointer to the dynamic array
 * \param c callback to test elements
 * \param u_data user data for callback
 * \param out_buffer buffer to copy the found element to
 * \return true if found and copied, false otherwise
 **/
bool da_find_get(t_da *da, t_comparator c, void *u_data, void *out_buffer) {
	void *elem;

	if (!da || !c || !out_buffer) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	elem = da_find(da, c, u_data);
	if (elem) {
		memcpy(out_buffer, elem, da->sz_e);
		LOG(C_MINI__DA_LOG_FINDGET, elem, out_buffer);
		return (true);
	}
	LOG(C_MINI__DA_LOG_GETFAIL);
	return (false);
}
