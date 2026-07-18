#include <string.h>

#include "dynamic_array.h"

/**
 * \brief advance the array by n elements, calling a deletor on removed elements
 * \param da pointer to the dynamic array
 * \param n number of elements to advance by
 * \param deletor callback to delete removed elements
 * \param u_data user data for deletor
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_advance(t_da *da, size_t n, t_callback deletor, void *u_data) {
	if (!da) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	if (n >= da->nb_e) {
		da_apply(da, deletor, u_data), da_clr(da);
		LOG(C_MINI__DA_LOG_RM, 0UL, n, da->nb_e + n, da->nb_e);
		return (true);
	}
	for (size_t i = 0; i < n; deletor(da_at(da, i++), u_data))
		;
	memcpy(da->array, da_at(da, n), (da->nb_e - n) * da->sz_e);
	da->nb_e -= n;
	LOG(C_MINI__DA_LOG_RM, 0UL, n, da->nb_e + n, da->nb_e);
	return (true);
}
