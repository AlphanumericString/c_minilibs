#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"

/**
 * \brief convert to array (doesn't free da)
 * \param vec pointer to the dynamic array
 * \return newly allocated array with elements, or NULL on failure
**/
void *da_to_array(t_da *vec) {
	void *data;

	if (!vec) return (NULL);
	data = malloc(vec->sz_e * vec->nb_e);
	if (!data) return (NULL);
	return (da_to_loc_array(vec, data));
}
/**
 * \brief convert to a given array (doesn't allocate array) (doesn't free da)
 * \param vec pointer to the dynamic array
 * \param arr target array where elements will be copied
 * \return pointer to the target array (arr), or NULL on failure
**/
void *da_to_loc_array(t_da *vec, void *arr) {
	if (!vec || !arr) return (NULL);
	memcpy(arr, vec->array, vec->sz_e * vec->nb_e);
	return (arr);
}
