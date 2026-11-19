#include "dynamic_array.h"

/**
 * \brief iterate over the elements in [start, end[ and apply a function to
 * each of them
 * \param da pointer to the dynamic array
 * \param start index of the first element to visit
 * \param end index of the end element (exclusive). Use -1 (SIZE_MAX) or any
 * value out of bounds to visit up to and including the last element
 * \param func function applied to each visited element, receives the absolute
 * position in the da and a boolean set on the last visited element
 * \param u_val user argument passed to the function
 * \return the user argument (u_val)
 * \note stops early if the callback returns false
 * \ingroup da
**/
void *da_iter(const t_cm_da *da, size_t start, size_t end,
              t_cm_callback_it func, void *u_val) {
	size_t i;
	size_t last;

	if (!da_is_valid(da) || da_is_empty(da) || !func || start >= da->nb_e ||
	    (end != (size_t)-1 && end < start)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (u_val);
	}
	if (end == (size_t)-1 || end > da->nb_e) end = da->nb_e;
	if (end == start) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (u_val);
	}
	last = end - 1;
	for (i = start; i < end && func(da_at(da, i), u_val, i, i == last); i++)
		;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (u_val);
}
