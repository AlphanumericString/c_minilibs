#include <string.h>

#include "dynamic_array.h"

/**
 * \brief pop the last element of the dynamic array and copy it to out
 * \param da pointer to the dynamic array
 * \param out buffer where the popped element is copied
 * \return true on success, false otherwise
 * \note when compiled with C_MINI__DA_AUTO_SHRINK the capacity is halved
 * whenever the array becomes at most 25% full
 * \ingroup da
**/
bool da_pop(t_cm_da *da, void *out) {
	size_t pos;

	if (!da_is_valid(da) || !out || !da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	pos = da->nb_e - 1;
	memcpy(out, da_at(da, pos), da->sz_e);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_RM, pos, (size_t)1UL, da->nb_e,
	       da->nb_e - 1)
	da->nb_e--;
	(void)da_shrink_auto(da);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
