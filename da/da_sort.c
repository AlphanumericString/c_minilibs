#include <stdlib.h>
#include "dynamic_array.h"

/**
 * \brief sort the elements of the dynamic array using a comparator function
 * \param da pointer to the dynamic array
 * \param c comparator function
 * \return true on success, false otherwise
 **/
bool da_sort(t_da *da, t_comparator c) {
	return (da_nsort(da, -1, c));
}

/**
 * \brief sort the first n elements of the dynamic array using a comparator function
 * \param da pointer to the dynamic array
 * \param n number of elements to sort
 * \param c comparator function
 * \return true on success, false otherwise
 **/
bool da_nsort(t_da *da, size_t n, t_comparator c) {
	if (!c) return (false);
	if (n > da->nb_e) n = da->nb_e;
	if (n < 2) return (true);
	return (qsort(da->array, da->nb_e, da->sz_e, c), true);
}
