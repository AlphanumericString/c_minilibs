#include "dynamic_array.h"

/**
 * \brief get element at offset pos respecting the size of elements and copies
 * it at elem
 * \param vec pointer to the dynamic array
 * \param pos index of the element
 * \param out_buffer pointer where the element will be copied
 * \return true on success, false on failure
**/
bool da_get(t_da *vec, size_t pos, void *out_buffer) {
	if (!vec || pos >= vec->nb_e) return (false);
	if (!out_buffer) return (false);
	return (memcpy(out_buffer, da_at(vec, pos), vec->sz_e), true);
}

/**
 * \brief get element at offset pos respecting the size of elements and copies
 * it at elem
 * \param da pointer to the dynamic array
 * \param n number of copied elements
 * \param pos index of the element
 * \param out_buffer pointer where the element will be copied
 * \return true on success, false on failure
**/
bool  da_nget(t_da *da, size_t n, size_t pos, void *out_buffer)
{
	if (!da || pos >= da->nb_e) return (false);
	if (!out_buffer) return (false);
	if (!n) return (true);
	return (memcpy(out_buffer, da_at(da, pos), da->sz_e * n), true);
}

/**
 * \brief get the raw inner array of the da
 * \param da pointer to the dynamic array
 * \return pointer to the raw inner array
**/
void *da_array(t_da *da) {
	if (!da) return (NULL);
	return (da->array);
}
