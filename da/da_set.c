#include "dynamic_array.h"

/**
 * \brief set element at offset pos respecting the size of elements
 * \param vec pointer to the dynamic array
 * \param pos index where the element should be set
 * \param elem pointer to the value of element to set
 * \return true on success, false on failure
 **/
bool da_set(t_da *vec, size_t pos, void *elem) {
	if (!vec || pos >= vec->nb_e) return (false);
	return (memcpy(vec->array + vec->sz_e * pos, elem, vec->sz_e), true);
}

/**
 * \brief set element at offset pos respecting the size of elements
 * \param da pointer to the dynamic array
 * \param n number of elements to update
 * \param pos index where the element should be set
 * \param elems pointer to the value of elements to set
 * \return true on success, false on failure
 **/
bool da_nset(t_da *da, size_t n, size_t pos, void *elems) {
	if (!da || (pos + n) >= da->nb_e) return (false);
	if (!n) return (true);
	return (memcpy(da_at(da, pos), elems, da->sz_e * n), true);
}
