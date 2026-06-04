#include "./dynamic_array.h"

/**
 * \brief get element at offset pos respecting the size of elements
 * \param vec pointer to the dynamic array
 * \param pos index of the element
 * \return pointer to the element at the specified position
 */
void *da_at(t_da *vec, size_t pos) {
	if (!vec || pos > vec->nb_e) return (NULL);
	return ((char *)vec->array + vec->sz_e * pos);
}

/**
 * \brief set element at offset pos respecting the size of elements
 * \param vec pointer to the dynamic array
 * \param pos index where the element should be set
 * \param elem pointer to the element to set
 * \return true on success, false on failure
 */
bool da_set(t_da *vec, size_t pos, void *elem) {
	if (!vec || pos >= vec->nb_e) return (false);
	return (memcpy(vec->array + vec->sz_e * pos, elem, vec->sz_e), true);
}

/**
 * \brief get element at offset pos respecting the size of elements and copies it at elem
 * \param vec pointer to the dynamic array
 * \param pos index of the element
 * \param elem pointer where the element will be copied
 * \return true on success, false on failure
 */
bool da_get(t_da *vec, size_t pos, void *elem) {
	if (!vec || pos >= vec->nb_e) return (false);
	if (!elem) return (false);
	return (memcpy(elem, da_at(vec, pos), vec->sz_e), true);
}
/**
 * \brief get the raw inner array of the da
 * \param da pointer to the dynamic array
 * \return pointer to the raw inner array
 */
void *da_array(t_da *da) {
	if (!da) return (NULL);
	return (da->array);
}