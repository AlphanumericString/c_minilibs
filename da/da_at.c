#include "dynamic_array.h"

/**
 * \brief get element at offset pos respecting the size of elements
 * \param vec pointer to the dynamic array
 * \param pos index of the element
 * \return pointer to the element at the specified position
**/
void *da_at(t_da *vec, size_t pos) {
	if (!vec || pos > vec->nb_e) return (NULL);
	return ((char *)vec->array + vec->sz_e * pos);
}
