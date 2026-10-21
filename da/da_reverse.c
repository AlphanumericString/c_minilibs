#include <stddef.h>

#include "dynamic_array.h"

/**
 * \brief reverse the order of elements in the dynamic array
 * \param da pointer to the dynamic array
 * \ingroup da
**/
void da_reverse(t_da *da) {
	da_nreverse(da, -1);
}
/**
 * \brief reverse the order of the first n elements in the dynamic array
 * \param da pointer to the dynamic array
 * \param n number of elements to reverse
 * \ingroup da
**/
void da_nreverse(t_da *da, size_t n) {
	size_t i;

	if (!da_is_valid(da)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return;
	}
	if (n < 2) return;
	if (n > da->nb_e) n = da->nb_e;
	n--;
	for (i = 0; i < n; i++, n--)
		da_swap(da, i, n);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return;
}
/**
 * \brief reverse the order of the last n elements in the dynamic array
 * \param da pointer to the dynamic array
 * \param n number of elements to reverse
 * \ingroup da
**/
void da_b_nreverse(t_da *da, size_t n) {
	size_t i;
	size_t m;

	if (!da) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return;
	}
	if (n < 2) return;
	if (n >= da->nb_e) {
		da_nreverse(da, da->nb_e);
		return;
	}
	m = da->nb_e - 1;
	for (i = (da->nb_e - n); i < m; i++, m--)
		da_swap(da, i, m);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return;
}
