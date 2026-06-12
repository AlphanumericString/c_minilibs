#include <string.h>

#include "dynamic_array.h"

/**
 * \brief advance the array by n elements, calling a deletor on removed elements
 * \param da pointer to the dynamic array
 * \param n number of elements to advance by
 * \param deletor callback to delete removed elements
 * \param u_data user data for deletor
 * \return true on success, false otherwise
 **/
bool da_advance(t_da *da, size_t n, t_callback deletor, void *u_data) {
	if (!da) return (false);
	if (n >= da->nb_e) return (da_apply(da, deletor, u_data), da_clr(da), true);
	for (size_t i = 0; i < n; deletor(da_at(da, i++), u_data))
		;
	memcpy(da->array, da_at(da, n), (da->nb_e - n) * da->sz_e);
	da->nb_e -= n;
	return (true);
}
